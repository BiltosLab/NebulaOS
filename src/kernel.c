#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"

uint16_t *video_mem = 0;
uint16_t terminal_row= 0;
uint16_t terminal_col= 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}
void terminal_putchar(int x,int y,char c,char colour){
    video_mem[(y * VGA_WIDTH) + x]=terminal_make_char(c,colour);
}
void terminal_writechar(char c,char colour){
    if(c=='\n'){
        terminal_row+=1;
        terminal_col=0;
        return;
    }
        if(c=='\t'){
        terminal_col+=4;
        return;
    }
    terminal_putchar(terminal_col,terminal_row,c,colour);
    terminal_col+=1;
    if(terminal_col>=VGA_WIDTH){
        terminal_col=0;
        terminal_row +=1;
    }
}
void terminal_initialize()
{
    terminal_row= 0;
    terminal_row= 0;
    video_mem = (uint16_t *)(0xB8000);
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x,y,' ',0);
        }
    }
}

void enablecursor(){
    unsigned short cursorLocation = ((terminal_row * VGA_WIDTH) + terminal_col);
    outb(0x3D4, 0x0F);
  	outb(0x3D5, (unsigned char)(cursorLocation));
  	//cursor HIGH port to vga INDEX register
  	outb(0x3D4, 0x0E);
  	outb(0x3D5, (unsigned char)((cursorLocation >> 8)));
}

void disablecursor(){
    //Disable Cursor
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}




size_t strlen(const char* str){
    size_t len=0;
    while(str[len]){
        len++;
    }
    return len;
}

void print(const char* str){
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i],15);
    }
    
}


static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main()
{
    terminal_initialize();
    print("Hello world!\nHelloWorld\tHelloWorld");

    //Initialize the heap
    kheap_init();

    //Initialize the interrupt descriptor table
    idt_init();
    
    // Setup Paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    enable_paging();

    char buf[512];
    disk_read_sector(0,1,buf);
    //Enable the system interrupts
    enable_interrupts();  

    
}

// Commands for GDB
// target remote | qemu-system-i386 -hda ./os.bin -S -gdb stdio
// add-symbol-file ../build/kernelfull.o 0x100000