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
public:
    using Task = std::function<void()>;

    explicit ThreadPool(int threadsNumber, int taskQueueSize = 0);

    bool isTaskOver() const {
        return unfinishedTask_ == 0;
    }

    bool isQueueFull() const {
        return queueSize_ && taskQueue_.size() == queueSize_;
    }

    // task应该满足：若未被调用，对外界无影响
    void pushTask(const Task &task, bool force = false);

    bool waitTaskOver(int ms = -1);

    // 让主线程运行一个task
    void runTask();

    ~ThreadPool();

private:
    std::vector<std::thread> threads_;
    std::queue<Task> taskQueue_;
    std::mutex mutex_;
    std::condition_variable taskJoin_;
    std::condition_variable taskOver_;
    int unfinishedTask_;
    int queueSize_;
    bool shouldQuit_;
};

#endif