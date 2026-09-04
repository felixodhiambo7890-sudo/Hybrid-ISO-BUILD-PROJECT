#ifndef __KERNEL_CPU_H__
#define __KERNEL_CPU_H__

/* Initialize CPU (GDT, IDT, exception handlers) */
void cpu_init(void);

/* Test functions to trigger exceptions safely */
void cpu_trigger_divide_by_zero(void);
void cpu_trigger_invalid_opcode(void);
void cpu_trigger_page_fault(void);
void cpu_trigger_breakpoint(void);

#endif // __KERNEL_CPU_H__
