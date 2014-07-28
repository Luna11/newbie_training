/*---------------------------------------------
Filename: 
	chardrv.c

Copyright (C) 2014 Wistron
	All rights reserved.

Description: 
	This file is a char device drvier.

Author/Created Date: 
	Luna Wang, Jul25'14
	
Modification History:
	v1. only create driver, but no related operation functions;
	v2. add file opretions such as read/write/llseek/ioctl;

Note:
	no
--------------------------------------------*/

/*  include files  */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/sched.h>

/* MACROS  */
#define MEM_SIZE    0x1000
#define MEM_CLEAR   0x1    /* clear all scull memory, and used in ioctl() */

struct scull_dev{
	struct cdev scull_cdev;
	unsigned char mem[MEM_SIZE];
	struct semaphore sem;
}* scull_devp;

static int major;

static struct class *cls;

static int scull_open(struct inode *inode, struct file *filp)
{
	filp->private_data=scull_devp;
	printk("%s\n", __func__);
	return 0;
}

static int scull_close(struct inode *inode, struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct scull_dev *scull_devp=filp->private_data;
	int retval=0;
	unsigned long p=*f_pos;

	if(down_interruptible(&scull_devp->sem))
		return -ERESTARTSYS;

	if(p>=MEM_SIZE)
		return count? -ENXIO:0;
	
	if((p+count)>MEM_SIZE)
		count=MEM_SIZE-p;

	if(copy_to_user(buf, (scull_devp->mem+p), count)){
		return -EFAULT;
	} else {
		*f_pos+=count;
		retval=count;
		printk("%s:copy %d bytes from %lu", __func__, retval, p);
	}
	return retval;
	
}

static ssize_t scull_write(struct file *filp, char const __user *buf, size_t count, loff_t *f_pos)
{
	struct scull_dev *scull_devp=filp->private_data;
	unsigned int retval=0;
	unsigned long p=*f_pos;

	if(down_interruptible(&scull_devp->sem))
		return -ERESTARTSYS;

	if(p>=MEM_SIZE)
		return count? -ENXIO:0;

	if((p+count)>MEM_SIZE)
		count=MEM_SIZE-p;

	if(copy_from_user((scull_devp+p), buf, count)){
		return -EFAULT;
	} else {
		*f_pos+=count;
		retval=count;
		printk("%s\n:copy %u types from user space %ld", __func__, retval, p);
		return retval;
	}
}

static loff_t scull_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t newpos;

	switch(orig)
	{
		case 0:
			if(offset>MEM_SIZE||offset<0){
				return -EFAULT;
			}
			newpos=offset;
			return newpos;

		case 1:
			if((filp->f_pos+offset)>MEM_SIZE || (filp->f_pos+offset)<0){
				return -EFAULT;
			}
			newpos=filp->f_pos+offset;
			return newpos;
			
		case 2:
			return -EFAULT;

		default:
			return -EFAULT;
	}
}

static long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct scull_dev *scull_devp=filp->private_data;

	switch(cmd){
		case MEM_CLEAR:
			memset(scull_devp->mem, 0, MEM_SIZE);
			printk("scull mem is set to 0\n");
			break;

		default:
			return -EINVAL;
	}

	return 0;
}



static struct file_operations scull_fops={
	.owner=THIS_MODULE,
	.open=scull_open,
	.release=scull_close,
	.read=scull_read,
	.write=scull_write,
	.llseek=scull_llseek,
	.unlocked_ioctl=scull_ioctl,
};

static int scull_init(void)
{
	dev_t devno;
	int res;

	res=alloc_chrdev_region(&devno, 0, 1, "scull");
	if(res<0)
	{
		return res;
	}
	major=MAJOR(devno);

	scull_devp=kmalloc(sizeof(struct scull_dev), GFP_KERNEL);
	if(!scull_devp)
	{
		res=-ENOMEM;
		goto fail_kmalloc;
	}
	memset(scull_devp, 0, sizeof(struct scull_dev));

	cdev_init(&(scull_devp->scull_cdev), &scull_fops);

	if(cdev_add(&(scull_devp->scull_cdev), devno, 1)<0)
	{
		res=-EFAULT;
		goto fail_cdev_add;
	}

	cls=class_create(THIS_MODULE, "scull");
	device_create(cls, NULL, devno, NULL, "scull0");

	printk("%s\n", __func__);
	return 0;

fail_cdev_add:kfree(scull_devp);
fail_kmalloc:unregister_chrdev_region(devno, 1);
	return res;
}


static void scull_exit(void)
{
	cdev_del(&(scull_devp->scull_cdev));
	unregister_chrdev_region(MKDEV(major, 0), 1);
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	kfree(scull_devp);

	printk("%s\n", __func__);
}

module_init(scull_init);
module_exit(scull_exit);
MODULE_LICENSE("GPL");
