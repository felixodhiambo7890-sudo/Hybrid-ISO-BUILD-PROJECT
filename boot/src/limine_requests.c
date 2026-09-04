#include <limine.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_paging_mode_request paging_mode_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL,
    .response = NULL
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

/* Get bootloader response functions */
volatile struct limine_bootloader_info_response *get_bootloader_info(void) {
    return bootloader_info_request.response;
}

volatile struct limine_framebuffer_response *get_framebuffer(void) {
    return framebuffer_request.response;
}

volatile struct limine_hhdm_response *get_hhdm(void) {
    return hhdm_request.response;
}

volatile struct limine_kernel_address_response *get_kernel_address(void) {
    return kernel_address_request.response;
}

volatile struct limine_memmap_response *get_memmap(void) {
    return memmap_request.response;
}

volatile struct limine_rsdp_response *get_rsdp(void) {
    return rsdp_request.response;
}
