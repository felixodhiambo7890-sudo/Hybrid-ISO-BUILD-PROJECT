/* Kernel linker script for x86_64 */

KERNEL_VIRT_BASE = 0xffffffff80000000;
KERNEL_PHYS_BASE = 0x100000;

ENTRY(kernel_entry)

SECTIONS
{
    . = KERNEL_PHYS_BASE;

    .text KERNEL_VIRT_BASE : AT(ADDR(.text) - KERNEL_VIRT_BASE + KERNEL_PHYS_BASE)
    {
        *(.multiboot2_header)
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
        *(.gnu.hash)
        *(.gnu.version)
    }

    .data :
    {
        *(.data*)
        *(.bss*)
        *(COMMON)
    }

    .symtab :
    {
        *(.symtab)
    }

    /DISCARD/
    {
        *(.note*)
        *(.comment*)
    }
}
