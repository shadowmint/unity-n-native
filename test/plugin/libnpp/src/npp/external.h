#pragma once

#include <npp/os.h>

#if TARGET_WIN
  #define EXTERN_C extern "C" __declspec(dllexport)
#else
  #define EXTERN_C extern "C"
#endif
