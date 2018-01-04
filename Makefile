#ubuntu的内核源码树，如果要编译在ubuntu中安装的模块就打开这2个
#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build	

		
# 开发板的linux内核的源码树目录
KERNEL_DIR = /home/keystar/samba_share/linux-2.6.30


obj-m	+= ldd_bus.o testmini.o mini-class.o

all:
	make -C $(KERNEL_DIR) M=`pwd` modules
	@#arm-none-linux-gnueabi-gcc app_led.c -o app_led
	
cp:
	@#sudo cp led_class_dev.ko app_led /tftpboot
	sudo cp ldd_bus.ko testmini.ko mini-class.ko /tftpboot

.PHONY: clean	
clean:
	make -C $(KERNEL_DIR) M=`pwd` modules clean
	@#rm app_led