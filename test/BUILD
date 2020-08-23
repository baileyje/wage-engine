

cc_binary(
  name = "test",
  srcs = ["test.cpp"],
  deps = [
    "@engine",
    "@engine_ext//audio-al"
  ],
  linkopts = ["-framework Cocoa -framework OpenGL -framework IOKit"],
  visibility = ["//visibility:public"]
)

genrule(
    name = "test_dsym",
    srcs = [":test"],
    outs = ["test.dSYM"],
    output_to_bindir = True,
    cmd = "dsymutil $(location :test) -o $@"
)