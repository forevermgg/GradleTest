#include "java_binding_context.hpp"

#include "java_class_global_def.hpp"
#include "java_method.hpp"

using namespace FOREVER;
using namespace FOREVER::_impl;
using namespace FOREVER::JNI_UTIL;

void JavaBindingContext::before_notify() {
  if (JniUtils::get_env()->ExceptionCheck()) {
    return;
  }
  if (m_java_notifier) {
    m_java_notifier.call_with_local_ref([&](JNIEnv* env, jobject notifier_obj) {
      // Method IDs from RealmNotifier implementation. Cache them as member
      // vars.
      static JavaMethod notify_by_other_method(
          env, JavaClassGlobalDef::forever_notifier(), "beforeNotify", "()V");
      env->CallVoidMethod(notifier_obj, notify_by_other_method);
    });
  }
}

void JavaBindingContext::did_change(std::vector<void*> const&,
                                    bool version_changed) {
  auto env = JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return;
  }
  if (version_changed) {
    m_java_notifier.call_with_local_ref(
        env, [&](JNIEnv*, jobject notifier_obj) {
          static JavaMethod forever_notifier_did_change_method(
              env, JavaClassGlobalDef::forever_notifier(), "didChange", "()V");
          env->CallVoidMethod(notifier_obj, forever_notifier_did_change_method);
        });
  }
}

void JavaBindingContext::set_forever_changed_callback(
    JNIEnv* env, jobject forever_changed_callback) {
  m_forever_changed_callback = JavaGlobalWeakRef(env, forever_changed_callback);
}

void JavaBindingContext::will_send_notifications() {
  auto env = JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return;
  }
  m_java_notifier.call_with_local_ref(env, [&](JNIEnv*, jobject notifier_obj) {
    static JavaMethod forever_notifier_will_send_notifications(
        env, JavaClassGlobalDef::forever_notifier(), "willSendNotifications",
        "()V");
    env->CallVoidMethod(notifier_obj, forever_notifier_will_send_notifications);
  });
}

void JavaBindingContext::did_send_notifications() {
  auto env = JniUtils::get_env();
  if (env->ExceptionCheck()) {
    return;
  }
  m_java_notifier.call_with_local_ref(env, [&](JNIEnv*, jobject notifier_obj) {
    static JavaMethod forever_notifier_did_send_notifications(
        env, JavaClassGlobalDef::forever_notifier(), "didSendNotifications",
        "()V");
    env->CallVoidMethod(notifier_obj, forever_notifier_did_send_notifications);
  });
}
