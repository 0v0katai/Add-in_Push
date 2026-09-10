cmake_minimum_required(VERSION 3.21)
project(push-cli LANGUAGES C)

include(FetchContent)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CAHUTE_CLI OFF CACHE BOOL "" FORCE)
set(CAHUTE_SHARED OFF CACHE BOOL "" FORCE)

FetchContent_Declare(cahute
  GIT_REPOSITORY https://gitlab.com/cahute/cahute
  GIT_TAG develop)
FetchContent_MakeAvailable(cahute)

add_executable(push-cli main.c)
target_include_directories(push-cli PRIVATE
  "${cahute_SOURCE_DIR}/include"
  "${cahute_BINARY_DIR}/include")
target_compile_options(push-cli PRIVATE -Os -flto=auto)
target_compile_definitions(push-cli PRIVATE CAHUTE_STATIC=1)
target_link_libraries(push-cli PRIVATE cahute-static)

if(CMAKE_CROSSCOMPILING AND WIN32)
  target_link_libraries(push-cli PRIVATE
    setupapi cfgmgr32 -Wl,-Bstatic ssp)
else()
  target_link_libraries(push-cli PRIVATE usb-1.0)
endif()

set_target_properties(push-cli PROPERTIES
  OUTPUT_NAME "push"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
