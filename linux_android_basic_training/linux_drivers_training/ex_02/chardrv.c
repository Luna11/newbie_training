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

/* Defines  */
//#define SCULL_READ  0x10001
//#define SCULL_WRITE 0x10002

struct scull_dev{
	struct cdev scull_cdev;
	unsigned char mem[MEM_SIZE];
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

static struct file_operations scull_fops={
	.owner=THIS_MODULE,
	.open=scull_open,
	.release=scull_close,
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
