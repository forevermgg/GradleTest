#ifndef JAVA_BINDING_CONTEXT_HPP
#define JAVA_BINDING_CONTEXT_HPP

#include <jni.h>

#include <memory>

#include "binding_context.hpp"
#include "java_global_weak_ref.hpp"

namespace FOREVER {

namespace _impl {
// Binding context which will be called from OS.
class JavaBindingContext final : public BindingContext {
 private:
  struct ConcreteJavaBindContext {
    JNIEnv* jni_env;
    jobject java_notifier;
  };

  // Weak global refs to the needed Java objects.
  // Java should hold a strong ref to them as long as the SharedFOREVER lives
  JNI_UTIL::JavaGlobalWeakRef m_java_notifier;
  JNI_UTIL::JavaGlobalWeakRef m_forever_changed_callback;

 public:
  virtual ~JavaBindingContext(){};
  void before_notify() override;
  void did_change(std::vector<void*> const& invalidated,
                  bool version_changed = true) override;
  void will_send_notifications() override;
  void did_send_notifications() override;
  explicit JavaBindingContext(const ConcreteJavaBindContext& concrete_context)
      : m_java_notifier(concrete_context.jni_env,
                        concrete_context.java_notifier),
        m_forever_changed_callback() {}
  JavaBindingContext(const JavaBindingContext&) = delete;
  JavaBindingContext& operator=(const JavaBindingContext&) = delete;
  JavaBindingContext(JavaBindingContext&&) = delete;
  JavaBindingContext& operator=(JavaBindingContext&&) = delete;

  void set_forever_changed_callback(JNIEnv* env,
                                    jobject forever_changed_callback);

  static inline std::unique_ptr<JavaBindingContext> create(JNIEnv* env,
                                                           jobject notifier) {
    return std::make_unique<JavaBindingContext>(
        ConcreteJavaBindContext{env, notifier});
  };
};

}  // namespace _impl

}  // namespace FOREVER

#endif
