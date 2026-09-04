#include <kernel/mem/mem.h>
#include <kernel/mem/pmm.h>
#include <kernel/mem/vmm.h>
#include <kernel/mem/heap.h>
#include <kernel/util/kprintf.h>

void mem_init(void) {
    kprintf("\n========================================\n");
    kprintf("  Phase 3: Memory Management\n");
    kprintf("========================================\n\n");
    
    /* Initialize physical memory manager */
    pmm_init();
    
    /* Initialize virtual memory manager */
    vmm_init();
    
    /* Initialize heap allocator */
    heap_init(1024 * 1024);  /* 1 MB initial heap */
    
    kprintf("\n========================================\n");
    kprintf("  Memory Management Initialized\n");
    kprintf("========================================\n\n");
}
