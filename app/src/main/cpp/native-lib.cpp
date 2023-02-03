#include <jni.h>
#include <string>

std::string say_hello() {
#ifdef WINDOWS
    return std::string("Hello from Windows!");
#elif LINUX
    return std::string("Hello from Linux!");
#elif MACOS
    return std::string("Hello from macOS!");
#elif Android
    return std::string("Hello from Android!");
#else
    return std::string("Hello from an unknown system!");
#endif
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_test_cmaketest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = say_hello();
    return env->NewStringUTF(hello.c_str());
}