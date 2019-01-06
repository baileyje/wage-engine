
cc_binary(
	name = "wage",
	srcs = ["wage.cpp"],
	deps = [
    "//engine",
    "//engine-ext/render-gl",
    "//engine-ext/physics-bullet",
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)