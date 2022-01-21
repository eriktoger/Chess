load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "board",
    srcs = glob(["chess/*.cpp"]),
    hdrs = glob(["chess/*.h"]),
)

cc_library(
    name = "game",
    srcs = glob(["chess/*.cpp"]),
    hdrs = glob(["chess/*.h"]),
)

cc_binary(
    name = "gameTests",
    srcs = ["test/gameTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":game"],
)

cc_binary(
    name = "boardTests",
    srcs = ["test/boardTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":board"],
)

cc_binary(
    name = "helpersTests",
    srcs = ["test/helpersTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":board"],
)

cc_binary(
    name = "moveTests",
    srcs = ["test/moveTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":board"],
)


cc_binary(
    name = "pieceTests",
    srcs = ["test/pieceTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":board"],
)



cc_binary(
    name = "squareTests",
    srcs = ["test/squareTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":board"],
)


cc_binary(
    name = "openingBookTests",
    srcs = ["test/openingBookTests.cpp"],
    deps=["@com_google_googletest//:gtest_main",":game"],
)


