#include <kernel/arch/x86_64/gdt.h>
#include <kernel/util/kprintf.h>
#include <string.h>

/* GDT data structure */
struct gdt_descriptor {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* TSS (Task State Segment) for x86_64 */
struct tss {
    uint32_t reserved0;
    uint64_t rsp0;          /* Stack pointer for ring 0 */
    uint64_t rsp1;          /* Stack pointer for ring 1 */
    uint64_t rsp2;          /* Stack pointer for ring 2 */
    uint64_t reserved1;
    uint64_t ist[7];        /* Interrupt Stack Table */
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t io_map_base;
} __attribute__((packed));

/* Global TSS */
static struct tss kernel_tss = {
    .rsp0 = 0,             /* Will be set during scheduler init */
    .rsp1 = 0,
    .rsp2 = 0,
    .io_map_base = sizeof(struct tss),
};

/* Forward declaration from gdt.asm */
extern void load_gdt(void);
extern struct gdt_descriptor *gdt_get_ptr(void);

void gdt_init(void) {
    kprintf("[GDT] Initializing Global Descriptor Table\n");
    
    /* TSS descriptor will be loaded by IDT setup */
    /* For now, just load the GDT */
    load_gdt();
    
    kprintf("[GDT] GDT loaded successfully\n");
    kprintf("[GDT]   Kernel Code:  0x08\n");
    kprintf("[GDT]   Kernel Data:  0x10\n");
    kprintf("[GDT]   User Code:    0x18\n");
    kprintf("[GDT]   User Data:    0x20\n");
    kprintf("[GDT]   TSS:          0x28\n");
}

void gdt_set_tss_stack(uint64_t stack_ptr) {
    kernel_tss.rsp0 = stack_ptr;
}

struct tss *gdt_get_tss(void) {
    return &kernel_tss;
}
