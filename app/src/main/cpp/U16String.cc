#include <jni.h>

#include <string>
#include <vector>

static void deleteU16String(std::vector<jchar>* instance) { delete instance; }

extern "C" JNIEXPORT jlong JNICALL Java_com_test_cmaketest_U16String__1nMake(
    JNIEnv* env, jclass clazz, jstring str) {
  jsize len = env->GetStringLength(str);
  std::vector<jchar>* instance = new std::vector<jchar>(len);
  env->GetStringRegion(str, 0, len, instance->data());
  return reinterpret_cast<jlong>(instance);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_test_cmaketest_U16String__1nGetFinalizer(JNIEnv* env, jclass clazz) {
  // std::vector<jchar>* text = reinterpret_cast<std::vector<jchar>*>(static_cast<uintptr_t>(textPtr));
  return static_cast<jlong>(reinterpret_cast<uintptr_t>(&deleteU16String));
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_test_cmaketest_U16String__1nToString(JNIEnv* env, jclass clazz,
                                              jlong ptr) {
  std::vector<jchar>* instance =
      reinterpret_cast<std::vector<jchar>*>(static_cast<uintptr_t>(ptr));
  return env->NewString(instance->data(), instance->size());
}