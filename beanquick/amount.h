#ifndef DEANQUICK_AMOUNT_H_
#define DEANQUICK_AMOUNT_H_

#include <iostream>
#include <regex>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "decimal.h"
#include "logging.h"

namespace beanquick {
inline static bool check_currency(const string &str) {
  std::regex CURRENCY_RE(R"([A-Z][A-Z0-9'._]{0,22}[A-Z0-9])");
  return std::regex_match(str, CURRENCY_RE);
}

class Amount {
 public:
  virtual ~Amount() {}

  Amount(const Decimal &number, const string &currency);

  Amount(const Amount &amount);
  Amount &operator=(const Amount &from);

  Decimal Number() const { return number_; }
  string Currency() const { return currency_; }

  Amount &operator+=(const Decimal &rhs);
  Amount &operator-=(const Decimal &rhs);
  Amount &operator*=(const Decimal &rhs);
  Amount &operator/=(const Decimal &rhs);
  Amount &operator%=(const Decimal &rhs);

  string ToString() const;

  // Create an Amount from a string.
  static Amount FromString(const string &str);

  // Absolutes an Amount.
  static Amount Abs(const Amount &amount);

  // Negatives an Amount.
  friend Amount operator-(const Amount &from);

  friend std::ostream &operator<<(std::ostream &os, const Amount &amount);

  // Compare two amounts, currency_ first, then number_.
  // Once this is defined, std::sort can apply to this class.
  friend bool operator<(const Amount &lhs, const Amount &rhs);

  friend bool operator==(const Amount &lhs, const Amount &rhs);

  friend bool operator!=(const Amount &lhs, const Amount &rhs);

  // Basic math operations between an amount and a decial.
  friend const Amount operator+(const Amount &lhs, const Decimal &rhs);
  friend const Amount operator-(const Amount &lhs, const Decimal &rhs);
  friend const Amount operator*(const Amount &lhs, const Decimal &rhs);
  friend const Amount operator/(const Amount &lhs, const Decimal &rhs);
  friend const Amount operator%(const Amount &lhs, const Decimal &rhs);

  // Add or sub two amounts.
  friend const Amount operator+(const Amount &lhs, const Amount &rhs);
  friend const Amount operator-(const Amount &lhs, const Amount &rhs);

 private:
  Decimal number_;
  string currency_;
};

inline Amount::Amount(const Decimal &number, const string &currency) {
  CHECK(check_currency(currency));
  number_ = number;
  currency_ = currency;
}

inline Amount::Amount(const Amount &amount) { *this = amount; }

inline Amount &Amount::operator=(const Amount &from) {
  if (*this == from) return *this;
  number_ = from.Number();
  currency_ = from.Currency();
  return *this;
}

inline Amount &Amount::operator+=(const Decimal &rhs) {
  number_ += rhs;
  return *this;
}

inline Amount &Amount::operator-=(const Decimal &rhs) {
  number_ -= rhs;
  return *this;
}

inline Amount &Amount::operator*=(const Decimal &rhs) {
  number_ *= rhs;
  return *this;
}

inline Amount &Amount::operator/=(const Decimal &rhs) {
  number_ /= rhs;
  return *this;
}

inline Amount &Amount::operator%=(const Decimal &rhs) {
  number_ %= rhs;
  return *this;
}

inline string Amount::ToString() const {
  string ret;
  ret = absl::StrCat(ret, number_.toString());
  ret = absl::StrCat(ret, " ");
  ret = absl::StrCat(ret, currency_);
  return ret;
}

inline Amount Amount::FromString(const string &str) {
  std::vector<string> ret;
  string tmp = str;
  absl::RemoveExtraAsciiWhitespace(&tmp);
  ret = absl::StrSplit(tmp, " ");
  CHECK_EQ(ret.size(), 2);
  CHECK(check_currency(ret[1]));
  return Amount(Decimal(ret[0]), ret[1]);
}

inline Amount Amount::Abs(const Amount &amount) {
  return amount.Number().isNegative() ? -amount : amount;
}

inline Amount operator-(const Amount &from) {
  return Amount(-from.Number(), from.Currency());
}

inline std::ostream &operator<<(std::ostream &os, const Amount &amount) {
  return os << amount.ToString();
}

inline bool operator<(const Amount &lhs, const Amount &rhs) {
  if (lhs.currency_ != rhs.currency_) {
    return lhs.Currency() < rhs.Currency();
  }
  else {
    return lhs.Number() < rhs.Number();
  }
}

inline bool operator==(const Amount &lhs, const Amount &rhs) {
  bool ret = true;
  ret &= lhs.Number() == rhs.Number();
  ret &= lhs.Currency() == rhs.Currency();
  return ret;
}

inline bool operator!=(const Amount &lhs, const Amount &rhs) {
  return !(lhs == rhs);
}

inline const Amount operator+(const Amount &lhs, const Decimal &rhs) {
  return Amount(lhs) += rhs;
}

inline const Amount operator-(const Amount &lhs, const Decimal &rhs) {
  return Amount(lhs) -= rhs;
}

inline const Amount operator*(const Amount &lhs, const Decimal &rhs) {
  return Amount(lhs) *= rhs;
}

inline const Amount operator/(const Amount &lhs, const Decimal &rhs) {
  return Amount(lhs) /= rhs;
}

inline const Amount operator%(const Amount &lhs, const Decimal &rhs) {
  return Amount(lhs) %= rhs;
}

const Amount operator+(const Amount &lhs, const Amount &rhs) {
  CHECK_EQ(lhs.Currency(), rhs.Currency()) << "different currencies cant add.";
  return Amount(lhs) += rhs.Number();
}

const Amount operator-(const Amount &lhs, const Amount &rhs) {
  CHECK_EQ(lhs.Currency(), rhs.Currency()) << "different currencies cant sub.";
  return Amount(lhs) -= rhs.Number();
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

// NOTE: if two amounts have the same number and currency,
// The hash values shoule be the same, right?
template <>
struct hash<::beanquick::Amount> {
  std::size_t operator()(const ::beanquick::Amount &a) const {
    std::size_t x = hash<string>{}(a.ToString());
    std::size_t y = hash<string>{}(a.Currency());
    return CombineHashes({x, y});
  }
};

}  // namespace std

#endif  // DEANQUICK_AMOUNT_H_
