User Mode Linux
==============

- Install dependencies: `./deps.sh`

- Download Linux kernel: `wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.14.tar.xz`

- Extract Linux Kernel: `tar -xvf linux-6.14.tar.xz`

- To run controller module, download the customized kernel instead `git clone https://github.com/Tingjia-0v0/linux-6.14.git` 

- Build User Mode Linux kernel: `make linux`

- Prepare root filesystem: `make rootfs`

- Run UML: `make run`
