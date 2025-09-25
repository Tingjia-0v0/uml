LINUX_DIR = linux-6.14

.PHONY: all
all: linux rootfs run

# Build the User Mode Linux kernel
.PHONY: linux
linux:
	${MAKE} -C ${LINUX_DIR} -j$(nproc) defconfig ARCH=um
	${MAKE} -C ${LINUX_DIR} -j$(nproc) ARCH=um

# Build the root filesystem
.PHONY: rootfs
rootfs:
	${MAKE} -C rootfs

# Run UML
.PHONY: run
run: rootfs
	./${LINUX_DIR}/linux ubd0=rootfs/rootfs.ext4 root=/dev/ubda mem=256M

.PHONY: clean
clean:
	${MAKE} -C rootfs clean
	${MAKE} -C ${LINUX_DIR} mrproper
