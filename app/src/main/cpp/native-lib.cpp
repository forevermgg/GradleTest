#include <jni.h>

#include <latch>
#include <string>

#include "TestData.h"
#include "binding_context.hpp"
#include "connection_change_notifier.hpp"
#include "java_binding_context.hpp"
#include "java_class.hpp"
#include "java_method.hpp"
#include "jni_utils.hpp"
#include "thread_pool.hpp"
#include "uuid.hpp"
#include "waitable_event.h"

std::string say_hello() {
#ifdef WINDOWS
  return std::string("Hello from Windows!");
#elif LINUX
  return std::string("Hello from Linux!");
#elif MACOS
  return std::string("Hello from macOS!");
#elif Android
  return {"Hello from Android!"};
#else
  return std::string("Hello from an unknown system!");
#endif
}

FOREVER::ConnectionChangeNotifier m_connection_change_notifier_;
std::unique_ptr<FOREVER::BindingContext> m_binding_context_;
std::shared_ptr<TestData> m_test_data_;

TestData m_test_data2_;

extern "C" JNIEXPORT jstring JNICALL
Java_com_test_cmaketest_MainActivity_stringFromJNI(JNIEnv *env,
                                                   jobject /* this */) {
  std::string hello = say_hello() + FOREVER::UTIL::uuid_string();
  return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_test_cmaketest_MainActivity_createListener(JNIEnv *env, jobject) {
  std::map<std::string, std::string> headers;
  headers["1"] = "1";
  jobject j_headers = FOREVER::JNI_UTIL::JniUtils::to_hash_map(env, headers);
  ThreadLatch wait_done;
  ThreadPool pool(1);
  m_test_data_ = std::make_shared<TestData>();
  auto weak_this = m_test_data_->weak_from_this();
  pool.PostTask([&wait_done, weak_this]() mutable {
    auto strong = weak_this.lock();
    if (strong) {
      strong->test = 1;
    }
    wait_done.task_started = true;
    wait_done.notify.Notify();
  });
  wait_done.notify.Wait();
  m_test_data_->TestExec();
  int test = m_test_data_->test;
  int test2 = m_test_data2_.test;
  return reinterpret_cast<jlong>(&m_connection_change_notifier_);
}

static jlong get_connection_value(FOREVER::ConnectionState state) {
  switch (state) {
    case FOREVER::ConnectionState::Disconnected:
      return 0;
    case FOREVER::ConnectionState::Connecting:
      return 1;
    case FOREVER::ConnectionState::Connected:
      return 2;
  }
  return static_cast<jlong>(-1);
}

extern "C" JNIEXPORT void JNICALL
Java_com_test_cmaketest_MainActivity_execListener(JNIEnv *env, jobject thiz,
                                                  jlong ptr) {
  auto test = reinterpret_cast<FOREVER::ConnectionChangeNotifier *>(ptr);
  test->invoke_callbacks(FOREVER::ConnectionState::Connected,
                         FOREVER::ConnectionState::Disconnected);

  if (m_binding_context_) {
    m_binding_context_->before_notify();
  }
}

bool RunOnThread(std::function<bool(void)> closure) {
  bool res = false;
  std::thread thread([&res, &closure] { res = closure(); });
  thread.join();
  return res;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_test_cmaketest_SyncNetWorkState_nativeAddConnectionListener(
    JNIEnv *env, jobject thiz, jlong app_native_pointer) {
  auto test =
      reinterpret_cast<FOREVER::ConnectionChangeNotifier *>(app_native_pointer);
  static FOREVER::JNI_UTIL::JavaClass java_syncmanager_class(
      env, "com/test/cmaketest/SyncNetWorkState");
  static FOREVER::JNI_UTIL::JavaMethod java_notify_connection_listener(
      env, java_syncmanager_class, "notifyConnectionListeners", "(JJ)V");

  auto callback = [session_ref = FOREVER::JNI_UTIL::JavaGlobalRefByCopy(
                       env, thiz)](FOREVER::ConnectionState old_state,
                                   FOREVER::ConnectionState new_state) {
    JNIEnv *local_env = FOREVER::JNI_UTIL::JniUtils::get_env(true);

    jlong old_connection_value = get_connection_value(old_state);
    jlong new_connection_value = get_connection_value(new_state);

    local_env->CallVoidMethod(session_ref.get(),
                              java_notify_connection_listener,
                              old_connection_value, new_connection_value);
    if (local_env->ExceptionCheck()) {
      local_env->ExceptionDescribe();
      throw std::runtime_error(
          "An unexpected Error was thrown from Java. See LogCat");
    }
  };
  // https://github.com/google/perfetto/blob/cb5a63c5a1/src/base/threading/thread_pool_unittest.cc
  ThreadLatch wait_done;
  ThreadPool pool(1);
  pool.PostTask([&wait_done] {
    wait_done.task_started = true;
    wait_done.notify.Notify();
  });
  wait_done.notify.Wait();
  /*std::latch wait_done{1};
  ThreadPool pool(1);
  pool.PostTask([&wait_done] {
      wait_done.count_down();
  });
  wait_done.wait();*/
  uint64_t token = test->add_callback(std::move(callback));
  return static_cast<jlong>(token);
}

extern "C" JNIEXPORT void JNICALL
Java_com_test_cmaketest_SyncNetWorkState_nativeRemoveConnectionListener(
    JNIEnv *env, jclass clazz, jlong app_native_pointer, jlong listener_id) {
  auto test =
      reinterpret_cast<FOREVER::ConnectionChangeNotifier *>(app_native_pointer);
  test->remove_callback(listener_id);
}

extern "C" JNIEXPORT jbyte JNICALL
Java_com_test_cmaketest_SyncNetWorkState_nativeGetConnectionState(
    JNIEnv *env, jclass clazz, jlong app_native_pointer) {}

extern "C" JNIEXPORT void JNICALL
Java_com_test_cmaketest_MainActivity_createJavaBindingContext(
    JNIEnv *env, jobject thiz, jobject forever_notifier) {
  m_binding_context_ =
      FOREVER::_impl::JavaBindingContext::create(env, forever_notifier);
}

extern "C" JNIEXPORT void JNICALL
Java_com_test_cmaketest_MainActivity_destroyJavaBindingContext(JNIEnv *env,
                                                               jobject thiz) {
  if (m_binding_context_) {
    m_binding_context_.release();
    m_binding_context_ = nullptr;
  }
}