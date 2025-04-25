#ifndef LINEAR_H
#define LINEAR_H

#include <limits>
#include <Eigen/Dense>

// 需要手算标准形
template<int N_B, int N_C, typename T>
bool simplexMethodMax(Eigen::Matrix<T, N_B, N_C> &A, Eigen::Vector<T, N_B> &b, Eigen::Vector<T, N_C> &c,
                      std::array<Eigen::Index, N_B> &base, Eigen::Vector<T, N_C> &x, T &f) {
    x.setZero();
    f = 0;
    for (Eigen::Index i = 0; i < N_B; ++i) {
        Eigen::Index colIdx;
        const float maxC = c.maxCoeff(&colIdx);
        if (maxC <= 0) {
            for (Eigen::Index j = 0; j < N_B; ++j) {
                x[base[j]] = b[j];
            }
            return true;
        }

        auto col = A.col(colIdx);
        Eigen::Index rowIdx = N_B;
        float minTheta = std::numeric_limits<T>::max();
        for (Eigen::Index j = 0; j < N_B; ++j) {
            if (col(j) > 0) {
                if (const float theta = b(j) / col(j); theta < minTheta) {
                    minTheta = theta;
                    rowIdx = j;
                }
            }
        }
        if (rowIdx == N_B) {
            return false;
        }

        auto row = A.row(rowIdx);
        b(rowIdx) /= col(rowIdx);
        row /= col(rowIdx);
        for (Eigen::Index j = 0; j < N_B; ++j) {
            if (j != rowIdx) {
                b(j) -= b(rowIdx) * col(j);
                A.row(j) -= row * col(j);
            }
        }

        f += b(rowIdx) * maxC;
        c -= row * maxC;
        base[rowIdx] = colIdx;
    }
    return false;
}

bool simplexMethodMax(Eigen::MatrixXf &A, Eigen::VectorXf &b, Eigen::VectorXf &c,
                      std::vector<Eigen::Index> &base, Eigen::VectorXf &x, float &f);

#endif //LINEAR_H
