#include "decimal.h"

#include "gtest/gtest.h"

namespace beanquick {

TEST(TestDecimal, Ctor) {
  Decimal d("1.23");
  Decimal d1("-1.23");
}

}  // namespace beanquick
