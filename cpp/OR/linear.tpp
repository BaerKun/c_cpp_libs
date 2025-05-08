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
            // choose max/min value(c)
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
            // choose min b
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
        const int numVar = objective_.cols();
        const size_t numConstr = constraints_.size();
        Eigen::VectorX<Eigen::Index> basic(constraints_.size());

        int numSlack = 0; // 松弛变量
        int numArtificial = 0; // 人工变量
        for (const auto &constr: constraints_) {
            switch (constr.symbol) {
                case Equal:
                    numArtificial += 1;
                    break;
                case Less_Equal:
                    numSlack += 1;
                    break;
                case Greater_Equal:
                    numSlack += 1;
                    numArtificial += 1;
                    break;
                default:
                    break;
            }
        }

        // 初始化标准型矩阵
        Matrix stdf1 = Matrix::Zero(numConstr + 1, numVar + numSlack + numArtificial + 1);
        auto objCoeff1 = stdf1.row(numConstr);
        auto rhs1 = stdf1.col(numVar + numSlack + numArtificial);

        for (Eigen::Index i = 0, s = 0, a = 0; i < numConstr; ++i) {
            const auto &constr = constraints_[i];
            auto row = stdf1.row(i);
            row.segment(0, numVar) = constr.coeffs;
            rhs1(i) = constr.rhs;

            Eigen::Index b;
            switch (constr.symbol) {
                case Equal:
                    b = numVar + numSlack + a++;
                    break;
                case Less_Equal:
                    b = numVar + s++;
                    break;
                case Greater_Equal:
                    row(numVar + s++) = -1;
                    b = numVar + numSlack + a++;
                    break;
                default:
                    break;
            }
            row(b) = 1;
            basic(i) = b;
        }
        Eigen::VectorX<T> stdfX1(numVar + numSlack + numArtificial);

        // 直接单纯形法
        if (numArtificial == 0) {
            objCoeff1.segment(0, numVar) = objective_;
            if (simplexMethod<Maximize>(stdf1, basic, stdfX1, f)) {
                x = stdfX1.segment(0, numVar);
                stdf_ = std::move(stdf1);
                return true;
            }
            return false;
        }

        // 两阶段法
        for (Eigen::Index i = 0; i < numArtificial; ++i) {
            objCoeff1(numVar + numSlack + i) = 1;
        }
        for (Eigen::Index i = 0; i < basic.size(); ++i) {
            if (basic[i] >= numVar + numSlack) {
                objCoeff1 -= stdf1.row(i);
            }
        }

        if (!simplexMethod<Minimize>(stdf1, basic, stdfX1, f)) {
            return false;
        }
        if (f > EPS) {
            return false;
        }

        // 第二阶段
        Matrix stdf2(numConstr + 1, numVar + numSlack + 1);
        auto objCoeff2 = stdf2.row(numConstr);

        stdf2.topLeftCorner(numConstr, numVar + numSlack) = stdf1.topLeftCorner(numConstr, numVar + numSlack);
        stdf2.col(numVar + numSlack) = rhs1;
        objCoeff2.setZero();
        objCoeff2.segment(0, numVar) = objective_;
        for (Eigen::Index i = 0; i < basic.size(); ++i) {
            objCoeff2 -= stdf2.row(i) * objective_(basic(i));
        }

        Eigen::VectorX<T> stdfX2(numVar + numSlack);
        if (!simplexMethod<Maximize>(stdf2, basic, stdfX2, f)) {
            return false;
        }
        x = stdfX2.segment(0, numVar);
        stdf_ = std::move(stdf2);
        return true;
    }
}
