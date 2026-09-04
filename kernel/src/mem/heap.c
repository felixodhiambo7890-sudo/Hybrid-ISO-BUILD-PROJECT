#include <kernel/mem/heap.h>
#include <kernel/mem/pmm.h>
#include <kernel/util/kprintf.h>
#include <string.h>
#include <config.h>

/* Simple heap allocator using linked list of free blocks */

struct heap_block {
    uint64_t size;           /* Size of block (including header) */
    int used;                /* Is block used? */
    struct heap_block *next; /* Next block in chain */
    struct heap_block *prev; /* Previous block in chain */
};

static struct heap_block *heap_start = NULL;
static uint64_t heap_size = 0;
static uint64_t heap_used = 0;

void heap_init(uint64_t initial_size) {
    kprintf("\n[HEAP] Initializing Kernel Heap\n");
    kprintf("[HEAP] Initial size: %lu KB\n", initial_size / 1024);
    
    /* Allocate initial heap pages */
    uint64_t pages_needed = (initial_size + PAGE_SIZE - 1) / PAGE_SIZE;
    uint64_t heap_phys = pmm_allocate_page();
    
    if (heap_phys == 0) {
        kprintf("[HEAP] ERROR: Could not allocate heap pages\n");
        return;
    }
    
    heap_start = (struct heap_block *)heap_phys;
    heap_size = initial_size;
    heap_used = sizeof(struct heap_block);
    
    /* Initialize first block */
    heap_start->size = heap_size;
    heap_start->used = 0;
    heap_start->next = NULL;
    heap_start->prev = NULL;
    
    kprintf("[HEAP] Kernel heap initialized at 0x%lx\n", (uint64_t)heap_start);
    kprintf("[HEAP] Total heap: %lu KB, Free: %lu KB\n",
            heap_size / 1024, (heap_size - heap_used) / 1024);
}

void *malloc(size_t size) {
    if (size == 0 || heap_start == NULL) {
        return NULL;
    }
    
    /* Add space for header */
    size_t total_size = size + sizeof(struct heap_block);
    
    /* Find first free block large enough */
    struct heap_block *block = heap_start;
    
    while (block != NULL) {
        if (!block->used && block->size >= total_size) {
            /* Split block if it's too large */
            if (block->size > total_size + sizeof(struct heap_block)) {
                struct heap_block *new_block = 
                    (struct heap_block *)((uint64_t)block + total_size);
                new_block->size = block->size - total_size;
                new_block->used = 0;
                new_block->next = block->next;
                new_block->prev = block;
                
                if (block->next != NULL) {
                    block->next->prev = new_block;
                }
                block->next = new_block;
                block->size = total_size;
            }
            
            block->used = 1;
            heap_used += block->size;
            
            /* Return pointer after header */
            return (void *)((uint64_t)block + sizeof(struct heap_block));
        }
        
        block = block->next;
    }
    
    kprintf("[HEAP] WARNING: malloc(%lu) failed - out of memory\n", size);
    return NULL;
}

void free(void *ptr) {
    if (ptr == NULL || heap_start == NULL) {
        return;
    }
    
    /* Get block header */
    struct heap_block *block = 
        (struct heap_block *)((uint64_t)ptr - sizeof(struct heap_block));
    
    if (block->used) {
        block->used = 0;
        heap_used -= block->size;
        
        /* Coalesce with next block if it's free */
        if (block->next != NULL && !block->next->used) {
            block->size += block->next->size;
            block->next = block->next->next;
            if (block->next != NULL) {
                block->next->prev = block;
            }
        }
        
        /* Coalesce with previous block if it's free */
        if (block->prev != NULL && !block->prev->used) {
            block->prev->size += block->size;
            block->prev->next = block->next;
            if (block->next != NULL) {
                block->next->prev = block->prev;
            }
        }
    }
}

void *calloc(size_t count, size_t size) {
    size_t total = count * size;
    void *ptr = malloc(total);
    
    if (ptr != NULL) {
        memset(ptr, 0, total);
    }
    
    return ptr;
}

void *realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);
    }
    
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    
    /* Get old block size */
    struct heap_block *old_block = 
        (struct heap_block *)((uint64_t)ptr - sizeof(struct heap_block));
    uint64_t old_size = old_block->size - sizeof(struct heap_block);
    
    /* Allocate new block */
    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }
    
    /* Copy old data */
    uint64_t copy_size = (size < old_size) ? size : old_size;
    memcpy(new_ptr, ptr, copy_size);
    
    /* Free old block */
    free(ptr);
    
    return new_ptr;
}

uint64_t heap_get_free(void) {
    return heap_size - heap_used;
}

uint64_t heap_get_used(void) {
    return heap_used;
}
