ORG 0
BITS 16
_start:
    jmp short start
    nop
 times 33 db 0

start:
    jmp 0x7c0:step2

handle_zero:
    mov si, messagedivz
    call print
    int 0x10
    iret

handle_one:
    mov ah, 0eh
    mov si, messageintone
    call print
    int 0x10
    iret

step2:
    cli ;Clear Interrupts
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enable Interruptes
    mov word[ss:0x00], handle_zero
    mov word[ss:0x02], 0x7c0


    mov word[ss:0x04], handle_one
    mov word[ss:0x06], 0x7c0
    
    int 1

    mov si, message
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

messagedivz: db 'DIVISION BY ZERO' , 0
messageintone: db 'INTERRUPT 1  ', 0
message: db 'Hello,World!' , 0

times 510-($ - $$) db 0
dw 0xAA55

;lec os 9