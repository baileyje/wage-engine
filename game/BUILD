

cc_library(
	name = "wage_lib",
	srcs = glob(["**/*.cpp"], exclude=["wage.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
    "@engine",
	],
	visibility = ["//visibility:public"]
)

cc_binary(
  name = "wage",
  srcs = ["wage.cpp"],
  deps = [
    "@engine",
    "@engine_ext//platform-glfw",
    "@engine_ext//input-glfw",
    "@engine_ext//render-gl",
    "@engine_ext//physics-bullet",
    ":wage_lib"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)

genrule(
    name = "wage_dsym",
    srcs = [":wage"],
    outs = ["wage.dSYM"],
    output_to_bindir = True,
    cmd = "dsymutil $(location :wage) -o $@"
)

cc_binary(
  name = "test",
  srcs = ["test.cpp"],
  deps = [
    "@engine",
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)

genrule(
    name = "test_dsym",
    srcs = [":test"],
    outs = ["test.dSYM"],
    output_to_bindir = True,
    cmd = "dsymutil $(location :test) -o $@"
)