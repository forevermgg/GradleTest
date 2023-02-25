#include <jni.h>

#include "app.hpp"
#include "jni_utils.hpp"

static void finalize_client(jlong ptr) {
  delete reinterpret_cast<FOREVER::APP::App *>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_test_cmaketest_OsApp_nativeGetFinalizerMethodPtr(JNIEnv *env,
                                                          jclass clazz) {
  return reinterpret_cast<jlong>(&finalize_client);
}

extern "C" JNIEXPORT jlong JNICALL Java_com_test_cmaketest_OsApp_nativeCreate(
    JNIEnv *env, jobject thiz, jstring j_app_id, jstring j_app_name,
    jstring j_app_version, jstring j_platform, jstring j_platform_version,
    jstring j_sdk_version) {
  auto app_id = FOREVER::JNI_UTIL::JniUtils::JavaStringToString(env, j_app_id);
  auto cached_app = FOREVER::APP::App::get_cached_app(app_id);
  if (cached_app) {
    return reinterpret_cast<jlong>(
        new std::shared_ptr<FOREVER::APP::App>(cached_app));
  }

  // App Config
  auto app_name =
      FOREVER::JNI_UTIL::JniUtils::JavaStringToString(env, j_app_name);
  auto app_version =
      FOREVER::JNI_UTIL::JniUtils::JavaStringToString(env, j_app_version);
  auto platform =
      FOREVER::JNI_UTIL::JniUtils::JavaStringToString(env, j_platform);
  auto platform_version =
      FOREVER::JNI_UTIL::JniUtils::JavaStringToString(env, j_platform_version);
  auto sdk_version =
      FOREVER::JNI_UTIL::JniUtils::JavaStringToString(env, j_sdk_version);

  auto app_config =
      FOREVER::APP::App::Config{app_id,
                                std::optional<std::string>(app_name),
                                std::optional<std::string>(app_version),
                                platform,
                                platform_version,
                                sdk_version};

  FOREVER::APP::SharedApp app = FOREVER::APP::App::get_shared_app(app_config);
  return reinterpret_cast<jlong>(new std::shared_ptr<FOREVER::APP::App>(app));
}