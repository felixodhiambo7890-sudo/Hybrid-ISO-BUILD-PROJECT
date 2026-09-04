#!/bin/bash
# Clean build script - removes all build artifacts

set -e

echo "🧹 Cleaning build artifacts..."

# Remove build directory
if [ -d "build" ]; then
    rm -rf build
    echo "✓ Removed build/ directory"
fi

# Remove generated files
if [ -d "kernel/src" ]; then
    find kernel/src -name "*.o" -delete 2>/dev/null || true
    find kernel/src -name "*.d" -delete 2>/dev/null || true
    echo "✓ Removed kernel object files"
fi

if [ -d "boot" ]; then
    find boot -name "*.o" -delete 2>/dev/null || true
    find boot -name "*.d" -delete 2>/dev/null || true
    echo "✓ Removed boot object files"
fi

echo "✅ Clean complete"
echo "Run 'make all' to rebuild"
