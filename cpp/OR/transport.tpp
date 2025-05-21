#pragma once

#include "transport.hpp"
#include <vector>
#include <functional>

namespace OR {
    struct BasicVariable { // link node 2d
        Eigen::Index rowIdx, colIdx;
        BasicVariable *next = nullptr;
        BasicVariable *down = nullptr;

        BasicVariable(const Eigen::Index r, const Eigen::Index c) {
            this->rowIdx = r;
            this->colIdx = c;
        }
    };

    class BasicContainer {
    public:
        BasicContainer(const Eigen::Index rows, const Eigen::Index cols) : row_(rows), col_(cols) {
            row_.assign(rows, nullptr);
            col_.assign(cols, nullptr);
        }

        ~BasicContainer() {
            for (const auto head: row_) {
                if (head == nullptr) continue;

                auto node = head;
                do {
                    const auto next = node->next;
                    delete node;
                    node = next;
                } while (node != head);
            }
        }

        BasicVariable *getHead() {
            return row_[0];
        }

        void push(const Eigen::Index r, const Eigen::Index c) {
            static const auto pushBack = [](BasicVariable *&head, BasicVariable *const node) {
                if (head == nullptr) {
                    head = node->next = node;
                } else {
                    node->next = head->next;
                    head->next = node;
                }
            };

            static const auto pushDown = [](BasicVariable *&head, BasicVariable *const node) {
                if (head == nullptr) {
                    head = node->down = node;
                } else {
                    node->down = head->down;
                    head->down = node;
                }
            };

            const auto node = new BasicVariable(r, c);
            pushBack(row_[r], node);
            pushDown(col_[c], node);
        }

    private:
        std::vector<BasicVariable *> row_;
        std::vector<BasicVariable *> col_;


    };

    template<int Optim, typename T, int Major>
    void northwestCorner(Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                         DynamicMatrix<T, Major> &x,
                         BasicContainer &basic) {
        Eigen::Index r = 0, c = 0;
        while (true) {
            basic.push(r, c);
            if (supply[r] < demand[c]) {
                x(r, c) = supply[r];
                demand[c] -= supply[r];
                supply[r] = 0;
                if (++r == supply.rows()) break;
            } else {
                x(r, c) = demand[c];
                supply[r] -= demand[c];
                demand[c] = 0;
                if (++c == demand.cols()) break;
            }
        }
    }

    // 位势法
    template<typename T, int Major>
    void potentialMethod(DynamicMatrix<T, Major> &cost, BasicContainer &basic,
                         DynamicMatrix<T, Major> &reducedCost) {
        Eigen::VectorX<T> u(cost.rows());
        Eigen::RowVectorX<T> v(cost.cols());

        std::function<void(const BasicVariable *, const BasicVariable *,
                           const BasicVariable *)> updateU, updateV;
        updateU = [&cost, &u, &v, &updateU, &updateV](const BasicVariable *const node,
                                                      const BasicVariable *const rowHead,
                                                      const BasicVariable *const colHead) {
            u(node->rowIdx) = cost(node->rowIdx, node->colIdx) - v(node->colIdx);
            if (node->next != rowHead)
                updateV(node->next, rowHead, node->next);
            if (node->down != colHead)
                updateU(node->down, node->down, colHead);
        };
        updateV = [&cost, &u, &v, &updateU, &updateV](const BasicVariable *const node,
                                                      const BasicVariable *const rowHead,
                                                      const BasicVariable *const colHead) {
            v(node->colIdx) = cost(node->rowIdx, node->colIdx) - u(node->rowIdx);
            if (node->next != rowHead)
                updateV(node->next, rowHead, node->next);
            if (node->down != colHead)
                updateU(node->down, node->down, colHead);
        };

        const auto head = basic.getHead();
        u(0) = 0;
        updateV(head, head, head);
        for (Eigen::Index r = 0; r < cost.rows(); ++r) {
            for (Eigen::Index c = 0; c < cost.cols(); ++c)
                reducedCost(r, c) = cost(r, c) - u(r) - v(c);
        }
    }

    template<int Optim, typename T, int Major>
    void transportationSimplexMethod(DynamicMatrix<T, Major> &cost,
                                     Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                                     DynamicMatrix<T, Major> &x, T &f) {
        BasicContainer basic(cost.rows(), cost.cols());
        DynamicMatrix<T, Major> reducedCost(cost.rows(), cost.cols());
        x = DynamicMatrix<T, Major>::Zero(cost.rows(), cost.cols());

        northwestCorner<Optim, T>(supply, demand, x, basic);
        potentialMethod(cost, basic, reducedCost);
    }
}
