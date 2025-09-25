BIN_DIR = rootfs/bin
EXES = $(patsubst %.c,$(BIN_DIR)/%,$(wildcard *.c))

.PHONY: all
all: rootfs.ext4

# Build the executables
$(BIN_DIR)/%: %.c
	gcc -Wall -Wextra -std=c99 -static -o $@ $<

# Create the root filesystem image
rootfs.ext4: $(EXES)
	dd if=/dev/zero of=rootfs.ext4 bs=1M count=64
	mkfs.ext4 -F rootfs.ext4
	sudo mount -o loop rootfs.ext4 mount
	sudo cp -r rootfs/* mount/
	sudo umount mount

.PHONY: clean
clean:
	rm -rf $(EXES)
