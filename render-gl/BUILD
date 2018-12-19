
cc_library(
	name = "render-gl",
	srcs = glob(["**/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"//core",
		"//platform",
		"//assets",
		"//entity",				
		"//math",
		"//render",
		"@glfw",
		"@glm",
		"//third_party/glad",
		"//third_party/stb_image"
	],
	visibility = ["//visibility:public"]	
)