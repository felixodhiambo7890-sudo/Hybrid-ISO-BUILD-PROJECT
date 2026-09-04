#include <kernel/util/kprintf.h>
#include <kernel/core/serial.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

static void kputchar(char c) {
    serial_putchar(c);
}

static void kputs(const char *str) {
    while (*str) {
        kputchar(*str++);
    }
}

static void kprint_hex(uint64_t value, int uppercase) {
    const char *hex = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    
    if (value == 0) {
        kputchar('0');
        return;
    }
    
    char buffer[20];
    int i = 0;
    while (value > 0) {
        buffer[i++] = hex[value % 16];
        value /= 16;
    }
    
    while (i > 0) {
        kputchar(buffer[--i]);
    }
}

static void kprint_dec(int64_t value) {
    if (value == 0) {
        kputchar('0');
        return;
    }
    
    if (value < 0) {
        kputchar('-');
        value = -value;
    }
    
    char buffer[20];
    int i = 0;
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    while (i > 0) {
        kputchar(buffer[--i]);
    }
}

void kprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            switch (format[i]) {
                case 'd':
                case 'i':
                    kprint_dec(va_arg(args, int));
                    break;
                case 'u':
                    kprint_dec((uint32_t)va_arg(args, unsigned int));
                    break;
                case 'l':
                    kprint_dec(va_arg(args, long));
                    break;
                case 'x':
                    kprint_hex(va_arg(args, uint64_t), 0);
                    break;
                case 'X':
                    kprint_hex(va_arg(args, uint64_t), 1);
                    break;
                case 'p':
                    kputs("0x");
                    kprint_hex((uint64_t)va_arg(args, void *), 0);
                    break;
                case 's':
                    kputs(va_arg(args, char *));
                    break;
                case 'c':
                    kputchar((char)va_arg(args, int));
                    break;
                case '%':
                    kputchar('%');
                    break;
                default:
                    kputchar('%');
                    kputchar(format[i]);
                    break;
            }
        } else if (format[i] == '\n') {
            kputchar('\r');
            kputchar('\n');
        } else {
            kputchar(format[i]);
        }
    }
    
    va_end(args);
}
