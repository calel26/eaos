section .text

%macro intr 1
extern %1
global %1_entry
%1_entry:
    cli

    ; push registers to stack
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    pushfq

    call %1

    ; pop registers from stack
    popfq
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ; Return from interrupt
    sti
    iretq
%endmacro

intr div_by_zero
intr single_step_interrupt
intr nmi_error
intr breakpoint
intr overflow
intr bound_range_exceeded
intr invalid_opcode
intr coprocessor_not_available
intr double_fault
intr invalid_tss
intr segment_invalid
intr stack_segment_fault
intr general_protection_fault
intr page_fault
intr x87_float_exception
intr alignment_check
intr machine_check
intr SIMD_float_exception
intr virtualization_exception
intr control_protection_exception

; PIC
intr timer_handle
intr kbd_handle
intr mouse_handle