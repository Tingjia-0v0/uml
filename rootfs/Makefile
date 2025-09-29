IMG ?= img.ext4

.PHONY: all
all: $(IMG)

$(IMG): $(shell find data -type f)
	dd if=/dev/zero of=$(IMG) bs=1M count=64
	mkfs.ext4 -F $(IMG)
	sudo mount -o loop $(IMG) mount
	sudo cp -r data/* mount/
	sudo umount mount

.PHONY: clean
clean:
	rm -f $(IMG)
