#include <kernel/mem/pmm.h>
#include <kernel/util/kprintf.h>
#include <string.h>
#include <config.h>
#include <limine.h>

/* Physical memory manager using bitmap allocator */

/* Bitmap for tracking page allocation */
static uint8_t pmm_bitmap[PMM_BITMAP_SIZE] = {0};
static uint64_t pmm_free_pages = 0;
static uint64_t pmm_total_pages = 0;

/* External memory map from bootloader */
extern volatile struct limine_memmap_response *memmap_info;

static inline void pmm_set_page(uint64_t page_num) {
    uint64_t byte_index = page_num / 8;
    uint8_t bit_index = page_num % 8;
    
    if (byte_index < PMM_BITMAP_SIZE) {
        pmm_bitmap[byte_index] |= (1 << bit_index);
    }
}

static inline void pmm_unset_page(uint64_t page_num) {
    uint64_t byte_index = page_num / 8;
    uint8_t bit_index = page_num % 8;
    
    if (byte_index < PMM_BITMAP_SIZE) {
        pmm_bitmap[byte_index] &= ~(1 << bit_index);
    }
}

static inline int pmm_is_page_used(uint64_t page_num) {
    uint64_t byte_index = page_num / 8;
    uint8_t bit_index = page_num % 8;
    
    if (byte_index < PMM_BITMAP_SIZE) {
        return (pmm_bitmap[byte_index] >> bit_index) & 1;
    }
    return 1;  /* Assume used if out of range */
}

void pmm_init(void) {
    kprintf("\n[PMM] Initializing Physical Memory Manager\n");
    
    if (memmap_info == NULL) {
        kprintf("[PMM] ERROR: No memory map available\n");
        return;
    }
    
    /* Clear bitmap */
    memset(pmm_bitmap, 0, PMM_BITMAP_SIZE);
    
    /* Parse memory map and mark pages */
    uint64_t total_pages = 0;
    uint64_t free_pages = 0;
    
    for (uint64_t i = 0; i < memmap_info->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_info->entries[i];
        uint64_t entry_start = entry->base;
        uint64_t entry_end = entry->base + entry->length;
        uint64_t entry_pages = entry->length / PAGE_SIZE;
        
        total_pages += entry_pages;
        
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            free_pages += entry_pages;
            kprintf("[PMM] Usable: 0x%016lx - 0x%016lx (%lu pages)\n",
                    entry_start, entry_end, entry_pages);
        } else {
            /* Mark non-usable pages as allocated */
            for (uint64_t j = 0; j < entry_pages; j++) {
                pmm_set_page((entry_start + j * PAGE_SIZE) / PAGE_SIZE);
            }
        }
    }
    
    pmm_total_pages = total_pages;
    pmm_free_pages = free_pages;
    
    kprintf("[PMM] Total Pages: %lu (%lu MB)\n", 
            pmm_total_pages, (pmm_total_pages * PAGE_SIZE) / (1024 * 1024));
    kprintf("[PMM] Free Pages: %lu (%lu MB)\n",
            pmm_free_pages, (pmm_free_pages * PAGE_SIZE) / (1024 * 1024));
    kprintf("[PMM] Physical Memory Manager initialized\n");
}

uint64_t pmm_allocate_page(void) {
    if (pmm_free_pages == 0) {
        kprintf("[PMM] ERROR: Out of physical memory\n");
        return 0;
    }
    
    /* Find first free page */
    for (uint64_t page = 0; page < pmm_total_pages; page++) {
        if (!pmm_is_page_used(page)) {
            pmm_set_page(page);
            pmm_free_pages--;
            return page * PAGE_SIZE;
        }
    }
    
    kprintf("[PMM] ERROR: Could not allocate page\n");
    return 0;
}

void pmm_free_page(uint64_t phys_addr) {
    uint64_t page_num = phys_addr / PAGE_SIZE;
    
    if (page_num < pmm_total_pages) {
        if (pmm_is_page_used(page_num)) {
            pmm_unset_page(page_num);
            pmm_free_pages++;
        }
    }
}

uint64_t pmm_get_free_pages(void) {
    return pmm_free_pages;
}

uint64_t pmm_get_total_pages(void) {
    return pmm_total_pages;
}
