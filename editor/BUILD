
cc_library(
	name = "wage-edit_lib",
	srcs = glob(["**/*.cpp"], exclude=["editor.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
    "@engine",
    "//third_party/imgui"
	],
	visibility = ["//visibility:public"]
)

cc_binary(
  name = "wage-edit",
  srcs = ["editor.cpp"],
  deps = [
    "@engine",
    "@engine_ext//platform-glfw",
    "@engine_ext//input-glfw",
    "@engine_ext//render-gl",
    "@engine_ext//render-vulkan",
    "@engine_ext//physics-bullet",
    "@engine_ext//audio-al",
    "@engine_ext//scene-serializer",
    ":wage-edit_lib"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL -framework IOKit"],
  visibility = ["//visibility:public"]
)

genrule(
    name = "wage-edit_dsym",
    srcs = [":wage-edit"],
    outs = ["wage-edit.dSYM"],
    output_to_bindir = True,
    cmd = "dsymutil $(location :wage-edit) -o $@"
)
