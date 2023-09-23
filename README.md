# Nebula OS

Learning OS development atm ,the goal is to create a small OS for educational purposes and to improve on my programming skills.


## Build Environment:
Install these packages(im using arch linux it may be different for your distro)

```nasm qemu-system-x86_64 i686-elf-gcc make```

If for whatever reason you cant find the i686-elf-gcc in your distro's repo
You will have to compile it on your own, refer to https://wiki.osdev.org/GCC_Cross-Compiler for more info.



## Run:
Build everything first with ```make all```            
Then boot up qemu with ```make run```

## Goals:
After finishing the course i plan on adding more functionality alone like 
1. Switching into long mode (64-bit mode)
2. Maybe make it POSIX Compliant? 
3. Adding everything neccessary to switch to VGA Mode so we can draw to the screen and have a GUI
4. maybe FAT32 file system driver
5. USB Driver?
6. Port/Write a C compiler (that's a huuuge thing for me)

(I'm still a beginner and it will prob take a few years to make all of it happen)


## Goals:
After finishing the course i plan on adding more functionality alone like 
1. Switching into long mode (64-bit mode)
2. Maybe make it POSIX Compliant? 
3. Adding everything neccessary to switch to VGA Mode so we can draw to the screen and have a GUI
4. maybe FAT32 file system driver
5. USB Driver?
6. Port/Write a C compiler (that's a huuuge thing for me)

(I'm still a beginner and it will prob take a few years to make all of it happen)


## Resources Used:
1. https://wiki.osdev.org/
2. https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/
