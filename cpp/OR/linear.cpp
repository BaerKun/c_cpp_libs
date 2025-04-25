#include "linear.h"


bool simplexMethodMax(Eigen::MatrixXf &A, Eigen::VectorXf &b, Eigen::VectorXf &c,
                      std::vector<Eigen::Index> &base, Eigen::VectorXf &x, float &f) {
    const Eigen::Index N_B = A.rows();
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
        float minTheta = std::numeric_limits<float>::max();
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
