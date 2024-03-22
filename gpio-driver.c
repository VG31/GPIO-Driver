#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/proc_fs.h>
#include <linux/slab.h>

#include <asm/io.h>

#define MAX_USER_SIZE 1024
#define BCM2837_GPIO_ADDRESS 0x3F200000

static struct proc_dir_entry *vg_proc=NULL; 		// used to track object when create a proc-fs file

static char data_buffer[MAX_USER_SIZE];

static unsigned int *gpio_registers = NULL;

static void gpio_pin_on(unsigned int pin)
{
	unsigned int fsel_index = pin/10;		//function select register
	unsigned int fsel_bitpos = pin%10;		//function select bit position, fuction select position which we have to enable in that register
	unsigned int* gpio_fsel = gpio_registers + fsel_index;		//makes the pin an output
	unsigned int* gpio_on_register = (unsigned int*)((char*)gpio_registers + 0x1c);		//calculate the address of register to turn pin on

	*gpio_fsel &= ~(7 << (fsel_bitpos*3));		//sets the fuction select for this pin to all zeros
	*gpio_fsel |= (1 << (fsel_bitpos*3));		//turn on least significant bit for that pin
	*gpio_on_register |= (1 << pin);		//turn the pin on

	return;
}

static void gpio_pin_off(unsigned int pin)
{
	unsigned int *gpio_off_register = (unsigned int*)((char*)gpio_registers + 0x28);
	*gpio_off_register |= (1<<pin);
	return;
}

// read(file name,buffer,size,offset pointer)
// offset pointer is how much data is already read
ssize_t vg_read(struct file *file,char __user *user,size_t size,loff_t *off)
{
	//copy_to_user copies data from kernel space to user space
	return copy_to_user(user,"Hello, Welcome to File\n",7) ? 0:7;
}

ssize_t vg_write(struct file *file,const char __user *user,size_t size,loff_t *off)
{
	unsigned int pin=UINT_MAX;
	unsigned int value=UINT_MAX;

	memset(data_buffer,0x0,sizeof(data_buffer));

	if(size > MAX_USER_SIZE)	
	{
		size = MAX_USER_SIZE;
	}

	if (copy_from_user(data_buffer,user,size))
	{
		return 0;
	}

	printk("You Wrote: %s!\n",data_buffer);		//To confirm we are able to take data from user	
	
	if(sscanf(data_buffer, "%d,%d", &pin, &value) != 2)
	{
		printk("Inproper pin number entered\n");
		return size;
	}

	if(pin>21 || pin<0)
	{
		printk("Invalid Pin Entered");
		return size;
	}

	if(value != 0 && value != 1)
	{
		printk("Invalid On/Off");
		return size;
	}

	printk("Pin On: %d, value: %d\n", pin, value);

	if(value==1)
	{
		gpio_pin_on(pin);
	}
	else if(value==0)
	{
		gpio_pin_off(pin);
	}

	return size;
}

static const struct proc_ops proc_fops = 
{
	//tells us when the user read or write our file
	.proc_read = vg_read,
	.proc_write = vg_write,
};

static int __init gpio_driver_init(void){
	printk("Welcome to my driver!!!\n");

	gpio_registers = (int*)ioremap(BCM2837_GPIO_ADDRESS,PAGE_SIZE);		//Map in memory of gpio interface,as gpio interface exists at a physical address 

	if(gpio_registers == NULL)
	{
		printk("Failed to map gpio driver\n");
		return -1;
	}

	printk("Successfully Mapped");

	//create an entry in proc-fs
	//proc_create(name,permission,flags,address);
	vg_proc=proc_create("vg-gpio",0666,NULL,&proc_fops);
	if(vg_proc == NULL)
	{
		return -1;
	}
	return 0;
}

static void __exit gpio_driver_exit(void){
	printk("Leaving the driver!!!\n");
	proc_remove(vg_proc);
	return;
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIVEK G");
MODULE_DESCRIPTION("Testing of drivers in RASPI");
MODULE_VERSION("1.0");
