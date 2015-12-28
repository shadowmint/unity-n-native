#pragma once

#include "result.h"
#include "option.h"
#include "types.h"
#include <vector>

namespace npp {

/// A generic iterator wrapper
/// This may seem awkward, but it provides a simple consistent
/// api to access the contents of ANY iterator, regardless of
/// call semantics.
template<typename TIn, typename TOut>
class Iterator {
  private:

    /// The actual source element generating an iterator
    TIn src;

    /// Return a value from the inner iterator
    /// You can use this to manually iterate over the content.
    std::function< void(
      TIn input,
      std::function< bool(TOut value)> each
    )> iter;

  public:

    /// Create a new wrapping iterator
    static Iterator<TIn, TOut> From(TIn input, std::function< void(
      TIn input,
      std::function< bool(TOut value)> handler
    ) > iter) {
      Iterator<TIn, TOut> rtn;
      rtn.src = input;
      rtn.iter = iter;
      return rtn;
    }

    /// Callback on elements until false is returned.
    void Iter(std::function< bool(TOut value)> each) {
      if (src != nullptr) {
        iter(src, each);
      }
    }

    /// Return an iterator using the matcher
    Iterator<std::function< bool(TOut value)>, TOut> Find(std::function< bool(TOut value)> matcher) {
      return Iterator<std::function<bool(TOut)>, TOut>::From(matcher, [&] (std::function<bool(TOut)> test, std::function<bool(TOut value)> handler) {
        this->Iter([&] (TOut value) -> bool {
          if (test(value)) {
            return handler(value);
          }
          return true;
        });
      });
    }

    /// Handle each value
    void Each(std::function< void(TOut value)> handler) {
      Iter([&] (TOut value) -> bool {
        handler(value);
        return true;
      });
    }

    /// Return the first matching item, or None
    Option<TOut> First() {
      Option<TOut> rtn = None<TOut>();
      Iter([&] (TOut value) -> bool {
        rtn = Some<TOut>(value);
        return false;
      });
      return rtn;
    }

    /// Return the count of items by iterating over them
    i32 Count() {
      i32 count = 0;
      Iter([&] (TOut value) -> bool {
        ++count;
        return true;
      });
      return count;
    }

    /// Return true if any matching items
    bool Any() {
      bool rtn = false;
      First().Then([&] (TOut out) { rtn = true; });
      return rtn;
    }

    /// Return true if any matching items
    bool Any(std::function< bool(TOut value) > matcher) {
      return Find(matcher).Any();
    }

    /// Return a list of all the values in the array
    std::vector<TOut> Collect() {
      std::vector<TOut> rtn;
      Iter([&] (TOut value) -> bool {
        rtn.push_back(value);
        return true;
      });
      return rtn;
    }
};

/// Shortcut to create an interator, using a pointer that must not be NULL
template<typename TIn, typename TOut>
Iterator<TIn*, TOut> iter(TIn *src, std::function< void( TIn *input, std::function< bool(TOut value)> each )> iter) {
  return Iterator<TIn*, TOut>::From(src, iter);
}

/// Shortcut to create an interator, using a value
template<typename TIn, typename TOut>
Iterator<TIn, TOut> iterv(TIn src, std::function< void(
  TIn input,
  std::function< bool(TOut value)> each
)> iter) {
  return Iterator<TIn, TOut>::From(src, iter);
}

} // namespace npp
