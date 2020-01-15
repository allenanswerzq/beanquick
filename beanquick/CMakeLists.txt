# # Build proto file.
# INCLUDE(FindProtobuf)
# FIND_PACKAGE(Protobuf REQUIRED)
# INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
# PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER schema.proto)

# # NOTE: must include this so that we can find the generated proto files.
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# bean_cc_library(
#   NAME
#     schema
#   HDRS
#     ${PROTO_HDR}
#   SRCS
#     ${PROTO_SRC}
#   COPTS
#     ${BEANQUICK_DEFAULT_COPTS}
#   DEPS
#     # NOTE: must link this protobuf lib
#     ${PROTOBUF_LIBRARY}
#   PUBLIC
# )

bean_cc_library(
  NAME
    util
  HDRS
    "logging.h"
  SRCS
    "logging.cc"
  COPTS
    ${BEANQUICK_DEFAULT_COPTS}
  PUBLIC
)

bean_cc_library(
  NAME
    core
  HDRS
    "debug.h"
    "base.h"
    "decimal.h"
    "amount.h"
  DEPS
    bean::util
    bean::fixed
    absl::strings
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
    bean::core
    gmock_main
)

bean_cc_test(
  NAME
    amount_test
  SRCS
    "amount_test.cc"
  COPTS
    ${BEANQUICK_TEST_COPTS}
  DEPS
    bean::core
    gmock_main
)


