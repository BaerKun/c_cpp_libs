#include "linear.hpp"

namespace OR {
template <typename T>
bool LinearProgram<T>::solve(Eigen::VectorX<T> &solution, T &minmax) {
  const Eigen::Index numObjVar = objective_.cols();
  const Eigen::Index numConstr = constraints_.size();

  Eigen::VectorX<Eigen::Index> basic(constraints_.size());
  Eigen::Index numSlackVar = 0;      // 松弛变量
  Eigen::Index numArtificialVar = 0; // 人工变量
  for (const auto &constr : constraints_) {
    switch (constr.comp) {
    case EQUAL:
      numArtificialVar += 1;
      break;
    case LESS_EQUAL:
      numSlackVar += 1;
      break;
    case GREATER_EQUAL:
      numSlackVar += 1;
      numArtificialVar += 1;
    default:
      break;
    }
  }
  const Eigen::Index numObjAndSlackVar = numObjVar + numSlackVar;
  const Eigen::Index totalVar = numObjAndSlackVar + numArtificialVar;

  // 初始化标准型矩阵
  tableau_ = SimplexTableau::Zero(numConstr + 1, totalVar + 1);
  auto rhs = tableau_.col(totalVar);

  for (Eigen::Index r = 0, slackIdx = numObjVar,
                    artificialIdx = numObjAndSlackVar;
       r < numConstr; ++r) {
    const auto &constr = constraints_[r];
    auto row = tableau_.row(r);
    row.head(numObjVar) = constr.coeff;
    rhs(r) = constr.rhs;

    Eigen::Index entering = -1;
    switch (constr.comp) {
    case EQUAL:
      entering = artificialIdx++;
      break;
    case LESS_EQUAL:
      entering = slackIdx++;
      break;
    case GREATER_EQUAL:
      row(slackIdx++) = -1;
      entering = artificialIdx++;
    default:
      break;
    }
    row(entering) = 1;
    basic(r) = entering;
  }

  Eigen::VectorX<T> var(totalVar);
  auto reducedCost = tableau_.row(numConstr);

  // 直接单纯形法
  if (numArtificialVar == 0) {
    reducedCost.head(numObjVar) = objective_;
    if (!simplexMethod<Maximize>(tableau_, basic, var, minmax)) return false;
    solution = var.head(numObjVar);
    return true;
  }

  // 两阶段法
  // 第一阶段：Minimize sum(artificial)
  reducedCost.segment(numObjAndSlackVar, numArtificialVar).setOnes();
  for (Eigen::Index i = 0; i < basic.size(); ++i) {
    if (basic[i] >= numObjAndSlackVar) reducedCost -= tableau_.row(i);
  }

  if (!simplexMethod<Minimize>(tableau_, basic, var, minmax)) return false;
  if (minmax >= EPSILON<T>) return false;

  // 第二阶段
  tableau_.col(numObjAndSlackVar) = rhs; // 复制rhs，防止resize后丢失
  tableau_.conservativeResize(
      numConstr + 1, numObjAndSlackVar + 1); // 重新分配内存，并保留部分原有数据

  auto reducedCost2 =
      tableau_.row(numConstr); // Block不能重新引用，必须声明新变量
  reducedCost2.head(numObjVar) = objective_;
  reducedCost2.segment(numObjVar, numSlackVar + 1).setZero();

  for (Eigen::Index i = 0; i < basic.size(); ++i) {
    if (const Eigen::Index b = basic(i); b < numObjVar)
      reducedCost2 -= tableau_.row(i) * objective_(b);
  }

  var.resize(numObjAndSlackVar);
  if (!simplexMethod<Maximize>(tableau_, basic, var, minmax)) return false;
  solution = var.head(numObjVar);
  return true;
}

template class LinearProgram<float>;

template class LinearProgram<double>;
} // namespace OR