User Mode Linux
==============

Install dependencies

```sh
sudo apt update
sudo apt install -y build-essential flex bison libncurses-dev libssl-dev libelf-dev bear
```

Download and extract Linux Kernel
```sh
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.14.tar.xz
tar -xvf linux-6.14.tar.xz
```

Build User Mode Linux kernel: `make linux`

Prepare root filesystem: `make rootfs`

Run UML with the root filesystem: `make run`
