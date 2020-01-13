#ifndef BEANQUICK_DECIMAL_H_
#define BEANQUICK_DECIMAL_H_

#include <iostream>

#include "third_party/libfixed/include/fixed/Number.h"

namespace beanquick {

class Decimal : public ::fixed::Number {
 public:
  using ::fixed::Number::Number;
};

}  // namespace beanquick

#endif  // BEANQUICK_DECIMAL_H_
