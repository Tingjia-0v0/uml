User Mode Linux
==============

- Install dependencies: `./deps.sh`

- Download Linux kernel: `wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.14.tar.xz`

- Extract Linux Kernel: `tar -xvf linux-6.14.tar.xz`

- Build User Mode Linux kernel: `make linux`

- Prepare root filesystem: `make rootfs`

- Run UML: `make run`
