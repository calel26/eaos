global schedule
extern schedule_inner

schedule:
    ; immediately save the caller's RBP before it's modified.
    mov rdi, rbp

    ; calculate the caller's RSP.
    ; RSP currently points to the return address pushed by 'call schedule'.
    ; the caller's stack pointer before the call was RSP+8.
    mov rsi, rsp
    add rsi, 8

    ; get the original return address from the stack without modifying RSP yet.
    mov rdx, [rsp]

    ; jump to the C function (tail call optimization).
    ; this replaces our stack frame with schedule_inner's frame.
    jmp schedule_inner