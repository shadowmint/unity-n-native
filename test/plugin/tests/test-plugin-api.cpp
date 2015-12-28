#include <npp/test/test_suite.h>
#include <plugin/plugin.h>
#include <string>

using npp::test::TestSuite;

DEFINE_TEST(test_foo) {
  ASSERT(plugin_return_int() == 1);
}

class Tests: public TestSuite {
 public:
  Tests() {
    TEST(test_foo)
  }
};

int main(int argc, char **argv) {
  auto tests = npp::make_unique<Tests>();
  return tests->Report();
}
