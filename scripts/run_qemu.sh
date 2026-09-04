#!/bin/bash
# QEMU emulator launcher script

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <iso_image>"
    exit 1
fi

ISO_IMAGE="$1"

# Check if ISO exists
if [ ! -f "$ISO_IMAGE" ]; then
    echo "❌ ISO image not found: $ISO_IMAGE"
    exit 1
fi

echo "🚀 Launching QEMU..."
echo "  ISO: $ISO_IMAGE"
echo "  RAM: 2G"
echo "  CPUs: 4 (2 cores each)"
echo "  Serial: stdio"
echo ""
echo "Press Ctrl+Alt+G to ungrab mouse, Ctrl+A then X to exit"
echo ""

# Launch QEMU
qemu-system-x86_64 \
    -m 2G \
    -smp 4,cores=2 \
    -cdrom "$ISO_IMAGE" \
    -serial stdio \
    -d int \
    -no-reboot \
    -monitor none \
    -boot d \
    2>&1 | tee -a logs/qemu.log

echo ""
echo "QEMU session ended"
