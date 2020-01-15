#include "amount.h"

#include <map>
#include <unordered_set>
#include <vector>

#include "decimal.h"
#include "gtest/gtest.h"

namespace beanquick {

#define D Decimal

TEST(TestAmount, Ctor) {
  Amount a1(D("1.23"), "RMB");
  Amount a2(a2);
  Amount a3(D("100,034.027456"), "USD");
}

TEST(TestAmount, FromString) {
  Amount a1(D("100"), "USD");
  Amount a2 = Amount::FromString("100 USD");
  EXPECT_EQ(a1, a2);

  Amount a3(D("0.00000001"), "BTC");
  Amount a4 = Amount::FromString("0.00000001 BTC");
  EXPECT_EQ(a3, a4);

  Amount::FromString("   100.00 USD    ");

  EXPECT_DEATH({ Amount::FromString("100"); }, "failed");

  EXPECT_DEATH({ Amount::FromString("USD"); }, "failed");

  // Creates with a non-valid currency.
  EXPECT_DEATH({ Amount::FromString("100.00 U"); }, "failed");
}

TEST(TestAmount, ToString) {
  Amount a1(D("100034.023"), "USD");
  EXPECT_EQ("100034.023 USD", a1.ToString());
  std::ostringstream oss;
  oss << a1;
  EXPECT_EQ("100034.023 USD", oss.str());

  Amount a2(D("0.00000001"), "BTC");
  EXPECT_EQ("0.00000001 BTC", a2.ToString());
}

TEST(TestAmount, Comparisons) {
  Amount a1(D("100"), "USD");
  Amount a2(D("100"), "USD");
  EXPECT_EQ(a1, a2);

  Amount a3(D("101"), "USD");
  EXPECT_NE(a1, a3);
}

TEST(TestAmount, Hash) {
  Amount a = Amount(D("100,034.027456"), "USD");
  std::map<Amount, int> mp;
  mp.insert({a, 1});
  std::unordered_set<Amount> st;
  st.insert(a);
  // Same number but with different currency.
  Amount a2 = Amount(D("100,034.027456"), "CAD");
  mp.insert({a2, 1});
  st.insert(a2);
  EXPECT_EQ(mp.size(), 2);
  EXPECT_EQ(st.size(), 2);
}

TEST(TestAmount, Sort) {
  std::vector<Amount> amounts = {
      Amount(D("1"), "USD"),
      Amount(D("201"), "EUR"),
      Amount(D("3"), "USD"),
      Amount(D("100"), "CAD"),
      Amount(D("2"), "USD"),
      Amount(D("200"), "EUR"),
  };
  sort(amounts.begin(), amounts.end());
  std::vector<Amount> right = {
      Amount(D("100"), "CAD"),
      Amount(D("200"), "EUR"),
      Amount(D("201"), "EUR"),
      Amount(D("1"), "USD"),
      Amount(D("2"), "USD"),
      Amount(D("3"), "USD"),
  };
  EXPECT_EQ(right, amounts);
}

TEST(TestAmount, Negative) {
  Amount a1 = Amount(D("100"), "RMB");
  EXPECT_EQ(Amount(D("-100"), "RMB"), -a1);

  Amount a2 = Amount(D("-100"), "RMB");
  EXPECT_EQ(Amount(D("100"), "RMB"), -a2);

  Amount a3 = Amount(D("0"), "RMB");
  EXPECT_EQ(Amount(D("0"), "RMB"), -a3);
}

TEST(TestAmount, Mult) {
  Amount a1 = Amount(D("100"), "RMB");
  EXPECT_EQ(Amount(D("102.1"), "RMB"), a1 * D("1.021"));
}

TEST(TestAmount, Div) {
  Amount a1 = Amount(D("100"), "RMB");
  EXPECT_EQ(Amount(D("20"), "RMB"), a1 / D("5"));
}

TEST(TestAmount, Add) {
  EXPECT_EQ(Amount(D("117.02"), "RMB"),
            Amount(D("100"), "RMB") + Amount(D("17.02"), "RMB"));

  // Different currenies cant add
  EXPECT_DEATH({
    Amount(D("100"), "RMB") + Amount(D("17.02"), "CAD");
  }, "failed");
}

TEST(TestAmount, Sub) {
  EXPECT_EQ(Amount(D("82.98"), "RMB"),
            Amount(D("100"), "RMB") - Amount(D("17.02"), "RMB"));

  // Different currenies cant add
  EXPECT_DEATH({
    Amount(D("100"), "RMB") - Amount(D("17.02"), "CAD");
  }, "failed");
}

TEST(TestAmount, Abs) {
  EXPECT_EQ(Amount(D("82.98"), "RMB"), Amount::Abs(Amount(D("82.98"), "RMB")));
  EXPECT_EQ(Amount(D("0"), "RMB"), Amount::Abs(Amount(D("0"), "RMB")));
  EXPECT_EQ(Amount(D("82.98"), "RMB"), Amount::Abs(Amount(D("-82.98"), "RMB")));
}

#undef D

}  // namespace beanquick
