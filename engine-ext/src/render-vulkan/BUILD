
cc_library(
	name = "render-vulkan",
	srcs = glob(["**/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"@engine",
		"//third_party/glad",
		"//third_party/stb_image",
		"@freetype",
		"@glfw",
		"@engine//third_party/glm",
		"@vulkan"
	],
	visibility = ["//visibility:public"]	
)