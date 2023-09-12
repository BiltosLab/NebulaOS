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


## Resources Used:
1- https://wiki.osdev.org/                                 
2- https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/
