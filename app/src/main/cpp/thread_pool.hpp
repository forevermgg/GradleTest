#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  // Initializes this thread_pool |thread_count| threads.
  explicit ThreadPool(uint32_t thread_count);
  ~ThreadPool();

  // Submits a task for execution by any thread in this thread pool.
  //
  // This task should not block for IO as this can cause starvation.
  void PostTask(std::function<void()>);

 private:
  void RunThreadLoop();

  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  // Start of mutex protected members.
  std::mutex mutex_;
  std::list<std::function<void()>> pending_tasks_;
  std::condition_variable thread_waiter_;
  uint32_t thread_waiting_count_ = 0;
  bool quit_ = false;
  // End of mutex protected members.

  std::vector<std::thread> threads_;
};
