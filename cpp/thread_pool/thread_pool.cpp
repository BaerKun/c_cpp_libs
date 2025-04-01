#include "thread_pool.hpp"

/*
 *  互斥锁mutex
    所有线程中只有一个能持有，持有时间从lock->unlock
    其余线程将被阻塞于lock

 *  lock_guard/unique_lock会随初始化和折构而 lock和unlock

 *  wait会释放锁的控制权，直到条件变量condition_variable将其唤醒notify
    此时线程会尝试与其他线程竞争锁的所有权
    获取锁后会检查回调条件，只有条件返回true(期间线程持有锁)，线程继续持有锁，并向下执行
    否则释放锁，继续等待下一次notify，本次notify结束

 *  atomic用于原子操作，功能上相当于封装了lock和unlock，但底层逻辑更快
 */

ThreadPool::ThreadPool(const int threadsNumber, const int taskQueueSize)
    : threads_(threadsNumber), unfinishedTask_(0),
      queueSize_(taskQueueSize), shouldQuit_(false) {
    for (auto &t: threads_) {
        t = std::thread([this]() {
            while (true) {
                std::unique_lock<std::mutex> lock(mutex_);
                if (!shouldQuit_ && taskQueue_.empty()) {
                    taskJoin_.wait(lock, [this]() { return !taskQueue_.empty() || shouldQuit_; });
                }
                if (shouldQuit_)
                    return;

                const Task task = taskQueue_.front();
                taskQueue_.pop();
                lock.unlock();

                task();

                lock.lock();
                --unfinishedTask_;
                lock.unlock();

                taskOver_.notify_all();
            }
        });
    }
}

void ThreadPool::pushTask(const Task &task, const bool force) {
    mutex_.lock();
    if (isQueueFull()) {
        if (force) {
            taskQueue_.pop();
            taskQueue_.push(task);
        }
        mutex_.unlock();
        return;
    }

    taskQueue_.push(task);
    ++unfinishedTask_;

    mutex_.unlock();
    taskJoin_.notify_one();
}

bool ThreadPool::waitTaskOver(const int ms) {
    std::unique_lock<std::mutex> lock(mutex_);

    if (isTaskOver())
        return true;

    if (ms == 0)
        return false;

    if (ms > 0) {
        // 作用和wait类似，但多了一个时间限制；
        // 返回 线程被唤醒&&条件为true&&未超时；
        return taskOver_.wait_for(lock, std::chrono::milliseconds(ms), [this]() { return isTaskOver(); });
    }

    taskOver_.wait(lock, [this]() { return isTaskOver(); });
    return true;
}

void ThreadPool::runTask() {
    mutex_.lock();
    if (taskQueue_.empty() || shouldQuit_) {
        mutex_.unlock();
        return;
    }

    const Task task = taskQueue_.front();
    taskQueue_.pop();
    mutex_.unlock();

    task();

    mutex_.lock();
    --unfinishedTask_;
    mutex_.unlock();

    taskOver_.notify_all();
}

ThreadPool::~ThreadPool() {
    mutex_.lock();
    shouldQuit_ = true;
    mutex_.unlock();

    taskJoin_.notify_all();
    for (auto &t: threads_)
        t.join();
}
