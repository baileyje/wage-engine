
licenses(["permissive"])

cc_library(
  name = "jsoncpp",
  includes = ["include"],
  hdrs = glob(["include/json/*.h", "src/lib_json/*.h", "src/lib_json/*.inl"]),
  srcs = glob(["src/lib_json/*.cpp"]),
  visibility = ["//visibility:public"],
)