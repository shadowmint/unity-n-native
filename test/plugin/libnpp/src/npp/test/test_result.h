#pragma once

#include "test.h"

namespace npp {

  namespace test {

    /// A result type, like in rust
    class TestResult {

      public:

        /// Name of this test
        const char *name;

        /// The test callback
        npp::test::Test handle;

        /// Did the test pass?
        bool success;

        /// Test a test instance
        TestResult(const char *name, Test handler) {
          this->name = name;
          this->handle = handler;
          this->success = false;
        }

        /// Run the test instance
        bool Run() {
          this->success = true;
          this->handle(this->success);
          return this->success;
        }
    };
  }
}
