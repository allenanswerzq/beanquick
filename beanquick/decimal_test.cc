#include "decimal.h"

#include "gtest/gtest.h"

namespace beanquick {

#define D Decimal

TEST(TestDecimal, Ctor) {
  // Construct with a String.
  Decimal d("1.23");

  // Construct with a Decimal obj.
  Decimal d1(d);
  EXPECT_EQ(d1, d);

  // Assign
  Decimal d2 = d;
  EXPECT_EQ(d2, d);

  // Decimal d3();
  // EXPECT_EQ(d3, D("0"));

  std::ostringstream oss;
  oss << d.toString();
  EXPECT_EQ(oss.str(), "1.23");
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
