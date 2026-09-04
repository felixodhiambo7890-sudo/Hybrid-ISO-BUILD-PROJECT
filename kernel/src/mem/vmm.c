#include <kernel/mem/vmm.h>
#include <kernel/mem/pmm.h>
#include <kernel/util/kprintf.h>
#include <string.h>
#include <config.h>

/* Virtual memory manager for paging */

/* Kernel page directory (top-level page table) */
static uint64_t kernel_pml4[512] __attribute__((aligned(PAGE_SIZE)));

/* PML4 entry flags */
#define PML4_PRESENT        0x001
#define PML4_WRITABLE       0x002
#define PML4_USER           0x004
#define PML4_WRITE_THROUGH  0x008
#define PML4_CACHE_DISABLE  0x010
#define PML4_ACCESSED       0x020
#define PML4_DIRTY          0x040
#define PML4_PAGE_SIZE      0x080
#define PML4_GLOBAL         0x100
#define PML4_ADDR_MASK      0x000FFFFFFFFFF000ULL
#define PML4_FLAG_MASK      0xFFFULL

/* Extract page table entry components */
static inline uint16_t vmm_pml4_index(uint64_t virt_addr) {
    return (virt_addr >> 39) & 0x1FF;
}

static inline uint16_t vmm_pdpt_index(uint64_t virt_addr) {
    return (virt_addr >> 30) & 0x1FF;
}

static inline uint16_t vmm_pd_index(uint64_t virt_addr) {
    return (virt_addr >> 21) & 0x1FF;
}

static inline uint16_t vmm_pt_index(uint64_t virt_addr) {
    return (virt_addr >> 12) & 0x1FF;
}

static inline uint64_t vmm_page_align(uint64_t addr) {
    return addr & ~(PAGE_SIZE - 1);
}

void vmm_init(void) {
    kprintf("\n[VMM] Initializing Virtual Memory Manager\n");
    
    /* Clear kernel PML4 */
    memset(kernel_pml4, 0, PAGE_SIZE);
    
    /* TODO: Identity map kernel space */
    /* TODO: Set up higher-half kernel mapping */
    
    /* Load CR3 (page table base register) */
    uint64_t pml4_phys = (uint64_t)kernel_pml4;
    asm volatile("mov %0, %%cr3" : : "r"(pml4_phys));
    
    /* Enable paging (CR0.PG bit 31) */
    uint64_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  /* Set bit 31 (PG) */
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
    
    kprintf("[VMM] Virtual Memory Manager initialized\n");
    kprintf("[VMM] Paging enabled\n");
}

int vmm_map_page(uint64_t virt_addr, uint64_t phys_addr, uint32_t flags) {
    /* Align addresses to page boundaries */
    virt_addr = vmm_page_align(virt_addr);
    phys_addr = vmm_page_align(phys_addr);
    
    uint16_t pml4_idx = vmm_pml4_index(virt_addr);
    
    /* Check if PML4 entry exists */
    if (!(kernel_pml4[pml4_idx] & PML4_PRESENT)) {
        /* Allocate new PDPT */
        uint64_t pdpt_phys = pmm_allocate_page();
        if (pdpt_phys == 0) {
            kprintf("[VMM] ERROR: Could not allocate PDPT\n");
            return -1;
        }
        
        /* Map PDPT into PML4 */
        kernel_pml4[pml4_idx] = pdpt_phys | (PML4_PRESENT | PML4_WRITABLE);
    }
    
    /* For now, just mark that mapping was requested */
    /* Full paging implementation will follow */
    
    return 0;
}

void vmm_unmap_page(uint64_t virt_addr) {
    /* TODO: Implement page unmapping */
    (void)virt_addr;
}

uint64_t vmm_translate(uint64_t virt_addr) {
    /* TODO: Translate virtual to physical address */
    /* For now, return physical address (identity mapping) */
    return virt_addr;
}
