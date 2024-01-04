KERNELDIR := /home/xerolyskinner/home/Linux-dev-template/linux-source-6.1
CURRENT_PATH := $(shell pwd)
obj-m := xerdev.o

build: kernel_modules
kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	@echo rmmod
	@rmmod xerdev
	@echo insmod
	@insmod xerdev.ko
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
