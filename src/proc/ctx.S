section .text

global save_ctx

save_ctx:
    ; push RIP to stack before calling
    mov [rdi], rdi
    mov [rdi + 8], rsi
    mov [rdi + 16], rbp
    mov [rdi + 24], rsp
    mov [rdi + 32], rax
    mov [rdi + 40], rbx
    mov [rdi + 48], rcx
    mov [rdi + 56], rdx
    mov [rdi + 64], r8
    mov [rdi + 72], r9
    mov [rdi + 80], r10
    mov [rdi + 88], r11
    mov [rdi + 96], r12
    mov [rdi + 104], r13
    mov [rdi + 112], r14
    mov [rdi + 120], r15

    ; load rip from stack
    pop rax
    mov [rdi + 128], rax

    pushfq
    pop rax
    mov [rdi + 136], rax

    ret

restore_ctx:
    mov rsi, [rdi + 8]
    mov rbp, [rdi + 16]
    mov rsp, [rdi + 24]
    mov rax, [rdi + 32]
    mov rbx, [rdi + 40]
    mov rcx, [rdi + 48]
    mov rdx, [rdi + 56]
    mov r8, [rdi + 64]
    mov r9, [rdi + 72]
    mov r10, [rdi + 80]
    mov r11, [rdi + 88]
    mov r12, [rdi + 96]
    mov r13, [rdi + 104]
    mov r14, [rdi + 112]
    mov r15, [rdi + 120]
    
    ; load rdi last
    ; (this causes issues and doesn't make sense, so I'm not doing it.)
    ; mov rdi, [rdi]

    ; jump back to that code
    jmp [rdi + 128]

new_ctx:
    xor rax, rax
