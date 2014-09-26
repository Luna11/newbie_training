/*
* Filename: test.c
*
* Copyright (C) 2014 Wistron
* All rights reserved.
*
* Description:
* This file to create nodes under /sys
* 安装之后/sys/devices/platform/att_test/test
* echo 1 > test 
* cat test
* dmesg之后会看到内核打印出调用store and show function
*
* Author/Created Date:
* Luna Wang, Sep26'14
*
* Modification History:
*
* Note:
*/

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>

struct att_dev{
	struct platform_device *pdev;
	struct kobject *kobj;
};

static ssize_t att_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	printk("echo debug buf\n");
	return count;
}

static ssize_t att_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("cat debug buf\n");
	return 0;
}

static DEVICE_ATTR(test, 0700, att_show, att_store);

static struct att_dev *dev = NULL;

static /* __devinit */ int att_probe(struct platform_device *ppdev)
{
	int ret;

	dev->kobj = kobject_create_and_add("attkobj", NULL);
	if(dev->kobj == NULL){
		ret = -ENOMEM;
		goto kobj_err;
	}

	ret = sysfs_create_file(&dev->pdev->dev.kobj, &dev_attr_test.attr); /* create an attribute file for an object */
	if(ret < 0){
		goto file_err;
	}
	return 0;

file_err:
	kobject_del(dev->kobj);
kobj_err:
	return ret;
}

static struct platform_driver att_driver = {
	.probe = att_probe,
	.driver = {
		.owner = THIS_MODULE,
		.name = "att_test",
	},
};

static int __init att_init(void)
{
	int ret;

	dev = kzalloc(sizeof(struct att_dev), GFP_KERNEL);
	if(dev == NULL){
		printk("%s get dev memory error.\n", __func__);
		return -ENOMEM;
	}

	dev->pdev = platform_device_register_simple("att_test", -1, NULL, 0);
	if(IS_ERR(dev->pdev)){
		PTR_ERR(dev->pdev);
		printk("%s pdev error.\n", __func__);
		return -1;
	}

	ret = platform_driver_register(&att_driver);
	if(ret < 0){
		printk("%s register driver error.\n", __func__);
		return ret;
	}

	return 0;
}

static void __exit att_exit(void)
{
	sysfs_remove_file(&dev->pdev->dev.kobj, &dev_attr_test.attr);
	kobject_del(dev->kobj);
	platform_device_unregister(dev->pdev);
	platform_driver_unregister(&att_driver);
	if(dev != NULL)
		kfree(dev);
}

module_init(att_init);
module_exit(att_exit);
MODULE_LICENSE("GPL");
