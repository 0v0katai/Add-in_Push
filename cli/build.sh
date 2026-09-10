#!/bin/bash
cd "$(dirname "$0")"

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
        echo "Usage: bash $0 <linux|win32|win64>"
        exit 1
        ;;
esac

${PREFIX}cmake -B build --fresh
cmake --build build --parallel
