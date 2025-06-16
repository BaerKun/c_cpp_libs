#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>
#include <queue>
#include <vector>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

/*
 * 保证单线程调用都是正确的
 * 多线程时，只保证对线程池的操作 (pushTask/runTask) 正确，
 *         不保证对线程池的观测 (allTaskOver/isQueueFull/waitTaskOver) 正确
 */
class ThreadPool {
  // 要实现所有操作的原子性，考虑使用 公平锁，及所有权先到先得
  // 并将 unfinishedTasks_ 放入锁

public:
  enum class TaskStatus { NORMAL, REJECTED, MAIN };
  using Task = std::function<void(TaskStatus, int)>;

  explicit ThreadPool(int threadsNumber, int taskQueueSize = 0);

  [[nodiscard]] bool allTaskOver() const { return unfinishedTasks_ == 0; }

  [[nodiscard]] bool isQueueFull() const {
    return queueSize_ && taskQueue_.size() == queueSize_;
  }

  void pushTask(const Task &task, const bool force = false) {
    pushTask_(Task(task), force);
  }

  void pushTask(Task &&task, const bool force = false) {
    pushTask_(std::move(task), force);
  }

  bool waitTaskOver(int ms = -1);

  // 让主线程运行一个task
  void runTask();

  ~ThreadPool();

private:
  void pushTask_(Task &&task, bool force);

  std::vector<std::thread> threads_;
  std::queue<Task> taskQueue_;

  std::mutex mutex_;
  std::condition_variable taskJoin_;
  std::condition_variable taskOver_;

  std::atomic<int> unfinishedTasks_;
  const int queueSize_;
  bool shouldQuit_;
};

#endif
