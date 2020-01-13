# Build proto file.
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER schema.proto)

# NOTE: must include this so that we can find the generated proto files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

bean_cc_library(
  NAME
    schema
  HDRS
    ${PROTO_HDR}
  SRCS
    ${PROTO_SRC}
  COPTS
    ${BEANQUICK_DEFAULT_COPTS}
  DEPS
    # NOTE: must link this protobuf lib
    ${PROTOBUF_LIBRARY}
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
    bean::schema
    bean::fixed
    gmock_main
)

