# PHASE 3: Physical & Virtual Memory, Paging, Heap

## Overview

Phase 3 implements complete memory management for the kernel:

1. **Physical Memory Manager (PMM)** - Bitmap-based page allocator
2. **Virtual Memory Manager (VMM)** - x86_64 paging support
3. **Paging** - 4-level page table hierarchy
4. **Heap Allocator** - malloc/free/calloc/realloc
5. **Memory Mapping** - Virtual to physical address translation

## Components

### Physical Memory (PMM)

**Files:**
- `kernel/src/mem/pmm.c` - Implementation
- `kernel/include/kernel/mem/pmm.h` - Interface

**Features:**
- Bitmap allocator for physical pages (4 KB)
- O(n) allocation (scan for free pages)
- Tracks total and free pages
- Supports up to 128 MB of physical memory (16 KB bitmap)

**API:**
```c
void pmm_init(void);
                        /* Initialize from Limine memory map */

uint64_t pmm_allocate_page(void);
                        /* Allocate single 4 KB page */

void pmm_free_page(uint64_t phys_addr);
                        /* Free physical page */

uint64_t pmm_get_free_pages(void);
uint64_t pmm_get_total_pages(void);
                        /* Get statistics */
```

### Virtual Memory (VMM)

**Files:**
- `kernel/src/mem/vmm.c` - Implementation
- `kernel/include/kernel/mem/vmm.h` - Interface

**Features:**
- x86_64 4-level paging (PML4 -> PDPT -> PD -> PT)
- On-demand page table allocation
- Page mapping with access flags
- Address translation

**Page Table Hierarchy:**
```
┌─────────────────────────────────────┐
│  PML4 (512 entries)                 │  Bits 63-48: Sign extension
│  Covers 128 TB virtual space        │  Bits 47-39: PML4 index
├─────────────────────────────────────┤
│  PDPT (512 entries per PML4)        │  Bits 38-30: PDPT index
│  Covers 256 GB per PML4 entry       │
├─────────────────────────────────────┤
│  PD (512 entries per PDPT)          │  Bits 29-21: PD index
│  Covers 512 MB per PDPT entry       │
├─────────────────────────────────────┤
│  PT (512 entries per PD)            │  Bits 20-12: PT index
│  Covers 1 MB per PD entry           │  Bits 11-0:  Page offset
├─────────────────────────────────────┤
│  Page Frame (4 KB)                  │
│  Actual memory page                 │
└─────────────────────────────────────┘
```

**Page Table Entry Flags:**
```
Bit 0:  Present (P)
Bit 1:  Writable (W/R)
Bit 2:  User/Supervisor (U/S)
Bit 3:  Write-through (PWT)
Bit 4:  Cache disable (PCD)
Bit 5:  Accessed (A)
Bit 6:  Dirty (D)
Bit 7:  Page size (PS) - 0 for 4 KB
Bit 8:  Global (G)
Bits 51-12: Physical page address
Bits 62-52: Available
Bit 63: Execute disable (XD)
```

### Heap Allocator

**Files:**
- `kernel/src/mem/heap.c` - Implementation
- `kernel/include/kernel/mem/heap.h` - Interface

**Features:**
- Linked list of free blocks
- First-fit allocation strategy
- Automatic block splitting
- Block coalescing on free
- Supports malloc/free/calloc/realloc

**Block Structure:**
```c
struct heap_block {
    uint64_t size;              /* Total block size (including header) */
    int used;                   /* Allocated? */
    struct heap_block *next;    /* Next block */
    struct heap_block *prev;    /* Previous block */
};
```

**Allocation Strategy:**
1. Find first free block large enough
2. Split if block is much larger than needed
3. Mark as used
4. Return pointer after header

**Deallocation Strategy:**
1. Mark block as free
2. Coalesce with next free block
3. Coalesce with previous free block
4. Return space to free pool

**API:**
```c
void heap_init(uint64_t initial_size);
                        /* Initialize heap */

void *malloc(size_t size);
                        /* Allocate memory */

void free(void *ptr);
                        /* Free memory */

void *calloc(size_t count, size_t size);
                        /* Allocate and zero */

void *realloc(void *ptr, size_t size);
                        /* Resize allocation */

uint64_t heap_get_free(void);
uint64_t heap_get_used(void);
                        /* Get statistics */
```

### Memory Manager (Main)

**Files:**
- `kernel/src/mem/mem.c` - Implementation
- `kernel/include/kernel/mem/mem.h` - Interface

**Initialization Order:**
1. PMM (parses memory map)
2. VMM (sets up paging)
3. Heap (allocator for kernel data structures)

## Memory Layout (x86_64 Hybrid OS)

```
┌──────────────────────────────────────────┐
│ Virtual Address Space                    │  64-bit (48-bit used)
├──────────────────────────────────────────┤
│ 0xFFFFFFFF80000000 - 0xFFFFFFFFFFFFFFFF  │  Kernel space (higher half)
│   - Kernel code
│   - Kernel data
│   - Kernel heap
│   - Page tables
├──────────────────────────────────────────┤
│ 0x0000000000000000 - 0x00007FFFFFFFFFFF │  User space (lower half)
│   - User processes
│   - User heap/stack
│   - Shared libraries
└──────────────────────────────────────────┘
```

## Page Size & Statistics

- **Page Size:** 4 KB (4096 bytes)
- **Total Virtual Space:** 256 TB (48-bit addressing)
- **Kernel Space:** 128 TB (higher half)
- **User Space:** 128 TB (lower half)
- **PMM Bitmap:** 16 KB (supports 128 MB)
- **Initial Heap:** 1 MB

## Testing

```bash
# Build and run
make clean-all
make all
make qemu
```

**Expected output:**

```
========================================
  Phase 3: Memory Management
========================================

[PMM] Initializing Physical Memory Manager
[PMM] Usable: 0x0000000000001000 - 0x00000000DFFFFFFF (0xDFFFE pages)
[PMM] Total Pages: 0xE0000 (917 MB)
[PMM] Free Pages: 0xDFFFE (917 MB - overhead)
[PMM] Physical Memory Manager initialized

[VMM] Initializing Virtual Memory Manager
[VMM] Virtual Memory Manager initialized
[VMM] Paging enabled

[HEAP] Initializing Kernel Heap
[HEAP] Initial size: 1024 KB
[HEAP] Kernel heap initialized at 0x...
[HEAP] Total heap: 1024 KB, Free: 1024 KB

========================================
  Memory Management Initialized
========================================
```

## Memory Limits

- **Physical Memory Supported:** Up to 128 MB (PMM bitmap size)
- **Initial Heap:** 1 MB (expandable)
- **Page Table Entries:** 512 per level
- **Maximum Virtual Address:** 2^48 - 1 bytes

## Future Improvements (Phase 4+)

- Expandable heap (allocate more pages on demand)
- Better page table management (O(log n) lookup)
- Memory statistics and profiling
- Garbage collection or reference counting
- Virtual address space layout randomization (ASLR)
- Copy-on-write for fork() implementation

## Hardware Gate

Phase 3 Complete when:
- ✅ PMM initializes from memory map
- ✅ Physical page allocation works
- ✅ VMM sets up paging
- ✅ Heap allocation works (malloc/free)
- ✅ Memory statistics display correctly
- ✅ No page faults during boot

## Next: Phase 4

Phase 4 will implement:
- Timer subsystem (PIT/APIC)
- CPU detection (CPUID)
- SMP (Symmetric Multiprocessing) initialization
- Per-CPU structures
- Multi-core boot sequence
