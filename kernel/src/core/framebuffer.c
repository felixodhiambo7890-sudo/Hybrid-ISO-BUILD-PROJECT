#include <kernel/core/framebuffer.h>
#include <kernel/util/kprintf.h>
#include <stdint.h>
#include <string.h>

/* Global framebuffer state */
static struct {
    uint32_t *address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
    uint32_t x;
    uint32_t y;
} fb_state = {0};

void framebuffer_init(struct limine_framebuffer *fb) {
    if (fb == NULL) {
        return;
    }
    
    fb_state.address = (uint32_t *)fb->address;
    fb_state.width = fb->width;
    fb_state.height = fb->height;
    fb_state.pitch = fb->pitch;
    fb_state.bpp = fb->bpp;
    fb_state.x = 0;
    fb_state.y = 0;
    
    /* Clear framebuffer */
    framebuffer_clear(0x000000);
    
    /* Draw boot message */
    framebuffer_puts("Hybrid OS Kernel", 10, 10, 0xFFFFFF);
    framebuffer_puts("Phase 1: Boot Complete", 10, 30, 0x00FF00);
}

void framebuffer_clear(uint32_t color) {
    if (fb_state.address == NULL) {
        return;
    }
    
    /* Fill entire framebuffer */
    for (uint32_t y = 0; y < fb_state.height; y++) {
        for (uint32_t x = 0; x < fb_state.width; x++) {
            uint32_t offset = (y * fb_state.pitch / sizeof(uint32_t)) + x;
            fb_state.address[offset] = color;
        }
    }
}

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (fb_state.address == NULL || x >= fb_state.width || y >= fb_state.height) {
        return;
    }
    
    uint32_t offset = (y * fb_state.pitch / sizeof(uint32_t)) + x;
    fb_state.address[offset] = color;
}

void framebuffer_fillrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    if (fb_state.address == NULL) {
        return;
    }
    
    for (uint32_t dy = 0; dy < height && (y + dy) < fb_state.height; dy++) {
        for (uint32_t dx = 0; dx < width && (x + dx) < fb_state.width; dx++) {
            framebuffer_putpixel(x + dx, y + dy, color);
        }
    }
}

/* Simple 8x8 bitmap font for terminal output */
static const uint8_t font8x8[256][8] = {
    /* ASCII 32-127 basic font */
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['A'] = {0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00},
    ['B'] = {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00},
    ['C'] = {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00},
};

void framebuffer_putchar(char c, uint32_t x, uint32_t y, uint32_t color) {
    if (fb_state.address == NULL || c < 32 || c > 126) {
        return;
    }
    
    /* Use simple character rendering (box for now) */
    framebuffer_fillrect(x, y, 8, 8, 0x222222);
    framebuffer_fillrect(x + 1, y + 1, 6, 6, color);
}

void framebuffer_puts(const char *str, uint32_t x, uint32_t y, uint32_t color) {
    if (fb_state.address == NULL || str == NULL) {
        return;
    }
    
    uint32_t current_x = x;
    uint32_t current_y = y;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            current_y += 16;
            current_x = x;
            continue;
        }
        
        framebuffer_putchar(str[i], current_x, current_y, color);
        current_x += 8;
        
        /* Wrap to next line */
        if (current_x >= fb_state.width - 8) {
            current_y += 16;
            current_x = x;
        }
    }
}

uint32_t framebuffer_width(void) {
    return fb_state.width;
}

uint32_t framebuffer_height(void) {
    return fb_state.height;
}

bool framebuffer_available(void) {
    return fb_state.address != NULL;
}
