KERNELDIR := /home/xerolyskinner/home/device/linux-source-6.1
CURRENT_PATH := $(shell pwd)
obj-m := xerdev.o

build: kernel_modules
kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean