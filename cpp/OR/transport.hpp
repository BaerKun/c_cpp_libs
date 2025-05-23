#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <Eigen/Core>
#include "share.hpp"

namespace OR {
    template<int Optim, typename T, int Major>
    void transportationSimplexMethod(DynamicMatrix<T, Major> &cost,
                                     Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                                     DynamicMatrix<T, Major> &solution, T &minmax);
}

#include "transport.tpp"

#endif //TRANSPORT_H
