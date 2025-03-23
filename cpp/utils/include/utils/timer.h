#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace cpp_libs {
    class Timer {
    public:
        using time_t = unsigned long long;

        Timer() : last(std::chrono::steady_clock::now()) {
        }

        void start() {
            last = std::chrono::steady_clock::now();
        }

        void stop() {
            const auto now = std::chrono::steady_clock::now();
            read += std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count();
            last = now;
        }

        void reset() {
            read = 0;
        }

        [[nodiscard]] time_t getReadNs() const {
            return read;
        }

    private:
        time_t read = 0;
        std::chrono::steady_clock::time_point last;
    };
}

#endif //TIMER_H
