#pragma once

#include "linear.hpp"
#include <limits>

namespace OR {
template <int Optim, typename T, int Major>
bool simplexMethod(DynamicMatrix<T, Major> &tableau,
                   Eigen::VectorX<Eigen::Index> &basic,
                   Eigen::VectorX<T> &solution, T &minmax) {
  auto rhs = tableau.col(tableau.cols() - 1).head(basic.rows());
  auto reducedCost = tableau.row(rhs.rows()).head(tableau.cols() - 1); // 检验数

  for (Eigen::Index iter = 0; iter < tableau.rows(); ++iter) {
    // choose max/min objective coefficient (c)
    Eigen::Index entering; // 入基变量
    if (Optim == Maximize ? reducedCost.maxCoeff(&entering) <= EPSILON<T>
                          : reducedCost.minCoeff(&entering) >= -EPSILON<T>) {
      solution = Eigen::VectorX<T>::Zero(reducedCost.cols());
      for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
        solution(basic(r)) = rhs(r);
      }
      minmax = -tableau(rhs.rows(), reducedCost.cols());
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

template <int Optim, typename T, int Major>
bool dualSimplexMethod(DynamicMatrix<T, Major> &tableau,
                       Eigen::VectorX<Eigen::Index> &basic,
                       Eigen::VectorX<T> &solution, T &minmax) {
  auto rhs = tableau.col(tableau.cols() - 1).head(basic.rows());
  auto reducedCost = tableau.row(rhs.rows()).head(tableau.cols() - 1);

  for (Eigen::Index i = 0; i < tableau.rows(); ++i) {
    // choose min rhs(b)
    Eigen::Index leaving;
    if (rhs.minCoeff(&leaving) >= -EPSILON<T>) {
      solution = Eigen::VectorX<T>::Zero(reducedCost.cols());
      for (Eigen::Index r = 0; r < rhs.rows(); ++r) {
        solution(basic(r)) = rhs(r);
      }
      minmax = -tableau(rhs.rows(), reducedCost.cols());
      return true;
    }
    auto row = tableau.row(leaving);

    // choose min/max theta
    Eigen::Index entering = reducedCost.cols();
    if constexpr (Optim == Maximize) {
      T minTheta = std::numeric_limits<T>::max();
      for (Eigen::Index c = 0; c < reducedCost.cols(); ++c) {
        if (row(c) < 0) {
          if (const T theta = reducedCost(c) / row(c); theta < minTheta) {
            minTheta = theta;
            entering = c;
          }
        }
      }
    } else {
      T maxTheta = -std::numeric_limits<T>::max();
      for (Eigen::Index c = 0; c < reducedCost.cols(); ++c) {
        if (row(c) < 0) {
          if (const T theta = reducedCost(c) / row(c); theta > maxTheta) {
            maxTheta = theta;
            entering = c;
          }
        }
      }
    }
    if (entering == reducedCost.cols()) {
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
} // namespace OR
