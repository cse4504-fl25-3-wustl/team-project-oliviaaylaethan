#!/bin/bash

# Build and Test Script
# This script builds the project with parallel compilation and runs all tests

set -e  # Exit on any error

echo "Building project..."
cd build
cmake --build . --parallel 8
cd ../

echo "Running tests..."
./run_all_tests.sh -f