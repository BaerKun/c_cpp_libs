#ifndef LINEAR_HPP
#define LINEAR_HPP

#include "share.hpp"
#include <Eigen/Core>
#include <vector>

namespace OR {
template <typename T = float> struct Constraint {
  Eigen::RowVectorX<T> coeff{}; // coefficients 系数
  int comp{};
  T rhs; // right-hand Side 即 b
};

/*
 * only Max
 */
template <typename T = float> class LinearProgram {
public:
  using SimplexTableau = DynamicMatrix<T, Eigen::RowMajor>;

  LinearProgram() = default;

  void setObjective(const Eigen::RowVectorX<T> &objective) {
    objective_ = objective;
  }

  void setObjective(Eigen::RowVectorX<T> &&objective) {
    objective_ = std::move(objective);
  }

  void addConstraint(const Constraint<T> &constraint) {
    if (constraint.rhs >= 0)
      constraints_.push_back(constraint);
    else
      constraints_.push_back(
          {-constraint.coeff, -constraint.comp, -constraint.rhs});
  }

  void addConstraint(Constraint<T> &&constraint) {
    if (constraint.rhs < 0) {
      constraint.coeff = -constraint.coeff;
      constraint.comp = -constraint.comp;
      constraint.rhs = -constraint.rhs;
    }
    constraints_.push_back(std::move(constraint));
  }

  [[nodiscard]] const SimplexTableau &getSimplexTableau() const {
    return tableau_;
  }

  bool solve(Eigen::VectorX<T> &solution, T &minmax);

private:
  Eigen::RowVectorX<T> objective_{};
  std::vector<Constraint<T>> constraints_{};
  SimplexTableau tableau_{};
};

extern template class LinearProgram<float>;

extern template class LinearProgram<double>;

/*
 * tableau = [ A  b
 *             c -Z ]
 * b >= 0
 */
template <int Optim, typename T, int Major>
bool simplexMethod(DynamicMatrix<T, Major> &tableau,
                   Eigen::VectorX<Eigen::Index> &basic,
                   Eigen::VectorX<T> &solution, T &minmax);

/*
 * tableau = [ A  b
 *             c -Z ]
 * c <= 0 (Max) or c >= 0 (Min)
 */
template <int Optim, typename T, int Major>
bool dualSimplexMethod(DynamicMatrix<T, Major> &tableau,
                       Eigen::VectorX<Eigen::Index> &basic,
                       Eigen::VectorX<T> &solution, T &minmax);
} // namespace OR

#include "linear.tpp"

#endif // LINEAR_HPP
