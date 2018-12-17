
cc_library(
	name = "render-gl",
	srcs = glob(["**/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"//core",
		"//entity",
		"//fs",
		"//platform",
		"//math",
		"//render",
		"@glfw",
		"@glm",
		"//third_party/glad",
		"//third_party/stb_image"
	],
	visibility = ["//visibility:public"]	
)