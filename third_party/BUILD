package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "libfixed",
    srcs = glob(
      [
        "fixed/**/*.cpp",
      ],
      exclude = [
        "fixed/**/*Tests.cpp",
      ],
    ),
    hdrs = glob([
        "fixed/include/*.h"
    ]),
    copts = [
        "-O3",
        "-Ithird_party/fixed/include",
    ]
)

