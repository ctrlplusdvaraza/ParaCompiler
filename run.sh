#!/bin/sh

USAGE="Usage: $0 <file_name> [--graphvizer] [--build-dir <dir>]"

BUILD_DIR="build"
GRAPHVIZ_RUN=0

if [ $# -eq 0 ]; then
    echo "$USAGE"
    exit 1
fi

FILE_NAME="$1"
shift

AST_FILE="${FILE_NAME%.*}.ast"
IMG_FILE="${FILE_NAME%.*}.svg"

while [ $# -gt 0 ]; do
    case "$1" in
        --graphviz)
            GRAPHVIZ_RUN=1
            ;;

        --build-dir)
            shift
            if [ $# -eq 0 ]; then
                echo "$USAGE"
                exit 1
            fi
            BUILD_DIR="$1"
            ;;

        *)
            echo "Unknown option: $1"
            echo "$USAGE"
            exit 1
            ;;
    esac
    shift
done

./"$BUILD_DIR"/frontend/frontend "$FILE_NAME" || exit 1

if [ $GRAPHVIZ_RUN -eq 1 ]; then
    ./"$BUILD_DIR"/graphvizer/graphvizer "$AST_FILE" "$IMG_FILE"
fi

./"$BUILD_DIR"/simulator/simulator "$AST_FILE"
