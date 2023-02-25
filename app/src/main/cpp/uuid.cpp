#include "uuid.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <functional>
#include <random>

namespace {

// Seed `engine` with as much random state as it requires, based on the approach
// outlined in P0205R0.
// <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0205r0.html>
template <typename T>
T create_and_seed_engine() {
  constexpr auto bytes_needed = T::state_size * sizeof(typename T::result_type);

  constexpr auto numbers_needed =
      sizeof(std::random_device::result_type) <
              sizeof(std::seed_seq::result_type)
          ? (bytes_needed / sizeof(std::random_device::result_type))
          : (bytes_needed / sizeof(std::seed_seq::result_type));

  std::array<std::random_device::result_type, numbers_needed> state;
  std::random_device rd;
  std::generate(begin(state), end(state), std::ref(rd));
  std::seed_seq seeds(begin(state), end(state));

  T engine;
  engine.seed(seeds);
  return engine;
}

}  // unnamed namespace

namespace FOREVER {
namespace UTIL {

std::string uuid_string() {
  static auto engine = create_and_seed_engine<std::mt19937>();

  std::array<uint8_t, 16> uuid_bytes;
  std::uniform_int_distribution<unsigned int> distribution(
      0, std::numeric_limits<uint8_t>::max());
  std::generate(begin(uuid_bytes), end(uuid_bytes),
                [&] { return distribution(engine); });

  // Version 4 UUID.
  uuid_bytes[6] = (uuid_bytes[6] & 0x0f) | 0x40;
  // IETF variant.
  uuid_bytes[8] = (uuid_bytes[8] & 0x3f) | 0x80;

  std::array<char, 37> uuid_formatted;
  snprintf(
      uuid_formatted.data(), uuid_formatted.size(),
      "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
      uuid_bytes[0], uuid_bytes[1], uuid_bytes[2], uuid_bytes[3], uuid_bytes[4],
      uuid_bytes[5], uuid_bytes[6], uuid_bytes[7], uuid_bytes[8], uuid_bytes[9],
      uuid_bytes[10], uuid_bytes[11], uuid_bytes[12], uuid_bytes[13],
      uuid_bytes[14], uuid_bytes[15]);

  return std::string(uuid_formatted.data(), uuid_formatted.size() - 1);
}

}  // namespace UTIL
}  // namespace FOREVER
