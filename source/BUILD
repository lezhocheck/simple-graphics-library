load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "sglib",
    srcs = [
        "Bitmap.cpp",
        "Canvas.cpp",
        "Color.cpp",
        "LinearGradient.cpp",
        "Pixels.cpp",
        "Shape.cpp",
    ],
    hdrs = [
        "Array.h",
        "Bitmap.h",
        "Canvas.h",
        "Color.h",
        "LinearGradient.h",
        "Pixels.h",
        "Point.h",
        "Shape.h",
    ],
    visibility = ["//visibility:public"]
)

cc_binary(
    name = "main",
    srcs = [
        "main.cpp"
    ],
    deps = [
        ":sglib"
    ]
)