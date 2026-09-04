#include <kernel/arch/x86_64/idt.h>
#include <kernel/util/kprintf.h>
#include <string.h>
#include <config.h>

/* IDT entry structure */
struct idt_entry {
    uint16_t offset_low;        /* Offset bits 0:15 */
    uint16_t selector;          /* Code segment selector */
    uint8_t ist;                /* IST index */
    uint8_t type_attr;          /* Gate type and attributes */
    uint16_t offset_mid;        /* Offset bits 16:31 */
    uint32_t offset_high;       /* Offset bits 32:63 */
    uint32_t reserved;
} __attribute__((packed));

/* IDT descriptor for IDTR */
struct idt_descriptor {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* IDT - 256 entries */
static struct idt_entry idt[256];
static struct idt_descriptor idt_ptr;

/* Exception names for debugging */
static const char *exception_names[32] = {
    "Divide by Zero",
    "Debug Exception",
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved (15)",
    "Floating-Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Flow Exception",
    "Reserved (22)",
    "Reserved (23)",
    "Reserved (24)",
    "Reserved (25)",
    "Reserved (26)",
    "Reserved (27)",
    "Reserved (28)",
    "Reserved (29)",
    "Reserved (30)",
    "Reserved (31)"
};

/* Forward declarations of ISR stubs from idt_stubs.asm */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);

/* Array of ISR handlers */
static void (*isr_handlers[])(void) = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
    isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,
    isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39,
    isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47,
};

/* Exception handler - called from ISR stubs */
void exception_handler(int int_num, struct interrupt_registers *regs) {
    kprintf("\n");
    kprintf("========================================\n");
    kprintf("  EXCEPTION: %s (0x%02x)\n",
            int_num < 32 ? exception_names[int_num] : "Unknown",
            int_num);
    kprintf("========================================\n");
    
    /* Print register state */
    kprintf("Register State:\n");
    kprintf("  RAX: 0x%016lx  RBX: 0x%016lx\n", regs->rax, regs->rbx);
    kprintf("  RCX: 0x%016lx  RDX: 0x%016lx\n", regs->rcx, regs->rdx);
    kprintf("  RSI: 0x%016lx  RDI: 0x%016lx\n", regs->rsi, regs->rdi);
    kprintf("  R8:  0x%016lx  R9:  0x%016lx\n", regs->r8, regs->r9);
    kprintf("  R10: 0x%016lx  R11: 0x%016lx\n", regs->r10, regs->r11);
    kprintf("  R12: 0x%016lx  R13: 0x%016lx\n", regs->r12, regs->r13);
    kprintf("  R14: 0x%016lx  R15: 0x%016lx\n", regs->r14, regs->r15);
    kprintf("  RBP: 0x%016lx  RSP: 0x%016lx\n", regs->rbp, (uint64_t)&regs);
    
    /* Special handling for page faults */
    if (int_num == 14) {
        uint64_t cr2;
        asm volatile("mov %%cr2, %0" : "=r"(cr2));
        kprintf("\nPage Fault Details:\n");
        kprintf("  Faulting Address: 0x%016lx\n", cr2);
        kprintf("  Error Code: 0x%08x\n", (uint32_t)regs->error_code);
        if (regs->error_code & 0x1) kprintf("    - Page present\n");
        if (regs->error_code & 0x2) kprintf("    - Write access\n");
        if (regs->error_code & 0x4) kprintf("    - User mode\n");
        if (regs->error_code & 0x8) kprintf("    - Reserved write\n");
        if (regs->error_code & 0x10) kprintf("    - Instruction fetch\n");
    }
    
    kprintf("========================================\n\n");
    
    /* Halt on unrecoverable exceptions */
    kprintf("System halted. Press Ctrl+C in QEMU to exit.\n");
    while (1) {
        asm volatile("hlt");
    }
}

/* Interrupt handler - for external interrupts */
void interrupt_handler(int int_num, struct interrupt_registers *regs) {
    kprintf("[INT] External Interrupt 0x%02x\n", int_num);
    
    /* TODO: Dispatch to appropriate handler */
    
    /* Send EOI (End Of Interrupt) to PIC if needed */
    if (int_num >= 32 && int_num < 48) {
        /* Legacy PIC handling - will be replaced with APIC in Phase 4 */
        if (int_num >= 40) {
            /* Secondary PIC */
            asm volatile("mov $0x20, %%al; out %%al, $0xa0" : : : "al");
        }
        /* Primary PIC */
        asm volatile("mov $0x20, %%al; out %%al, $0x20" : : : "al");
    }
}

/* Set IDT entry */
static void idt_set_entry(int index, uint64_t handler, uint16_t selector,
                           uint8_t type, uint8_t dpl) {
    struct idt_entry *entry = &idt[index];
    
    entry->offset_low = (uint16_t)(handler & 0xFFFF);
    entry->offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
    entry->offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    entry->selector = selector;
    entry->ist = 0;  /* IST index - 0 means use RSP0 */
    entry->type_attr = type | (dpl << 5) | 0x80;  /* Type | DPL | Present */
    entry->reserved = 0;
}

/* Load IDT */
static inline void load_idt(void) {
    asm volatile("lidt %0" : : "m"(idt_ptr));
}

void idt_init(void) {
    kprintf("[IDT] Initializing Interrupt Descriptor Table\n");
    
    /* Set up IDT descriptor */
    idt_ptr.base = (uint64_t)&idt[0];
    idt_ptr.limit = (sizeof(idt) - 1);
    
    /* Clear all IDT entries */
    memset(idt, 0, sizeof(idt));
    
    /* Set exception handlers (0-31) */
    for (int i = 0; i < 32; i++) {
        idt_set_entry(i, (uint64_t)isr_handlers[i], 0x08, 0x8E, 0);
    }
    
    /* Set interrupt handlers (32-47) - PIC IRQs */
    for (int i = 32; i < 48; i++) {
        if (i < 32 + sizeof(isr_handlers) / sizeof(isr_handlers[0])) {
            idt_set_entry(i, (uint64_t)isr_handlers[i], 0x08, 0x8E, 0);
        }
    }
    
    /* Load IDT into IDTR */
    load_idt();
    
    kprintf("[IDT] IDT loaded successfully\n");
    kprintf("[IDT]   Exceptions: 0x00 - 0x1F\n");
    kprintf("[IDT]   PIC IRQs:   0x20 - 0x2F\n");
    kprintf("[IDT]   Reserved:   0x30 - 0xFF\n");
}
