#include <kernel/arch/x86_64/gdt.h>
#include <kernel/arch/x86_64/idt.h>
#include <kernel/util/kprintf.h>

void cpu_init(void) {
    kprintf("\n[CPU] Initializing CPU subsystems...\n");
    
    /* Initialize GDT */
    gdt_init();
    
    /* Initialize IDT */
    idt_init();
    
    kprintf("[CPU] CPU initialization complete\n");
}

/* Trigger divide by zero exception for testing */
void cpu_trigger_divide_by_zero(void) {
    kprintf("\n[TEST] Triggering divide by zero exception...\n");
    int x = 1;
    int y = 0;
    int z = x / y;  /* This will trigger exception 0 */
    (void)z;        /* Suppress unused warning */
}

/* Trigger invalid opcode exception for testing */
void cpu_trigger_invalid_opcode(void) {
    kprintf("\n[TEST] Triggering invalid opcode exception...\n");
    asm volatile("ud2");  /* Undefined instruction */
}

/* Trigger page fault for testing */
void cpu_trigger_page_fault(void) {
    kprintf("\n[TEST] Triggering page fault exception...\n");
    volatile uint64_t *ptr = (uint64_t *)0xDEADBEEF;
    *ptr = 0x12345678;  /* Write to invalid address */
}

/* Trigger breakpoint exception for testing */
void cpu_trigger_breakpoint(void) {
    kprintf("\n[TEST] Triggering breakpoint exception...\n");
    asm volatile("int $3");  /* Breakpoint interrupt */
}
