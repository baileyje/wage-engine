load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def engine_ext_repositories():

  http_archive(
    name = "jsoncpp",
    urls = ["https://github.com/open-source-parsers/jsoncpp/archive/1.8.0.tar.gz"],
    strip_prefix = "jsoncpp-1.8.0",
    sha256 = "5deb2462cbf0c0121c9d6c9823ec72fe71417e34242e3509bc7c003d526465bc",
    build_file_content = """
cc_library(
  name = "jsoncpp",
  includes = ["include"],
  hdrs = glob(["include/json/*.h", "src/lib_json/*.h", "src/lib_json/*.inl"]),
  srcs = glob(["src/lib_json/*.cpp"]),
  visibility = ["//visibility:public"],
)
    """
  )

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



  native.new_local_repository(
      name = "vulkan_osx",
      path = "/Users/john/vulkan/macOS/",
      build_file_content = """
cc_library(
  name = "vulkan",
  includes = ["lib", "include"],
  srcs = ["lib/libvulkan.1.2.148.dylib", "lib/libVkLayer_khronos_validation.dylib", "lib/libVkLayer_api_dump.dylib", "lib/libdxcompiler.3.7.dylib"],
  hdrs = glob(["**/*.h"]),
  visibility = ["//visibility:public"]
)
      """
  )


  native.new_local_repository(
    name = "vulkan",
    path = ".",
    build_file_content = """
cc_library(
  name = "vulkan",
    deps = select({
    "@bazel_tools//src/conditions:darwin":["@vulkan_osx//:vulkan"] ,
    "//conditions:default": [],
  }),
  visibility = ["//visibility:public"]
)
    """
  )  