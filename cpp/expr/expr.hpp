#ifndef EXPRESSION_H
#define EXPRESSION_H

// auto只保留类型值，加上decltype会保留const/&/&&等类型修饰
#define EXACT_AUTO decltype(auto)
#define EXPR_FUNC_PREFIX constexpr inline

namespace Expr {
    template<typename L, typename R>
    class Base {
    public:
        EXPR_FUNC_PREFIX Base(const L &lhs, const R &rhs) : lhs_(lhs), rhs_(rhs) {
        }

    protected:
        const L &lhs_;
        const R &rhs_;
    };

    template<typename L, typename R>
    class Add : public Base<L, R> {
    public:
        using Base<L, R>::Base;

        EXPR_FUNC_PREFIX EXACT_AUTO operator[](const size_t i) const {
            return this->lhs_[i] + this->rhs_[i];
        }
    };

    /*
     * c++重载解析优先级：非模板 > 模板
     * c++只会在当前命名空间匹配重载，
     * 除非
     *     1. Expr::operator+(a, b);
     *     2. using namespace Expr;
     *     3. 加数或被加数类型为 Expr::T
     * 所以不用担心会挟持主程序运算符重载
     */
    template<typename L, typename R>
    EXPR_FUNC_PREFIX EXACT_AUTO operator+(const L &lhs, const R &rhs) {
        return Expr::Add<L, R>(lhs, rhs);
    }
}

#endif //EXPRESSION_H
