//
// Created by centforever on 2023/2/19.
//

#ifndef FOREVER_CONNECTION_CHANGE_NOTIFIER_HPP
#define FOREVER_CONNECTION_CHANGE_NOTIFIER_HPP

#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace FOREVER {

enum class ConnectionState {
  Disconnected,
  Connecting,
  Connected,
};
using ConnectionStateChangeCallback = void(ConnectionState old_state,
                                           ConnectionState new_state);
class ConnectionChangeNotifier {
 public:
  uint64_t add_callback(std::function<ConnectionStateChangeCallback> callback);
  void remove_callback(uint64_t token);
  void invoke_callbacks(ConnectionState old_state, ConnectionState new_state);

  ConnectionChangeNotifier();
  ~ConnectionChangeNotifier();

 private:
  struct Callback {
    std::function<ConnectionStateChangeCallback> fn;
    uint64_t token;
  };

  std::mutex m_callback_mutex;
  std::vector<Callback> m_callbacks;

  size_t m_callback_index = -1;
  size_t m_callback_count = -1;
  uint64_t m_next_token = 0;
};

}  // namespace FOREVER

#endif  // FOREVER_CONNECTION_CHANGE_NOTIFIER_HPP
