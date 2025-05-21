#include "transport.hpp"
#include <list>

namespace OR {
    struct BasicVariable { // link node 2d
        Eigen::Index rowIdx, colIdx;
        BasicVariable *next{};
        BasicVariable *down{};

        BasicVariable(const Eigen::Index r, const Eigen::Index c) {
            this->rowIdx = r;
            this->colIdx = c;
        }
    };

    struct BasicContainer {
        std::vector<BasicVariable *> row;
        std::vector<BasicVariable *> col;

        BasicContainer(const Eigen::Index rows, const Eigen::Index cols) : row(rows), col(cols) {
            row.assign(rows, nullptr);
            col.assign(cols, nullptr);
        }

        ~BasicContainer() {
            for (const auto head: row) {
                if (head == nullptr) continue;

                auto node = head;
                do {
                    const auto next = node->next;
                    delete node;
                    node = next;
                } while (node != head);
            }
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
            pushBack(row[r], node);
            pushDown(col[c], node);
        }
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

        std::function<void(const BasicVariable *)> forwardRow, forwardCol;
        forwardRow = [&](const BasicVariable *node) {
            v(node->colIdx) = cost(node->rowIdx, node->colIdx) - u(node->rowIdx);
        };

        u(0) = 0;
    }

    template<int Optim, typename T, int Major>
    void TransportationSimplexMethod(DynamicMatrix<T, Major> &cost,
                                     Eigen::VectorX<T> &supply, Eigen::RowVectorX<T> &demand,
                                     DynamicMatrix<T, Major> &x, T &f) {
        BasicContainer basic(cost.rows(), cost.cols());
        Eigen::MatrixX<T> reducedCost(cost.rows(), cost.cols());
        x = DynamicMatrix<T, Major>::Zero(cost.rows(), cost.cols());

        northwestCorner<Optim, T>(supply, demand, x, basic);
        potentialMethod(cost, basic, reducedCost);
    }
}
