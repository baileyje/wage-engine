cmake_minimum_required(VERSION 3.14.0)
project(wageEngineExtensions VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# FetchContent_Declare(
#   bullet
#   GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
#   GIT_TAG 2.89
# )
# # FetchContent_MakeAvailable(bullet)
# FetchContent_GetProperties(bullet)
# if(NOT bullet_POPULATED)
#   message(STATUS "Populating bullet, please wait...")

#   #http://cmake.3232098.n2.nabble.com/Passing-CMake-Arguments-to-FetchContent-td7599242.html
#   set(USE_DOUBLE_PRECISION ON CACHE INTERNAL "" FORCE)
#   set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "" FORCE)
#   set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "" FORCE)
#   set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "" FORCE)

#   if (WIN32)
#     set(USE_MSVC_RUNTIME_LIBRARY_DLL YES CACHE INTERNAL "" FORCE)
#     set(INSTALL_LIBS YES CACHE INTERNAL "" FORCE)
#     set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
#   else()
#     set(BUILD_SHARED_LIBS ON CACHE INTERNAL "" FORCE)
#   endif()

#   FetchContent_Populate(bullet)
#   message(STATUS "finished building bullet")
#   add_subdirectory(${bullet_SOURCE_DIR} ${bullet_BINARY_DIR})
#   target_include_directories(Bullet3Common INTERFACE
#     ${bullet_SOURCE_DIR}/src
#     ${bullet_SOURCE_DIR}/src/Bullet3Common
#     ${bullet_SOURCE_DIR}/src/Bullet3Collision
#   )
# endif()

# set(BULLET_INCLUDE_DIRS "${bullet_SOURCE_DIR}/src")

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.34
)
FetchContent_MakeAvailable(glad)

find_package(freetype REQUIRED FATAL_ERROR)

find_package(bullet REQUIRED FATAL_ERROR)

# FetchContent_Declare(
#   harfbuzz
#   GIT_REPOSITORY https://github.com/harfbuzz/harfbuzz.git
#   GIT_TAG 2.9.0
# )
# FetchContent_MakeAvailable(harfbuzz)

# FetchContent_Declare(
#   freetype
#   GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
#   GIT_TAG VER-2-11-0
# )
# FetchContent_MakeAvailable(freetype)

# message(STATUS "\n\n\n\n\n\n\n\n -----> ${freetype_BINARY_DIR}\n\n\n\n\n\n ")
# if(NOT freetype2_POPULATED)
# 	FetchContent_Populate(freetype2)
# 	add_subdirectory(${freetype2_SOURCE_DIR} ${freetype2_BINARY_DIR})
# endif()

# FetchContent_GetProperties(freetype2 SOURCE_DIR FREETYPE_SRC_DIR)

# set(FREETYPE_LIBRARIES freetype2)

set(FREETYPE_INCLUDE_DIRS "${freetype_SOURCE_DIR}/include")

FetchContent_Declare(
  jsoncpp
  GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
  GIT_TAG 1.9.4
)
FetchContent_MakeAvailable(jsoncpp)
set(JSONCPP_INCLUDE_DIRS "${jsoncpp_SOURCE_DIR}/include")


find_package(Vulkan REQUIRED FATAL_ERROR)

set(ENGINE_INCLUDE_DIRS "${wageEngine_SOURCE_DIR}/src")


include_directories(src ${BULLET_INCLUDE_DIR} ${ENGINE_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} third_party/stb_image)

file(GLOB_RECURSE source_files LIST_DIRECTORIES true CONFIGURE_DEPENDS
  "src/**/*.cpp"
)

add_library(wageEngineExtensions ${source_files})

set_target_properties(wageEngineExtensions PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PRIVATE wageEngine bullet glfw glm glad Vulkan::Vulkan jsoncpp_lib Freetype::Freetype)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
