#ifndef __KERNEL_CORE_H__
#define __KERNEL_CORE_H__

#include <stdint.h>
#include <limine.h>

/* Bootloader info pointers */
extern volatile struct limine_bootloader_info_response *bootloader_info;
extern volatile struct limine_framebuffer_response *framebuffer_info;
extern volatile struct limine_memmap_response *memmap_info;
extern volatile struct limine_kernel_address_response *kernel_addr;

/* Main kernel entry point (from entry.asm) */
void kernel_main(void);

#endif // __KERNEL_CORE_H__
