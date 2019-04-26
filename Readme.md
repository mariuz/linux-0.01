linux-0.01 On Ubuntu 18.04 with GCC-7.3

First of all, thanks Mariuz!

After a lot of time, I managed to compile this revised version of the first kernel on machines with Ubuntu 18.04 64 and 32 bit versions. So you can compile them and try them on 64 and 32 bit Intel machines.

The kernel runs in both emulators: QEMU ver. 2.11.1 and Bochs ver 2.6. and that was a great success for me ...

I also uploaded the bochsrc.txt file so that the bochs runs from the command line from the root where the kernel(Image) is
and qemu runs from the cmd line by the command: make run, but first unzip the file hd_oldlinux.img.zip.

The error I noticed after initializing the system is that the app "vim" (if you try) has a problem with opening files.
But the general  problem is creating new files and writing in them. The app "touch" works smoothly. 
The app "cat" can read existing files and app "echo" can't writing in files.
I will try to fix this.

Enjoy work and improvement.
sincerely 
Isoux
