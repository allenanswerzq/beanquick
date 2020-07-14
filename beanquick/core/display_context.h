#ifndef DEANQUICK_DISPLAY_CONTEXT_H_
#define DEANQUICK_DISPLAY_CONTEXT_H_

#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "beanquick/core/amount.h"
#include "beanquick/core/logging.h"
#include "gmock/gmock.h"

namespace beanquick {

// -----------------------------------------------------------------------------
// Distribution Definition.
// -----------------------------------------------------------------------------
template <class T>
class Distribution {
 public:
  Distribution() {}

  bool Empty() { return hist_.empty(); }

  void Update(T value) {
    if (Empty()) {
      min_ = max_ = value;
      mode_.first = 0;
    }
    else {
      min_ = std::min(value, min_);
      max_ = std::max(value, max_);
    }
    hist_[value]++;
    int count = hist_[value];
    if (count >= mode_.first) {
      mode_.first = hist_[value];
      mode_.second = value;
    }
  }

  // Returns the std::min value in this distribution.
  T Min() { return min_; };

  // Returns the std::max value in this distribution
  T Max() { return max_; }

  // Returns the the most counted value.
  T Mode() { return mode_.second; }

 private:
  T min_;
  T max_;
  std::unordered_map<T, int> hist_;
  std::pair<int, T> mode_;
};

//
// -----------------------------------------------------------------------------
// CurrencyContext Definition.
//
// -----------------------------------------------------------------------------
enum class DisplayPrecision {
  MOST_COMMON,
  MAXIMUM,
  NONE,
};

enum class DisplayAlignment {
  NATURAL,
  DOT,
  RIGHT,
  NONE,
};

class CurrencyContext {
 public:
  CurrencyContext() {};

  // Use a decial to update this context.
  void Update(const Decimal& decimal) {
    has_sign_ = decimal.HasSign();
    frac_dist_.Update(decimal.Fractional());
    integer_max_ = std::max(decimal.Integer(), integer_max_);
  }

  // Whether this context has signed currency.
  bool HasSign() { return has_sign_; }

  int Integer() { return integer_max_; }

  int Fractional(DisplayPrecision precision) {
    if (frac_dist_.Empty()) {
      return 0;
    }
    if (precision == DisplayPrecision::MOST_COMMON) {
      return frac_dist_.Mode();
    }
    else if (precision == DisplayPrecision::MAXIMUM) {
      return frac_dist_.Max();
    }
    else {
      LOG(FATAL) << "Unknown precision: ";
      return 0;
    }
  }

  void DebugPrint() {
    string fmt = "sign=%d integer_max=%d frac_common=%d frac_max=%d %s %s";
    string example;
    if (has_sign_) {
      absl::StrAppend(&example, "-");
    }
    absl::StrAppend(&example, string(integer_max_, '0'));

    string example_common = example;
    int frac_common = Fractional(DisplayPrecision::MOST_COMMON);
    if (frac_common > 0) {
      absl::StrAppend(&example_common, ".");
      absl::StrAppend(&example_common, string(frac_common, '0'));
    }

    string example_max = example;
    int frac_max = Fractional(DisplayPrecision::MAXIMUM);
    if (frac_max > 0) {
      absl::StrAppend(&example_max, ".");
      absl::StrAppend(&example_max, string(frac_max, '0'));
    }

    printf(fmt.c_str(), has_sign_, integer_max_, frac_dist_.Mode(),
           frac_dist_.Max(), example_common.c_str(), example_max.c_str());
  }

 private:
  // Ture if at least one of the numbers has a negative or explicit plus sign
  // for this currency.
  bool has_sign_ = false;

  // The maximum number of digits for the integer part of this currency.
  int integer_max_ = 1;

  // A frequency distribution of fractionals seen for this currency.
  Distribution<int> frac_dist_;
};

//
// -----------------------------------------------------------------------------
// DisplayContext Definition.
//
// -----------------------------------------------------------------------------
struct DisplayConfig {
  bool noinit = true;
  int reserved = 0;
  int comma_position = 3;
  DisplayAlignment alignment = DisplayAlignment::NATURAL;
  DisplayPrecision precision = DisplayPrecision::MOST_COMMON;
};

class DisplayFormatter;

//
// DisplayContext dcontext;
// DisplayConfig config;
// config.precision = DisplayPrecision::MAXIMUM;
// config.alignment = DisplayAlignment::RIGHT;
// dcontext.Build(config);
//
class DisplayContext {
 public:
  DisplayContext() { ccontexts_[kDefulatCurrency] = CurrencyContext(); }

  const int kDefaultUninitPrecision = 8;

  const int kDefulatNoComma = 0;

  static const std::string kDefulatCurrency;

  static const DisplayConfig kDefaultConfig;

  void SetCommaPosition(int comma_position) {
    comma_position_ = comma_position;
  }

  void Update(const Decimal& decimal) {
    ccontexts_[kDefulatCurrency].Update(decimal);
  }

  void Update(const Amount& amout) {
    ccontexts_[amout.Currency()].Update(amout.Number());
  }

  // string Quantize(string number, );

  DisplayFormatter Build(DisplayConfig config = kDefaultConfig);

 private:
  UMAPSS_PTR build_dot(const DisplayConfig& config);

  UMAPSS_PTR build_right(const DisplayConfig& config);

  UMAPSS_PTR build_natural(const DisplayConfig& config);

  int comma_position_ = kDefulatNoComma;

  std::unordered_map<string, CurrencyContext> ccontexts_;

  std::function<UMAPSS_PTR(const DisplayConfig&)> build_method_;
};

//
// -----------------------------------------------------------------------------
// DisplayFormatter Definition.
// -----------------------------------------------------------------------------
class DisplayFormatter {
 public:
  explicit DisplayFormatter(std::unique_ptr<DisplayConfig> display_config,
                            UMAPSS_PTR fmtstrings) {
    dconfig_ = std::move(display_config);
    fmtstrings_ = std::move(fmtstrings);
  };

  template <class T>
  string Format(const T& number,
              const string& currency = DisplayContext::kDefulatCurrency) {
    if (dconfig_->comma_position == 0) {
      CHECK(fmtstrings_->count(currency)) << "Not find currency: " << currency;
      ::testing::internal::CaptureStdout();
      printf(fmtstrings_->find(currency)->second.c_str(), number);
      return ::testing::internal::GetCapturedStdout();
    }
    else {
      return "TODO";
    }
  }

 private:
  std::unique_ptr<DisplayConfig> dconfig_;
  UMAPSS_PTR fmtstrings_;
};

template <>
string DisplayFormatter::Format(const Decimal& number, const string& currency);

}  // namespace beanquick

#endif  // DEANQUICK_DISPLAY_CONTEXT_H_
