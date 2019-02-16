
cc_library(
	name = "render-gl",
	srcs = glob(["**/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"@engine",
		"//third_party/glad",
		"//third_party/stb_image",
		"@freetype",
		"@glfw",
		"@engine//third_party/glm",
	],
	visibility = ["//visibility:public"]	
)