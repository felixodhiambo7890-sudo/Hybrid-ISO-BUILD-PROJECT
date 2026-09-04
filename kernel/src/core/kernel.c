#include <kernel/core/kernel.h>
#include <kernel/core/serial.h>
#include <kernel/core/framebuffer.h>
#include <kernel/core/cpu.h>
#include <kernel/util/kprintf.h>
#include <config.h>
#include <limine.h>

/* Bootloader info stored globally */
volatile struct limine_bootloader_info_response *bootloader_info = NULL;
volatile struct limine_framebuffer_response *framebuffer_info = NULL;
volatile struct limine_memmap_response *memmap_info = NULL;
volatile struct limine_kernel_address_response *kernel_addr = NULL;

/* Forward declarations from limine_requests.c */
extern volatile struct limine_bootloader_info_response *get_bootloader_info(void);
extern volatile struct limine_framebuffer_response *get_framebuffer(void);
extern volatile struct limine_memmap_response *get_memmap(void);
extern volatile struct limine_kernel_address_response *get_kernel_address(void);

void kernel_main(void) {
    /* Initialize serial output first */
    serial_init();
    
    kprintf("\n========================================\n");
    kprintf("  %s v%s\n", KERNEL_NAME, KERNEL_VERSION);
    kprintf("  Boot: Kernel Entry Reached\n");
    kprintf("========================================\n\n");
    
    /* Get bootloader information */
    bootloader_info = get_bootloader_info();
    if (bootloader_info != NULL) {
        kprintf("[BOOT] Bootloader: %s v%s\n",
                bootloader_info->name,
                bootloader_info->version);
    } else {
        kprintf("[BOOT] ERROR: No bootloader info!\n");
    }
    
    /* Get kernel address information */
    kernel_addr = get_kernel_address();
    if (kernel_addr != NULL) {
        kprintf("[BOOT] Kernel Virtual Base: 0x%lx\n",
                kernel_addr->virtual_base);
        kprintf("[BOOT] Kernel Physical Base: 0x%lx\n",
                kernel_addr->physical_base);
    }
    
    /* Get memory map */
    memmap_info = get_memmap();
    if (memmap_info != NULL) {
        kprintf("[BOOT] Memory Map Entries: %lu\n",
                memmap_info->entry_count);
        
        /* Calculate total RAM */
        uint64_t total_ram = 0;
        for (uint64_t i = 0; i < memmap_info->entry_count; i++) {
            struct limine_memmap_entry *entry = memmap_info->entries[i];
            if (entry->type == LIMINE_MEMMAP_USABLE) {
                total_ram += entry->length;
            }
        }
        
        uint64_t total_mb = total_ram / (1024 * 1024);
        kprintf("[BOOT] Total Usable RAM: %lu MB\n", total_mb);
    }
    
    /* Initialize framebuffer */
    kprintf("[BOOT] Initializing framebuffer...\n");
    framebuffer_info = get_framebuffer();
    
    if (framebuffer_info != NULL && framebuffer_info->framebuffer_count > 0) {
        struct limine_framebuffer *fb = framebuffer_info->framebuffers[0];
        
        kprintf("[BOOT] Framebuffer found:\n");
        kprintf("       Resolution: %ux%u\n", fb->width, fb->height);
        kprintf("       Pitch: %u bytes\n", fb->pitch);
        kprintf("       BPP: %u\n", fb->bpp);
        kprintf("       Address: 0x%lx\n", (uint64_t)fb->address);
        
        framebuffer_init(fb);
        kprintf("[BOOT] Framebuffer initialized\n");
    } else {
        kprintf("[BOOT] WARNING: No framebuffer available\n");
    }
    
    kprintf("\n[BOOT] Kernel initialization complete!\n\n");
    
    /* Phase 2: Initialize CPU (GDT, IDT, exception handling) */
    kprintf("========================================\n");
    kprintf("  Phase 2: CPU Initialization\n");
    kprintf("========================================\n\n");
    
    cpu_init();
    
    kprintf("\n========================================\n");
    kprintf("  Phase 2: Complete\n");
    kprintf("========================================\n\n");
    
    /* Test exception handling */
    kprintf("[TEST] Testing exception handlers...\n");
    kprintf("[TEST] Press Ctrl+C in QEMU to exit\n\n");
    
    /* Demonstrate exception handling with safe tests */
    kprintf("[TEST] Ready to test exceptions\n");
    kprintf("[TEST] Uncomment cpu_trigger_* calls in kernel.c to test\n\n");
    
    /* Optional: Trigger test exceptions */
    /* Uncomment ONE of these to test exception handling */
    // cpu_trigger_divide_by_zero();     /* Test exception 0 */
    // cpu_trigger_invalid_opcode();     /* Test exception 6 */
    // cpu_trigger_breakpoint();         /* Test exception 3 */
    // cpu_trigger_page_fault();         /* Test exception 14 */
    
    kprintf("[KERNEL] Awaiting Phase 3 implementation (Physical & Virtual Memory)\n\n");
    
    /* Halt for now */
    while (1) {
        asm volatile("hlt");
    }
}
