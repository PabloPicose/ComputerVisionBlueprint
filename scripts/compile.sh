#!/bin/bash

# Initialize variables
QT_DIR=""
BUILD_TYPE=""

# Function to print the usage of the script
print_usage() {
    echo "Usage: $0 --qt <path-to-Qt-directory> --type <debug|release>"
    echo "Example: $0 --qt /opt/Qt5.12.10 --type debug"
}

# Parse command line arguments
while [ "$#" -gt 0 ]; do
    case "$1" in
        --qt)
            QT_DIR="$2"
            shift 2
            ;;
        --type)
            BUILD_TYPE="$2"
            if [[ "$BUILD_TYPE" != "debug" && "$BUILD_TYPE" != "release" ]]; then
                echo "Error: --type must be 'debug' or 'release'."
                print_usage
                exit 1
            fi
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            print_usage
            exit 1
            ;;
    esac
done

# Check if both arguments were provided
if [ -z "$QT_DIR" ] || [ -z "$BUILD_TYPE" ]; then
    echo "Error: Both --qt and --type arguments are required."
    print_usage
    exit 1
fi

# Configure and generate the build system
if [ "$BUILD_TYPE" = "debug" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=.conan_debug/conan_toolchain.cmake -B cmake-build-debug -S . -DCMAKE_PREFIX_PATH="$QT_DIR"
    cmake --build cmake-build-debug --target all
else
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=.conan_rel/conan_toolchain.cmake -B cmake-build-release -S . -DCMAKE_PREFIX_PATH="$QT_DIR"
    cmake --build cmake-build-release --target all
fi
