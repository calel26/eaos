section .header
; bootloader header
dw 26985 ; magic number
db 4, "eaos" ; kernel ID

section .text
_start: 
