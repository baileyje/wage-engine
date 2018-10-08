
cc_binary(
	name = "wage",
	srcs = ["wage.cpp"],
	deps = [
    "//core",
    "//jsrt",
    "//physics",
    "//engine",
    "//render"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)