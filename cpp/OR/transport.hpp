#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <Eigen/Core>
#include "share.hpp"

namespace OR {
    template<int Optim, typename T, int Major>
    void TransportationSimplexMethod(DynamicMatrix<T, Major> &cost,
                                     Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                                     DynamicMatrix<T, Major> &x, T &f);
}

#include "transport.tpp"

#endif //TRANSPORT_H
