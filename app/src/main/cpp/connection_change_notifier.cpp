//
// Created by centforever on 2023/2/19.
//

#include "connection_change_notifier.hpp"

namespace FOREVER {
const size_t npos = size_t(-1);
const size_t not_found = npos;
uint64_t ConnectionChangeNotifier::add_callback(
    std::function<ConnectionStateChangeCallback> callback) {
  std::lock_guard<std::mutex> lock(m_callback_mutex);
  auto token = m_next_token++;
  m_callbacks.push_back({std::move(callback), token});
  return token;
}

void ConnectionChangeNotifier::remove_callback(uint64_t token) {
  Callback old;
  {
    std::lock_guard<std::mutex> lock(m_callback_mutex);
    auto it = std::find_if(begin(m_callbacks), end(m_callbacks),
                           [=](const auto& c) { return c.token == token; });
    if (it == end(m_callbacks)) {
      return;
    }

    size_t idx = distance(begin(m_callbacks), it);
    if (m_callback_index != npos) {
      if (m_callback_index >= idx) --m_callback_index;
    }
    --m_callback_count;

    old = std::move(*it);
    m_callbacks.erase(it);
  }
}

void ConnectionChangeNotifier::invoke_callbacks(ConnectionState old_state,
                                                ConnectionState new_state) {
  std::unique_lock lock(m_callback_mutex);
  m_callback_count = m_callbacks.size();
  for (++m_callback_index; m_callback_index < m_callback_count;
       ++m_callback_index) {
    // acquire a local reference to the callback so that removing the
    // callback from within it can't result in a dangling pointer
    auto cb = m_callbacks[m_callback_index].fn;
    lock.unlock();
    cb(old_state, new_state);
    lock.lock();
  }
  m_callback_index = npos;
}

ConnectionChangeNotifier::ConnectionChangeNotifier() {
  m_callback_index = -1;
  m_callback_count = -1;
  m_next_token = 0;
}
ConnectionChangeNotifier::~ConnectionChangeNotifier() {
  std::unique_lock lock(m_callback_mutex);
  m_callbacks.clear();
  m_callback_index = -1;
  m_callback_count = -1;
  m_next_token = 0;
}
}  // namespace FOREVER