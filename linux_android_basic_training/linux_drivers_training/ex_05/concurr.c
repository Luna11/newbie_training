/*
* Filename: concurr.c
*
* Copyright (C) 2014 Wistron
* 	All rights reserved.
*
* Description: 
*	This file is a char device drvier with semaphore
*
* Author/Created Date: 
*	Luna Wang, Aug12'14
*
* Modification History:
*
* Note:
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <linux/semaphore.h>

#define MEM_SIZE  0x1000
#define SCULL_MAJOR  0

struct scull_chardev{
	char a[MEM_SIZE];
	struct semaphore sem;
	struct cdev cdev;
}* scull_cdevp;

static struct class *cls;
static int major;

static int scull_open(struct inode *inode, struct file *filp)
{
	filp->private_data=scull_cdevp;
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
	int retval;
	unsigned long p=*f_pos;
	struct scull_chardev *scull_devp=filp->private_data;
	
	printk("%s\n", __func__);
	
// * get sem 
	retval=down_interruptible(&scull_devp->sem);
	if(retval!=0){
		printk("****interrupted by user.****\n");
		retval=-ERESTARTSYS;
		goto out;
	} else {
		printk("****now, get sem and you can read.****\n");
	}

	if(p>MEM_SIZE){
		if(count){
			retval=-ENXIO;
		} else {
			retval=0;
		}

		goto out;
	}

	if((p+count)>MEM_SIZE){
		count=MEM_SIZE-p;
	}

	if(copy_to_user(buf, scull_devp->a+p, count)){
		retval=-EFAULT;
		goto out;
	} else {
		*f_pos+=count;
		retval=count;
		printk("*****%s: copy %d bytes to user from pos %lu", __func__, (int)count, p);
	}

out:
	up(&scull_devp->sem);
	return retval;
}

static ssize_t scull_write(struct file *filp, char const __user *buf, size_t count, loff_t *f_pos)
{
	int retval;
	unsigned long p=*f_pos;
	struct scull_chardev *scull_devp=filp->private_data;
	
	retval=down_interruptible(&scull_devp->sem);
	if(retval!=0){
		printk("****interrupted by user.****\n ");
		retval=-ERESTARTSYS;
		goto out;
	} else {
		printk("****get sema, you can write now.****\n");
	}

	if(p>MEM_SIZE){
		if(count){
			retval=-ENXIO;
		} else {
			retval=0;
		}
		goto out;
	}

	if((p+count)>MEM_SIZE){
		count=MEM_SIZE-p;
	}

	if(copy_from_user(scull_devp->a+p, buf, count)){
		retval=-EFAULT;
		goto out;
	} else {
		*f_pos+=count;
		retval=count;
		printk("*****%s: copy %d bytes from user pos %lu", __func__, (int)count, p);
	}

out:
	up(&scull_devp->sem);
	return retval;
}

static loff_t scull_lseek(struct file *filp, loff_t offset, int org)
{
	int retval=0;

	switch(org){
		case 0:
			if(offset<0){
				retval=-EINVAL;
				break;
			}
			if(offset>MEM_SIZE){
				retval=-EINVAL;
				break;
			}

			filp->f_pos=offset;
			retval=filp->f_pos;
			break;

		case 1:
			if((offset+filp->f_pos)<0 || (offset+filp->f_pos)>MEM_SIZE ){
				retval=-EINVAL;
				break;
			}

			filp->f_pos+=offset;
			retval=filp->f_pos;
			break;

		case 2:
			return -EINVAL;

		default:
			return -EINVAL;
	}

	return retval;
}   

static struct file_operations scull_fops={
	.owner=THIS_MODULE,
	.open=scull_open,
	.release=scull_close,
	.read=scull_read,
	.write=scull_write,
	.llseek=scull_lseek, 
};

static int scull_init(void)
{
	dev_t devno;
	int result=0;
	major=SCULL_MAJOR;
	
	if(major){
		devno=MKDEV(major, 0);
		result=register_chrdev_region(devno, 1, "scull");
	} else {
		result=alloc_chrdev_region(&devno, 0, 1, "scull");
		major=MAJOR(devno);
	}

	if(result<0){
		return result;
	}

//	sema_init(&(scull_cdevp->sem), 1);
	
	scull_cdevp=kmalloc(sizeof(struct scull_chardev), GFP_KERNEL);
	if(!scull_cdevp){
		return -ENOMEM;
	}
	memset(scull_cdevp, 0, sizeof(struct scull_chardev));

	/* create and initialize semaphore */
	sema_init(&(scull_cdevp->sem), 1);

	cdev_init(&(scull_cdevp->cdev), &scull_fops);

	if(cdev_add(&(scull_cdevp->cdev), devno, 1)<0){
		return -EFAULT;
	}

	cls=class_create(THIS_MODULE, "scull0");
	device_create(cls, NULL, devno, NULL, "scull0");

//sema_init(&(scull_cdevp->sem), 1);

	printk("%s\n", __func__);
	return 0;

}

static void scull_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	cdev_del(&(scull_cdevp->cdev));
	kfree(scull_cdevp);
	unregister_chrdev_region(MKDEV(major, 0), 1);

	printk("%s\n", __func__);
}

module_init(scull_init);
module_exit(scull_exit);
MODULE_LICENSE("GPL");
