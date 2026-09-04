#!/bin/bash
# ISO creation script using xorriso

set -e

if [ $# -ne 2 ]; then
    echo "Usage: $0 <kernel_elf> <output_iso>"
    exit 1
fi

KERNEL_ELF="$1"
OUTPUT_ISO="$2"
BUILD_DIR="$(dirname "$OUTPUT_ISO")"
ISO_STAGE="${BUILD_DIR}/iso_stage"

echo "📦 Creating bootable ISO image..."

# Check if kernel exists
if [ ! -f "$KERNEL_ELF" ]; then
    echo "❌ Kernel ELF not found: $KERNEL_ELF"
    exit 1
fi

# Copy kernel to ISO staging area
echo "✓ Staging kernel..."
cp "$KERNEL_ELF" "${ISO_STAGE}/boot/hybrid-os.elf"

# Create Limine configuration if not present
if [ ! -f "${ISO_STAGE}/boot/limine.cfg" ]; then
    echo "✓ Creating Limine configuration..."
    cat > "${ISO_STAGE}/boot/limine.cfg" << 'EOF'
TIMEOUT=3

:Hybrid OS
PROTOCOL=multiboot2
KERNEL_PATH=boot:///boot/hybrid-os.elf
MODULE_PATH=boot:///boot/hybrid-os.elf
EOF
fi

# Create ISO using xorriso
echo "✓ Building ISO with xorriso..."
xorriso -as mkisofs \
    -R -J \
    -boot_image grub embedded \
    -boot_image grub boot_info_table \
    -eltorito-boot boot/grub/i386-pc/eltorito.img \
    -eltorito-catalog boot/grub/i386-pc/boot.cat \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    -isohybrid-mbr /usr/lib/grub/i386-pc/boot_hybrid.img \
    -output "$OUTPUT_ISO" \
    "$ISO_STAGE" 2>/dev/null || {
    echo "⚠️  xorriso with GRUB failed, using simpler ISO creation..."
    xorriso -as mkisofs \
        -R -J \
        -output "$OUTPUT_ISO" \
        "$ISO_STAGE"
}

if [ -f "$OUTPUT_ISO" ]; then
    SIZE=$(du -h "$OUTPUT_ISO" | cut -f1)
    echo "✅ ISO created successfully: $OUTPUT_ISO ($SIZE)"
else
    echo "❌ Failed to create ISO"
    exit 1
fi
