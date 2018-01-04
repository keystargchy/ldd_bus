#include <linux/module.h>
#include <linux/init.h>



#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <asm/gpio.h>


#include "ldd_bus.h"

#include "mini-class.h"

MODULE_AUTHOR("ChunYu Guan <guanchunyu123@126.com>");
MODULE_DESCRIPTION("ldd bus add");
MODULE_LICENSE("GPL");



static int mymini_probe(struct ldd_device *dev)
{
	printk(KERN_INFO "ldd_device probe: mymini_probe\n");
	mini_classdev_register(&dev->dev);                       //在设备匹配上以后，在probe中进行class接口的设置
	return 0;
}



struct ldd_device mymini_dev = {
	.name = "mymini",

};


struct ldd_driver mymini_drv = {
	.driver = {
		.name	= "mymini",
		.owner	= THIS_MODULE,
	},
	.probe = mymini_probe,
};








static int __init my_mini_init(void)
{
	register_ldd_device(&mymini_dev);
	return register_ldd_driver(&mymini_drv);
}


static void __exit my_mini_exit(void)
{
	unregister_ldd_device(&mymini_dev);
	unregister_ldd_driver(&mymini_drv);

}

module_init(my_mini_init);
module_exit(my_mini_exit);


