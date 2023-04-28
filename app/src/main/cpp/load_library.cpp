#include <jni.h>

#include "java_class_global_def.hpp"
#include "jni_utils.hpp"
#include "android/log_settings.h"
#include "android/jni_util.h"
#include "android/logging.h"

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
  FOREVER::LogSettings log_settings;
  log_settings.min_log_level = FOREVER::LOG_INFO;
  FOREVER::SetLogSettings(log_settings);

  // Initialize the Java VM.
  FOREVER::JNI::InitJavaVM(vm);

  JNIEnv* env = FOREVER::JNI::AttachCurrentThread();
  bool result = false;

   FOREVER_CHECK(result);

  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  } else {
    FOREVER::JNI_UTIL::JniUtils::initialize(vm, JNI_VERSION_1_6);
    FOREVER::_impl::JavaClassGlobalDef::initialize(env);
  }
  result = true;
  FOREVER_CHECK(result);
  return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNI_OnUnload(JavaVM* vm, void*) {
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    return;
  } else {
    FOREVER::_impl::JavaClassGlobalDef::release();
    FOREVER::JNI_UTIL::JniUtils::release();
  }
}