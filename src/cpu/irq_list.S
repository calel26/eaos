section .text

%macro intr 1
extern %1
global %1_entry
%1_entry:
    ; Prologue: Save registers to the stack
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    pushfq

    ; Call the C handler passed as a parameter
    call %1

    ; Epilogue: Restore registers from the stack
    popfq
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ; Return from interrupt
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