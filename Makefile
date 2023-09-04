# This is a comment line
CC=gcc
CFLAGS= 
ASM=nasm
all: os
os:
	$(ASM) -f bin boot/boot.asm -o x86bin/boot.bin
run:
	qemu-system-x86_64 -drive format=raw,file=x86bin/boot.bin 
clean:
	rm -rf /osbin

test:
	$(ASM) -f bin boot/boot.asm -o x86bin/boot.bin
	qemu-system-x86_64 -drive format=raw,file=x86bin/boot.bin 
