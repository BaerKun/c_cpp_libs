#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <Eigen/Core>

namespace OR {
    template<int Optim, typename T>
    void TransportationSimplexMethod(Eigen::MatrixX<T> &cost,
                                     Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                                     Eigen::MatrixX<T> &x, T &f);
}

#include "transport.tpp"

#endif //TRANSPORT_H
