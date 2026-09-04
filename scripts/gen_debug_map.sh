#!/bin/bash
# Generate debug symbol map from kernel ELF

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <kernel.elf>"
    exit 1
fi

KERNEL_ELF="$1"
OUTPUT="${KERNEL_ELF%.elf}.sym"

echo "📍 Generating debug symbol map..."

if [ ! -f "$KERNEL_ELF" ]; then
    echo "❌ Kernel ELF not found: $KERNEL_ELF"
    exit 1
fi

# Extract symbols
x86_64-elf-objdump -t "$KERNEL_ELF" | grep -E "^[0-9a-f]+" > "$OUTPUT"

if [ -f "$OUTPUT" ]; then
    echo "✓ Symbol map created: $OUTPUT"
else
    echo "❌ Failed to create symbol map"
    exit 1
fi
