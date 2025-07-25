#!/bin/bash

cd build || { echo "Failed to cd into build/"; exit 1; }
make || { echo "Make failed"; exit 1; }
cd .. || { echo "Failed to cd back to parent directory"; exit 1; }

if [ -n "$1" ]; then
    TEST_FILE="./test/$1"
    if [ -f "$TEST_FILE" ]; then
        echo "Running compiler with test file: $TEST_FILE"
        ./build/compiler "$TEST_FILE"
    else
        echo "Test file '$TEST_FILE' not found."
        exit 1
    fi
else
    ./scripts/run_tests.sh
fi
