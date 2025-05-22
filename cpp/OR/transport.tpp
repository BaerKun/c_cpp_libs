#pragma once

#include "transport.hpp"
#include <vector>
#include <functional>
#include <limits>

namespace OR {
    // 二维循环链表，一定程度可以视作二叉树
    struct BasicVariable {
        Eigen::Index rowIdx, colIdx;
        BasicVariable *next = nullptr;
        BasicVariable *down = nullptr;

        explicit BasicVariable(const Eigen::Index r = -1, const Eigen::Index c = -1)
                : rowIdx(r), colIdx(c) {
        }
    };

    class BasicContainer {
    public:
        BasicContainer(const Eigen::Index rows, const Eigen::Index cols)
                : row_(rows, nullptr), col_(cols, nullptr) {
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

        [[nodiscard]] BasicVariable *getHead() const {
            return row_[0];
        }

        void push(BasicVariable *const node) {
            if (BasicVariable *&left = row_[node->rowIdx]; left == nullptr) {
                left = node->next = node;
            } else {
                node->next = left->next;
                left->next = node;
            }
            if (BasicVariable *&top = col_[node->colIdx]; top == nullptr) {
                top = node->down = node;
            } else {
                node->down = top->down;
                top->down = node;
            }
        }

        void unlink(const BasicVariable *const node) {
            auto left = row_[node->rowIdx];
            if (node == left)
                row_[node->rowIdx] = node->next;
            for (; left->next != node; left = left->next);
            left->next = node->next;

            auto top = col_[node->colIdx];
            if (node == top)
                col_[node->colIdx] = node->down;
            for (; top->down != node; top = top->down);
            top->down = node->down;
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
            basic.push(new BasicVariable(r, c));
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

    template<typename T, int Major>
    void potentialMethod(DynamicMatrix < T, Major > &cost, BasicContainer & basic,
                         DynamicMatrix < T, Major > &reducedCost) {
        Eigen::VectorX<T> u(cost.rows());
        Eigen::RowVectorX<T> v(cost.cols());

        std::function<void(const BasicVariable *, const BasicVariable *, const BasicVariable *)> recursionStep;
        recursionStep = [&cost, &u, &v, &recursionStep](const BasicVariable *const node,
                                                        const BasicVariable *const rowHead,
                                                        const BasicVariable *const colHead) {
            const auto next = node->next;
            if (next != rowHead) {
                v(next->colIdx) = cost(next->rowIdx, next->colIdx) - u(next->rowIdx);
                recursionStep(next, rowHead, next);
            }
            const auto down = node->down;
            if (down != colHead) {
                u(down->rowIdx) = cost(down->rowIdx, down->colIdx) - v(down->colIdx);
                recursionStep(down, down, colHead);
            }
        };

        const auto head = basic.getHead();
        u(head->rowIdx) = 0;
        v(head->colIdx) = cost(head->rowIdx, head->colIdx);
        recursionStep(head, head, head);
        for (Eigen::Index r = 0; r < cost.rows(); ++r) {
            for (Eigen::Index c = 0; c < cost.cols(); ++c)
                reducedCost(r, c) = cost(r, c) - u(r) - v(c);
        }
    }

    class ClosedLoopHelper {
    public:
        ClosedLoopHelper(BasicContainer &basic, std::vector<BasicVariable *> &closedLoop)
                : basic_(basic), closedLoop_(closedLoop) {
            entering_ = new BasicVariable;
        }

        ~ClosedLoopHelper() {
            delete entering_;
        }

        void find(const Eigen::Index enteringRow, const Eigen::Index enteringCol) {
            entering_->rowIdx = enteringRow;
            entering_->colIdx = enteringCol;
            basic_.push(entering_);
            closedLoop_.clear();
            recursionStep(entering_);
        }

        void update(BasicVariable *const leaving) {
            entering_ = leaving;
        }

    private:
        bool recursionStep(BasicVariable *const node) {
            closedLoop_.push_back(node);
            for (auto nodeRow = node->next; nodeRow != node; nodeRow = nodeRow->next) {
                closedLoop_.push_back(nodeRow);

                for (auto nodeCol = nodeRow->down; nodeCol != nodeRow; nodeCol = nodeCol->down) {
                    if (nodeCol == entering_ || recursionStep(nodeCol))
                        return true;
                }
                closedLoop_.pop_back();
            }
            closedLoop_.pop_back();
            return false;
        }

        BasicVariable *entering_;
        BasicContainer &basic_;
        std::vector<BasicVariable *> &closedLoop_;
    };

    template<int Optim, typename T, int Major>
    void closedLoopAdjust(DynamicMatrix < T, Major > &cost,
                          DynamicMatrix < T, Major > &reducedCost,
                          BasicContainer & basic,
                          DynamicMatrix < T, Major > &x) {
        std::vector<BasicVariable *> closedLoop;
        ClosedLoopHelper helper(basic, closedLoop);
        closedLoop.reserve(cost.rows() + cost.cols());

        while (true) {
            Eigen::Index enteringRow, enteringCol;
            const T minReducedCost = reducedCost.minCoeff(&enteringRow, &enteringCol);
            if (minReducedCost >= 0)
                break;
            helper.find(enteringRow, enteringCol);

            T adjustment = std::numeric_limits<T>::max();
            BasicVariable *leaving = nullptr;
            for (size_t i = 1; i < closedLoop.size(); ++i) {
                if (i & 1) {
                    const auto node = closedLoop[i];
                    if (x(node->rowIdx, node->colIdx) < adjustment) {
                        adjustment = x(node->rowIdx, node->colIdx);
                        leaving = closedLoop[i];
                    }
                }
            }
            for (size_t i = 0; i < closedLoop.size(); ++i) {
                const auto node = closedLoop[i];
                if (i & 1)
                    x(node->rowIdx, node->colIdx) -= adjustment;
                else
                    x(node->rowIdx, node->colIdx) += adjustment;
            }
            basic.unlink(leaving);
            helper.update(leaving);
            potentialMethod(cost, basic, reducedCost);
        }
    }


    template<int Optim, typename T, int Major>
    void transportationSimplexMethod(DynamicMatrix < T, Major > &cost,
                                     Eigen::VectorX<T> & supply, Eigen::RowVectorX<T> & demand,
                                     DynamicMatrix < T, Major > &x, T & f) {
        BasicContainer basic(cost.rows(), cost.cols());
        DynamicMatrix < T, Major > reducedCost(cost.rows(), cost.cols());
        x = DynamicMatrix < T, Major > ::Zero(cost.rows(), cost.cols());

        northwestCorner<Optim, T>(supply, demand, x, basic);
        potentialMethod(cost, basic, reducedCost);
        closedLoopAdjust<Optim, T>(cost, reducedCost, basic, x);

        f = 0;
        for (Eigen::Index r = 0; r < cost.rows(); ++r) {
            for (Eigen::Index c = 0; c < cost.cols(); ++c) {
                f += cost(r, c) * x(r, c);
            }
        }
    }
}
