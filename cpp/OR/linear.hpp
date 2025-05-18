#ifndef LINEAR_HPP
#define LINEAR_HPP

#include <vector>
#include <Eigen/Core>
#include "share.hpp"

namespace OR {
    template<typename T=float>
    struct Constraint {
        Eigen::RowVectorX<T> coeff{}; // coefficients 系数
        int comp{};
        T rhs; // right-hand Side 即 b
    };

    /*
     * only Max
     */
    template<typename T=float>
    class LinearProgram {
    public:
        using SimplexTableau = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

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
                constraints_.push_back({
                    -constraint.coeff,
                    -constraint.comp,
                    -constraint.rhs
                });
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

        bool solve(Eigen::VectorX<T> &x, T &f);

    private:
        Eigen::RowVectorX<T> objective_{};
        std::vector<Constraint<T> > constraints_{};
        SimplexTableau tableau_{};
    };

    extern template
    class LinearProgram<float>;

    extern template
    class LinearProgram<double>;

    /*
     * tableau = [ A  b
     *             c -Z ]
     * b >= 0
     */
    template<int Optim, typename T, int Rows, int Cols, int Major>
    bool simplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &tableau,
                       Eigen::Vector<Eigen::Index, DIMS_MINUS_ONE<Rows> > &basic,
                       Eigen::Vector<T, DIMS_MINUS_ONE<Cols> > &x, T &f);

    /*
     * tableau = [ A  b
     *             c -Z ]
     * c <= 0 (Max) or c >= 0 (Min)
     */
    template<int Optim, typename T, int Rows, int Cols, int Major>
    bool dualSimplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &tableau,
                           Eigen::Vector<Eigen::Index, DIMS_MINUS_ONE<Rows> > &basic,
                           Eigen::Vector<T, DIMS_MINUS_ONE<Cols> > &x, T &f);
}

#include "linear.tpp"

#endif //LINEAR_HPP
