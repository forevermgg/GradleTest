#include <jni.h>

#include <iostream>

typedef void (*FreeFunction)(void*);

extern "C" JNIEXPORT void JNICALL
Java_com_test_cmaketest_impl_Managed__1nInvokeFinalizer(JNIEnv* env,
                                                        jclass clazz,
                                                        jlong finalizerPtr,
                                                        jlong ptr) {
  void* instance = reinterpret_cast<void*>(static_cast<uintptr_t>(ptr));
  FreeFunction finalizer =
      reinterpret_cast<FreeFunction>(static_cast<uintptr_t>(finalizerPtr));
  finalizer(instance);
}