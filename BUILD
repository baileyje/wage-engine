
cc_binary(
	name = "wage",
	srcs = ["wage.cpp"],
	deps = [
    "//core",
    "//platform",
    "//input",
    "//jsrt",
    "//physics",
    "//engine",
    "//render"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)