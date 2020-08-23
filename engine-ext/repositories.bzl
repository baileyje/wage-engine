def engine_ext_repositories():

  native.new_local_repository(
      name = "bullet_osx",
      path = "/usr/local/Cellar/bullet/2.89",
      build_file_content = """
cc_library(
  name = "bullet",
  includes = ["lib", "include"],
  srcs = glob(["**/*.dylib"]),
  hdrs = glob(["include/bullet/**/*.h"]),
  strip_include_prefix = "include/bullet",
  visibility = ["//visibility:public"]
)
      """
  )

  native.new_local_repository(
      name = "bullet",
      path = ".",
      build_file_content = """
cc_library(
  name = "bullet",
  deps = select({
  "@bazel_tools//src/conditions:darwin":["@bullet_osx//:bullet"] ,
  "//conditions:default": [],
  }),
  visibility = ["//visibility:public"]
)
      """
  )

  native.new_local_repository(
      name = "freetype_osx",
      path = "/usr/local/Cellar/freetype/2.10.2",
      build_file_content = """
cc_library(
  name = "freetype",
    includes = ["include/freetype2"],
    srcs = glob(["**/*.dylib"]),
  hdrs = glob(["**/*.h"]),
    visibility = ["//visibility:public"]
)
      """
  )

  native.new_local_repository(
      name = "freetype",
      path = ".",
      build_file_content = """
cc_library(
  name = "freetype",
  deps = select({
    "@bazel_tools//src/conditions:darwin":["@freetype_osx//:freetype"] ,
    "//conditions:default": [],
  }),
  visibility = ["//visibility:public"]
)
      """
  )


  native.new_local_repository(
    name = "glfw_osx",
    path = "/usr/local/Cellar/glfw/3.3.2",
    build_file_content = """
cc_library(
  name = "glfw",
  includes = ["lib", "include"],
  srcs = glob(["**/*.dylib"]),
  hdrs = glob(["**/*.h"]),
  visibility = ["//visibility:public"]
)
    """
  )

  native.new_local_repository(
    name = "glfw",
    path = ".",
    build_file_content = """
cc_library(
  name = "glfw",
    deps = select({
    "@bazel_tools//src/conditions:darwin":["@glfw_osx//:glfw"] ,
    "//conditions:default": [],
  }),
  visibility = ["//visibility:public"]
)
    """
  )


  native.new_local_repository(
      name = "openal_osx",
      path = "/usr/local/Cellar/openal-soft/1.20.1/",
      build_file_content = """
cc_library(
  name = "openal",
  includes = ["lib", "include"],
  srcs = glob(["**/*.dylib"]),
  hdrs = glob(["include/AL/**/*.h"]),
  strip_include_prefix = "include/AL",
  visibility = ["//visibility:public"]
)
      """
  )


  native.new_local_repository(
    name = "openal",
    path = ".",
    build_file_content = """
cc_library(
  name = "openal",
    deps = select({
    "@bazel_tools//src/conditions:darwin":["@openal_osx//:openal"] ,
    "//conditions:default": [],
  }),
  visibility = ["//visibility:public"]
)
    """
  )