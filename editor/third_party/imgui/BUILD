
licenses(["permissive"])


cc_library(
	name = "imgui",
	includes = ["include"],
	srcs = glob(["src/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"@glfw",
		"@vulkan"
	],
	visibility = ["//visibility:public"]
)