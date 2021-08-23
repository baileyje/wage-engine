cmake_minimum_required(VERSION 3.14.0)
project(wageEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

include_directories(src third_party/tinyloader)

file(GLOB_RECURSE source_files LIST_DIRECTORIES true CONFIGURE_DEPENDS
  "src/**/*.cpp",
  "third_party/tinyloader/tiny_obj_loader.cc"
)

add_library(wageEngine ${source_files})

set_target_properties(wageEngine PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PRIVATE glm)

