#ifndef __KERNEL_ARCH_IDT_H__
#define __KERNEL_ARCH_IDT_H__

#include <stdint.h>

/* CPU registers at interrupt time */
struct interrupt_registers {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t error_code;  /* Some exceptions have error code */
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

/* Exception handler - called from ISR stubs */
void exception_handler(int int_num, struct interrupt_registers *regs);

/* Interrupt handler - for external interrupts */
void interrupt_handler(int int_num, struct interrupt_registers *regs);

/* Initialize IDT */
void idt_init(void);

#endif // __KERNEL_ARCH_IDT_H__
