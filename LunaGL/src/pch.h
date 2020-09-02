#pragma once

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "Log.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t U8;

typedef int64_t I64;
typedef int32_t I32;
typedef int16_t I16;
typedef int8_t I8;

typedef double F64;
typedef float F32;

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}