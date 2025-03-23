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
                std::unique_lock lock(mutex_);
                if (taskQueue_.empty()) {
                    aTaskJoin_.wait(lock, [this]() { return !taskQueue_.empty() || shouldQuit_; });
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

                aTaskOver_.notify_all();
            }
        });
    }
}

void ThreadPool::pushTask(const Task &task) {
    mutex_.lock();
    if (queueSize_ && taskQueue_.size() == queueSize_) {
        taskQueue_.pop();
    } else {
        ++unfinishedTask_;
    }
    taskQueue_.push(task);
    mutex_.unlock();
    aTaskJoin_.notify_one();
}

bool ThreadPool::waitTaskOver(const int ms) {
    std::unique_lock lock(mutex_);
    const auto isOver = [this]() { return unfinishedTask_ == 0; };

    if (isOver())
        return true;

    if (ms == 0)
        return false;

    if (ms > 0) {
        // 作用和wait类似，但多了一个时间限制；
        // 返回 线程被唤醒&&条件为true&&未超时；
        return aTaskOver_.wait_for(lock, std::chrono::milliseconds(ms), isOver);
    }

    aTaskOver_.wait(lock, isOver);
    return true;
}

ThreadPool::~ThreadPool() {
    shouldQuit_ = true;
    aTaskJoin_.notify_all();
    for (auto &t: threads_)
        t.join();
}
