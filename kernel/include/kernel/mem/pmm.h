#ifndef __KERNEL_PMM_H__
#define __KERNEL_PMM_H__

#include <stdint.h>
#include <config.h>

#define PMM_BITMAP_SIZE (16 * 1024)  /* 16 KB bitmap = 128 MB of physical memory */

/* Initialize physical memory manager */
void pmm_init(void);

/* Allocate one physical page (4 KB) */
uint64_t pmm_allocate_page(void);

/* Free physical page */
void pmm_free_page(uint64_t phys_addr);

/* Get statistics */
uint64_t pmm_get_free_pages(void);
uint64_t pmm_get_total_pages(void);

#endif // __KERNEL_PMM_H__
