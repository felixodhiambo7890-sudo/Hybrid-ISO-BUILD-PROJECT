; GDT (Global Descriptor Table) Setup for x86_64
; Defines kernel and user code/data segments

bits 64

section .data
align 16

; GDT entries structure
gdt_start:
    ; Null descriptor (required)
    dq 0x0000000000000000

    ; Kernel code segment (0x08)
    ; DPL=0 (kernel), Type=0xA (execute/read), Granularity=1 (4KB)
    dq 0x00209a0000000000

    ; Kernel data segment (0x10)
    ; DPL=0 (kernel), Type=0x2 (read/write), Granularity=1 (4KB)
    dq 0x0020920000000000

    ; User code segment (0x18)
    ; DPL=3 (user), Type=0xA (execute/read), Granularity=1 (4KB)
    dq 0x0020fa0000000000

    ; User data segment (0x20)
    ; DPL=3 (user), Type=0x2 (read/write), Granularity=1 (4KB)
    dq 0x0020f20000000000

    ; TSS (Task State Segment) descriptor (0x28)
    ; Will be filled in by kernel code
    dq 0x0000000000000000
    dq 0x0000000000000000

gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1  ; Limit (size - 1)
    dq gdt_start                 ; Base address

section .text
global load_gdt

; Load GDT and reload segment registers
; void load_gdt()
load_gdt:
    lgdt [gdt_ptr]              ; Load GDTR
    
    ; Reload code segment (CS)
    lea rax, [rel .reload_cs]
    push 0x08                   ; Kernel code segment
    push rax
    retfq

.reload_cs:
    ; Reload data segments
    mov ax, 0x10                ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ret

global gdt_get_ptr

; Get pointer to GDT
gdt_get_ptr:
    lea rax, [rel gdt_ptr]
    ret
