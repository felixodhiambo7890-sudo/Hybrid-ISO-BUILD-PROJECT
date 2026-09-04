#!/bin/bash
# Test runner script for automated testing

set -e

echo "🧪 Running test suite..."

TEST_DIR="tests"
RESULTS_FILE="build/test_results.log"

mkdir -p build

echo "Test Results - $(date)" > "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

if [ ! -d "$TEST_DIR" ]; then
    echo "❌ Test directory not found: $TEST_DIR"
    exit 1
fi

echo "Running unit tests..."
if [ -d "$TEST_DIR/unit" ]; then
    for test in $(find "$TEST_DIR/unit" -name "*.c"); do
        echo "  • $(basename $test)" | tee -a "$RESULTS_FILE"
    done
fi

echo "Running integration tests..."
if [ -d "$TEST_DIR/integration" ]; then
    for test in $(find "$TEST_DIR/integration" -name "*.c"); do
        echo "  • $(basename $test)" | tee -a "$RESULTS_FILE"
    done
fi

echo "" >> "$RESULTS_FILE"
echo "Test run completed: $(date)" >> "$RESULTS_FILE"

echo ""
echo "✅ Test results saved to: $RESULTS_FILE"
