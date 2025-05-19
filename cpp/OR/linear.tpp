#pragma once

#include <limits>
#include "linear.hpp"

namespace OR {
    template<int Optim, typename T, int Major>
    bool simplexMethod(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Major> &tableau,
                       Eigen::VectorX<Eigen::Index> &basic,
                       Eigen::VectorX<T> &x, T &f) {
        auto rhs = tableau.col(tableau.cols() - 1).head(basic.rows());
        auto objCoeff = tableau.row(rhs.rows()).head(tableau.cols() - 1);

        for (Eigen::Index iter = 0; iter < tableau.rows(); ++iter) {
            // choose max/min objective coefficient (c)
            Eigen::Index entering; // 入基变量
            if (Optim == Maximize
                    ? objCoeff.maxCoeff(&entering) <= EPSILON<T>
                    : objCoeff.minCoeff(&entering) >= -EPSILON<T>) {
                x = Eigen::VectorX<T>::Zero(objCoeff.cols());
                for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
                    x(basic(r)) = rhs(r);
                }
                f = -tableau(rhs.rows(), objCoeff.cols());
                return true;
            }
            auto col = tableau.col(entering);

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
            auto row = tableau.row(leaving);
            row /= row(entering);
            for (Eigen::Index r = 0; r < tableau.rows(); ++r) {
                if (r != leaving) {
                    tableau.row(r) -= row * col(r);
                }
            }
            basic(leaving) = entering;
        }
        return false;
    }

    template<int Optim, typename T, int Major>
    bool dualSimplexMethod(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Major> &tableau,
                           Eigen::VectorX<Eigen::Index> &basic,
                           Eigen::VectorX<T> &x, T &f) {
        auto rhs = tableau.col(tableau.cols() - 1).head(basic.rows());
        auto objCoeff = tableau.row(rhs.rows()).head(tableau.cols() - 1);

        for (Eigen::Index i = 0; i < tableau.rows(); ++i) {
            // choose min rhs(b)
            Eigen::Index leaving;
            if (rhs.minCoeff(&leaving) >= -EPSILON<T>) {
                x = Eigen::VectorX<T>::Zero(objCoeff.cols());
                for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
                    x(basic(r)) = rhs(r);
                }
                f = -tableau(rhs.rows(), objCoeff.cols());
                return true;
            }
            auto row = tableau.row(leaving);

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
            auto col = tableau.col(entering);
            row /= row(entering);
            for (Eigen::Index r = 0; r < tableau.rows(); ++r) {
                if (r != leaving) {
                    tableau.row(r) -= row * col(r);
                }
            }
            basic(leaving) = entering;
        }
        return false;
    }
}
