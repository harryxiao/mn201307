#include <linux/module.h>
#include <linux/init.h>

#include <linux/device.h>

int usb_bus_match(struct device *dev, struct device_driver *drv)
{	//仅仅配对名字的前9个字母是否相同
	if(!strncmp(dev->bus_id, drv->name, 9)){
		printk("match success\n");
		return 1;
	}else{
		printk("match failed\n");
		return 0;
	}
}

struct bus_type usb_bus = {
	.name = "usb",	//注册成功后将在/sys/bus目录下看到目录usb
	.match = usb_bus_match,
};

static int __init usb_bus_init(void)
{
	int ret;
	/*总线注册，必须检测返回值*/
	ret = bus_register(&usb_bus);
	if(ret){
		printk("bus register failed!\n");
		return ret;	
	}

	printk("usb bus init\n");
	return 0;
}

static void __exit usb_bus_exit(void)
{
	bus_unregister(&usb_bus);
	printk("usb bus bye!\n");
}

EXPORT_SYMBOL(usb_bus);		//导出符号usb_bus，让device和driver可以引用

module_init(usb_bus_init);
module_exit(usb_bus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiao bai");
