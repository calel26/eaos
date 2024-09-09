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
    jz printed ; if the char == NUL, we're done!
    int 0x10 ; print char interrupt
    jmp loop ; continue

printed:
    ; let's enable proected mode!
    jmp enable_protected

idle:
    hlt
    jmp $

enable_protected:
    ; enabling protected mode involves flipping one bit on
    ; the cr0 register (the first bit, PE, aka Protected mode Enabled)
    mov eax, cr0 ; move it into a scratch register
    or eax, 1 ; flip the bit
    mov cr0, eax ; save the modified control register
    ; what this means:
    ; - paging can be enabled
    ; 1MB --> 4GB of addressable memory
    ; 32bit (or 16bit) operand sizes (going to use 32)
    jmp protected_start

; PROTECTED MODE SECTION!
; now using 32 bit operand sizes
bits 32

protected_start:
    hlt
    jmp $

message: db "hola mundo!", 0

; mbr boot signature
times 510-($-$$) db 0
dw 0xAA55