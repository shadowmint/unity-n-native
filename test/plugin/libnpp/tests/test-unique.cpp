#include <npp/test/test_suite.h>
#include <npp/result.h>
#include <npp/types.h>
#include <npp/unique.h>

using namespace npp;
using namespace npp::test;

class Foo {
  public:
    u32 x;
    Foo(u32 x) : x(x) {}
    virtual u32 doit(u32 v) {
      return x + v;
    }
};

class Bar : public Foo {
  public:
    Bar(u32 x) : Foo(x) {}
    u32 doit(u32 v) override {
      return x - v;
    }
};

template<typename T, typename... Args>
std::unique_ptr<Foo> test_factory(Args&&... args) {
  return make_unique<T>(std::forward<Args>(args)...);
}

DEFINE_TEST(test_make_unique) {
  auto f = make_unique<Foo>(100);
  ASSERT(f->doit(1) == 101);
}

DEFINE_TEST(test_make_unique_from_derived) {
  auto b = make_unique<Bar>(100);
  ASSERT(b->doit(1) == 99);

  std::unique_ptr<Foo> c = make_unique<Bar>(100);
  ASSERT(c->doit(1) == 99);

  auto d = test_factory<Bar>(100);
  ASSERT(d->doit(1) == 99);
}

class Tests: public TestSuite {
  public:
    Tests() {
      TEST(test_make_unique)
      TEST(test_make_unique_from_derived)
    }
};

int main(int argc, char **argv) {
  auto tests = npp::make_unique<Tests>();
  return tests->Report();
}
