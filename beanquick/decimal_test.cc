#include "decimal.h"

#include "gtest/gtest.h"

namespace beanquick {
#define D Decimal

TEST(TestDecimal, Ctor) {
  // Construct with a String.
  Decimal d("1.238");

  // Construct with a Decimal obj.
  Decimal d1(d);
  EXPECT_EQ(d1, d);

  // Assign
  Decimal d2 = d;
  EXPECT_EQ(d2, d);

  Decimal d3("100,034.12");

  std::ostringstream oss;
  oss << d.toString();
  EXPECT_EQ(oss.str(), "1.238");
}

TEST(TestDecimal, CountDecimalNumber) {
  Decimal d("111.222");
  EXPECT_FALSE(d.HasSign());
  EXPECT_EQ(d.Integer(), 3);
  EXPECT_EQ(d.Fractional(), 3);

  Decimal d1("+1231.234567");
  EXPECT_TRUE(d1.HasSign());
  EXPECT_EQ(d1.Integer(), 4);
  EXPECT_EQ(d1.Fractional(), 6);

  Decimal d2("-1.234567");
  EXPECT_TRUE(d2.HasSign());
  EXPECT_EQ(d2.Integer(), 1);
  EXPECT_EQ(d2.Fractional(), 6);
}

TEST(TestDecimal, Operation) {
  Decimal d("1.0");
  Decimal d1("1.0");
  Decimal d2("2.01");
  Decimal d3("2.00");
  // Equal
  EXPECT_EQ(d1, d);
  // Greater
  EXPECT_GE(d2, d);
  // Less
  EXPECT_LE(d3, d2);

  // Add
  EXPECT_EQ(d + d1, D("2.0"));
  // Sub
  EXPECT_EQ(d2 - d3, D("0.01"));
  // MUL
  EXPECT_EQ(d2 * d3, D("4.02"));
}

#undef D

}  // namespace beanquick
