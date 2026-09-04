#include <kernel/core/serial.h>
#include <config.h>
#include <stdint.h>

/* Serial port I/O */
#define SERIAL_PORT SERIAL_PORT  /* 0x3F8 for COM1 */

/* Serial port registers */
#define SERIAL_THR 0  /* Transmitter Holding Register */
#define SERIAL_RBR 0  /* Receiver Buffer Register */
#define SERIAL_DLL 0  /* Divisor Latch Low */
#define SERIAL_DLM 1  /* Divisor Latch High */
#define SERIAL_IER 1  /* Interrupt Enable Register */
#define SERIAL_IIR 2  /* Interrupt Identification Register */
#define SERIAL_FCR 2  /* FIFO Control Register */
#define SERIAL_LCR 3  /* Line Control Register */
#define SERIAL_MCR 4  /* Modem Control Register */
#define SERIAL_LSR 5  /* Line Status Register */
#define SERIAL_MSR 6  /* Modem Status Register */
#define SERIAL_SCR 7  /* Scratch Register */

/* Line Control Register bits */
#define SERIAL_LCR_DLAB 0x80  /* Divisor Latch Access Bit */
#define SERIAL_LCR_8BIT 0x03  /* 8 bits */
#define SERIAL_LCR_1STOP 0x00 /* 1 stop bit */
#define SERIAL_LCR_PARITY_OFF 0x00 /* No parity */

/* Line Status Register bits */
#define SERIAL_LSR_READY 0x20 /* Transmitter Holding Register Empty */
#define SERIAL_LSR_DATA_READY 0x01 /* Data Ready */

/* Modem Control Register bits */
#define SERIAL_MCR_DTR 0x01   /* Data Terminal Ready */
#define SERIAL_MCR_RTS 0x02   /* Request To Send */
#define SERIAL_MCR_OUT1 0x04  /* Output 1 */
#define SERIAL_MCR_OUT2 0x08  /* Output 2 (enables IRQ) */

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void serial_init(void) {
    uint16_t port = SERIAL_PORT;
    
    /* Disable all interrupts */
    outb(port + SERIAL_IER, 0x00);
    
    /* Enable DLAB (Divisor Latch Access Bit) */
    outb(port + SERIAL_LCR, SERIAL_LCR_DLAB);
    
    /* Set baud rate to 115200 */
    /* Divisor = 115200 / desired_baud */
    /* For 115200: divisor = 1 */
    uint16_t divisor = 115200 / SERIAL_BAUDRATE;
    outb(port + SERIAL_DLL, (uint8_t)divisor);
    outb(port + SERIAL_DLM, (uint8_t)(divisor >> 8));
    
    /* Disable DLAB, set line control */
    outb(port + SERIAL_LCR, SERIAL_LCR_8BIT | SERIAL_LCR_1STOP | SERIAL_LCR_PARITY_OFF);
    
    /* Enable FIFO, clear FIFO */
    outb(port + SERIAL_FCR, 0xC7);
    
    /* Set modem control */
    outb(port + SERIAL_MCR, SERIAL_MCR_DTR | SERIAL_MCR_RTS | SERIAL_MCR_OUT2);
    
    /* Enable receive interrupts */
    outb(port + SERIAL_IER, 0x01);
}

void serial_putchar(char c) {
    uint16_t port = SERIAL_PORT;
    
    /* Wait for transmitter to be ready */
    while ((inb(port + SERIAL_LSR) & SERIAL_LSR_READY) == 0) {
        asm volatile("pause");
    }
    
    /* Send character */
    outb(port + SERIAL_THR, (uint8_t)c);
}

char serial_getchar(void) {
    uint16_t port = SERIAL_PORT;
    
    /* Wait for data to be available */
    while ((inb(port + SERIAL_LSR) & SERIAL_LSR_DATA_READY) == 0) {
        asm volatile("pause");
    }
    
    /* Read character */
    return (char)inb(port + SERIAL_RBR);
}

void serial_write(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        serial_putchar(str[i]);
    }
}
