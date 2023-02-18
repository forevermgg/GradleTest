#include <jni.h>

#include "java_class_global_def.hpp"
#include "jni_utils.hpp"

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
  JNIEnv* env;
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  } else {
    FOREVER::JNI_UTIL::JniUtils::initialize(vm, JNI_VERSION_1_6);
    FOREVER::_impl::JavaClassGlobalDef::initialize(env);
  }
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