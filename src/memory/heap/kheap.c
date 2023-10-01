#include "kheap.h"
#include "config.h"
#include "heap.h"
#include "kernel.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init()
{

    int total_table_entries = NEBULAOS_HEAP_SIZE_BYTES/NEBULAOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(NEBULAOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = (void*)(NEBULAOS_HEAP_ADDRESS + NEBULAOS_HEAP_SIZE_BYTES);

    int res = heap_create(&kernel_heap, (void*)(NEBULAOS_HEAP_ADDRESS), end ,&kernel_heap_table);
    if(res < 0){
        print("Failed to create heap\n"); // should be panic but will be implemented later
    }




}


void* kmalloc(size_t size){
    return heap_malloc(&kernel_heap, size);
}

void kfree(void* ptr){
    heap_free(&kernel_heap,ptr);
}