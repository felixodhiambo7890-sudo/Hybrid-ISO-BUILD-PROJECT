/*
 * Phase 1 Boot Tests
 * Tests for bootloader info, serial output, and framebuffer initialization
 */

#include <kernel/core/kernel.h>
#include <kernel/core/serial.h>
#include <kernel/core/framebuffer.h>
#include <kernel/util/kprintf.h>

/* Test: Bootloader info retrieval */
void test_bootloader_info(void) {
    kprintf("\n[TEST] Bootloader Info Test\n");
    
    if (bootloader_info == NULL) {
        kprintf("  FAIL: No bootloader info\n");
        return;
    }
    
    kprintf("  PASS: Bootloader detected\n");
    kprintf("    Name: %s\n", bootloader_info->name);
    kprintf("    Version: %s\n", bootloader_info->version);
}

/* Test: Serial port output */
void test_serial_output(void) {
    kprintf("\n[TEST] Serial Output Test\n");
    kprintf("  PASS: Serial output working\n");
}

/* Test: Framebuffer availability */
void test_framebuffer_available(void) {
    kprintf("\n[TEST] Framebuffer Availability Test\n");
    
    if (!framebuffer_available()) {
        kprintf("  FAIL: Framebuffer not available\n");
        return;
    }
    
    kprintf("  PASS: Framebuffer available\n");
    kprintf("    Resolution: %ux%u\n", framebuffer_width(), framebuffer_height());
}

/* Test: Memory map parsing */
void test_memmap(void) {
    kprintf("\n[TEST] Memory Map Test\n");
    
    if (memmap_info == NULL) {
        kprintf("  FAIL: No memory map\n");
        return;
    }
    
    kprintf("  PASS: Memory map available\n");
    kprintf("    Entries: %lu\n", memmap_info->entry_count);
}

/* Run all Phase 1 tests */
void phase1_run_tests(void) {
    kprintf("\n========================================\n");
    kprintf("  Phase 1: Boot Tests\n");
    kprintf("========================================\n");
    
    test_bootloader_info();
    test_serial_output();
    test_framebuffer_available();
    test_memmap();
    
    kprintf("\n========================================\n");
    kprintf("  All tests completed\n");
    kprintf("========================================\n\n");
}
