#ifndef LINEAR_H
#define LINEAR_H

#include <limits>
#include <Eigen/Dense>

// 需要手算标准形
template<int Rows, int Cols, typename T>
bool simplexMethodMax(Eigen::Matrix<T, Rows, Cols> &A, Eigen::Vector<T, Rows> &b, Eigen::RowVector<T, Cols> &c,
                      Eigen::Vector<Eigen::Index, Rows> &base, Eigen::Vector<T, Cols> &x, T &f) {
    const Eigen::Index rows = Rows >= 0 ? Rows : b.size();
    x.setZero();
    f = 0;
    for (Eigen::Index i = 0; i <= rows; ++i) {
        Eigen::Index colIdx;
        const float maxC = c.maxCoeff(&colIdx);
        if (maxC <= 0) {
            for (Eigen::Index r = 0; r < rows; ++r) {
                x[base[r]] = b[r];
            }
            return true;
        }
        auto col = A.col(colIdx);

        Eigen::Index rowIdx = rows;
        float minTheta = std::numeric_limits<T>::max();
        for (Eigen::Index r = 0; r < rows; ++r) {
            if (col(r) > 0) {
                if (const float theta = b(r) / col(r); theta < minTheta) {
                    minTheta = theta;
                    rowIdx = r;
                }
            }
        }
        if (rowIdx == rows) {
            return false;
        }

        auto row = A.row(rowIdx);
        b(rowIdx) /= row(colIdx);
        row /= row(colIdx);
        for (Eigen::Index r = 0; r < rows; ++r) {
            if (r != rowIdx) {
                b(r) -= b(rowIdx) * col(r);
                A.row(r) -= row * col(r);
            }
        }

        f += b(rowIdx) * maxC;
        c -= row * maxC;
        base[rowIdx] = colIdx;
    }
    return false;
}

#endif //LINEAR_H
