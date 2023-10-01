#ifndef CONFIG_H
#define CONFIG_H

#define NEBULAOS_TOTAL_INTERRUPTS 512
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

// 100MB heap size maybe later on we try to find how much memory is on the host system then make the heap based on that
#define NEBULAOS_HEAP_SIZE_BYTES 104857600
#define NEBULAOS_HEAP_BLOCK_SIZE 4096
#define NEBULAOS_HEAP_ADDRESS 0x01000000
#define NEBULAOS_HEAP_TABLE_ADDRESS 0x00007E00



#endif