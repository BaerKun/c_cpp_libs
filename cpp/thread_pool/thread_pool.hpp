#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
// #include <atomic>

enum class TaskState { NORMAL, REJECTED, MAIN };

using Task = std::function<void(TaskState, int)>;

template<unsigned QueueSize>
class FixedTaskQueue {
public:
    [[nodiscard]] bool empty() const;

    [[nodiscard]] unsigned size() const;

    void push(const Task &task);

    void push(Task &&task);

    const Task &front();

    void pop();

private:
    std::array<Task, QueueSize> queue_;
    unsigned head_{}, tail_{}, size_{};
};

template<typename QueueType = std::queue<Task>>
class ThreadPool {
public:
    // TODO: 优化queueSize
    explicit ThreadPool(unsigned threadsNumber, unsigned queueSize = 0);

    ThreadPool(const ThreadPool &) = delete;

    [[nodiscard]] bool isTaskOver() const {
        return unfinishedTask_ == 0;
    }

    [[nodiscard]] bool isQueueFull() const {
        return queueSize_ && taskQueue_.size() == queueSize_;
    }

    void pushTask(const Task &task, bool force = false);

    void pushTask(Task &&task, bool force = false);

    bool waitTaskOver(int ms = -1);

    // 让主线程运行一个task
    void runTask();

    ~ThreadPool();

private:
    QueueType taskQueue_;
    std::vector<std::thread> threads_;
    std::mutex mutex_;
    std::condition_variable taskJoin_;
    std::condition_variable taskOver_;
    unsigned unfinishedTask_;
    unsigned queueSize_;
    bool shouldQuit_;
};

#include "thread_pool.tpp"
#endif
