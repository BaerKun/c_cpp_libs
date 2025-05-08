#ifndef LINEAR_H
#define LINEAR_H

#include <vector>
#include <Eigen/Core>

namespace OR {
    enum Optim { Maximize, Minimize };

    enum CompareSymbol { Equal, LessEqual, GreaterEqual };

    template<int Dims>
    constexpr int DIMS_MINUS_ONE = Dims == Eigen::Dynamic ? Eigen::Dynamic : Dims - 1;

    template<typename T>
    struct Constraint {
        Eigen::RowVectorX<T> coeffs; // coefficient 系数
        CompareSymbol symbol;
        T rhs; // Right-hand Side 即 b
    };

    /*
     * only Max
     * b >= 0
     */
    template<typename T>
    class LinearProblem {
    public:
        using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

        LinearProblem() = default;

        void setObjective(const Eigen::RowVectorX<T> &objective) {
            objective_ = objective;
        }

        void addConstraint(const Constraint<T> &constraint) {
            constraints_.push_back(constraint);
        }

        void addConstraint(Constraint<T> &&constraint) {
            constraints_.push_back(std::move(constraint));
        }

        const Matrix &getSimplexMatrix() {
            return stdf_;
        }

        bool solve(Eigen::VectorX<T> &x, T &f);

    private:
        Eigen::RowVectorX<T> objective_;
        std::vector<Constraint<T> > constraints_;
        Matrix stdf_;
    };

    /*
     * stdf = [ A  b
     *          c -Z ]
     * b >= 0
     */
    template<int Optim, typename T, int Rows, int Cols, int Major>
    bool simplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &stdf,
                       Eigen::Vector<Eigen::Index, DIMS_MINUS_ONE<Rows> > &basic,
                       Eigen::Vector<T, DIMS_MINUS_ONE<Cols> > &x, T &f);

    /*
     * stdf = [ A  b
     *          c -Z ]
     * c <= 0 (Max) or c >= 0 (Min)
     */
    template<int Optim, typename T, int Rows, int Cols, int Major>
    bool dualSimplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &stdf,
                           Eigen::Vector<Eigen::Index, DIMS_MINUS_ONE<Rows> > &basic,
                           Eigen::Vector<T, DIMS_MINUS_ONE<Cols> > &x, T &f);
}

#include "linear.tpp"

#endif //LINEAR_H
