//
// Created by centforever on 2023/3/19.
//

#ifndef GRADLE_TEST_TASK_RUNNER_H
#define GRADLE_TEST_TASK_RUNNER_H

#include <stdint.h>

#include <functional>

#include "platform_handle.h"

class TaskRunner {
 public:
  virtual ~TaskRunner();

  // Schedule a task for immediate execution. Immediate tasks are always
  // executed in the order they are posted. Can be called from any thread.
  virtual void PostTask(std::function<void()>) = 0;

  // Schedule a task for execution after |delay_ms|. Note that there is no
  // strict ordering guarantee between immediate and delayed tasks. Can be
  // called from any thread.
  virtual void PostDelayedTask(std::function<void()>, uint32_t delay_ms) = 0;

  // Schedule a task to run when the handle becomes readable. The same handle
  // can only be monitored by one function. Note that this function only needs
  // to be implemented on platforms where the built-in ipc framework is used.
  // Can be called from any thread.
  // TODO(skyostil): Refactor this out of the shared interface.
  virtual void AddFileDescriptorWatch(PlatformHandle,
                                      std::function<void()>) = 0;

  // Remove a previously scheduled watch for the handle. If this is run on the
  // target thread of this TaskRunner, guarantees that the task registered to
  // this handle will not be executed after this function call.
  // Can be called from any thread.
  virtual void RemoveFileDescriptorWatch(PlatformHandle) = 0;

  // Checks if the current thread is the same thread where the TaskRunner's task
  // run. This allows single threaded task runners (like the ones used in
  // perfetto) to inform the caller that anything posted will run on the same
  // thread/sequence. This can allow some callers to skip PostTask and instead
  // directly execute the code. Can be called from any thread.
  virtual bool RunsTasksOnCurrentThread() const = 0;
};

#endif  // GRADLE_TEST_TASK_RUNNER_H
