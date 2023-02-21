linux-0.01 On Ubuntu 18.04/20.04 compilable with GCC


Revised version of the first kernel that can be compiled with GCC on machines with Ubuntu 18.04/20.04 64 and 32 bit versions on Intel/AMD. 

The kernel runs in both emulators: QEMU ver. 2.11.1 and Bochs ver 2.6. 

Usage instructions 

https://mapopa.blogspot.com/2022/09/linux-001-compiling-on-ubuntu-64.html

I also uploaded the bochsrc.txt file so that the bochs runs from the command line from the root where the kernel(Image) is
and qemu runs from the cmd line by the command: make run, but first unzip the file hd_oldlinux.img.zip.

For more modern code there is Isoux fork of Linux-0.01 is compilable using NASM assembler and LLVM's Clang compiler with a few more changes

https://github.com/isoux/linux-0.01

