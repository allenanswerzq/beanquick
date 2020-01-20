#ifndef BEANQUICK_DECIMAL_H_
#define BEANQUICK_DECIMAL_H_

#include <cassert>
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

  // TODO(zq7): add string validation and checks.
  Decimal(const string &str) : Base(helper(str)) {
    string tmp = helper(str);
    if (tmp[0] == '+' || tmp[0] == '-') {
      has_sign_ = true;
    }
    else {
      has_sign_ = false;
    }

    std::size_t pos = tmp.find('.');
    int one = (has_sign_ ? 1 : 0);
    if (pos != std::string::npos) {
      integer_count_ = pos - one;
      frac_count_ = tmp.size() - (pos + 1);
    }
    else {
      integer_count_ = tmp.size() - one;
      frac_count_ = 0;
    }
  }

  string helper(const string &str) {
    string tmp = str;
    tmp = absl::StrReplaceAll(tmp, {{",", ""}});
    return tmp;
  }

  bool HasSign() const { return has_sign_; }

  int Fractional() const { return frac_count_; }

  int Integer() const { return integer_count_; }

  friend Decimal operator-(const Decimal &from);

 private:
  bool has_sign_;
  int integer_count_;
  int frac_count_;
};

inline Decimal operator-(const Decimal &from) {
  Decimal ret = from;
  ret.negate();
  return ret;
}

}  // namespace beanquick

#endif  // BEANQUICK_DECIMAL_H_
