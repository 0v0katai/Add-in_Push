cmake_minimum_required(VERSION 3.15)
project(push LANGUAGES C)

include(GenerateG3A)

set(SOURCES src/main.c src/mmu.c)

add_executable(push ${SOURCES})

target_include_directories(push PRIVATE include)
target_compile_options(push PRIVATE -Wall -Wextra -Os -g -flto=auto)
target_link_options(push PRIVATE
  -L ${CMAKE_CURRENT_SOURCE_DIR}
  -T ${CMAKE_CURRENT_SOURCE_DIR}/prizm.ld
  -Wl,-print-memory-usage -Wl,-Map=map)
target_link_libraries(push PRIVATE -lc -lgcc -lsyscall-cg)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET push VERSION "01.01.0000" OUTPUT "LinkApp2.g3a"
    NAME "Add-in Push" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
