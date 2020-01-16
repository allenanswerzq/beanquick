#include "display_context.h"

#include "absl/strings/str_split.h"
#include "amount.h"
#include "gtest/gtest.h"

namespace beanquick {

#define D Decimal
#define A Amount

std::vector<A> amountlize(std::vector<string> number) {
  std::vector<A> ret;
  for (auto v : number) {
    ret.push_back(A(v));
  }
  return ret;
}

class DisplayContextTest : public ::testing::Test {
 protected:
  typedef struct BuildArgs {
    DisplayPrecision precision;
    bool noinit = false;
    bool commas = false;
    int reserved = 0;
  };

  void AssertFormatNumbers(std::vector<string> number_strings&,
                           std::vector<string>& expected_fmt_number,
                           BuildArg& build_args = build_args_) {
    DisplayContext dcontext;
    numbers = amountlize(number_strings);
    if (!build_args.noinit) {
      for (auto number : numbers) {
      }
    }
  }

  BuildArg build_args_;
  DisplayAlignment alignment_ = DisplayAlignment::NONE;
}

// clang-format off

TEST_P(TestNaturalUninitialized, DisplayContextTest) {
  alignment_ = DisplayAlignment::NAUTRAL;
  AssertFormatNumbers({"1.2345", "764", "-7409.01", "0.00000125"},
                      {"1.2345", "764", "-7409.01", "0.00000125"});
}

TEST_P(TestNaturalNoClearMode, DisplayContextTest) {
  AssertFormatNumbers(
      {"1.2345", "764", "-7409.01", "0.00000125"},
      {"1.23450000", "764.00000000", "-7409.01000000", "0.00000125"});
}

TEST_P(TestNaturalClearMode, DisplayContextTest) {
  AssertFormatNumbers({"1.2345", "1.23", "234.26", "38.019"},
                           {"1.23", "1.23", "234.26", "38.02"});
}


TEST_P(TestNaturalMaximum, DisplayContextTest) {
  build_args_.precision = DisplayPrecision::MAXIMUM;
  AssertFormatNumbers({"1.2345", "1.23", "234.26", "38.019"},
                           {"1.2345", "1.2300", "234.2600", "38.0190"});
}

TEST_P(TestNaturalCommas, DisplayContextTest) {
  build_args_.commas = true;
  AssertFormatNumbers({"1.2345", "1.23", "234.26", "38.019"},
                           {"1.2345", "1.2300", "234.2600", "38.0190"});
}

TEST_P(TestNaturalReserved, DisplayContextTest) {
  build_args_.reserved = 10;
  AssertFormatNumbers({"1.2345", "1.23", "234.26", "38.019"},
                           {"1.23", "1.23", "234.26", "38.02"});
}

class DisplayContextAlignRightTest : public  DisplayContextTest {
 protected:
  this->alignment_ = DisplayAlignment::Right;
};

TEST_P(TestRightUnlitialized, DisplayContextAlignRightTest) {
  build_args_.noinit = true;
  AssertFormatNumbers(
      {"1.2345", "764", "-7409.01", "0.00000125"},
      {"1.2345",
       "764",
       "-7409.01",
       "0.00000125"});
}

TEST_P(TestRightSign, DisplayContextAlignRightTest) {
  AssertFormatNumbers(
      {"7409.01", "0.1"},
      {"7409.01",
       "   0.10"});

  AssertFormatNumbers(
      {"-7409.01", "0.1"},
      {"-7409.01",
       "    0.10"});
}

TEST_P(TestRightInteger, DisplayContextAlignRightTest) {
  AssertFormatNumbers(
      {"1", "20", "300", "4000", "50000"},
      {"    1",
       "   20",
       "  300",
       " 4000",
       "50000"});

  build_args_.precision = DisplayPrecision::MAXIMUM;
  AssertFormatNumbers(
      {"1", "20", "300", "4000", "50000", "0.001"},
      {"    1.000",
       "   20.000",
       "  300.000",
       " 4000.000",
       "50000.000",
       "    0.001",});
}

TEST_P(TestRightInteger, DisplayContextAlignRightTest) {
  build_args_.commas = true;
  alignment_ = DisplayAlignment::Right;
  AssertFormatNumbers(
      {"1", "20", "300", "4000", "50000"},
      {"     1",
       "    20",
       "   300",
       " 4,000",
       "50,000"});
}

TEST_P(TestRightFractional, DisplayContextAlignRightTest) {
  alignment_ = DisplayAlignment::Right;
  AssertFormatNumbers(
      {"4000", "0.01", "0.02", "0.0002"},
      {"4000.00",
       "   0.01",
       "   0.02",
       "   0.00"});
}

TEST_P(TestRightFractionalCommas, DisplayContextAlignRightTest) {
  build_args_.commas = ture;
  AssertFormatNumbers(
      {"4000", "0.01", "0.02", "0.0002"},
      {"4,000.00",
       "    0.01",
       "    0.02",
       "    0.00"});
}

class DisplayContextAlignDotTest : public  DisplayContextTest {
 protected:
  this->alignment_ = DisplayAlignment::Dot;
};

TEST_P(TestDotUninitialized, DisplayContextAlignDotTest) {
  build_args_.noinit = true;
  AssertFormatNumbers(
      {"1.2345", "764", "-7409.01", "0.00000125"},
      {"1.23450000",
       "764.00000000",
       "-7409.01000000",
       "0.00000125"});
}

TEST_P(TestDotBasic, DisplayContextAlignDotTest) {
  AssertFormatNumbers(
      {"1.2345", "764", "-7409.01", "0.00", "0.00000125"},
      {"    1.23", "  764.00", "-7409.01", "    0.00", "    0.00"});
}

TEST_P(TestDotBasicMulti, DisplayContextAlignDotTest) {
  AssertFormatNumbers(
      {{"1.2345", "USD"},
       {"764", "CAD"},
       {"-7409.01", "EUR"},
       {"0.00", "XAU"},
       {"0.00000125", "RBFF"}},
      {"    1.2345    ",
       "  764         ",
       "-7409.01      ",
       "    0.00      ",
       "    0.00000125"});
};

TEST_P(TestDotSign, DisplayContextAlignDotTest) {
  AssertFormatNumbers(
      {{"7409.01", "USD"}, "0.1"},
      {"7409.01",
       "   0.1 "});

  AssertFormatNumbers(
      {("-7409.01", "USD"), "0.1"},
      {"-7409.01",
       "    0.1 "});
};

TEST_P(TestDotInteger, DisplayContextAlignDotTest) {
  AssertFormatNumbers(
      {"1", "20", "300", "4000", "50000"},
      {"    1",
       "   20",
       "  300",
       " 4000",
       "50000"});

  this->build_args_.precision = DisplayPrecision::MAXIMUM;
  AssertFormatNumbers(
      {"1", "20", "300", "4000", "50000", "0.001", {"0.1", "USD"}},
      {"    1.000",
       "   20.000",
       "  300.000",
       " 4000.000",
       "50000.000",
       "    0.001",
       "    0.1  ",});
};

TEST_P(TestDotIntegerCommas, DisplayContextAlignDotTest) {
  this->build_args_.commas = true;
  AssertFormatNumbers(
      {"1", "20", "300", "4000", "50000"},
      {"     1",
       "    20",
       "   300",
       " 4,000",
       "50,000"});
};

TEST_P(TestDotFractional, DisplayContextAlignDotTest) {
    AssertFormatNumbers(
        {{"4000", "USD"}, "0.01", "0.02", "0.0002"},
        {"4000   ",
         "   0.01",
         "   0.02",
         "   0.00"});
};

TEST_P(TestDotFractionalCommas, DisplayContextAlignDotTest) {
  this->build_args_.commas = true;
  AssertFormatNumbers(
      {{"4000", "USD"}, "0.01", "0.02", "0.0002"},
      {"4,000   ",
       "    0.01",
       "    0.02",
       "    0.00"});
};

// clang-format on

TEST(TestQuantizeBasic) {
  DisplayContext dcontext;
  dcontext.update(Decimal("1.23"), "USD");
  EXPECT_EQ(Decimal("3.23"), dcontext.quantize(Decimal("3.23253343"), "USD"));

  dcontext.update(Decimal("1.2301"), "USD");
  dcontext.update(Decimal("1.2302"), "USD");
  EXPECT_EQ(Decimal("3.2325"), dcontext.quantize(Decimal("3.23253343"), "USD"));
};

#undef D
#undef A

}  // namespace beanquick
