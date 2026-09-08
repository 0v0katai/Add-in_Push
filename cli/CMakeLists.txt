cmake_minimum_required(VERSION 3.16)
project(push-cli LANGUAGES C)

include(ExternalProject)

set(CAHUTE_BUILD_ARGS
  -DCAHUTE_CLI=OFF
  -DCAHUTE_SHARED=OFF
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
)

if(CMAKE_TOOLCHAIN_FILE)
  list(APPEND CAHUTE_BUILD_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
endif()

set(CAHUTE_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cahute/build")

ExternalProject_Add(cahute-build
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cahute"
  BINARY_DIR "${CAHUTE_BUILD_DIR}"
  CMAKE_ARGS ${CAHUTE_BUILD_ARGS}
  BUILD_BYPRODUCTS
    "${CAHUTE_BUILD_DIR}/libcahute-static.a"
  INSTALL_COMMAND ""
)

file(MAKE_DIRECTORY "${CAHUTE_BUILD_DIR}/include")
add_library(cahute-static STATIC IMPORTED GLOBAL)
set_target_properties(cahute-static PROPERTIES
  IMPORTED_LOCATION "${CAHUTE_BUILD_DIR}/libcahute-static.a"
  INTERFACE_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/cahute/include;${CAHUTE_BUILD_DIR}/include"
)
add_dependencies(cahute-static cahute-build)

add_executable(push-cli main.c)
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
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
