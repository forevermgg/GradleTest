#include <stdexcept>
#include <string>

namespace FOREVER::C_API {

template <class F>
inline auto wrap_err(F &&f) noexcept -> decltype(f()) {
  try {
    return f();
  } catch (...) {
    set_last_exception(std::current_exception());
    return {};
  };
}

template <class F>
inline auto wrap_err(F &&f, const decltype(f()) &e) noexcept {
  try {
    return f();
  } catch (...) {
    set_last_exception(std::current_exception());
    return e;
  }
}
}  // namespace FOREVER::C_API