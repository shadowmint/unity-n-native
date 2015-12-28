#pragma once

#include <vector>
#include <npp/unique.h>
#include "test_result.h"

namespace npp {

  namespace test {

    /// A result type, like in rust
    class TestSuite {

      private:

        /// The list of test functions to run
        std::vector<std::unique_ptr<TestResult>> tests;

      protected:

        /// Add a new a test
        void Test(const char *name, Test target) {
          this->tests.push_back(std::move(npp::make_unique<TestResult>(name, target)));
        }

      public:

        /// Run the test suite and return a count of failures
        int Report() {
          auto failures = 0;
          auto count = 0;
          for (auto i = 0; i < this->tests.size(); ++i) {
            TestResult *t = this->tests.at(i).get();
            count += 1;
            printf("exec: %s ", t->name);
            if (t->Run()) {
              printf("\033[32mOK\033[0m\n");
            }
            else {
              failures += 1;
              printf("\033[31;1mFAIL\033[0m\n");
            }
          }
          if (failures > 0) {
            printf("\033[31;1mFAILED (%d/%d passed)\033[0m\n", count - failures, count);
          }
          else {
            printf("SUCCESS\n");
          }
          return failures;
        }
    };
  }
}
