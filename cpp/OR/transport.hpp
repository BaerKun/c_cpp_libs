#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <Eigen/Core>

namespace OR {
    template<int Optim, typename T, int Rows, int Cols, int Major>
    void TransportationSimplexMethod(Eigen::Matrix<T, Rows, Cols, Major> &cost,
                                     Eigen::Vector<T, Rows> &supply, Eigen::Vector<T, Cols> &demand,
                                     Eigen::Matrix<T, Rows, Cols, Major> &x, T &f);
}

#include "transport.tpp"

#endif //TRANSPORT_H
