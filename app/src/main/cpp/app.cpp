#include "app.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

using namespace FOREVER;
using namespace FOREVER::APP;
namespace {
static std::unordered_map<std::string, std::shared_ptr<App>> s_apps_cache;
std::mutex s_apps_mutex;
}  // anonymous namespace

namespace FOREVER {
namespace APP {
SharedApp App::get_shared_app(const Config& config) {
  std::lock_guard<std::mutex> lock(s_apps_mutex);
  auto& app = s_apps_cache[config.app_id];
  if (!app) {
    app = std::make_shared<App>(config);
  }
  return app;
}

SharedApp App::get_uncached_app(const Config& config) {
  auto app = std::make_shared<App>(config);
  return app;
}

std::shared_ptr<App> App::get_cached_app(const std::string& app_id) {
  std::lock_guard<std::mutex> lock(s_apps_mutex);
  if (auto it = s_apps_cache.find(app_id); it != s_apps_cache.end()) {
    return it->second;
  }
  return nullptr;
}

void App::clear_cached_apps() {
  std::lock_guard<std::mutex> lock(s_apps_mutex);
  s_apps_cache.clear();
}
App::App(const Config& config) : m_config(std::move(config)) {}

App::~App() {}
}  // namespace APP
}  // namespace FOREVER
