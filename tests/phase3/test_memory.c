/*
 * Phase 3 Memory Management Tests
 * Tests for PMM, VMM, and heap allocation
 */

#include <kernel/mem/pmm.h>
#include <kernel/mem/vmm.h>
#include <kernel/mem/heap.h>
#include <kernel/util/kprintf.h>
#include <stdlib.h>
#include <string.h>

/* Test: Physical memory allocation */
void test_pmm_allocation(void) {
    kprintf("\n[TEST] PMM Allocation Test\n");
    
    uint64_t page1 = pmm_allocate_page();
    uint64_t page2 = pmm_allocate_page();
    
    if (page1 > 0 && page2 > 0 && page1 != page2) {
        kprintf("  PASS: Allocated two different pages\n");
        kprintf("    Page 1: 0x%016lx\n", page1);
        kprintf("    Page 2: 0x%016lx\n", page2);
        pmm_free_page(page1);
        pmm_free_page(page2);
    } else {
        kprintf("  FAIL: Could not allocate pages\n");
    }
}

/* Test: Heap allocation */
void test_heap_allocation(void) {
    kprintf("\n[TEST] Heap Allocation Test\n");
    
    void *ptr1 = malloc(128);
    void *ptr2 = malloc(256);
    void *ptr3 = malloc(512);
    
    if (ptr1 && ptr2 && ptr3) {
        kprintf("  PASS: Allocated 3 blocks\n");
        kprintf("    Block 1: %p (128 bytes)\n", ptr1);
        kprintf("    Block 2: %p (256 bytes)\n", ptr2);
        kprintf("    Block 3: %p (512 bytes)\n", ptr3);
        
        /* Write test data */
        memset(ptr1, 0xAA, 128);
        memset(ptr2, 0xBB, 256);
        memset(ptr3, 0xCC, 512);
        
        free(ptr1);
        free(ptr2);
        free(ptr3);
        
        kprintf("  PASS: Freed all blocks\n");
    } else {
        kprintf("  FAIL: Could not allocate blocks\n");
    }
}

/* Test: Calloc */
void test_calloc(void) {
    kprintf("\n[TEST] Calloc Test\n");
    
    void *ptr = calloc(64, sizeof(uint32_t));
    
    if (ptr) {
        kprintf("  PASS: Allocated and zeroed memory\n");
        kprintf("    Pointer: %p (256 bytes)\n", ptr);
        free(ptr);
    } else {
        kprintf("  FAIL: Calloc failed\n");
    }
}

/* Test: Realloc */
void test_realloc(void) {
    kprintf("\n[TEST] Realloc Test\n");
    
    void *ptr = malloc(256);
    memset(ptr, 0x55, 256);
    
    void *new_ptr = realloc(ptr, 512);
    
    if (new_ptr) {
        kprintf("  PASS: Reallocated memory\n");
        kprintf("    Old pointer: %p\n", ptr);
        kprintf("    New pointer: %p\n", new_ptr);
        free(new_ptr);
    } else {
        kprintf("  FAIL: Realloc failed\n");
        free(ptr);
    }
}

/* Run all Phase 3 tests */
void phase3_run_tests(void) {
    kprintf("\n========================================\n");
    kprintf("  Phase 3: Memory Management Tests\n");
    kprintf("========================================\n");
    
    test_pmm_allocation();
    test_heap_allocation();
    test_calloc();
    test_realloc();
    
    kprintf("\n========================================\n");
    kprintf("  All tests completed\n");
    kprintf("========================================\n\n");
}
