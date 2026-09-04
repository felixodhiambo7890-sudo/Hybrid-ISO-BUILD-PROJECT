; IDT (Interrupt Descriptor Table) Stubs
; Exception and interrupt handlers for x86_64

bits 64

section .data
align 16

; ISR stub macro - for exceptions with error code
%macro ISR_ERROR_CODE 1
global isr%1
isr%1:
    ; Error code already pushed by CPU
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    ; Save registers
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    ; Call C handler: exception_handler(int_number, registers *regs)
    mov rdi, %1
    mov rsi, rsp
    call exception_handler
    
    ; Restore registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    add rsp, 16
    pop rbp
    iretq
%endmacro

; ISR stub macro - for exceptions without error code
%macro ISR_NO_ERROR 1
global isr%1
isr%1:
    ; No error code, so push 0
    push 0
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    ; Save registers
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    ; Call C handler
    mov rdi, %1
    mov rsi, rsp
    call exception_handler
    
    ; Restore registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    add rsp, 16
    pop rbp
    add rsp, 8  ; Skip error code
    iretq
%endmacro

; Exception handlers (0-31 are reserved)
; 0 - Divide Error (no error code)
ISR_NO_ERROR 0
; 1 - Debug Exception (no error code)
ISR_NO_ERROR 1
; 2 - NMI (no error code)
ISR_NO_ERROR 2
; 3 - Breakpoint (no error code)
ISR_NO_ERROR 3
; 4 - Overflow (no error code)
ISR_NO_ERROR 4
; 5 - BOUND Range Exceeded (no error code)
ISR_NO_ERROR 5
; 6 - Invalid Opcode (no error code)
ISR_NO_ERROR 6
; 7 - Device Not Available (no error code)
ISR_NO_ERROR 7
; 8 - Double Fault (HAS error code)
ISR_ERROR_CODE 8
; 9 - Coprocessor Segment Overrun (no error code)
ISR_NO_ERROR 9
; 10 - Invalid TSS (HAS error code)
ISR_ERROR_CODE 10
; 11 - Segment Not Present (HAS error code)
ISR_ERROR_CODE 11
; 12 - Stack-Segment Fault (HAS error code)
ISR_ERROR_CODE 12
; 13 - General Protection Fault (HAS error code)
ISR_ERROR_CODE 13
; 14 - Page Fault (HAS error code)
ISR_ERROR_CODE 14
; 15 - Reserved (no error code)
ISR_NO_ERROR 15
; 16 - Floating-Point Error (no error code)
ISR_NO_ERROR 16
; 17 - Alignment Check (HAS error code)
ISR_ERROR_CODE 17
; 18 - Machine Check (no error code)
ISR_NO_ERROR 18
; 19 - SIMD Floating-Point Exception (no error code)
ISR_NO_ERROR 19
; 20-31 Reserved (no error code)
ISR_NO_ERROR 20
ISR_NO_ERROR 21
ISR_NO_ERROR 22
ISR_NO_ERROR 23
ISR_NO_ERROR 24
ISR_NO_ERROR 25
ISR_NO_ERROR 26
ISR_NO_ERROR 27
ISR_NO_ERROR 28
ISR_NO_ERROR 29
ISR_NO_ERROR 30
ISR_NO_ERROR 31

section .text

; External interrupt handlers (32-255)
%macro ISR_INT 1
global isr%1
isr%1:
    ; No error code
    push 0
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    mov rdi, %1
    mov rsi, rsp
    call interrupt_handler
    
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    add rsp, 16
    pop rbp
    add rsp, 8
    iretq
%endmacro

; External interrupts (32-255) - create stubs on demand
ISR_INT 32
ISR_INT 33
ISR_INT 34
ISR_INT 35
ISR_INT 36
ISR_INT 37
ISR_INT 38
ISR_INT 39
ISR_INT 40
ISR_INT 41
ISR_INT 42
ISR_INT 43
ISR_INT 44
ISR_INT 45
ISR_INT 46
ISR_INT 47
