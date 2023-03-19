#include <condition_variable>
#include <mutex>

// A waitable event for cross-thread synchronization.
// All methods on this class can be called from any thread.
class WaitableEvent {
 public:
  WaitableEvent();
  ~WaitableEvent();
  WaitableEvent(const WaitableEvent&) = delete;
  WaitableEvent operator=(const WaitableEvent&) = delete;

  // Synchronously block until the event is notified.
  void Wait();

  // Signal the event, waking up blocked waiters.
  void Notify();

 private:
  std::mutex mutex_;
  std::condition_variable event_;
  bool notified_ = false;
};

struct ThreadLatch {
  WaitableEvent notify;
  WaitableEvent wait;
  bool task_started = false;
};