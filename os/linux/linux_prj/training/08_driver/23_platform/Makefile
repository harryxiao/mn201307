KERNELDIR ?=/lib/modules/$(shell uname -r)/build
#KERNELDIR ?=/home/linux/linux-2.6.35

module:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	#rm -rf *.ko *.o *.mod* modules* Mo* .*.cmd .tmp_versions
	make -C $(KERNELDIR) M=`pwd` clean

obj-m=platform_device.o platform_driver.o
