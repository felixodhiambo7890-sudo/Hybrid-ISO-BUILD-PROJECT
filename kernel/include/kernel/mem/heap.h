#ifndef __KERNEL_HEAP_H__
#define __KERNEL_HEAP_H__

#include <stddef.h>
#include <stdint.h>

/* Initialize kernel heap */
void heap_init(uint64_t initial_size);

/* Memory allocation functions */
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);

/* Heap statistics */
uint64_t heap_get_free(void);
uint64_t heap_get_used(void);

#endif // __KERNEL_HEAP_H__
