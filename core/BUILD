package(default_visibility = ["//visibility:public"])

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
	name = "ChakraCore",
	srcs = ["libChakraCore.dylib"],
	hdrs = glob(["include/*.h"])
)

cc_library(
	name = "core",
	srcs = ["core.cpp", "jsrt.cpp"],
	hdrs = ["core.h", "jsrt.h"],
	deps = [":ChakraCore"]
)

cc_binary(
	name = "main",
	srcs = ["main.cpp"],
	deps = [":core"]
)