#include <npp/test/test_suite.h>
#include <npp/result.h>

using namespace npp;
using namespace npp::test;

Result<int, const char *> test_fixture(int value) {
  if (value >= 0) {
    return Ok<int, const char *>(value + 10);
  }
  return Err<int, const char *>("Invalid request, must be +ve");
}

DEFINE_TEST(test_ok_helper) {
  auto value = Ok<int, int>(100);
  ASSERT(value);
  ASSERT(value.Ok() == 100);
}

DEFINE_TEST(test_err_helper) {
  auto value = Err<int, int>(100);
  ASSERT(!value);
  ASSERT(value.Err() == 100);
}

DEFINE_TEST(test_then_fixture) {
  auto value = test_fixture(100);
  ASSERT(value && value.Ok() == 110);
  value = test_fixture(-10);
  ASSERT(!value);
}

DEFINE_TEST(test_then) {
  test_fixture(100).Then([&] (int v) { ASSERT(v == 110); }, [&] (const char * err) { UNREACHABLE; });
  test_fixture(-10).Then([&] (int v) { UNREACHABLE; }, [&] (const char * err) { ASSERT(err != NULL); });
}

DEFINE_TEST(test_then_as_lambdas) {
  int v0 = 0;
  test_fixture(100).Then([&] (int v) { v0 = 100; }, [&] (const char * err) { UNREACHABLE; });
  ASSERT(v0 == 100);

  int v1 = 0;
  test_fixture(-10).Then([&] (int v) { UNREACHABLE; }, [&] (const char * err) { v1 = 100; });
  ASSERT(v1 == 100);
}

class Tests: public TestSuite {
  public:
    Tests() {
      TEST(test_ok_helper)
      TEST(test_err_helper)
      TEST(test_then_fixture)
      TEST(test_then)
      TEST(test_then_as_lambdas)
    }
};

int main(int argc, char **argv) {
  auto tests = npp::make_unique<Tests>();
  return tests->Report();
}
