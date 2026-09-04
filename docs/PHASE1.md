# PHASE 1: Boot + Kernel Entry + Serial + Framebuffer

## Overview

Phase 1 implements the core bootloader integration and early kernel initialization:

1. **Bootloader Protocol** - Limine bootloader requests and responses
2. **Kernel Entry Point** - Assembly entry point and stack setup
3. **Serial Output** - COM1 (0x3F8) serial port initialization and I/O
4. **Framebuffer** - Graphics framebuffer detection and initialization
5. **Kernel Main** - Early kernel initialization gathering bootloader info

## Components

### Boot Files

- `boot/src/entry.asm` - Kernel entry point (called by Limine)
- `boot/src/limine_requests.c` - Limine bootloader requests
- `boot/include/limine.h` - Limine protocol definitions

### Kernel Core

- `kernel/src/core/kernel.c` - Main kernel initialization
- `kernel/src/core/serial.c` - Serial port driver (COM1)
- `kernel/src/core/framebuffer.c` - Framebuffer operations
- `kernel/src/util/kprintf.c` - Kernel printf for debugging

### Headers

- `kernel/include/kernel/core/kernel.h`
- `kernel/include/kernel/core/serial.h`
- `kernel/include/kernel/core/framebuffer.h`
- `kernel/include/kernel/util/kprintf.h`
- `kernel/include/types.h` - Basic type definitions
- `kernel/include/string.h` - String/memory operations

## Bootloader Flow

```
Limine Bootloader
    |
    v
    +--> Initialize x86_64
    |
    +--> Load kernel ELF
    |
    +--> Process requests (framebuffer, memmap, etc.)
    |
    +--> Jump to kernel_entry (boot/src/entry.asm)
         |
         +--> Disable interrupts (cli)
         |
         +--> Set up kernel stack
         |
         +--> Call kernel_main()
```

## Kernel Initialization

In `kernel/src/core/kernel.c`, `kernel_main()` performs:

1. **Serial Initialization** - Set up COM1 for debug output
2. **Bootloader Info** - Display bootloader name and version
3. **Kernel Address Info** - Virtual/physical base addresses
4. **Memory Map** - Display total usable RAM
5. **Framebuffer Setup** - Initialize graphics output
6. **Early Tests** - Verify all subsystems operational

## Building

```bash
# Configure and build
make configure
make build

# Create ISO
make iso

# Run in QEMU
make qemu
```

## Serial Output

Serial port configuration:
- **Port**: 0x3F8 (COM1)
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None

Output automatically goes to QEMU console/stdout.

## Framebuffer

The framebuffer is detected via Limine and provides:
- Resolution detection
- Pixel writing
- Rectangle fills
- Basic text rendering

## Testing

You should see:

```
========================================
  Hybrid OS v0.1.0
  Boot: Kernel Entry Reached
========================================

[BOOT] Bootloader: Limine v...
[BOOT] Kernel Virtual Base: 0xffffffff80000000
[BOOT] Kernel Physical Base: 0x100000
[BOOT] Memory Map Entries: ...
[BOOT] Total Usable RAM: ...
[BOOT] Initializing framebuffer...
[BOOT] Framebuffer found:
       Resolution: ...x...
       Pitch: ...
       BPP: ...
       Address: 0x...
[BOOT] Framebuffer initialized
[BOOT] Kernel initialization complete!
[BOOT] Awaiting Phase 2 implementation (GDT, IDT, Exceptions)
```

## Hardware Gate

Phase 1 Complete when:
- ✅ Limine bootloader successfully boots kernel
- ✅ Serial output displays boot messages
- ✅ Framebuffer initializes (if available)
- ✅ Memory information displays correctly
- ✅ Kernel can halt gracefully

## Next: Phase 2

Phase 2 will implement:
- GDT (Global Descriptor Table) setup
- IDT (Interrupt Descriptor Table) setup
- Exception handling
- Interrupt handlers
- Safe exception triggering for testing
