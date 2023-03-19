#include "waitable_event.h"

WaitableEvent::WaitableEvent() = default;
WaitableEvent::~WaitableEvent() = default;

void WaitableEvent::Wait() {
  std::unique_lock<std::mutex> lock(mutex_);
  return event_.wait(lock, [this] { return notified_; });
}

void WaitableEvent::Notify() {
  std::unique_lock<std::mutex> lock(mutex_);
  notified_ = true;
  event_.notify_all();
}