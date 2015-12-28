#pragma once

#include <functional>

namespace npp {

/// A Option type, like in rust
template<typename TValue>
class Option {
 private:
  /// Currently ok?
  bool valid;

  /// Current value
  TValue value;

 public:
  /// New ok value
  static Option<TValue> Some(TValue value) {
    Option<TValue> rtn;
    rtn.value = value;
    rtn.valid = true;
    return rtn;
  }

  /// New err value
  static Option<TValue>None() {
    Option<TValue> rtn;
    return rtn;
  }

  /// Create a new blank value
  Option() : valid(false), value(TValue()) {}

  /// Overload bool cast
  operator bool() const {
    return this->valid;
  }

  /// Overload * to get value
  TValue operator*() const {
    return this->value;
  }

  /// Overload -> to get value of a pointer
  TValue& operator->() {
    return this->value;
  }

  /// Take the value leaving None
  TValue Take() {
    valid = false;
    return value;
  }

  /// Perform some action on success or error
  bool Then(std::function< void(TValue t)> on_some) {
    if (this->valid) {
      on_some(this->value);
      return true;
    }
    return false;
  }

  /// Get the value or the default value provided
  TValue Or(TValue default_value) {
    return this->valid ? this->value : default_value;
  }
};

/// Overload Option<T> == <T>, etc.
template<typename T>
inline bool operator==(const Option<T>& lhs, const T& rhs) {
  if (lhs) {
    return rhs == *lhs;
  }
  return false;
}
template<typename T>
inline bool operator==(const T& lhs, const Option<T>& rhs) {
  return operator==(rhs, lhs);
}
template<typename T>
inline bool operator!=(const Option<T>& lhs, const T& rhs) {
  return !operator==(lhs, rhs);
}
template<typename T>
inline bool operator!=(const T& lhs, const Option<T>& rhs) {
  return !operator==(rhs, lhs);
}

/// Shortcut to create an Ok value
template<typename TValue> Option<TValue> Some(TValue ok) {
  return Option<TValue>::Some(ok);
}

/// Shortcut to create an Err value
template<typename TValue> Option<TValue> None() {
  return Option<TValue>::None();
}

}  // namespace npp
