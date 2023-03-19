#include "thread_pool.hpp"

#include <mutex>
#include <thread>

ThreadPool::ThreadPool(uint32_t thread_count) {
  for (uint32_t i = 0; i < thread_count; ++i) {
    threads_.emplace_back(std::bind(&ThreadPool::RunThreadLoop, this));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    quit_ = true;
  }
  thread_waiter_.notify_all();
  for (auto& thread : threads_) {
    thread.join();
  }
}

void ThreadPool::PostTask(std::function<void()> fn) {
  std::lock_guard<std::mutex> guard(mutex_);
  pending_tasks_.emplace_back(std::move(fn));
  if (thread_waiting_count_ == 0) {
    return;
  }
  thread_waiter_.notify_one();
}

void ThreadPool::RunThreadLoop() {
  for (;;) {
    std::function<void()> fn;
    {
      std::unique_lock<std::mutex> guard(mutex_);
      if (quit_) {
        return;
      }
      if (pending_tasks_.empty()) {
        thread_waiting_count_++;
        thread_waiter_.wait(
            guard, [this]() { return quit_ || !pending_tasks_.empty(); });
        thread_waiting_count_--;
        continue;
      }
      fn = std::move(pending_tasks_.front());
      pending_tasks_.pop_front();
    }
    fn();
  }
}