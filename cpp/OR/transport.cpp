#include "transport.hpp"
#include <tuple>

namespace OR {
    template<int Optim, typename T>
    void northwestCorner(Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                         Eigen::MatrixX<T> &x,
                         std::vector<std::tuple<Eigen::Index, Eigen::Index>> &basic) {
        x.setZero();
        for (Eigen::Index i = 0, j = 0, k = 0; i < supply.rows() && j < demand.rows();) {
            basic[k++] = std::make_tuple(i, j);
            if (supply[i] < demand[j]) {
                x(i, j) = supply[i];
                demand[j] -= supply[i];
                supply[i] = 0;
            } else {
                x(i, j) = demand[j];
                supply[i] -= demand[j];
                demand[j] = 0;
            }
        }
    }

    template<int Optim, typename T>
    void TransportationSimplexMethod(Eigen::MatrixX<T> &cost,
                                     Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                                     Eigen::MatrixX<T> &x, T &f) {
        std::vector<std::tuple<Eigen::Index, Eigen::Index>> basic(cost.rows() * cost.cols() - 1);
        northwestCorner<Optim, T>(supply, demand, x, basic);
    }
}
