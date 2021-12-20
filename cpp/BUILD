load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "chess",
    srcs = ["chess/chess.cpp"],
    hdrs = ["chess/chess.h"],
)

cc_binary(
    name = "test",
    srcs = ["test/test.cpp"],
    deps=["@com_google_googletest//:gtest_main",":chess"],
)