#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
//#define DEBUG
uint16_t *video_mem = 0;
uint16_t terminal_row= 0;
uint16_t terminal_col= 0;

typedef char* va_list;

#define va_start(ap, last_arg) (ap = (va_list)&last_arg + sizeof(last_arg))
#define va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = (va_list)0)


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
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        // Swap characters at start and end
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        // Move towards the center
        start++;
        end--;
    }
}

char* itoa(int num, char str[], int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int remainder = num % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        num = num / base;
    }

    // Append negative sign for base 10
    if (isNegative && base == 10) {
        str[i++] = '-';
    }

    str[i] = '\0';  // Null-terminate the string

    // Reverse the string
    reverse(str, i);

    return str;
}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++; // Move past '%'
            switch (*format) {
                case 's': {
                    char* str = va_arg(args, char*);
                    while (*str != '\0') {
                        terminal_writechar(*str, 15); // Adjust color as needed
                        str++;
                    }
                    break;
                }
                case 'd': {
                        long long num = va_arg(args, int);
                        char buffer[20];       // Adjust the buffer size as needed
                        itoa(num, buffer, 10); // Convert integer to string in base 10
                        size_t buffer_len = strlen(buffer);

                        // Print each character in the buffer
                        for (size_t j = 0; j < buffer_len; j++)
                        {
                            terminal_writechar(buffer[j], 15); // Adjust color as needed
                        }
                        break;
}
                // Add more cases for other format specifiers as needed
                default:
                    // Handle unsupported format specifiers or just print '%'
                    terminal_writechar('%', 15); // Adjust color as needed
                    terminal_writechar(*format, 15); // Adjust color as needed
                    break;
            }
        } else {
            terminal_writechar(*format, 15); // Adjust color as needed
        }
        format++;
    }

    va_end(args);
}


void printok()
{   
    char a[]="  [OK!]\n";
        size_t len = strlen(a);
    for (int i = 0; i < len; i++)
    {
        if (a[i]=='[' || a[i]==']')
        {
            terminal_writechar(a[i],15);
        }
        else{
            terminal_writechar(a[i],10);

        }
    }
}

int memorydetect()
{
    unsigned short total;
    unsigned char lowmem, highmem;
 
    outb(0x70, 0x30);
    lowmem = insb(0x71);
    outb(0x70, 0x31);
    highmem = insb(0x71);
 
    total = lowmem | highmem << 8;
    return total;
}



static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main()
{
    long long x = memorydetect();
    terminal_initialize();
   // print("Hello world!\nHelloWorld\tHelloWorld\n");
    print("Nebula OS version 0.001 starting\n");

    print("Initializing the heap");
    //Initialize the heap
    kheap_init();
    printok();
    print("Search and initilaize the disks");
    //Search and initilaize the disks
    disk_search_and_init();
    //start at lecture 27
    printok();

    //Initialize the interrupt descriptor table
    print("Initialize the interrupt descriptor table");

    idt_init();
    printok();
    // Setup Paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    print("enable paging");

    enable_paging();
    printok();
    //Enable the system interrupts
    print("enable interrupts");
    enable_interrupts();  
    
    printok();
    printf("Free Memory %d KB\n", x);

}

// Commands for GDB
// target remote | qemu-system-i386 -hda ./os.bin -S -gdb stdio
// add-symbol-file ../build/kernelfull.o 0x100000