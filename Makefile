#Makefile for nebula OS 
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
FILES= ./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o ./build/memory/memory.o ./build/idt/idt.o ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o
INCLUDES= -I./src
ASM=nasm
CC=i686-elf-gcc
LD=i686-elf-ld
all: ./bin/boot.bin ./bin/kernel.bin 
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./build/kernelfull.o
	$(CC) $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	$(ASM) -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	$(ASM) -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	$(CC) $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	$(ASM) -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.c
	$(CC) $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o: ./src/memory/memory.c
	$(CC) $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.asm.o: ./src/io/io.asm
	$(ASM) -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o


./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	$(CC) $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	$(CC) $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o


./build/memory/paging/paging.o: ./src/memory/paging/paging.c
	$(CC) $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	$(ASM) -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

run:
	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin
clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ./build/kernelfull.o
	rm -rf $(FILES)
iso:
	dd if=/dev/zero of=boot.iso bs=512 count=2880
	dd if=./bin/os.bin of=boot.iso conv=notrunc bs=512 seek=0 count=1
