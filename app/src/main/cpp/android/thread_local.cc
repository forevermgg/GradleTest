// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "thread_local.h"

#if FOREVER_THREAD_LOCAL_PTHREADS

#include <cstring>

namespace FOREVER {
namespace INTERNAL {

ThreadLocalPointer::ThreadLocalPointer(void (*destroy)(void*)) {
  FOREVER_CHECK(pthread_key_create(&key_, destroy) == 0);
}

ThreadLocalPointer::~ThreadLocalPointer() {
  FOREVER_CHECK(pthread_key_delete(key_) == 0);
}

void* ThreadLocalPointer::get() const { return pthread_getspecific(key_); }

void* ThreadLocalPointer::swap(void* ptr) {
  void* old_ptr = get();
  int err = pthread_setspecific(key_, ptr);
  if (err) {
    FOREVER_CHECK(false) << "pthread_setspecific failed (" << err
                         << "): " << strerror(err);
  }
  return old_ptr;
}

}  // namespace INTERNAL
}  // namespace FOREVER

#endif  // FOREVER_THREAD_LOCAL_PTHREADS
