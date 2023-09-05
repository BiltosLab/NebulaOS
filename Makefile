# This is a comment line
CC=gcc
CFLAGS= 
ASM=nasm
all: os
os:
	$(ASM) -f bin boot/boot.asm -o ./bin/boot.bin
run:
	qemu-system-x86_64 -drive format=raw,file=./bin/boot.bin
clean:
	rm -rf ./bin/boot.bin

test:
	$(ASM) -f bin src/boot/boot.asm -o ./bin/boot.bin
	qemu-system-x86_64 -drive format=raw,file=./bin/boot.bin
