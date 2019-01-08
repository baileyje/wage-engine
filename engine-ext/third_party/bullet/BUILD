
licenses(["permissive"])

cc_library(
	name = "bullet",  
  includes = ["lib", "include"],
	srcs = glob(["**/*.dylib"]),
	hdrs = glob(["**/*.h"]),
	strip_include_prefix = "include/bullet",
  visibility = ["//visibility:public"]
)