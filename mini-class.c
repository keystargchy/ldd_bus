#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>


#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <asm/gpio.h>


MODULE_AUTHOR("ChunYu Guan <guanchunyu123@126.com>");
MODULE_DESCRIPTION("ldd bus add");
MODULE_LICENSE("GPL");



struct class *mini_class;
struct device *my_device;

int mini_classdev_register(struct device *dev)
{
	my_device = device_create(mini_class, dev, 0, NULL, "mymod0");    //创建雷设备接口必须要用到这个目录，不然总是提示cannot create duplicate filename这样的错误

	return 0;
}
EXPORT_SYMBOL_GPL(mini_classdev_register);


static int __init mini_class_init(void)
{
	mini_class = class_create(THIS_MODULE, "mini-class");
	if (IS_ERR(mini_class))
		return PTR_ERR(mini_class);

	return 0;
}

static void __exit mini_class_exit(void)
{
	class_destroy(mini_class);
}

module_init(mini_class_init);
module_exit(mini_class_exit);

