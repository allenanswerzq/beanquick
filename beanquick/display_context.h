#ifndef DEANQUICK_DISPLAY_CONTEXT_H_
#define DEANQUICK_DISPLAY_CONTEXT_H_

#include "decimal.h"
#include "logging.h"

namespace beanquick {

template <class T>
class Distribution {
 public:
  Distribution() {}

  bool Empty() { return hist_.empty(); }

  void Update(T value) {
    hist_[value]++;
    if (Empty()) {
      min_ = max_ = value;
      mode_.first = 0;
    }
    else {
      min_ = min(value, min_);
      max_ = max(value, max_);
    }
    if (hist_[value] >= mode_.first) {
      mode_.first = hist_[value];
      mode_.second = value;
    }
  }

  // Returns the min value in this distribution.
  T Min() { return min_; };

  // Returns the max value in this distribution
  T Max() { return max_; }

  // Returns the the most counted value.
  T Mode() { return mode_.second; }

 private:
  T min_;
  T max_;
  std::unordere_map<T, int> hist_;
  std::pair<int, T> mode_;
};

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
  CurrencyContext(){};

  void Update(const Decimal& decimal) {
    has_sign_ = decimal.HasSign();
    fractional_dist_.Update(decimal.Fractionals());
    integer_max_ = max(decimal.Integer(), integer_max_);
  }

  int Fractionals(DisplayPrecision precision) {
    if (fractional_dist_.Empty()) {
      return 0;
    }
    if (precision == DisplayPrecision::MOST_COMMON) {
      return fractional_dist_.Mode();
    }
    else if (precision == DisplayPrecision::MAXIMUM) {
      return fractional_dist_.Max();
    }
    else {
      CHECK(false) << "Unknown precision: " << precision;
    }
  }

  // string ToString() {
  //   string fmt = R("sign=%d  integer_max=%d
  //          fractional_common=%d  fractional_max=%d
  //          %d %d");
  //   string example;
  // }

 private:
  // Ture if at least one of the numbers has a negative or explicit plus sign
  // for this currency.
  bool has_sign_ = false;

  // The maximum number of digits for the integer part of this currency.
  int integer_max_ = 1;

  // A frequency distribution of fractionals seen for this currency.
  Distribution<int> fractional_dist_;
};

//
// DisplayContext dcontext;
// DisplayConfig config;
// config.precision = DisplayPrecision::MAXIMUM;
// config.alignment = DisplayAlignment::RIGHT;
// dcontext.Build(config);
//
typedef struct {
  int reserved = 0;
  bool commas = false;
  DisplayAlignment alignment = NATURAL;
  DisplayPrecision precision = MOST_COMMON;
} DisplayConfig;

// Define default display config.
static DisplayConfig DEFAULT_DISPLAY_CONFIG;

class DisplayContext {
 public:
  DisplayContext() = default;

  bool SetCommas(bool commas) { commas_ = commas; }

  void Build(const DisplayConfig& display_config);

 private:
  string build_dot(const DisplayConfig& display_config);

  string build_right(const DisplayConfig& display_config);

  string build_natural(const DisplayConfig& display_config);

  bool commas_ = false;

  std::unordere_map<string, CurrencyContext> ccontexts_;

  std::function<string(const DisplayConfig&)> build_method_;
};

inline void DisplayContext::Build(
    const DisplayConfig& display_config = DEFAULT_DISPLAY_CONFIG) {
  if (!display_config.commas) {
    display_config.commas = commas_;
  }
  if (display_config.alignment == DisplayAlignment::NATURAL) {
    build_method_ = build_natural;
  }
  else if (display_config.alignment == DisplayAlignment::RIGHT) {
    build_method_ = build_right;
  }
  else if (display_config.alignment == DisplayAlignment::DOT) {
    build_method_ = build_dot;
  }
  else {
    CHECK(false) << "Unknown alignment: " << display_config.alignment;
  }
  string fmtsting = build_method_(display_config);
  return DisplayFormatter(this, display_config, fmtsting);
}

class DisplayFormatter : {};

}  // namespace beanquick

#endif  // DEANQUICK_DISPLAY_CONTEXT_H_
