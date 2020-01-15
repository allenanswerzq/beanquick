#ifndef BEANQUICK_BASE_H_
#define BEANQUICK_BASE_H_

#include <cstdlib>
#include <iostream>
#include <string>

#include "debug.h"

// #define NDEBUG

#ifndef NDEBUG
#define TRACER(...) \
  ::debug::__trace(__LINE__, __func__, #__VA_ARGS__, __VA_ARGS__)
#define DBGER(...) \
  ::debug::__dbg(__LINE__, __func__, #__VA_ARGS__, (__VA_ARGS__))
#else
#define TRACER(...)
#define DBGER(...)
#endif

namespace beanquick {

using std::string;

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

// Compiler attributes
#if (defined(__GNUC__) || defined(__APPLE__)) && !defined(SWIG)
// Compiler supports GCC-style attributes
#define BEAN_ATTRIBUTE_NORETURN __attribute__((noreturn))
#define BEAN_ATTRIBUTE_NOINLINE __attribute__((noinline))
#define BEAN_ATTRIBUTE_UNUSED __attribute__((unused))
#define BEAN_ATTRIBUTE_COLD __attribute__((cold))
#define BEAN_PACKED __attribute__((packed))
#define BEAN_MUST_USE_RESULT __attribute__((warn_unused_result))
#define BEAN_PRINBEAN_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__printf__, string_index, first_to_check)))
#define BEAN_SCANF_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__scanf__, string_index, first_to_check)))

#else
// Non-GCC equivalents
#define BEAN_ATTRIBUTE_NORETURN
#define BEAN_ATTRIBUTE_NOINLINE
#define BEAN_ATTRIBUTE_UNUSED
#define BEAN_ATTRIBUTE_COLD
#define BEAN_MUST_USE_RESULT
#define BEAN_PACKED
#define BEAN_PRINBEAN_ATTRIBUTE(string_index, first_to_check)
#define BEAN_SCANF_ATTRIBUTE(string_index, first_to_check)
#endif

}  // namespace beanquick

#endif  // BEANQUICK_BASE_H_
