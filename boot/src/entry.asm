; Hybrid OS Kernel Entry Point (x86_64)
; This is the entry point called by the Limine bootloader

bits 64

section .text
global kernel_entry
extern kernel_main

; Kernel entry point
; Stack is set up by bootloader
; RDI = struct limine_bootinfo *
kernel_entry:
    ; Disable interrupts
    cli
    
    ; Set up new stack (if needed)
    mov rsp, kernel_stack_top
    
    ; Preserve bootloader info pointer in RDI
    ; (will be passed to kernel_main)
    
    ; Clear registers
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    xor r8, r8
    xor r9, r9
    xor r10, r10
    xor r11, r11
    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15
    
    ; Call kernel main
    ; RDI already contains bootloader info
    call kernel_main
    
    ; If kernel_main returns (shouldn't happen), halt
    jmp .halt
    
.halt:
    cli
    hlt
    jmp .halt

section .bss
align 4096
kernel_stack:
    resb 65536  ; 64 KB stack
kernel_stack_top:
