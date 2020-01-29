#include "display_context.h"

namespace beanquick {

inline std::unordered_map<string, string> DisplayContext<T>::build_natural(
    const DisplayConfig& config) {
  std::unordered_map<string, string> fmtstrings;
  // for (auto& [currency, ccontexts] : ccontexts_) { C++17 feature
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    int frac_num = ccontext.Fractional(config.precision);
    string fmt_str;
    // Construct "%.3f"
    absl::StrAppend(&fmt_str, "%.");
    absl::StrAppend(&fmt_str, std::to_string(frac_num));
    absl::StrAppend(&fmt_str, "f");
    fmtstrings[currency] = fmt_str;
  }
  return fmtstrings;
}

inline std::unordered_map<string, string> DisplayContext<T>::build_right(
    const DisplayConfig& config) {
  std::unordered_map<string, string> fmtstrings;
  int max_width = 0;
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    int curr_num = 0;
    if (ccontext.HasSign()) {
      curr_num += 1;
    }
    int frac_num = ccontext.Fractional(config.precision);
    curr_num += (frac_num > 0 ? 1 : 0);  // period
    curr_num += frac_num;
    int integer_num = ccontext.Integer();
    curr_num += (integer_num - 1) / 3 + integer_num;
    curr_num += config.reserved;  // reserved
    max_width = std::max(max_width, curr_num);
  }
  // Compute the format string
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    int frac_num = ccontext.Fractional(config.precision);
    string fmt_str;
    // Construct "%12.6f"
    absl::StrAppend(&fmt_str, "%");
    absl::StrAppend(&fmt_str, std::to_string(max_width));
    absl::StrAppend(&fmt_str, ".");
    absl::StrAppend(&fmt_str, std::to_string(frac_num));
    absl::StrAppend(&fmt_str, "f");
    fmtstrings[currency] = fmt_str;
  }
  return fmtstrings;
}

inline std::unordered_map<string, string> DisplayContext<T>::build_dot(
    const DisplayConfig& config) {
  int max_sign = 0;
  int max_integer = 0;
  int max_width = 0;
  int max_frac = -1;
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    if (ccontext.HasSign()) {
      max_sign = 1;
    }
    int integer_num = ccontext.Integer();
    if (config.comma_position != kDefulatNoComma) {
      integer_num += (integer_num - 1) / config.comma_position;
    }
    max_integer = std::max(max_integer, integer_num);
    int frac_num = ccontext.Fractional(config.precision);
    max_frac = std::max(max_frac, frac_num + (frac_num > 0));
  }
  if (max_frac == -1) {
    max_frac = kDefaultUninitPrecision;
  }
  max_width = std::max(max_sign + max_integer + max_frac, max_width);
  // Compute the format string for each currency.
  std::unordered_map<string, string> fmtstrings;
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    // Construct "%{max_width}.{max_frac}"
    string fmt_str;
    absl::StrAppend(&fmt_str, "%");
    absl::StrAppend(&fmt_str, std::to_string(max_width));
    absl::StrAppend(&fmt_str, ".");
    absl::StrAppend(&fmt_str, std::to_string(max_frac));
    absl::StrAppend(&fmt_str, "f");
    fmtstrings[currency] = fmt_str;
  }
  return fmtstrings;
}

inline DisplayFormatter DisplayContext::Build(
    const DisplayConfig& display_config = DisplayContext::kDefaultConfig) {
  if (!display_config.comma_position) {
    display_config.comma_position = comma_position_;
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
  return DisplayFormatter(this, &display_config, &fmtsting);
}

}  // namespace beanquick
