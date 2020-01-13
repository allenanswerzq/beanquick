#ifndef DEANQUICK_AMOUNT_H_
#define DEANQUICK_AMOUNT_H_

#include <iostream>
#include "decimal.h"

namespace beanquick {

class Amount {
 private:
  Decimal number_;
  string currency_;
}

}  // namespace beanquick

#endif  // DEANQUICK_AMOUNT_H_
