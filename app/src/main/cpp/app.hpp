#ifndef FOREVER_APP_HPP
#define FOREVER_APP_HPP

#include <functional>
#include <iostream>
#include <optional>
#include <string>

namespace FOREVER {
namespace APP {

class App;

typedef std::shared_ptr<App> SharedApp;

class App : public std::enable_shared_from_this<App> {
 public:
  struct Config {
    // Information about the device where the app is running
    struct DeviceInfo {
      std::string platform;           // json: platform
      std::string platform_version;   // json: platformVersion
      std::string sdk_version;        // json: sdkVersion
      std::string sdk;                // json: sdk
      std::string cpu_arch;           // json: cpuArch
      std::string device_name;        // json: deviceName
      std::string device_version;     // json: deviceVersion
      std::string framework_name;     // json: frameworkName
      std::string framework_version;  // json: frameworkVersion
      // Other parameters provided to server no included here:
      // * CoreVersion - populated by Sync when the device info is sent
    };

    std::string app_id;
    std::optional<std::string> local_app_name;
    std::optional<std::string> local_app_version;
    DeviceInfo device_info;
  };
  App(const Config& config);
  App(App&&) noexcept = default;
  App& operator=(App&&) noexcept = default;
  ~App();

  const Config& config() const { return m_config; }

  static SharedApp get_shared_app(const Config& config);
  static SharedApp get_uncached_app(const Config& config);
  static std::shared_ptr<App> get_cached_app(const std::string& app_id);
  static void clear_cached_apps();

 private:
  Config m_config;
};
}  // namespace APP
}  // namespace FOREVER

#endif /* FOREVER_APP_HPP */
