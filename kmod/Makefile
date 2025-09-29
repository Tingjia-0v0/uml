OUT ?= $(abspath ../rootfs/data/kmod)
SRC ?= $(shell pwd)
KERNELDIR ?= $(abspath ../linux-6.14)

obj-m := main.o controller.o

main.ko  controller.ko: main.c controller.c
	$(MAKE) -C ${KERNELDIR} M=$(SRC) modules ARCH=um
	cp $(SRC)/*.ko $(OUT)/

.PHONY: clean
clean:
	$(MAKE) -C ${KERNELDIR} M=$(SRC) clean

