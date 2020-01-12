INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER beanquick.proto)

# NOTE: must include this so that we can find the generated proto files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Add third_party includes
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/third_party/libfixed/include)

bean_cc_library(
  NAME
    protolib
  HDRS
    ${PROTO_HDR}
  SRCS
    ${PROTO_SRC}
  COPTS
    ${BEANQUICK_DEFAULT_COPTS}
  PUBLIC
)

bean_cc_library(
  NAME
    decimal
  HDRS
    "decimal.h"
  COPTS
    ${BEANQUICK_DEFAULT_COPTS}
  PUBLIC
)

bean_cc_test(
  NAME
    decimal_test
  SRCS
    "decimal_test.cc"
  COPTS
    ${BEANQUICK_TEST_COPTS}
  DEPS
    bean::decimal
    bean::protolib
    gmock_main
)

