#!/usr/bin/env bash

# Make sure file uses Unix line endings:
# On Windows, run this in your terminal: dos2unix run_all_tests.sh

total_tests=0
passed_tests=0
failed_tests=0
failed_paths=()
passed_paths=()

ONLY_FAILED=0 # Default is 0 (false), meaning print both passed and failed.
PYTHON_ERRORS_FILE="$(dirname "$0")/.python_errors.tmp"

# --- Command Line Argument Handling ---
# Parse arguments for the -f or --only-failed flag
for arg in "$@"; do
    case "$arg" in
        -f|--only-failed)
            ONLY_FAILED=1
            shift # Remove argument from list
            ;;
        *)
            # Unknown option (can be handled or ignored)
            shift # Remove argument from list
            ;;
    esac
done

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# --- Helper: realpath fallback (mac vs windows) ---
realpath_compat() {
    if command -v realpath >/dev/null 2>&1; then
        realpath "$1"
    else
        python3 -c "import os,sys; print(os.path.abspath(sys.argv[1]))" "$1"
    fi
}

# --- Detect platform to choose executable ---
get_executable() {
    local exe_dir="$(dirname "$0")/build"
    local exe_name="main"
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        exe_name="main.exe"
    fi
    echo "$(realpath_compat "$exe_dir/$exe_name")"
}


# note: opening subshells works fine on mac, but windows destroys all the subshell variables (which can't be passed to the summary)
# solution: using a temporary array
process_csv_files() {
    local input_dir="$1"
    local accept_crates="$2"
    
    # 1. Use mapfile (or readarray) to get all file paths into an array in the main shell
    local csv_files=()
    # Note: Using python to print paths is often more robust than find -print0 in mixed environments
    while IFS= read -r -d $'\0' line; do
        csv_files+=("$line")
    done < <(find "$input_dir" -type f -name "*.csv" -print0)

    # --- FIX 1: CHECK IF ANY CSV FILES WERE FOUND ---
    if [ ${#csv_files[@]} -eq 0 ]; then
        echo -e "${YELLOW}WARNING:${NC} No *.csv files found in $input_dir. Skipping test run."
        return 0 # Exit the function cleanly
    fi
    # -----------------------------------------------

    # Check if cygpath is available for Windows path conversion (re-adding the key fix)
    local use_cygpath=0
    if command -v cygpath >/dev/null 2>&1; then
        use_cygpath=1
    fi
    local executable
    executable=$(get_executable)

    # Show the user where bash is looking for the executable
    echo -e "PATH TO EXECUTABLE: $executable"

    # Check if the executable exists at the given path
    if [ ! -f "$executable" ]; then
        echo -e "${RED}ERROR: Executable not found at $executable${NC}"
        echo -e "${RED}SUGGESTED FIX: if main.exe is located inside build/Debug (or another subfolder), move it directly under /build"
        exit 1  # Exit the script with a non-zero status to indicate failure
    fi

    local output_dir="$(dirname "$0")/build"
    local output_path="$output_dir/bashOutput.json"
    
    # Ensure output directory exists before running tests
    mkdir -p "$output_dir"

    # Set up Windows output path conversion once
    local win_output_path="$output_path"
    if [ "$use_cygpath" -eq 1 ]; then
        win_output_path=$(cygpath -w "$output_path")
    fi

    # 2. Loop over the array (which runs in the main shell's scope)
    for csv_file_path in "${csv_files[@]}"; do
        # ALL variable modifications now happen in the main script scope!
        total_tests=$((total_tests + 1))
        
        local csv_file="${csv_file_path}"
        local current_config="$client_config"
        
        # Apply realpath_compat and cygpath conversion for executable arguments
        local current_csv_file
        current_csv_file=$(realpath_compat "$csv_file")
        if [ "$use_cygpath" -eq 1 ]; then
            current_csv_file=$(cygpath -w "$current_csv_file")
            current_config=$(cygpath -w "$current_config")
        fi

        rm -f "$output_path"

        "$executable" "$current_csv_file" "$accept_crates" "$win_output_path" > /dev/null 2>&1

        if [ -f "$output_path" ]; then
            expected_output_path="$(dirname "$csv_file")/expected_output.json"
            
            local PYTHON_FLAG=""
            # Pass the -f flag to Python ONLY when ONLY_FAILED is true
            if [ "$ONLY_FAILED" -eq 1 ]; then
                PYTHON_FLAG="-f"
            fi
            
            # Execute comparison. Redirect stdout to /dev/null ONLY IF the -f flag is used.
            local REDIRECT=""
            if [ "$ONLY_FAILED" -eq 1 ]; then
                REDIRECT="> /dev/null"
            fi

            # Using eval to execute the dynamically constructed command
            if eval "python3 testing/compare.py \"$output_path\" \"$expected_output_path\" $PYTHON_FLAG $REDIRECT 2> \"$PYTHON_ERRORS_FILE\""; then
                if [ "$ONLY_FAILED" -eq 0 ]; then
                    # Restore original success message for non-f runs
                    echo -e "${GREEN}TEST PASSED:${NC} $csv_file"
                fi
                passed_tests=$((passed_tests + 1))
                passed_paths+=("$csv_file")
            else
                # A failed test: print newline, failed status, AND the captured errors
                echo -e "\n${RED}TEST FAILED:${NC} $csv_file"
                echo -e "${YELLOW}EXPECTED OUTPUT PATH:${NC} $expected_output_path" # <--- ADDED LINE
                cat "$PYTHON_ERRORS_FILE" # Print the detailed errors from Python

                failed_tests=$((failed_tests + 1))
                failed_paths+=("$csv_file")
            fi
        else
            # --- FIX 2: A failure because the output file is missing (Executable failed) ---
            echo -e "\n${YELLOW}FAILURE WITH $csv_file:${NC} Output file does not exist."
            echo -e "${YELLOW}EXPECTED OUTPUT PATH:${NC} $output_path" # Print the actual intended output path
            # -----------------------------------------------------------------------------
            failed_tests=$((failed_tests + 1))
            failed_paths+=("$csv_file")
        fi
        rm -f "$PYTHON_ERRORS_FILE" # Clean up the temporary file
    done
    
    # Removed the final echo "" cleanup line
}

# Get the absolute directory path of the script itself (e.g., C:/Users/me/class/project)
SCRIPT_DIR="$(dirname "$(realpath "$0")")"

# Go up one level (../) to C:/Users/me/class, then append test_cases.
INPUT_DIR=$(realpath_compat "$SCRIPT_DIR/../test_cases")

# For verification: echo "Input Dir is: $INPUT_DIR"
# You should see: C:/Users/me/class/test_cases

# pass client configuration that does not allow crates
process_csv_files "$INPUT_DIR/box_packing" "N"
process_csv_files "$INPUT_DIR/pallet_packing" "N"
# pass client configuration that allows crates
process_csv_files "$INPUT_DIR/crate_packing" "Y"
process_csv_files "$INPUT_DIR/stress_tests" "N"

# Print summary after all calls
if [ "$ONLY_FAILED" -eq 0 ] && [ $passed_tests -gt 0 ]; then # only print information for successful tests when extra flag not passed
    echo -e "\n\n${GREEN}\033[1m--------------- PASSED TEST PATHS ---------------\033[0m${NC}"
    for path in "${passed_paths[@]}"; do
        echo "$path"
    done
fi

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