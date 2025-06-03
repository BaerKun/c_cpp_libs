#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>
#include <queue>
#include <vector>

#include <condition_variable>
#include <mutex>
#include <thread>
// #include <atomic>

class ThreadPool {
public:
  enum class TaskStatus { NORMAL, REJECTED, MAIN };
  using Task = std::function<void(TaskStatus, int)>;

  explicit ThreadPool(int threadsNumber, int taskQueueSize = 0);

  [[nodiscard]] bool allTaskOver() const { return unfinishedTask_ == 0; }

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
