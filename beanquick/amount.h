#ifndef DEANQUICK_AMOUNT_H_
#define DEANQUICK_AMOUNT_H_

#include <iostream>
#include <vector>
#include <regex>

#include "decimal.h"
#include "logging.h"
#include "third_party/abseil/absl/strings/str_cat.h"
#include "third_party/abseil/absl/strings/str_split.h"
#include "third_party/abseil/absl/strings/string_view.h"

namespace beanquick {

inline static bool is_currency(const string& str) {
  std::regex CURRENCY_RE(R"([A-Z][A-Z0-9'._]{0,22}[A-Z0-9])");
  return std::regex_match(str, CURRENCY_RE);
}

class Amount {
 public:
  virtual ~Amount();

  Amount(const Decimal& number, const string& currency);

  Amount(const Amount& amount);
  Amount& operator=(const Amount& from);

  Decimal GetNumber() const { return number_; }
  string GetCurrency() const { return currency_; }

  Amount& operator+=(const Amount& rhs);
  Amount& operator-=(const Amount& rhs);
  Amount& operator*=(const Amount& rhs);
  Amount& operator/=(const Amount& rhs);
  Amount& operator%=(const Amount& rhs);

  Amount operator-(const Amount& from);

  string ToString();

  // Create an Amount from a string.
  static Amount FromString(const string& str);

  static Amount Abs(const Amount& amount);

  friend std::ostream& operator<<(std::ostream& os, const Amount& amount);

  // Compare two amounts, currency_ first, then number_.
  // Once this is defined, std::sort can apply to this class.
  friend bool operator<(const Amount& lhs, const Amount& rhs);

  friend bool operator==(const Amount& lhs, const Amount& rhs);

  friend const Amount operator+(const Amount& lhs, const Amount& rhs);
  friend const Amount operator-(const Amount& lhs, const Amount& rhs);
  friend const Amount operator*(const Amount& lhs, const Amount& rhs);
  friend const Amount operator/(const Amount& lhs, const Amount& rhs);

  friend const Amount operator%(const Amount& lhs, const Amount& rhs);

 private:
  Decimal number_;
  string currency_;
};

inline Amount::Amount(const Decimal& number, const string& currency) {
  CHECK(is_currency(currency));
  number_ = number;
  currency_ = currency;
}

inline Amount::Amount(const Amount& amount) { *this = amount; }

inline Amount& Amount::operator=(const Amount& from) {
  if (*this == from) return *this;
  number_ = from.GetNumber();
  currency_ = from.GetCurrency();
  return *this;
}

inline Amount& Amount::operator+=(const Amount& rhs) {
  number_ += rhs.GetNumber();
  return *this;
}

inline Amount& Amount::operator-=(const Amount& rhs) {
  number_ -= rhs.GetNumber();
  return *this;
}

inline Amount& Amount::operator*=(const Amount& rhs) {
  number_ *= rhs.GetNumber();
  return *this;
}

inline Amount& Amount::operator/=(const Amount& rhs) {
  number_ /= rhs.GetNumber();
  return *this;
}

inline Amount& Amount::operator%=(const Amount& rhs) {
  number_ %= rhs.GetNumber();
  return *this;
}

inline Amount Amount::operator-(const Amount& from) {
  // number_ = -from.GetNumber();
  return *this;
}

inline string Amount::ToString() {
  string ret;
  ret = absl::StrCat(ret, number_.toString());
  ret = absl::StrCat(ret, " ");
  ret = absl::StrCat(ret, currency_);
  return ret;
}

inline Amount Amount::FromString(const string& str) {
  std::vector<absl::string_view> results;
  results = absl::StrSplit(str, " ");
  CHECK_EQ(results.size(), 2);
  CHECK(is_currency(string(results[1])));
  Decimal number = Decimal(results[0]);
  string currency = string(results[1]);
  return Amount(number, currency);
}

inline Amount Abs(const Amount& amount) {
  return amount.GetNumber().isNegative() ? -number_ : number_;
}

inline std::ostream& operator<<(std::ostream& os, const Amount& amount) {
  return os << amount.ToString();
}

inline bool operator<(const Amount& lhs, const Amount& rhs) {
  if (lhs.currency_ != rhs.currency_) {
    return lhs.GetCurrency() < rhs.GetCurrency();
  } else {
    return lhs.GetNumber() < rhs.GetNumber();
  }
}

inline bool operator==(const Amount& lhs, const Amount& rhs) {
  bool ret = true;
  ret &= lhs.GetNumber() == rhs.GetNumber();
  ret &= lhs.GetCurrency() == rhs.GetCurrency();
  return ret;
}

inline Amount operator+(const Amount& lhs, const Amount& rhs) {
  return lhs += rhs;
}

inline Amount operator-(const Amount& lhs, const Amount& rhs) {
  return lhs -= rhs;
}

inline Amount operator*(const Amount& lhs, const Amount& rhs) {
  return lhs *= rhs;
}

inline Amount operator/(const Amount& lhs, const Amount& rhs) {
  return lhs /= rhs;
}

inline Amount operator%(const Amount& lhs, const Amount& rhs) {
  return lhs %= rhs;
}

}  // namespace beanquick

namespace std {

// Hash combiner used by TensorFlow core.
// See tensorflow/lite/util.cc:
size_t CombineHashes(std::initializer_list<size_t> hashes) {
  size_t result = 0;
  for (size_t hash : hashes) {
    result = result ^
             (hash + 0x9e3779b97f4a7800ULL + (result << 10) + (result >> 4));
  }
  return result;
}

template <>
struct hash<Amount> {
  std::size_t operator()(const Amount& amount) {
    return CombineHashes(hash<Decimal>(amount.number_),
                         hash<string>(amount.currency_));
  }
};

}  // namespace std

#endif  // DEANQUICK_AMOUNT_H_
