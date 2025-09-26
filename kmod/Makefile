OUT ?= $(abspath ../rootfs/data/kmod)
SRC ?= $(shell pwd)
KERNELDIR ?= $(abspath ../linux-6.14)

obj-m := main.o
CFLAGS_main.o := -I$(KERNELDIR)

main.ko: main.c
	$(MAKE) -C ${KERNELDIR} M=$(SRC) modules ARCH=um
	cp $(SRC)/main.ko $(OUT)/main.ko

.PHONY: clean
clean:
	$(MAKE) -C ${KERNELDIR} M=$(SRC) clean
