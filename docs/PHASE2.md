# PHASE 2: GDT, IDT, Exceptions, Interrupts

## Overview

Phase 2 implements CPU initialization with GDT (Global Descriptor Table), IDT (Interrupt Descriptor Table), and full exception/interrupt handling for x86_64:

1. **GDT Setup** - Kernel and user code/data segments, TSS
2. **IDT Setup** - All 32 CPU exceptions + PIC IRQ handlers
3. **Exception Handlers** - Safe exception handling with register state
4. **Interrupt Framework** - External interrupt support (PIC/legacy)
5. **Test Framework** - Safe exception triggering for testing

## Components

### Assembly Files

- `kernel/src/arch/x86_64/gdt.asm` - GDT loading and segment setup
- `kernel/src/arch/x86_64/idt_stubs.asm` - ISR stubs for all 32 exceptions + IRQs 0-15

### C Implementation

- `kernel/src/arch/x86_64/gdt.c` - GDT initialization
- `kernel/src/arch/x86_64/idt.c` - IDT initialization and exception handler dispatch
- `kernel/src/core/cpu.c` - CPU subsystem init and exception testing

### Headers

- `kernel/include/kernel/arch/x86_64/gdt.h`
- `kernel/include/kernel/arch/x86_64/idt.h`
- `kernel/include/kernel/core/cpu.h`

## GDT (Global Descriptor Table)

Defines memory segments for the CPU:

```
Index   Selector   Type       DPL   Purpose
0x00    0x00       Null       -     Null descriptor (required)
0x08    0x08       Code       0     Kernel code (ring 0)
0x10    0x10       Data       0     Kernel data (ring 0)
0x18    0x18       Code       3     User code (ring 3)
0x20    0x20       Data       3     User data (ring 3)
0x28    0x28       TSS        0     Task State Segment
```

### TSS (Task State Segment)

Holds CPU state for task switching and ring transitions:
- RSP0, RSP1, RSP2 - Stack pointers for rings 0-2
- IST[0-6] - Interrupt Stack Table entries
- I/O Map Base - For I/O permission checking

## IDT (Interrupt Descriptor Table)

256 entries for CPU exceptions and interrupts:

### Exceptions (0-31)

```
0  - Divide Error
1  - Debug Exception
2  - NMI Interrupt
3  - Breakpoint
4  - Overflow
5  - BOUND Range Exceeded
6  - Invalid Opcode
7  - Device Not Available
8  - Double Fault (has error code)
9  - Coprocessor Segment Overrun
10 - Invalid TSS (has error code)
11 - Segment Not Present (has error code)
12 - Stack-Segment Fault (has error code)
13 - General Protection Fault (has error code)
14 - Page Fault (has error code)
15 - Reserved
16 - Floating-Point Error
17 - Alignment Check (has error code)
18 - Machine Check
19 - SIMD Floating-Point Exception
20-31 - Reserved
```

### External Interrupts (32-47)

PIC (Programmable Interrupt Controller) IRQs 0-15:

```
32-39   - Primary PIC (IRQ0-7)
40-47   - Secondary PIC (IRQ8-15)
48-255  - Reserved for future use
```

## Exception Handling

### ISR Stub Flow

1. CPU pushes error code (if applicable) and return info
2. ASM stub saves all registers
3. Calls C exception handler with exception number and register state
4. Handler displays exception info and register dump
5. Special handling for page faults (display faulting address)
6. System halts (can be extended to recover in future)

### Register State Structure

```c
struct interrupt_registers {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t error_code;  /* From CPU or 0 if not applicable */
    uint64_t rip, cs, rflags, rsp, ss;  /* CPU-saved state */
};
```

## Testing Exceptions

Safe exception testing functions in `kernel/src/core/cpu.c`:

```c
void cpu_trigger_divide_by_zero(void);  /* Exception 0 */
void cpu_trigger_invalid_opcode(void);   /* Exception 6 */
void cpu_trigger_breakpoint(void);       /* Exception 3 */
void cpu_trigger_page_fault(void);       /* Exception 14 */
```

Uncomment one in `kernel_main()` to test:

```c
// cpu_trigger_divide_by_zero();
// cpu_trigger_invalid_opcode();
// cpu_trigger_breakpoint();
// cpu_trigger_page_fault();
```

## Example Exception Output

When divide by zero is triggered:

```
========================================
  EXCEPTION: Divide by Zero (0x00)
========================================
Register State:
  RAX: 0x0000000000000001  RBX: 0x0000000000000000
  RCX: 0x0000000000000000  RDX: 0x0000000000000000
  RSI: 0x0000000000000000  RDI: 0x0000000000000000
  R8:  0x0000000000000000  R9:  0x0000000000000000
  R10: 0x0000000000000000  R11: 0x0000000000000000
  R12: 0x0000000000000000  R13: 0x0000000000000000
  R14: 0x0000000000000000  R15: 0x0000000000000000
  RBP: 0xfffffffff0000000  RSP: 0xfffffffff0001234
========================================
System halted. Press Ctrl+C in QEMU to exit.
```

## Building & Testing

```bash
# Build
make clean-all
make all

# Run in QEMU
make qemu
```

## Expected Output

```
========================================
  Hybrid OS v0.1.0
  Boot: Kernel Entry Reached
========================================

[BOOT] Bootloader: Limine v...
...

========================================
  Phase 2: CPU Initialization
========================================

[GDT] Initializing Global Descriptor Table
[GDT] GDT loaded successfully
[GDT]   Kernel Code:  0x08
[GDT]   Kernel Data:  0x10
[GDT]   User Code:    0x18
[GDT]   User Data:    0x20
[GDT]   TSS:          0x28
[IDT] Initializing Interrupt Descriptor Table
[IDT] IDT loaded successfully
[IDT]   Exceptions: 0x00 - 0x1F
[IDT]   PIC IRQs:   0x20 - 0x2F
[IDT]   Reserved:   0x30 - 0xFF
[CPU] CPU initialization complete

========================================
  Phase 2: Complete
========================================

[TEST] Testing exception handlers...
[TEST] Press Ctrl+C in QEMU to exit
[TEST] Ready to test exceptions
[TEST] Uncomment cpu_trigger_* calls in kernel.c to test

[KERNEL] Awaiting Phase 3 implementation (Physical & Virtual Memory)
```

## Hardware Gate

Phase 2 Complete when:
- ✅ GDT loads successfully
- ✅ IDT loads successfully
- ✅ Exception handlers installed and working
- ✅ Exceptions can be triggered safely and handled
- ✅ Register state displays correctly
- ✅ Page fault handler shows faulting address

## Architecture Details

### Error Code Format

Some exceptions provide error codes:

```
Bit 0 (P):   Page present (0=not present, 1=present)
Bit 1 (W):   Write access (0=read, 1=write)
Bit 2 (U):   User mode (0=supervisor, 1=user)
Bit 3 (R):   Reserved write
Bit 4 (I):   Instruction fetch
Bits 5-15:   Selector index (for some exceptions)
Bits 16+:    Reserved
```

### Exception Types in IDT

- **0x8E** - Interrupt Gate (interrupts disabled while handling)
- **0xEE** - Trap Gate (interrupts enabled while handling)

Phase 2 uses Interrupt Gates (0x8E) with DPL=0 (kernel only).

## Next: Phase 3

Phase 3 will implement:
- Physical memory manager (PMM)
- Virtual memory manager (VMM)
- Paging (page tables)
- Heap allocation
