/*------------------------------------------
file name:
	my_timer.c
Copyright (C) 2014 Wistron
	All rights reserved.
Description:
	This file is a timer.
Author/Created Date:
	Luna Wang, Aug21'14
Modification History:
	no
Note:
	no
--------------------------------------------*/

/* INCLUDE FILES */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
//#include <linux/mm.h>
#include <linux/slab.h>

#define SECOND_MAJOR 0

static int second_major=SECOND_MAJOR;

struct second_dev {
	struct cdev cdev;
	atomic_t counter;              /* atomic variable */
	struct timer_list s_timer;
};

struct second_dev *second_devp;
struct class *cls;

/* the timer handle function */
static void second_timer_handle(unsigned long arg)
{
	mod_timer(&second_devp->s_timer, jiffies + HZ);    /*激活定时器，且设置新的定时值*/
	atomic_inc(&second_devp->counter);   /* 对变量counter用锁定总线的单指令进行不可分解的"原子"级增量操作，避免counter的值由于中断或多处理器同时操作造成不确定状态 */

	printk("%s\n", __func__);
	printk("current jiffies is %ld\n", jiffies);
}

/* open function */
static int second_open(struct inode *inode, struct file *filp)
{
	/* initialize timer */
	init_timer(&second_devp->s_timer);
	second_devp->s_timer.function = &second_timer_handle;   /* the callback function after the delay */
	second_devp->s_timer.expires = jiffies + 10*HZ;    /* expires stores the end time of delay, here it means delay 10s */

	add_timer(&second_devp->s_timer);   /* add or register timer */

	atomic_set(&second_devp->counter, 0);   /* set counter to 0 */

	printk("%s: open timer.\n", __func__);
	return 0;
}

static int second_release(struct inode *inode, struct file *filp)
{
	del_timer(&second_devp->s_timer);

	return 0;
}

static ssize_t second_read(struct file *filp, char __user *buf, size_t count, loff_t *fpos)
{
	int counter = atomic_read(&second_devp->counter);
	if(put_user(counter, (int *)buf)) {
		return -EFAULT;
	}
	else {
		return sizeof(int);
	}
}

static struct file_operations second_fops = {
	.owner = THIS_MODULE,
	.open = second_open,
	.release = second_release,
	.read = second_read,
};

static int second_init(void)
{
	int retval;
	dev_t devno = MKDEV(second_major, 0);

	if(second_major) {
		retval=register_chrdev_region(devno, 1, "second");
	} 
	else {
		retval = alloc_chrdev_region(&devno, 0, 1, "second");
		second_major = MAJOR(devno);
	}

	if(retval < 0) {
		printk("Device ID register failed!!\n");
		return retval;
	}

	second_devp = (struct second_dev *)kmalloc(sizeof(struct second_dev), GFP_KERNEL);
	if(!second_devp) {
		printk("malloc fail.\n");
		retval = -ENOMEM;
		goto fail_malloc;
	}

	memset(second_devp, 0, sizeof(struct second_dev));

	cdev_init(&second_devp->cdev, &second_fops);
	if (cdev_add(&second_devp->cdev, devno, 1) < 0) {
		printk("device second add error\n");
		retval = -EFAULT;
		goto fail_cdev_add;
	}

	cls = class_create(THIS_MODULE, "second0");
	device_create(cls, NULL, devno, NULL, "second0");

	printk("%s\n", __func__);
	return 0;

fail_cdev_add:
	kfree(second_devp);
fail_malloc:
	unregister_chrdev_region(devno, 1);
	return retval;
}

static void second_exit(void)
{
	device_destroy(cls, MKDEV(second_major, 0));
	class_destroy(cls);
	cdev_del(&second_devp->cdev);
	kfree(second_devp);
	unregister_chrdev_region(MKDEV(second_major, 0), 0);

	printk("%s\n", __func__);
}

module_init(second_init);
module_exit(second_exit);
MODULE_LICENSE("GPL");
