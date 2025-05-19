#ifndef SHARE_HPP
#define SHARE_HPP

namespace OR {
    enum Optim {
        Maximize, Minimize
    };

    enum Comparison {
        LESS_EQUAL = -1, EQUAL = 0, GREATER_EQUAL = 1
    };

    template<typename T>
    constexpr T EPSILON = std::numeric_limits<T>::epsilon() * 100;
}

#endif //SHARE_HPP
