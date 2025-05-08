#pragma once

#include <limits>
#include "linear.hpp"

namespace OR {
    constexpr double EPS = 1e-6;

    template<int Optim, typename T, int Rows, int Cols, int Major>
    bool simplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &stdf,
                       Eigen::Vector<Eigen::Index, DIMS_MINUS_ONE<Rows> > &basic,
                       Eigen::Vector<T, DIMS_MINUS_ONE<Cols> > &x, T &f) {
        auto rhs = stdf.col(x.rows()).segment(0, basic.rows());
        auto objCoeff = stdf.row(rhs.rows()).segment(0, x.rows());

        for (Eigen::Index iter = 0; iter < stdf.rows(); ++iter) {
            // choose max/min objective coefficient (c)
            Eigen::Index entering; // 入基变量
            if (Optim == Maximize ? objCoeff.maxCoeff(&entering) <= EPS : objCoeff.minCoeff(&entering) >= -EPS) {
                x.setZero();
                for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
                    x(basic(r)) = rhs(r);
                }
                f = -stdf(rhs.rows(), objCoeff.cols());
                return true;
            }
            auto col = stdf.col(entering);

            // choose min theta(b/a)
            Eigen::Index leaving = rhs.rows(); // 出基
            T minTheta = std::numeric_limits<T>::max();
            for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
                if (col(r) > 0) {
                    if (const T theta = rhs(r) / col(r); theta < minTheta) {
                        minTheta = theta;
                        leaving = r;
                    }
                }
            }
            if (leaving == rhs.rows()) {
                return false;
            }

            // update
            auto row = stdf.row(leaving);
            row /= row(entering);
            for (Eigen::Index r = 0; r < stdf.rows(); ++r) {
                if (r != leaving) {
                    stdf.row(r) -= row * col(r);
                }
            }
            basic(leaving) = entering;
        }
        return false;
    }

    template<int Optim, typename T, int Rows, int Cols, int Major>
    bool dualSimplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &stdf,
                           Eigen::Vector<Eigen::Index, DIMS_MINUS_ONE<Rows> > &basic,
                           Eigen::Vector<T, DIMS_MINUS_ONE<Cols> > &x, T &f) {
        auto rhs = stdf.col(x.rows()).segment(0, basic.rows());
        auto objCoeff = stdf.row(rhs.rows()).segment(0, x.rows());

        for (Eigen::Index i = 0; i < stdf.rows(); ++i) {
            // choose min rhs(b)
            Eigen::Index leaving;
            if (rhs.minCoeff(&leaving) >= -EPS) {
                x.setZero();
                for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
                    x(basic(r)) = rhs(r);
                }
                f = -stdf(rhs.rows(), objCoeff.cols());
                return true;
            }
            auto row = stdf.row(leaving);

            // choose min/max theta
            Eigen::Index entering = objCoeff.cols();
            if constexpr (Optim == Maximize) {
                T minTheta = std::numeric_limits<T>::max();
                for (Eigen::Index c = 0; c < objCoeff.cols(); ++c) {
                    if (row(c) < 0) {
                        if (const T theta = objCoeff(c) / row(c); theta < minTheta) {
                            minTheta = theta;
                            entering = c;
                        }
                    }
                }
            } else {
                T maxTheta = -std::numeric_limits<T>::max();
                for (Eigen::Index c = 0; c < objCoeff.cols(); ++c) {
                    if (row(c) < 0) {
                        if (const T theta = objCoeff(c) / row(c); theta > maxTheta) {
                            maxTheta = theta;
                            entering = c;
                        }
                    }
                }
            }
            if (entering == objCoeff.cols()) {
                return false;
            }

            // update
            auto col = stdf.col(entering);
            row /= row(entering);
            for (Eigen::Index r = 0; r < stdf.rows(); ++r) {
                if (r != leaving) {
                    stdf.row(r) -= row * col(r);
                }
            }
            basic(leaving) = entering;
        }
        return false;
    }

    template<typename T>
    bool LinearProblem<T>::solve(Eigen::VectorX<T> &x, T &f) {
        const Eigen::Index numVar = objective_.cols();
        const Eigen::Index numConstr = constraints_.size();
        Eigen::VectorX<Eigen::Index> basic(constraints_.size());

        Eigen::Index numSlack = 0; // 松弛变量
        Eigen::Index numArtificial = 0; // 人工变量
        for (const auto &constr: constraints_) {
            switch (constr.symbol) {
                case Equal:
                    numArtificial += 1;
                    break;
                case LessEqual:
                    numSlack += 1;
                    break;
                case GreaterEqual:
                    numSlack += 1;
                    numArtificial += 1;
                default:
                    break;
            }
        }
        const Eigen::Index numVarSlack = numVar + numSlack;
        const Eigen::Index totalVar = numVarSlack + numArtificial;

        // 初始化标准型矩阵
        stdf_.resize(numConstr + 1, totalVar + 1);
        auto objCoeff = stdf_.row(numConstr);
        auto rhs = stdf_.col(totalVar);

        stdf_.rightCols(numSlack + numArtificial + 1).setZero();
        for (Eigen::Index r = 0, slackIdx = numVar, artificialIdx = numVarSlack; r < numConstr; ++r) {
            const auto &constr = constraints_[r];
            auto row = stdf_.row(r);
            row.segment(0, numVar) = constr.coeffs;
            rhs(r) = constr.rhs;

            Eigen::Index entering;
            switch (constr.symbol) {
                case Equal:
                    entering = artificialIdx++;
                    break;
                case LessEqual:
                    entering = slackIdx++;
                    break;
                case GreaterEqual:
                    row(slackIdx++) = -1;
                    entering = artificialIdx++;
                default:
                    break;
            }
            row(entering) = 1;
            basic(r) = entering;
        }
        Eigen::VectorX<T> stdfX(totalVar);

        // 直接单纯形法
        if (numArtificial == 0) {
            objCoeff.segment(0, numVar) = objective_;
            if (!simplexMethod<Maximize>(stdf_, basic, stdfX, f)) return false;
            x = stdfX.segment(0, numVar);
            return true;
        }

        // 两阶段法
        // 第一阶段：Minimize sum(artificial)
        objCoeff.segment(0, numVar).setZero();
        objCoeff.segment(numVarSlack, numArtificial).setOnes();
        for (Eigen::Index i = 0; i < basic.size(); ++i) {
            if (basic[i] >= numVarSlack)
                objCoeff -= stdf_.row(i);
        }

        if (!simplexMethod<Minimize>(stdf_, basic, stdfX, f)) return false;
        if (f > EPS) return false;

        // 第二阶段
        stdf_.col(numVarSlack) = rhs; // 复制rhs，防止resize后丢失
        stdf_.conservativeResize(numConstr + 1, numVarSlack + 1); // 重新分配内存，并保留部分原有数据

        auto objCoeff2 = stdf_.row(numConstr); // Block不能重新引用，必须声明新变量
        objCoeff2.segment(0, numVar) = objective_;
        objCoeff2.segment(numVar, numSlack + 1).setZero();
        for (Eigen::Index i = 0; i < basic.size(); ++i) {
            objCoeff2 -= stdf_.row(i) * objective_(basic(i));
        }

        stdfX.resize(numVarSlack);
        if (!simplexMethod<Maximize>(stdf_, basic, stdfX, f)) return false;
        x = stdfX.segment(0, numVar);
        return true;
    }
}
