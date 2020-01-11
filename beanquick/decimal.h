#ifndef BEANQUICK_DECIMAL_H_
#define BEANQUICK_DECIMAL_H_

#include "beanquick.pb.h"
#include "third_party/libfixed/include/fixed/Number.h"

namespace beanquick {

class Decimal {
 public:

  Decimal(const string& decimal);

  Decimal(const Decimal& from);

  inline Decimal& operator=(const Decimal& from) {
    CopyFrom(from);
    return *this;
  }

  Decimal& operator+=(const Decimal& rhs);
  Decimal& operator-=(const Decimal& rhs);
  Decimal& operator*=(const Decimal& rhs);
  Decimal& operator/=(const Decimal& rhs);
  Decimal& operator%=(const Decimal& rhs);

  string ToString() const;

  const DecimalDef& def() const;

 private:
  Number number_;
  DecimalDef decimal_def_;
};

friend bool operator==(const Decimal& lhs, const Decimal& rhs);
friend bool operator<(const Decimal& lhs, const Decimal& rhs);
friend bool operator<=(const Decimal& lhs, const Decimal& rhs);

friend const Decimal operator+(const Decimal& lhs, const Decimal& rhs);
friend const Decimal operator-(const Decimal& lhs, const Decimal& rhs);
friend const Decimal operator*(const Decimal& lhs, const Decimal& rhs);
friend const Decimal operator/(const Decimal& lhs, const Decimal& rhs);
friend const Decimal operator%(const Decimal& lhs, const Decimal& rhs);

friend ostream operator<<(const ostream& os, Decimal& decimal);

//------------------------------------------------------------------------

} // namespace beanquick

#endif // BEANQUICK_DECIMAL_H_
