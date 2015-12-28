#pragma once

#include <stdio.h>
#include <vector>
#include <functional>

namespace npp {

  namespace test {

    /// A unit test static function type
    using Test = std::function< void(bool&) >;

    /// Trace some value, using printf like syntax: TRACE("%d", 100);
    #define TRACE(...) \
      printf("\n: "); \
      printf(__VA_ARGS__); \
      fflush(stdout);

    /// Define a test value
    #define DEFINE_TEST(name) void name(bool &success)

    /// Add a test to the current test scope by name
    #define TEST(name) this->Test(#name, &name);

    /// Asset some truth value
    #define ASSERT(truth) \
      if (!(truth)) { \
        TRACE("%s:%d ", __FILE__, __LINE__); \
        success = false; \
        return; \
      }

    /// Assert a floating point value is close to a value, because floats
    /// don't have exact matching values.
    #define ASSERT_NEAR(value, target, interval) \
      if (!((value >= (target - interval)) && (value <= (target + interval)))) { \
        TRACE("%f is not withing bounded range %f of target %f", value, interval, target); \
        TRACE("%s:%d ", __FILE__, __LINE__); \
        success = false; \
        return; \
      }

    /// Assert that two pointers have the same pointer address
    #define ASSERT_EQUIV(p1, p2) \
      if (((long) p1) != ((long) p2)) { \
        TRACE("Pointer not address equivalent, 0x%lx != 0x%lx", (long) p1, (long) p2); \
        TRACE("%s:%d ", __FILE__, __LINE__); \
        success = false; \
        return; \
      }

    /// Unreachable code, for tests
    #define UNREACHABLE ASSERT(false)
  }
}
