#include "kernel.h"

uint16_t *video_mem = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_initialize()
{
    video_mem = (uint16_t *)(0xB8000);
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            video_mem[(y * VGA_WIDTH) + x]=terminal_make_char(' ',0);
        }
    }
}


void kernel_main()
{
    
    terminal_initialize();
    video_mem[0] = terminal_make_char('B', 4); // a small tip? the processor stores this first as 0x41 then 0x03 thats why its flipped
    video_mem[1] = terminal_make_char('L', 4);
  
}

/* //Functions i wrote outside the course just to test things :D
int strlen(const char* ch){
    int count=0;
    while(ch[count] != '\0'){ // ngl took a while to figure out that '\0' this is null but i cant use normal (NULL)
        count++;
    }
    return count;
}

void termprint(char* ch,char c){
    int x=strlen(ch);
    for (int i = 0; i < x; i++)
    {
       video_mem[i] = terminal_make_char(ch[i],c);
    }
    
}
// Animation function i wrote just for fun it uses the first termprint and just iterates between color codes 1-7 and rewrites the string with a different color 
void animation(){
char a[]="Hello World :D ";
for (int i = 1; i <= 7; i++)
{
    termprint(a,i);
    delay(1000);
}

}
//Found this function with a google search forgot from where i got it :(
void delay(unsigned int milliseconds) {
    unsigned int count = milliseconds * 3000; // Adjust this based on your system's clock
    for (unsigned int i = 0; i < count; i++) {
        __asm__("nop"); // A no-op assembly instruction, may need adjustment depending on your CPU
    }
}
*/