#pragma once

#include <functional>

namespace npp {
namespace result {

/// Either A or B as a single return value
template<typename TOk, typename TErr>
union Value {

  /// Value for success
  TOk ok;

  /// Value for error
  TErr err;
};

} // namespace result

/// A result type, like in rust
template<typename TOk, typename TErr>
class Result {
 private:
  /// Current value
  result::Value<TOk, TErr> value;

  /// Currently ok?
  bool valid;

 public:
  /// New ok value
  static Result<TOk, TErr> Ok(TOk ok) {
    Result<TOk, TErr> rtn;
    rtn.value.ok = ok;
    rtn.valid = true;
    return rtn;
  }

  /// New err value
  static Result<TOk, TErr>Err(TErr err) {
    Result<TOk, TErr> rtn;
    rtn.value.err = err;
    rtn.valid = false;
    return rtn;
  }

  /// Overload bool cast
  operator bool() {
    return this->valid;
  }

  /// Ok value
  TOk Ok() {
    return this->value.ok;
  }

  /// Err value
  TErr Err() {
    return this->value.err;
  }

  /// Perform some action on success or error
  bool Then(std::function< void(TOk t)> on_ok, std::function< void(TErr err)> on_err) {
    if (this->valid) {
      on_ok(this->value.ok);
      return true;
    }
    on_err(this->value.err);
    return false;
  }
};

/// Shortcut to create an Ok value
template<typename TOk, typename TErr> Result<TOk, TErr> Ok(TOk ok) {
  return Result<TOk, TErr>::Ok(ok);
}

/// Shortcut to create an Err value
template<typename TOk, typename TErr> Result<TOk, TErr> Err(TErr err) {
  return Result<TOk, TErr>::Err(err);
}

} // namespace npp
