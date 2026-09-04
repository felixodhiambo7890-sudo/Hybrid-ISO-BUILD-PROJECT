# Boot Files

Bootloader and early kernel entry code.

## Files

- `entry.asm` - Kernel entry point (called by Limine bootloader)
- `limine_requests.c` - Limine bootloader protocol requests
- `limine.h` - Limine protocol definitions and structures

## Phase 1 Status

✅ Limine bootloader integration  
✅ Kernel entry point (x86_64 assembly)  
✅ Stack setup  
✅ Call to kernel_main()  
