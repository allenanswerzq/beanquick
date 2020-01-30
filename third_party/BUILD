package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "libfixed",
    srcs = glob(
        [
          "fixed/src/*.cpp",
        ],
    ),
    hdrs = glob([
        "fixed/include/*.h",
        "fixed/src/*.h"
    ]),
    copts = [
        "-O3",
        "-std=gnu++11",
        "-Ithird_party/fixed/include",
    ]
)

cc_test(
    name = "fixed_tests",
    srcs = glob(
        [
          "fixed/test/*.cpp",
          "fixed/test/*.h"
        ],
    ),
    deps = [
        ":libfixed",
    ],
    copts = [
        "-O3",
        "-std=gnu++11",
        "-Ithird_party/fixed/include",
    ]
)



