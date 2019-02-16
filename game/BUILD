
cc_binary(
  name = "wage",
  srcs = ["wage.cpp"],
  deps = [
    "@engine",
    "@engine_ext//render-gl",
    "@engine_ext//physics-bullet"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)


cc_binary(
  name = "test",
  srcs = ["test.cpp"],
  deps = [
    "@engine",
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)

genrule(
    name = "wage_dsym",
    srcs = [":wage"],
    outs = ["wage.dSYM"],
    output_to_bindir = True,
    cmd = "dsymutil $(location :wage) -o $@"
)