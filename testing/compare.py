import json
import sys
from typing import Any, Dict, List, Tuple

# --- GLOBAL FLAG TO CONTROL OUTPUT ---
# Check sys.argv for the '-f' or '--only-failed' flag
ONLY_FAILED = '-f' in sys.argv or '--only-failed' in sys.argv

def load_json(filepath: str) -> Dict:
    """Load JSON data from a file."""
    try:
        with open(filepath, 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        print(f"Error: File '{filepath}' not found.", file=sys.stderr)
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error: Invalid JSON in '{filepath}': {e}", file=sys.stderr)
        sys.exit(1)

def compare_values(path: str, actual_val: Any, expected_val: Any, errors: List[str]) -> bool:
    """Compare two values and record any differences."""
    if type(actual_val) != type(expected_val):
        if (type(actual_val) == int and type(expected_val) == float):
            pass  # Allow int and float comparison
        else:
            errors.append(f"{path}: Type mismatch - got {type(actual_val).__name__}, expected {type(expected_val).__name__}")
            return False
    
    if isinstance(actual_val, dict):
        return compare_dicts(path, actual_val, expected_val, errors)
    elif isinstance(actual_val, list):
        return compare_lists(path, actual_val, expected_val, errors)
    else:
        # Convert strings to compare numerically if possible
        try:
            actual_num = float(str(actual_val))
            expected_num = float(str(expected_val))
            if abs(actual_num - expected_num) > 1e-9: # Using small tolerance for float comparison
                errors.append(f"{path}: Value mismatch - got '{actual_val}', expected '{expected_val}'")
                return False
        except (ValueError, TypeError):
            if actual_val != expected_val:
                errors.append(f"{path}: Value mismatch - got '{actual_val}', expected '{expected_val}'")
                return False
    
    return True

def compare_dicts(path: str, actual: Dict, expected: Dict, errors: List[str]) -> bool:
    """Compare two dictionaries recursively. Expected can be a subset of actual."""
    all_match = True
    
    # Check for missing keys in actual output
    missing_keys = set(expected.keys()) - set(actual.keys())
    for key in missing_keys:
        errors.append(f"{path}.{key}: Missing required key")
        all_match = False
    
    # Note: We no longer check for extra keys - actual can have more fields than expected
    
    # Compare common keys
    for key in set(actual.keys()) & set(expected.keys()):
        new_path = f"{path}.{key}" if path else key
        if not compare_values(new_path, actual[key], expected[key], errors):
            all_match = False
    
    return all_match

def compare_lists(path: str, actual: List, expected: List, errors: List[str]) -> bool:
    """Compare two lists."""
    all_match = True
    
    if len(actual) != len(expected):
        errors.append(f"{path}: List length mismatch - got {len(actual)} items, expected {len(expected)} items")
        all_match = False
    
    # Compare each element
    for i in range(min(len(actual), len(expected))):
        new_path = f"{path}[{i}]"
        if not compare_values(new_path, actual[i], expected[i], errors):
            all_match = False
    
    return all_match

def compare_json_files(actual_file: str, expected_file: str) -> Tuple[bool, List[str]]:
    """Compare actual JSON output with expected JSON."""
    actual_data = load_json(actual_file)
    expected_data = load_json(expected_file)
    
    errors = []
    match = compare_dicts("", actual_data, expected_data, errors)
    
    return match, errors

def main():
    # Filter out flags to get only the file paths for a cleaner check
    file_args = [arg for arg in sys.argv[1:] if arg not in ('-f', '--only-failed')]
    
    # Check for correct number of file arguments
    if len(file_args) != 2:
        print("Usage: python compare_json.py <actual_file.json> <expected_file.json> [-f|--only-failed]", file=sys.stderr)
        sys.exit(1)
    
    actual_file = file_args[0]
    expected_file = file_args[1]
    
    # Only print the comparison files if NOT in only-failed mode
    if not ONLY_FAILED:
        # Print to stdout, which is visible by default (and not redirected by the bash script)
        print(f"Comparing:\n  Actual:  {actual_file}\n  Expected: {expected_file}\n")
    
    match, errors = compare_json_files(actual_file, expected_file)
    
    if match:
        # Only print PASS message if NOT in only-failed mode
        if not ONLY_FAILED:
            # Print to stdout
            print("PASS: Actual output matches expected output!")
        # Always exit successfully on match
        sys.exit(0)
    else:
        # Always print FAIL message and errors to stderr, which is captured by the bash script
        print(f"FAIL: Found {len(errors)} difference(s):\n", file=sys.stderr)
        for error in errors:
            print(f"  • {error}", file=sys.stderr)
        # Always exit with failure code
        sys.exit(1)

if __name__ == "__main__":
    main()