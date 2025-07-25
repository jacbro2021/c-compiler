#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' 

for test_file in ./test/test*; do
    test_name=$(basename "$test_file")   
    test_num="${test_name//[!0-9]/}"     
    expected_output_file="./test/out${test_num}"

    if ./build/compiler "$test_file" && gcc out.s -o out_exec; then
        actual_output=$(./out_exec)
        expected_output=$(<"$expected_output_file")

        if [[ "$actual_output" == "$expected_output" ]]; then
            echo -e "${GREEN}${test_name^^} PASSED${NC}"
        else
            echo " "
            echo -e "${RED}${test_name^^} FAILED: Output does not match expected${NC}"
            echo -e "${RED}Expected:\n${NC}$expected_output"
            echo -e "${RED}Got:\n${NC}$actual_output"
            exit 1
        fi
    else
        echo " "
        echo -e "${RED}${test_name^^} FAILED: Compilation or execution error${NC}"
        exit 1
    fi
done
