#include <jni.h>

#include <stdexcept>

#include "connection_change_notifier.hpp"

static jlong get_connection_value(FOREVER::ConnectionState state) {
  switch (state) {
    case FOREVER::ConnectionState::Disconnected:
      return 0;
    case FOREVER::ConnectionState::Connecting:
      return 1;
    case FOREVER::ConnectionState::Connected:
      return 2;
  }
  return static_cast<jlong>(-1);
}