#ifndef BINDING_CONTEXT_HPP
#define BINDING_CONTEXT_HPP

#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace FOREVER {
// A simple implementation which lets the user register functions to be
// called on refresh could look like the following:
//
// class BindingContextImplementation : public BindingContext {
// public:
//     // A token returned from add_notification that can be used to remove the
//     // notification later
//     struct token : private std::list<util::UniqueFunction<void ()>>::iterator
//     {
//         token(std::list<util::UniqueFunction<void ()>>::iterator it)
//         : std::list<util::UniqueFunction<void ()>>::iterator(it) { }
//         friend class DelegateImplementation;
//     };
//
//     token add_notification(util::UniqueFunction<void ()> func)
//     {
//         m_registered_notifications.push_back(std::move(func));
//         return token(std::prev(m_registered_notifications.end()));
//     }
//
//     void remove_notification(token entry)
//     {
//         m_registered_notifications.erase(entry);
//     }
//
//     // Override the did_change method to call each registered notification
//     void did_change(std::vector<ObserverState> const&, std::vector<void*>
//     const&, bool) override
//     {
//         // Loop oddly so that unregistering a notification from within the
//         // registered function works
//         for (auto it = m_registered_notifications.begin(); it !=
//         m_registered_notifications.end(); ) {
//             (*it++)();
//         }
//     }
//
// private:
//     std::list<util::UniqueFunction<void ()>> m_registered_notifications;
// };
class BindingContext {
 public:
  virtual ~BindingContext() = default;
  virtual void will_send_notifications() {}
  virtual void did_send_notifications() {}
  virtual void before_notify() {}
  virtual void changes_available() {}
  virtual void will_change(std::vector<void*> const& invalidated);
  virtual void did_change(std::vector<void*> const& invalidated,
                          bool version_changed = true);
};

inline void BindingContext::will_change(std::vector<void*> const&) {}
inline void BindingContext::did_change(std::vector<void*> const&, bool) {}
}  // namespace FOREVER

#endif /* BINDING_CONTEXT_HPP */
