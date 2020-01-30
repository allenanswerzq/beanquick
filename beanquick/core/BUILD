package(default_visibility = ["//visibility:public"])

cc_library(
    name = "util",
    hdrs = [
        "logging.h",
        "base.h",
        "debug.h",
    ],
    srcs = [
      "logging.cc"
    ],
)

cc_library(
    name = "core",
    hdrs = [
        "decimal.h",
        "amount.h",
        "display_context.h",
    ],
    srcs = [
        "display_context.cc"
    ],
    deps = [
        ":util",
        "//third_party:libfixed",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:variant",
        "@com_google_absl//absl/functional:bind_front",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "decimal_test",
    srcs = [
        "decimal_test.cc"
    ],
    deps = [
        ":core",
    ]
)

cc_test(
    name = "amount_test",
    srcs = [
        "amount_test.cc"
    ],
    deps = [
        ":core",
    ]
)

cc_test(
    name = "display_context_test",
    srcs = [
        "display_context_test.cc",
    ],
    deps = [
        ":core",
    ]
)
