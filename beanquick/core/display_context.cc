#include "beanquick/core/display_context.h"

#include "absl/functional/bind_front.h"

namespace beanquick {

UMAPSS_PTR DisplayContext::build_natural(std::unique_ptr<DisplayConfig> config) {
  auto fmtstrings = absl::make_unique<UMAPSS>();
  // for (auto& [currency, ccontexts] : ccontexts_) { C++17 feature
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    int frac_num = ccontext.Fractional(config->precision);
    string fmt_str;
    // Construct "%.3f"
    absl::StrAppend(&fmt_str, "%.");
    absl::StrAppend(&fmt_str, std::to_string(frac_num));
    absl::StrAppend(&fmt_str, "f");
    fmtstrings->insert({currency, fmt_str});
  }
  return fmtstrings;
}

UMAPSS_PTR DisplayContext::build_right(std::unique_ptr<DisplayConfig> config) {
  auto fmtstrings = absl::make_unique<UMAPSS>();
  int max_width = 0;
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    int curr_num = 0;
    if (ccontext.HasSign()) {
      curr_num += 1;
    }
    int frac_num = ccontext.Fractional(config->precision);
    curr_num += (frac_num > 0 ? 1 : 0);  // period
    curr_num += frac_num;
    int integer_num = ccontext.Integer();
    curr_num += (integer_num - 1) / 3 + integer_num;
    curr_num += config->reserved;  // reserved
    max_width = std::max(max_width, curr_num);
  }
  // Compute the format string
  for (auto& it : ccontexts_) {
    string currency = it.first;
    CurrencyContext ccontext = it.second;
    int frac_num = ccontext.Fractional(config->precision);
    string fmt_str;
    // Construct "%12.6f"
    absl::StrAppend(&fmt_str, "%");
    absl::StrAppend(&fmt_str, std::to_string(max_width));
    absl::StrAppend(&fmt_str, ".");
    absl::StrAppend(&fmt_str, std::to_string(frac_num));
    absl::StrAppend(&fmt_str, "f");
    fmtstrings->insert({currency, fmt_str});
  }
  return fmtstrings;
}

UMAPSS_PTR DisplayContext::build_dot(std::unique_ptr<DisplayConfig> config) {
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
    if (config->comma_position != kDefulatNoComma) {
      integer_num += (integer_num - 1) / config->comma_position;
    }
    max_integer = std::max(max_integer, integer_num);
    int frac_num = ccontext.Fractional(config->precision);
    max_frac = std::max(max_frac, frac_num + (frac_num > 0));
  }
  if (max_frac == -1) {
    max_frac = kDefaultUninitPrecision;
  }
  max_width = std::max(max_sign + max_integer + max_frac, max_width);
  // Compute the format string for each currency.
  auto fmtstrings = absl::make_unique<UMAPSS>();
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
    fmtstrings->insert({currency, fmt_str});
  }
  return fmtstrings;
}

inline DisplayFormatter DisplayContext::Build(DisplayConfig config) {
  if (!config.comma_position) {
    config.comma_position = comma_position_;
  }
  if (config.alignment == DisplayAlignment::NATURAL) {
    build_method_ =
        absl::bind_front(&DisplayContext::build_natural, this);
  }
  else if (config.alignment == DisplayAlignment::RIGHT) {
    build_method_ = absl::bind_front(&DisplayContext::build_right, this);
  }
  else if (config.alignment == DisplayAlignment::DOT) {
    build_method_ = absl::bind_front(&DisplayContext::build_dot, this);
  }
  else {
    LOG(FATAL) << "Unknown alignment: " << int(config.alignment);
  }
  // TODO(zq7): looks ugly I would say.
  auto umass_ptr = build_method_(absl::make_unique<DisplayConfig>(config));
  return DisplayFormatter(absl::make_unique<DisplayConfig>(config),
                          std::move(umass_ptr));
}

template <>
string DisplayFormatter::Format(const Decimal& number, const string& currency) {
  if (dconfig_->comma_position == 0) {
    CHECK(fmtstrings_->count(currency)) << "Not find currency: " << currency;
    ::testing::internal::CaptureStdout();
    printf(fmtstrings_->find(currency)->second.c_str(), number);
    return ::testing::internal::GetCapturedStdout();
  }
  else {
    // Print with comma_position
    return "TODO";
  }
}

//
// -----------------------------------------------------------------------------
// Default stuff Definition.
// -----------------------------------------------------------------------------

// Define default display config.
const DisplayConfig DisplayContext::kDefaultConfig = DisplayConfig();

const string DisplayContext::kDefulatCurrency = "DEFAULT_CURRENTY";

}  // namespace beanquick
