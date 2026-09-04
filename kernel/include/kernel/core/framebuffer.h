#ifndef __KERNEL_FRAMEBUFFER_H__
#define __KERNEL_FRAMEBUFFER_H__

#include <stdint.h>
#include <stdbool.h>
#include <limine.h>

/* Initialize framebuffer */
void framebuffer_init(struct limine_framebuffer *fb);

/* Clear entire framebuffer with color */
void framebuffer_clear(uint32_t color);

/* Put single pixel */
void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color);

/* Fill rectangle */
void framebuffer_fillrect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);

/* Put character at position */
void framebuffer_putchar(char c, uint32_t x, uint32_t y, uint32_t color);

/* Put null-terminated string at position */
void framebuffer_puts(const char *str, uint32_t x, uint32_t y, uint32_t color);

/* Get framebuffer dimensions */
uint32_t framebuffer_width(void);
uint32_t framebuffer_height(void);

/* Check if framebuffer is available */
bool framebuffer_available(void);

#endif // __KERNEL_FRAMEBUFFER_H__
