[BITS 32]
global _start
CODE_SEG equ 0x08
DATA_SEG equ 0x10
_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    in al, 0x92 ; Enable the A20 line using the Fast A20 Gate
    or al, 2
    out 0x92, al
    jmp $