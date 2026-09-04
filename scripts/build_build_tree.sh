#!/bin/bash
# Build tree structure setup script

set -e

echo "🔨 Setting up Hybrid OS build system..."

# Check for required tools
echo "✓ Checking for required tools..."
command -v x86_64-elf-gcc >/dev/null 2>&1 || { echo "❌ x86_64-elf-gcc not found. Install cross-compiler."; exit 1; }
command -v cmake >/dev/null 2>&1 || { echo "❌ cmake not found. Install CMake."; exit 1; }
command -v xorriso >/dev/null 2>&1 || { echo "❌ xorriso not found. Install xorriso."; exit 1; }
command -v qemu-system-x86_64 >/dev/null 2>&1 || { echo "❌ qemu-system-x86_64 not found. Install QEMU."; exit 1; }

echo "✓ All required tools found"

# Create build directory
echo "✓ Creating build directory..."
mkdir -p build

# Create logs directory
echo "✓ Creating logs directory..."
mkdir -p logs/kernel logs/boot logs/services logs/drivers logs/security logs/updates logs/ai

# Create ISO staging directory
echo "✓ Creating ISO staging directory..."
mkdir -p build/iso_stage/boot build/iso_stage/EFI/BOOT

echo "✅ Build system setup complete!"
echo ""
echo "Next steps:"
echo "  1. Run: make configure"
echo "  2. Run: make build"
echo "  3. Run: make iso"
echo "  4. Run: make qemu"
echo ""
