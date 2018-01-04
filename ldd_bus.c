#include <linux/module.h>
#include <linux/init.h>



#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <asm/gpio.h>


#include "ldd_bus.h"

MODULE_AUTHOR("ChunYu Guan <guanchunyu123@126.com>");
MODULE_DESCRIPTION("ldd bus add");
MODULE_LICENSE("GPL");




static ssize_t show_myfun(struct bus_type *bus, char *buf)
{
	printk(KERN_INFO "I am in show_myfun\n");
	return 0;
}


static ssize_t store_myfun(struct bus_type *bus,
				       const char *buf, size_t count)
{
	printk(KERN_INFO "I am in store_myfun\n");
	return 0;
}


static BUS_ATTR(myattr, S_IWUSR | S_IRUGO, show_myfun, store_myfun);    // 这里添加的属性文件会在bus_register函数中添加进去
																		//然后显示在文件系统中


static int ldd_match(struct device *dev, struct device_driver *drv)
{
	struct ldd_device *ldev = to_ldd_device(dev);
	printk(KERN_INFO "ldev->name: %s\n", ldev->name);
	printk(KERN_INFO "rv->name: %s\n", drv->name);
	return !strncmp(ldev->name, drv->name, strlen(drv->name));	
}
																		

struct device ldd_bus = {
	.init_name = "ldd",
	
};

struct bus_type ldd_bus_type = {
	.name = "ldd_bus",
	.bus_attrs = &bus_attr_myattr,
	.match = ldd_match,
};




int register_ldd_device(struct ldd_device *ldddev)
{
	ldddev->dev.bus = &ldd_bus_type;
	ldddev->dev.parent = &ldd_bus;      //如果此处为NULL，也就是不设置dev的parent，那么会在device_register中的device_initialize函数
										//来设置dev->kobj.kset,然后在device_add中定位目录位置
	ldddev->dev.init_name = ldddev->name;   //这里必须给dev.init_name赋值，因为device_register中会根据此判断是否给kobj中的name赋值已决定函数是否正确
	return device_register(&ldddev->dev);    //不同于platform_device_register，这个平台总线在platform_device_add中又封装了一层  
}

void unregister_ldd_device(struct ldd_device *ldddev)
{
	device_unregister(&ldddev->dev);
}
EXPORT_SYMBOL(register_ldd_device);  
EXPORT_SYMBOL(unregister_ldd_device);


static int ldd_drv_probe(struct device *_dev)
{
	struct ldd_driver *drv = to_ldd_driver(_dev->driver);    //这个设备是什么时候指向的驱动的呢？
	struct ldd_device *dev = to_ldd_device(_dev);
	printk(KERN_INFO "ldd_drv_probe\n");
	return drv->probe(dev);     //此处的drv就是驱动人员手动编写的平台驱动结构体（不同的设备有不同的平台驱动结构体）
								//在具体的设备驱动文件中编写此函数的实现
}

static int ldd_drv_remove(struct device *_dev)
{
	struct ldd_driver *drv = to_ldd_driver(_dev->driver);
	struct ldd_device *dev = to_ldd_device(_dev);

	return drv->remove(dev);
}

static void ldd_drv_shutdown(struct device *_dev)
{
	struct ldd_driver *drv = to_ldd_driver(_dev->driver);
	struct ldd_device *dev = to_ldd_device(_dev);

	drv->shutdown(dev);
}

static int ldd_drv_suspend(struct device *_dev, pm_message_t state)
{
	struct ldd_driver *drv = to_ldd_driver(_dev->driver);
	struct ldd_device *dev = to_ldd_device(_dev);

	return drv->suspend(dev, state);
}

static int ldd_drv_resume(struct device *_dev)
{
	struct ldd_driver *drv = to_ldd_driver(_dev->driver);
	struct ldd_device *dev = to_ldd_device(_dev);

	return drv->resume(dev);
}

int register_ldd_driver(struct ldd_driver *drv)
{
	drv->driver.bus = &ldd_bus_type;
	if (drv->probe)
		drv->driver.probe = ldd_drv_probe;
	if (drv->remove)
		drv->driver.remove = ldd_drv_remove;
	if (drv->shutdown)
		drv->driver.shutdown = ldd_drv_shutdown;
	if (drv->suspend)
		drv->driver.suspend = ldd_drv_suspend;
	if (drv->resume)
		drv->driver.resume = ldd_drv_resume;
	return driver_register(&drv->driver);
}

void unregister_ldd_driver(struct ldd_driver *drv)
{
	driver_unregister(&drv->driver);
}

EXPORT_SYMBOL(register_ldd_driver);  
EXPORT_SYMBOL(unregister_ldd_driver);




static int __init ldd_bus_init(void)
{
	int error = 0;

	error = device_register(&ldd_bus);    /* 如果parent和bus为NULL  在/sys/devices目录下创建一个设备目录，设备有一个属性uevent */
	if(error)
		return error;

	error = bus_register(&ldd_bus_type);		/* 在/sys/bus目录下创建一个总线目录 */
	if(error)
		bus_unregister(&ldd_bus_type);
	
	printk(KERN_INFO "error = %d\n", error);	

	return error;
}


static void __exit ldd_bus_exit(void)
{
	bus_unregister(&ldd_bus_type);
	device_unregister(&ldd_bus);
}

module_init(ldd_bus_init);
module_exit(ldd_bus_exit);

