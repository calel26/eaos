org 0x7C00

bits 16
section .text
global _start

_start:
    cli ; turn off interrupts
    mov si, message
    mov ah, 0x0E ; set up interrupt. 0x0E means we're printing chars
loop:
    lodsb ; loads the next byte into al
    or al, al
    jz idle ; if the char == NUL, we're done!
    int 0x10 ; print char interrupt
    jmp loop ; continue

idle:
    mwait
    jmp $

message: db "hola mundo!", 0

; mbr boot signature
times 510-($-$$) db 0
dw 0xAA55