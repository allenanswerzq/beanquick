
# Add the include directory for this lib.
list(APPEND BEANQUICK_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# TODO(zq7): figure out why this can't build out a corrent lib.
bean_cc_library(
  NAME
    fixed
  SRCS
    src/Number.cpp
    src/Precision.cpp
    src/Rounding.cpp
  COPTS
    ${BEANQUICK_DEFAULT_COPTS}
  PUBLIC
)

# bean_cc_test(
#   NAME
#     tests
#   SRCS
#     test/FirstBitSetTests.cpp
#     test/NumberAbsoluteTests.cpp
#     test/NumberArithmeticTests.cpp
#     test/NumberIntConstructorFailTests.cpp
#     test/NumberIntConstructorTests.cpp
#     test/NumberFpConstructorFailTests.cpp
#     test/NumberFpConstructorTests.cpp
#     test/NumberNegateTests.cpp
#     test/NumberRelationalTests.cps
#     test/NumberToFpTests.cpp
#     test/RoundingTests.cpp
#     test/SqueezeZerosTests.cpp
#     test/UnitTest.cpp
#   COPTS
#     ${BEANQUICK_TEST_COPTS}
#   DEPS
#     bean::fixed
# )
