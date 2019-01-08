
cc_binary(
  name = "wage",
  srcs = ["wage.cpp"],
  deps = [
    "@engine",
    "@engine_ext//render-gl",
    "@engine_ext//physics-bullet"
    # "//engine",
    # "//engine-ext/render-gl",
    # "//engine-ext/physics-bullet",
  ],
  linkopts = ["-framework Cocoa -framework OpenGL"],
  visibility = ["//visibility:public"]
)