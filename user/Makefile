OUT ?= $(abspath ../rootfs/data/bin)
EXES = $(patsubst %.c,$(OUT)/%,$(wildcard *.c))

all: $(EXES)

$(OUT)/%: %.c
	gcc -Wall -Wextra -std=c99 -static -o $@ $<

.PHONY: clean
clean:
	rm -rf $(EXES)
