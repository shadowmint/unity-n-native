#include <npp/test/test_suite.h>
#include <npp/iterator.h>
#include <npp/types.h>

using namespace std;
using namespace npp;
using namespace npp::test;

vector<i32> test_fixture() {
  vector<i32> vec;
  for (i32 i = 0; i < 25; ++i) {
    vec.push_back(100+i);
  }
  return vec;
}

Iterator<vector<i32>*, i32> test_iterator_factory(vector<i32> *vec) {
  return iter<vector<i32>, i32>(vec, [] (vector<i32> *vec, std::function< bool(i32 value) > handler) {
    for (auto v = vec->begin(); v != vec->end(); ++v) {
      if (!handler(*v)) {
        break;
      }
    }
  });
}

Iterator<vector<i32>*, i32> fail_iterator_factory(vector<i32> *vec) {
  return iter<vector<i32>, i32>(nullptr, [] (vector<i32> *vec, std::function< bool(i32 value) > handler) {
    for (auto v = vec->begin(); v != vec->end(); ++v) {
      if (!handler(*v)) {
        break;
      }
    }
  });
}

Iterator<vector<i32>*, i32> empty_iterator_factory(vector<i32> *vec) {
  return iter<vector<i32>, i32>(vec, [] (vector<i32> *vec, std::function< bool(i32 value) > handler) {});
}

DEFINE_TEST(test_simple_iterator) {
  auto vec = test_fixture();
  auto item = test_iterator_factory(&vec);
  i32 count = 0;
  item.Each([&success, &count] (i32 value) { ++count; });
  ASSERT(count == 25);
}

DEFINE_TEST(test_fail_iterator) {
  auto vec = test_fixture();
  auto item = fail_iterator_factory(&vec);
  ASSERT(item.Count() == 0);
}

DEFINE_TEST(test_first) {
  auto vec = test_fixture();
  auto item = test_iterator_factory(&vec);
  auto value = item.First();
  ASSERT(value);
  ASSERT(*value == 100);
}

DEFINE_TEST(test_count) {
  auto vec = test_fixture();
  auto item = test_iterator_factory(&vec);
  auto value = item.Count();
  ASSERT(value == 25);
}

DEFINE_TEST(test_any) {
  auto vec = test_fixture();
  auto valid = test_iterator_factory(&vec);
  auto not_valid = empty_iterator_factory(&vec);
  ASSERT(valid.Any());
  ASSERT(!not_valid.Any());
}

DEFINE_TEST(test_collect) {
  auto vec = test_fixture();
  auto valid = test_iterator_factory(&vec);
  auto all = valid.Collect();
  ASSERT(all.size() == 25);
}

DEFINE_TEST(test_find) {
  auto vec = test_fixture();
  auto valid = test_iterator_factory(&vec);
  auto query = valid.Find([] (i32 value) -> bool {
    return value == 110 || value == 115;
  });
  ASSERT(query.Count() == 2);
  ASSERT(query.Any([] (i32 value) { return value == 110; }));
  ASSERT(query.Any([] (i32 value) { return value == 115; }));
  ASSERT(!query.Any([] (i32 value) { return value == 1; }));
}

class Tests: public TestSuite {
  public:
    Tests() {
      TEST(test_simple_iterator)
      TEST(test_fail_iterator)
      TEST(test_first)
      TEST(test_count)
      TEST(test_any)
      TEST(test_collect)
      TEST(test_find)
    }
};

i32 main(i32 argc, char **argv) {
  auto tests = npp::make_unique<Tests>();
  return tests->Report();
}
