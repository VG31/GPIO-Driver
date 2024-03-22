# GPIO-Driver

**What is GPIO?**

A "General Purpose Input/Output" (GPIO) is a flexible software-controlled digital signal. They are provided from many kinds of chips and are familiar to Linux developers working with embedded and custom hardware.

**Using GPIO Lines in Linux:-**

The Linux kernel exists to abstract and present hardware to users. GPIO lines as such are normally not user-facing abstractions. The most obvious, natural, and preferred way to use GPIO lines is to let kernel hardware drivers deal with them.

> [!IMPORTANT]
> Key information users need to know to achieve their goals.

**Raspberry Pi 3B Hardware Documentation:-**
![image](https://github.com/VG31/GPIO-Driver/assets/111217691/394cb222-22e2-4f9f-a2f5-66e02d5e97df)

> [!IMPORTANT]
> Key information users need to know to achieve their goals.

**The Problem:-**

When you are writing a code in user space, you are trying to access the GPIO pin you are trying to do two things illegally:-

**1)Memory Boundary:-** User space, we are accessing memory which is virtually translated to physical address either in RAM or physical interface. When we are accessing this without the permission of Kernel, this creates our first error. 

**2)Hardware Abstraction Layer:-** Directly accessing the GPIO bus without talking to the hardware abstraction layer.

![image](https://github.com/VG31/GPIO-Driver/assets/111217691/d5f3146d-782b-4030-944f-ea327da0226f)


**The Solution:-**

We can use Drivers.

**What is Driver?**

Drivers are  really an interface to legally cross memory boundary and legally cross hardware abstraction layer.


![image](https://github.com/VG31/GPIO-Driver/assets/111217691/c6c51abe-5eb3-4bb6-8900-8eacba97b162)


**Now, we have a driver, but how does user space talk to driver to get access to kernel space?**

The way to solve this is there are two types of filesystem 

**1)Device File:-** Special file maps into the kernel.

**2)Procfs File:-** Meant to do more generic things, not only meant to map to particularly to single hardware interface.

> [!NOTE]
> Useful information that users should know, even when skimming content.

So, for this project, we will use Procfs file.


In this project, I have written a code that finishes up the driver that allows a user to turn led on and off from user space via this driver's interface.

**Output:-**


https://github.com/VG31/GPIO-Driver/assets/111217691/c82b605d-cb0c-4769-ae04-6b99dac1b438



https://github.com/VG31/GPIO-Driver/assets/111217691/1f297380-c8ac-4cb1-9b04-4e33606c4acf



