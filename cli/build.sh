#!/bin/bash
cd "$(dirname "$0")"
git submodule update --remote --merge

PREFIX=
case "$1" in
    linux)
        ;;
    win32)
        PREFIX=i686-w64-mingw32-
        ;;
    win64)
        PREFIX=x86_64-w64-mingw32-
        ;;
    *)
        echo "Usage: $0 <linux|win32|win64> [clean]"
        exit 1
        ;;
esac

if [ "$2" = "clean" ]; then
    rm -rf build cahute/build
fi

${PREFIX}cmake -B build -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build
