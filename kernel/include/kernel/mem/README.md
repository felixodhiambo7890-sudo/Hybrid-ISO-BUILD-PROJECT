# Memory Management

Memory subsystem including PMM, VMM, and heap.

## Subdirectories

None yet - all memory code in kernel/src/mem/ and kernel/include/kernel/mem/

## Files (Phase 3+)

### Physical Memory Manager
- `mem/pmm.c` - Bitmap allocator
- `mem/pmm.h` - PMM interface

### Virtual Memory Manager
- `mem/vmm.c` - Paging and address translation
- `mem/vmm.h` - VMM interface

### Heap Allocator
- `mem/heap.c` - malloc/free implementation
- `mem/heap.h` - Heap interface

### Main Memory Manager
- `mem/mem.c` - Memory subsystem initialization
- `mem/mem.h` - Main interface

## Phase 3 Implementation

✅ Physical Memory Manager (PMM)  
✅ Virtual Memory Manager (VMM)  
✅ Paging (4-level page tables)  
✅ Heap allocator (malloc/free)  
✅ Memory mapping  
✅ Address translation  
