#ifndef BEANQUICK_DECIMAL_H_
#define BEANQUICK_DECIMAL_H_

#include <iostream>
#include <string>

#include "base.h"
#include "absl/strings/str_replace.h"
#include "fixed/Number.h"

namespace beanquick {

class Decimal : public ::fixed::Number {
 public:
  using Base = ::fixed::Number;

  Decimal() : Base() {}

  Decimal(const string &str) : Base(helper(str)) {}

  // TODO(zq7): add string validation.
  string helper(const string &str) {
    // Add Ctor to handle "100,013.034" kind of string
    string tmp = str;
    return absl::StrReplaceAll(tmp, {{",", ""}});
  }

  friend Decimal operator-(const Decimal &from);
};

inline Decimal operator-(const Decimal &from) {
  Decimal ret = from;
  ret.negate();
  return ret;
}

}  // namespace beanquick

#endif  // BEANQUICK_DECIMAL_H_
