/*---------------------------------------------
Filename:
misc.c

Copyright (C) 2014 Wistron
All rights reserved.

Description:
This file is a misc char device driver.

Author/Created Date:
Luna Wang, Aug04'14

Modification History:

Note:

--------------------------------------------*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include "misc_cmd.h"

#define MEM_SIZE 0x1000

/*  remove to file misc_cmd.h    
#define MISC_IOC_MAGIC  'y'
#define MISC_IOC_MAXNR  3
#define MISC_READ  _IOR(MISC_IOC_MAGIC, 1, int)
#define MISC_WRITE  _IOW(MISC_IOC_MAGIC, 2, int)
#define MISC_IOC_TEST  _IO(MISC_IOC_MAGIC,3)
*/

struct misc_dev {
	struct miscdevice miscdev;
	unsigned char a[MEM_SIZE];
};

struct misc_dev *misc_devp;

static int misc_open(struct inode *inode, struct file *filp)
{
	filp->private_data=misc_devp;
	printk("%s\n", __func__);
	return 0;
}

static int misc_close(struct inode *inode, struct file *filp)
{
	printk("%s\n", __func__);
	return 0;
}

static ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int retval=0;
	unsigned long p=*f_pos;
	struct misc_dev *misc_devp=filp->private_data;

	if(p>MEM_SIZE){
		return retval;
	}

	if((count+p)>MEM_SIZE){
		count=MEM_SIZE-p;
	}

	if(copy_to_user(buf, misc_devp->a+p, count)){
		return -EFAULT;
	} else {
		*f_pos+=count;
		retval=count;
		printk("%s:copy %d bytes to user from %ld\n", __func__,(int)count, p);
	}

	return retval;
}

static ssize_t misc_write(struct file *filp, char const __user *buf, size_t count, loff_t *f_pos)
{
	int retval=0;
	unsigned long p=*f_pos;
	struct misc_dev *misc_devp=filp->private_data;

	if(p>MEM_SIZE)
	{
		return retval;
	}

	if((p+count)>MEM_SIZE)
	{
		count=MEM_SIZE-p;
	}

	if(copy_from_user(misc_devp->a+p, buf, count))
	{
		return -EFAULT;
	}
	else
	{
		*f_pos+=count;
		retval=count;
		printk("%s:copy %d bytes from user space from %ld", __func__, (int)count, p);
	}

	return retval;
}

static loff_t misc_llseek(struct file *filp, loff_t offset, int org)
{
//	struct misc_dev *misc_devp=filp->private_data;
	int retval=0;
	switch(org)
	{
		case 0:
			if(offset<0){
				retval=-EINVAL;
				break;
			}
			if(offset>MEM_SIZE){
				retval=-EINVAL;
				break;
			}
			filp->f_pos=(unsigned int)offset;
			retval=filp->f_pos;
			break;
		
		case 1:
			if((filp->f_pos+offset)<0){
				retval=-EINVAL;
				break;
			}
			if((filp->f_pos+offset)>MEM_SIZE){
				retval=-EINVAL;
				break;
			}
			filp->f_pos+=offset;
			retval=filp->f_pos;
			break;

		case 2:
			return -EINVAL;
			break;
			
		default:
			return -EINVAL;
			break;
	}
	return retval;
}

static long misc_ioctl(/*struct inode *inode,*/ struct file *filp, unsigned int cmd, unsigned long arg)
{
	if(_IOC_TYPE(cmd)!=MISC_IOC_MAGIC)
		return -EINVAL;
	if(_IOC_NR(cmd)>MISC_IOC_MAXNR)
		return -EINVAL;

	switch(cmd){
		case MISC_READ:   //read from kernel driver
			{	int data=33312;
				copy_to_user((int *)arg, &data, sizeof(int));
				printk("%s\tMISC_READ\t%d\n", __func__, data);
			}
			break;

		case MISC_WRITE:   //WRITE TO KERNEL DRIVER
			{
				int data=0;
				copy_from_user(&data, (int *)arg, sizeof(int));
				printk("%s\tMISC_WRITE\t%d\n", __func__, data);
			}
			break;

		case MISC_IOC_TEST:
			{
				printk("%s\tMISC_NO_ARGUMENT_TEST\n", __func__);
			}
			break;

		default:
			break;
	}

	return 0;
}

static struct file_operations misc_fops={
	.owner=THIS_MODULE,
	.open=misc_open,
	.release=misc_close,
	.read=misc_read,
	.write=misc_write,
	.llseek=misc_llseek,
	.unlocked_ioctl=misc_ioctl,
};

static struct miscdevice miscdev={
	.minor=MISC_DYNAMIC_MINOR,
	.name="miscdev",
	.fops=&misc_fops,
};

static int misc_init(void)
{
	int retval;
	misc_devp=kmalloc(sizeof(struct misc_dev), GFP_KERNEL);
	if(!misc_devp)
	{
		retval=-ENOMEM;
		goto malloc_fail;
	}
	memset(misc_devp, 0, sizeof(struct misc_dev));

	misc_register(&miscdev);

	printk("%s\n", __func__);
	return 0;

malloc_fail:
	kfree(misc_devp);
	return retval;
}

static void misc_exit(void)
{
	misc_deregister(&miscdev);
	kfree(misc_devp);

	printk("%s\n", __func__);
}

module_init(misc_init);
module_exit(misc_exit);
MODULE_LICENSE("GPL");
