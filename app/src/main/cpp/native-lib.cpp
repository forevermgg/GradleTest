#include <jni.h>

#include <string>

#include "connection_change_notifier.hpp"
#include "java_class.hpp"
#include "java_method.hpp"
#include "jni_utils.hpp"

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

extern "C" JNIEXPORT jstring JNICALL
Java_com_test_cmaketest_MainActivity_stringFromJNI(JNIEnv *env,
                                                   jobject /* this */) {
  std::string hello = say_hello();
  return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_test_cmaketest_MainActivity_createListener(JNIEnv *env, jobject thiz) {
  std::map<std::string, std::string> headers;
  headers["1"] = "1";
  jobject j_headers = FOREVER::JNI_UTIL::JniUtils::to_hash_map(env, headers);
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