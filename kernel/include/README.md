# Kernel Headers

All public kernel header files and interfaces.

## Subdirectories

- `core/` - Core kernel services (kernel.h, serial.h, framebuffer.h)
- `util/` - Utility functions (kprintf.h, string functions)
- `arch/` - Architecture-specific (x86_64)
- `proc/` - Process/thread management (Phase 5+)
- `mem/` - Memory management (Phase 3+)
- `sync/` - Synchronization primitives (Phase 9+)

## Phase 1 Headers

✅ `kernel/core/kernel.h` - Kernel core structures  
✅ `kernel/core/serial.h` - Serial port interface  
✅ `kernel/core/framebuffer.h` - Framebuffer interface  
✅ `kernel/util/kprintf.h` - Kernel printf  
✅ `types.h` - Basic type definitions  
✅ `string.h` - String/memory operations  
