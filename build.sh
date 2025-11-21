#!/bin/sh

BUILD_DIR="build"

BUILD_TYPE=Debug
SANITIZE=TRUE

while [ $# -gt 0 ]; do
    case "$1" in
        --release)
            BUILD_TYPE=Release
            SANITIZE=FALSE
            ;;
        --debug)
            BUILD_TYPE=Debug
            SANITIZE=TRUE
            ;;

        --build-dir)
            shift
            BUILD_DIR="$1"
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
    shift
done

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DSANITIZE="$SANITIZE" ..
cmake --build .
