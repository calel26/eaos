section .text

global save_ctx
global restore_ctx

; clobbers rax and r8
save_ctx:
    ; it really doesn't make sense to store RDI, and it isn't restored
    ; oh well, removing it would mean I have to change all the offsets...
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

    ; rip is saved by the caller (b/c it's easier)

    pushfq
    pop rax
    mov [rdi + 136], rax

    ret

restore_ctx:
    ; why/how would I load RDI? it doesn't make sense, not doing it.
    mov rsi, [rdi + 8]
    mov rbp, [rdi + 16]
    mov rsp, [rdi + 24]
    ; rax is clobbered later to restore flags, so it will be restored at the end
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
    
    ; restore processor flags
    mov rax, [rdi + 136]
    push rax
    popfq

    ; finally restore rax
    mov rax, [rdi + 32]

    ; jump back to that code
    jmp [rdi + 128]
