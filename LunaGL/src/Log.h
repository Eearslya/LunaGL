#pragma once

#include <spdlog/spdlog.h>
//
#include <spdlog/fmt/ostr.h>

#define LOGF spdlog::critical
#define LOGE spdlog::error
#define LOGW spdlog::warn
#define LOGI spdlog::info
#define LOGD spdlog::debug
#define LOGT spdlog::trace

#define ASSERT(x, ...)                            \
  {                                               \
    if (!(x)) {                                   \
      LOGE("Assertion Failed: {0}", __VA_ARGS__); \
      __debugbreak();                             \
    }                                             \
  }