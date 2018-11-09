
cc_library(
	name = "render",
	srcs = glob(["**/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"//core",
		"//entity",
		"//fs",
		"//platform",
		"//math",
		"@glfw",
		"@glm",
		"//third_party/glad",
		"//third_party/stb_image"
	],
	visibility = ["//visibility:public"]	
)