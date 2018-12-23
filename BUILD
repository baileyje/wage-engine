
cc_binary(
	name = "wage",
	srcs = ["wage.cpp"],
	deps = [
    "//core",
    "//platform",
    "//input",
    "//jsrt",
    "//physics",
    "//physics-bullet",
    "//engine",
    "//fs",
    "//assets",
    "//render",
    "//render-gl",
    "//ui"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)