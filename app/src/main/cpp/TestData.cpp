//
// Created by centforever on 2023/3/19.
//

#include "TestData.h"

#include "thread_pool.hpp"
#include "waitable_event.h"

TestData::TestData() {}

TestData::~TestData() {}

void TestData::TestExec() {
  ThreadLatch wait_done;
  ThreadPool pool(1);
  auto weak_this = weak_from_this();
  pool.PostTask([&wait_done, weak_this]() mutable {
    auto strong = weak_this.lock();
    if (strong) {
      strong->test = strong->test + 2;
    }
    wait_done.task_started = true;
    wait_done.notify.Notify();
  });
  wait_done.notify.Wait();
}