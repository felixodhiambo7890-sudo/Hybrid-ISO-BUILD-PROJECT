# Architecture-Specific Code

This directory contains architecture-specific implementations for x86_64.

## Subdirectories

- `x86_64/` - Intel/AMD 64-bit architecture

## Files (Phase 2+)

- `x86_64/gdt.asm` - GDT loading
- `x86_64/gdt.c` - GDT initialization
- `x86_64/idt_stubs.asm` - ISR stubs
- `x86_64/idt.c` - IDT initialization and exception handling

## Phase 2 Implementation

✅ GDT (Global Descriptor Table)  
✅ IDT (Interrupt Descriptor Table)  
✅ Exception handlers (0-31)  
✅ Interrupt handlers (32-47, PIC)  
✅ Safe exception testing  
