.PHONY: all
all: rootfs.ext4

# Build the init program
rootfs/bin/init: init.c
	mkdir -p rootfs/bin
	gcc -Wall -Wextra -std=c99 -static -o rootfs/bin/init init.c

# Create the root filesystem image
rootfs.ext4: rootfs/bin/init
	dd if=/dev/zero of=rootfs.ext4 bs=1M count=64
	mkfs.ext4 -F rootfs.ext4
	sudo mount -o loop rootfs.ext4 mount
	sudo cp -r rootfs/* mount/
	sudo umount mount

.PHONY: clean
clean:
	rm -rf rootfs/bin/init
