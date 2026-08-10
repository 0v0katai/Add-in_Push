# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(AddinPush LANGUAGES C)

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)

set(SOURCES
  src/main.c
  src/mmu.c
  # ...
)
set(ASSETS
  # ...
)
set(ASSETS_fx
  # ...
)
set(ASSETS_cg
  # ...
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(AddinPush ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})

target_compile_options(AddinPush PRIVATE -I include
                       -Wall -Wextra -Os -g -flto=auto)
target_link_options(AddinPush PRIVATE
                    -L ${CMAKE_CURRENT_SOURCE_DIR}
                    -T ${CMAKE_CURRENT_SOURCE_DIR}/prizm.ld
                    -Wl,--print-memory-usage -Wl,-Map=map)
target_link_libraries(AddinPush PRIVATE -lc -lgcc -lsyscall-cg)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET AddinPush VERSION "01.00.0000" OUTPUT "Push.g3a"
    NAME "Add-in Push" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
