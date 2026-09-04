#ifndef __KERNEL_ARCH_GDT_H__
#define __KERNEL_ARCH_GDT_H__

#include <stdint.h>

/* GDT segment selectors */
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE 0x18
#define GDT_USER_DATA 0x20
#define GDT_TSS 0x28

/* Initialize GDT */
void gdt_init(void);

/* Set TSS kernel stack pointer */
void gdt_set_tss_stack(uint64_t stack_ptr);

/* Get TSS structure */
struct tss *gdt_get_tss(void);

#endif // __KERNEL_ARCH_GDT_H__
