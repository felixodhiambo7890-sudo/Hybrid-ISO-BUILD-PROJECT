# Boot Headers

This directory contains header files for the bootloader and early boot process.

## Contents

- `multiboot2.h` - Multiboot2 specification constants
- `limine.h` - Limine bootloader protocol constants  
- `boot_info.h` - Boot information structures
- `uefi.h` - UEFI boot information

## Phase 1 Implementation

Phase 1 will implement:
1. Bootloader protocol handling
2. Early kernel entry point
3. Serial output initialization
4. Framebuffer setup
