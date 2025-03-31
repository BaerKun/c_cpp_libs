#include "thread_pool.hpp"
#include <iostream>

int main() {
    ThreadPool pool(4, 20);
    long long result[100] = {};

    for (long long i = 0; i < 100; ++i){
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
        ThreadPool::Task task = [i, output = result + i](){
            long long sum = 0;
            for(long long j = i; j < i * 400; ++j){
                sum += j;
            }
            *output = sum;
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        };
        pool.pushTask(task, [task]() {
            task();
        });
    }

    const bool flag = pool.waitTaskOver(10);

    for (int i = 99; i >= 0; --i){
        std::cout << i << ": " << result[i] << std::endl;
    }

    std::cout << (flag ? "true" : "false") << std::endl;
    return 0;
}
