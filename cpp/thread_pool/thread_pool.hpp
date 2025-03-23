#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
// #include <atomic>

class ThreadPool {
    using Task = std::function<void()>;

public:
    explicit ThreadPool(int threadsNumber, int taskQueueSize = 0);

    void pushTask(const Task &task);

    bool waitTaskOver(int ms = -1);

    ~ThreadPool();

private:
    std::vector<std::thread> threads_;
    std::queue<Task> taskQueue_;
    std::mutex mutex_;
    std::condition_variable aTaskJoin_;
    std::condition_variable aTaskOver_;
    int unfinishedTask_;
    int queueSize_;
    bool shouldQuit_;
};

#endif