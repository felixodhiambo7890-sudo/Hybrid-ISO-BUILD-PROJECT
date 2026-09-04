#ifndef __KERNEL_SERIAL_H__
#define __KERNEL_SERIAL_H__

/* Initialize serial port (COM1) */
void serial_init(void);

/* Send single character to serial port */
void serial_putchar(char c);

/* Receive single character from serial port */
char serial_getchar(void);

/* Send null-terminated string to serial port */
void serial_write(const char *str);

#endif // __KERNEL_SERIAL_H__
