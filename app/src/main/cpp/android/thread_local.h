// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FOREVER_THREAD_LOCAL_H_
#define FOREVER_THREAD_LOCAL_H_

#include <memory>

#include "macros.h"

#define FOREVER_THREAD_LOCAL_PTHREADS \
  FOREVER_OS_MACOSX || FOREVER_OS_LINUX || FOREVER_OS_ANDROID

#if FOREVER_THREAD_LOCAL_PTHREADS
#include <pthread.h>
#endif

namespace FOREVER {

#if FOREVER_THREAD_LOCAL_PTHREADS

#define FOREVER_THREAD_LOCAL static

namespace INTERNAL {

class ThreadLocalPointer {
 public:
  explicit ThreadLocalPointer(void (*destroy)(void*));
  ~ThreadLocalPointer();

  void* get() const;
  void* swap(void* ptr);

 private:
  pthread_key_t key_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(ThreadLocalPointer);
};

}  // namespace INTERNAL

template <typename T>
class ThreadLocalUniquePtr {
 public:
  ThreadLocalUniquePtr() : ptr_(destroy) {}

  T* get() const { return reinterpret_cast<T*>(ptr_.get()); }
  void reset(T* ptr) { destroy(ptr_.swap(ptr)); }

 private:
  static void destroy(void* ptr) { delete reinterpret_cast<T*>(ptr); }

  INTERNAL::ThreadLocalPointer ptr_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(ThreadLocalUniquePtr);
};

#else  // FOREVER_THREAD_LOCAL_PTHREADS

#define FOREVER_THREAD_LOCAL static thread_local

template <typename T>
class ThreadLocalUniquePtr {
 public:
  ThreadLocalUniquePtr() = default;

  T* get() const { return ptr_.get(); }
  void reset(T* ptr) { ptr_.reset(ptr); }

 private:
  std::unique_ptr<T> ptr_;

  FOREVER_DISALLOW_COPY_AND_ASSIGN(ThreadLocalUniquePtr);
};

#endif  // FOREVER_THREAD_LOCAL_PTHREADS

#ifndef FOREVER_THREAD_LOCAL

#error Thread local storage unavailable on the platform.

#endif  // FOREVER_THREAD_LOCAL

}  // namespace FOREVER

#endif  // FOREVER_THREAD_LOCAL_H_
