
cc_library(
	name = "render-gl",
	srcs = glob(["**/*.cpp"]),
	hdrs = glob(["**/*.h"]),
	deps = [
		"@engine//core",
		"@engine//platform",
		"@engine//assets",
		"@engine//entity",				
		"@engine//math",
		"@engine//render",		
		"//third_party/glad",
		"//third_party/stb_image",
		"@freetype",
		"@glfw",
		"@glm",
	],
	visibility = ["//visibility:public"]	
)