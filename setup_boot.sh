#!/bin/bash
# Initial boot environment setup

set -e

echo "⚙️  Setting up Hybrid OS boot environment..."

# Check system requirements
echo "✓ Checking system requirements..."

REQUIRED_TOOLS=("cmake" "make" "x86_64-elf-gcc" "xorriso" "qemu-system-x86_64" "nasm")

for tool in "${REQUIRED_TOOLS[@]}"; do
    if ! command -v "$tool" &> /dev/null; then
        echo "❌ Missing: $tool"
        echo "Install instructions:"
        echo "  Ubuntu/Debian: sudo apt install build-essential cross-compiler cmake xorriso qemu-system-x86 nasm"
        echo "  Fedora: sudo dnf install gcc gcc-c++ cmake xorriso qemu-system-x86 nasm"
        exit 1
    fi
done

echo "✓ All required tools found"

# Create directory structure
echo "✓ Creating directory structure..."
bash scripts/build_build_tree.sh

# Display information
echo ""
echo "📋 System Information:"
echo "  CMake version: $(cmake --version | head -n1)"
echo "  GCC version: $(x86_64-elf-gcc --version | head -n1)"
echo "  QEMU version: $(qemu-system-x86_64 --version | head -n1)"
echo ""
echo "✅ Boot environment ready!"
echo ""
echo "Quick start:"
echo "  1. make configure   # Configure build system"
echo "  2. make build       # Build kernel and bootloader"
echo "  3. make iso         # Create ISO image"
echo "  4. make qemu        # Run in QEMU emulator"
echo ""
echo "Or simply: make all && make qemu"
echo ""
