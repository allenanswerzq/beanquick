#ifndef BEANQUICK_DECIMAL_H_
#define BEANQUICK_DECIMAL_H_

#include <iostream>
#include <string>

#include "absl/strings/str_replace.h"
#include "base.h"
#include "fixed/Number.h"

namespace beanquick {
class Decimal : public ::fixed::Number {
 public:
  using Base = ::fixed::Number;

  Decimal() : Base() {}

  Decimal(const string &str) : Base(helper(str)) {}

  // TODO(zq7): add string validation.
  string helper(const string &str) {
    string tmp = str;
    // Remove commas sign from input, for case like "100,013.034".
    absl::StrReplaceAll(tmp, {{",", ""}});
    has_sign_ = (tmp[0] == '+' || tmp[0] == '-');
    std::size_t pos = tmp.find('.');
    if (pos != std::npos) {
      integer_count_ = pos - (has_sign_ ? 1 : 0);
      fractional_count_ = tmp.size() - pos - (has_sign_ ? 1 : 0);
    }
  }

  bool HasSign() { return has_sign_; }

  int Fractionals() { return fractional_count; }

  int Integer() { return integer_count_; }

  friend Decimal operator-(const Decimal &from);

 private:
  bool has_sign_ = false;
  int integer_count_ = 0;
  int fractional_count = 0;
};

inline Decimal operator-(const Decimal &from) {
  Decimal ret = from;
  ret.negate();
  return ret;
}

}  // namespace beanquick

#endif  // BEANQUICK_DECIMAL_H_
