#!/bin/bash
# Assembly compilation script

set -e

if [ $# -lt 2 ]; then
    echo "Usage: $0 <input.asm> <output.o> [flags]"
    exit 1
fi

INPUT="$1"
OUTPUT="$2"
FLAGS="${@:3}"

echo "📝 Compiling: $INPUT"

if [ ! -f "$INPUT" ]; then
    echo "❌ File not found: $INPUT"
    exit 1
fi

# Compile using nasm
nasm -f elf64 $FLAGS -o "$OUTPUT" "$INPUT"

if [ -f "$OUTPUT" ]; then
    SIZE=$(du -h "$OUTPUT" | cut -f1)
    echo "✓ Compiled: $OUTPUT ($SIZE)"
else
    echo "❌ Compilation failed"
    exit 1
fi
