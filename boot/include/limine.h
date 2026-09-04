#ifndef __LIMINE_H__
#define __LIMINE_H__

#include <stdint.h>

/* Limine Request Magic */
#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b
#define LIMINE_COMMON_MAGIC_2 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

/* Request/Response IDs */
#define LIMINE_BOOTLOADER_INFO_REQUEST 0xf55038d8e2a1202f
#define LIMINE_FRAMEBUFFER_REQUEST 0x9d5827dcd881dd7b
#define LIMINE_PAGING_MODE_REQUEST 0x95c1a0efab0c31db
#define LIMINE_HHDM_REQUEST 0x48dcf1cb8ad2b852
#define LIMINE_MEMMAP_REQUEST 0x67cf3d9d378a806f
#define LIMINE_KERNEL_ADDRESS_REQUEST 0x71ba51431b37c8c4
#define LIMINE_RSDP_REQUEST 0xc5e77b6b397e7b43

/* Paging modes */
#define LIMINE_PAGING_MODE_X86_64_4LVL 0
#define LIMINE_PAGING_MODE_X86_64_5LVL 1

/* Memory map entry types */
#define LIMINE_MEMMAP_USABLE 0
#define LIMINE_MEMMAP_RESERVED 1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE 2
#define LIMINE_MEMMAP_ACPI_NVS 3
#define LIMINE_MEMMAP_BAD_MEMORY 4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_KERNEL_AND_MODULES 6
#define LIMINE_MEMMAP_FRAMEBUFFER 7

/* Bootloader Info */
struct limine_bootloader_info_response {
    uint64_t revision;
    char *name;
    char *version;
};

struct limine_bootloader_info_request {
    uint64_t id;
    uint64_t revision;
    volatile struct limine_bootloader_info_response *response;
};

/* Framebuffer */
struct limine_framebuffer {
    uint64_t address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
    uint8_t memory_model;
    uint8_t red_mask_size;
    uint8_t red_mask_shift;
    uint8_t green_mask_size;
    uint8_t green_mask_shift;
    uint8_t blue_mask_size;
    uint8_t blue_mask_shift;
    uint8_t unused;
    uint64_t edid_size;
    uint8_t *edid;
};

struct limine_framebuffer_response {
    uint64_t revision;
    uint64_t framebuffer_count;
    struct limine_framebuffer **framebuffers;
};

struct limine_framebuffer_request {
    uint64_t id;
    uint64_t revision;
    volatile struct limine_framebuffer_response *response;
};

/* HHDM (Higher Half Direct Map) */
struct limine_hhdm_response {
    uint64_t revision;
    uint64_t offset;
};

struct limine_hhdm_request {
    uint64_t id;
    uint64_t revision;
    volatile struct limine_hhdm_response *response;
};

/* Paging Mode */
struct limine_paging_mode_response {
    uint64_t revision;
    uint64_t mode;
};

struct limine_paging_mode_request {
    uint64_t id;
    uint64_t revision;
    uint64_t mode;
    volatile struct limine_paging_mode_response *response;
};

/* Memory Map */
struct limine_memmap_entry {
    uint64_t base;
    uint64_t length;
    uint64_t type;
};

struct limine_memmap_response {
    uint64_t revision;
    uint64_t entry_count;
    struct limine_memmap_entry **entries;
};

struct limine_memmap_request {
    uint64_t id;
    uint64_t revision;
    volatile struct limine_memmap_response *response;
};

/* Kernel Address */
struct limine_kernel_address_response {
    uint64_t revision;
    uint64_t physical_base;
    uint64_t virtual_base;
};

struct limine_kernel_address_request {
    uint64_t id;
    uint64_t revision;
    volatile struct limine_kernel_address_response *response;
};

/* RSDP */
struct limine_rsdp_response {
    uint64_t revision;
    uint8_t *address;
};

struct limine_rsdp_request {
    uint64_t id;
    uint64_t revision;
    volatile struct limine_rsdp_response *response;
};

#endif // __LIMINE_H__
