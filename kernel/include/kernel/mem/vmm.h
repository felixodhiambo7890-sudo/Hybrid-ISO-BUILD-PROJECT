#ifndef __KERNEL_VMM_H__
#define __KERNEL_VMM_H__

#include <stdint.h>
#include <config.h>

/* Virtual memory mapping flags */
#define VMM_PRESENT     0x001
#define VMM_WRITABLE    0x002
#define VMM_USER        0x004
#define VMM_WRITE_THRU  0x008
#define VMM_NO_CACHE    0x010
#define VMM_ACCESSED    0x020
#define VMM_DIRTY       0x040
#define VMM_GLOBAL      0x100

/* Initialize virtual memory manager */
void vmm_init(void);

/* Map virtual page to physical page */
int vmm_map_page(uint64_t virt_addr, uint64_t phys_addr, uint32_t flags);

/* Unmap virtual page */
void vmm_unmap_page(uint64_t virt_addr);

/* Translate virtual address to physical address */
uint64_t vmm_translate(uint64_t virt_addr);

#endif // __KERNEL_VMM_H__
