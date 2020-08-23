
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
    "@engine_ext//audio-al",
    ":wage_lib"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL -framework IOKit"],
  visibility = ["//visibility:public"]
)

genrule(
    name = "wage_dsym",
    srcs = [":wage"],
    outs = ["wage.dSYM"],
    output_to_bindir = True,
    cmd = "dsymutil $(location :wage) -o $@"
)
