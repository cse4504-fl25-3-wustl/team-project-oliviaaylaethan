#!/bin/bash

total_tests=0
passed_tests=0
failed_tests=0
failed_paths=()

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

process_csv_files() {
    local input_dir="$1"
    local client_config="$2"    

    while IFS= read -r -d '' csv_file; do
        total_tests=$((total_tests + 1))
        csv_file=$(realpath "$csv_file")
        
        executable=$(find "$(dirname "$0")/build" -type f -name "main" -exec realpath {} \;)
        output_path="$(dirname "$0")/build/bashOutput.json"
        
        rm -f "$output_path"

        "$executable" "$csv_file" "$client_config" "$output_path"

        if [ -f "$output_path" ]; then
            expected_output_path="$(dirname "$csv_file")/expected_output.json"
            if python3 testing/compare.py "$output_path" "$expected_output_path"; then
                echo -e "${GREEN}TEST PASSED:${NC} $csv_file"
                passed_tests=$((passed_tests + 1))
            else
                echo -e "${RED}TEST FAILED:${NC} $csv_file"
                failed_tests=$((failed_tests + 1))
                failed_paths+=("$csv_file")
            fi
        else
            echo -e "${YELLOW}FAILURE WITH $csv_file:${NC} Output does not exist."
            failed_tests=$((failed_tests + 1))
            failed_paths+=("$csv_file")
        fi
    done < <(find "$input_dir" -type f -name "*.csv" -print0)
}

INPUT_DIR="$(realpath ./testing/testdata/inputsFeature2)"
CLIENT_CONFIG="$(realpath ./testing/testdata/siteRequirements/feature_2_site.csv)"

# pass client configuration that does not allow crates
process_csv_files "$INPUT_DIR"/box_packing $CLIENT_CONFIG
process_csv_files "$INPUT_DIR"/pallet_packing $CLIENT_CONFIG
# pass client configuration that allows crates
process_csv_files "$INPUT_DIR"/crate_packing $CLIENT_CONFIG

# Print summary after all calls
if [ $failed_tests -gt 0 ]; then
    echo -e "\n\n${RED}\033[1m--------------- FAILED TEST PATHS ---------------\033[0m${NC}"
    for path in "${failed_paths[@]}"; do
        echo "$path"
    done
fi

echo -e "\n\n${BLUE}\033[1m-------------------- SUMMARY --------------------\033[0m${NC}"
echo -e "${GREEN}Passed tests:${NC} $passed_tests"
echo -e "${RED}Failed tests:${NC} $failed_tests"
echo -e "${BLUE}Total tests:${NC} $total_tests"