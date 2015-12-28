#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
#include "os.h"

using std::string;
using std::unique_ptr;

namespace npp {

#if TARGET_WIN
#define snprintf _snprintf
#endif

/// Use printf style formatting to format a string value
template<typename ... Args> string Format(const string& format, Args ... args) {
  size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args ...);
  unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args ...);
  return string(buf.get(), buf.get() + size);
}

/// Inline c-str helper
#define CStr(INSTANCE) INSTANCE.AsStr().c_str()

} // namespace npp
