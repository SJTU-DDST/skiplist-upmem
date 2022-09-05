# SKIPLIST-UPMEM

在upmem中实现skiplist数据结构。

<img src="https://user-images.githubusercontent.com/83715643/188350201-0fce7b2d-df4a-4d9d-8ac9-a3b00d11e02d.png" alt="image-20220905103607298" style="zoom: 67%;" />

# Environment

- The UPMEM DPU toolchain

  参考upmem官网：https://www.upmem.com/

## Project structure

* `dpu` directory contains the DPU code (i.e., the code running on the memory)
* `host` directory contains the Host code (i.e., running on the CPU)

## How to build

In order to build the program and tools, just type:

```shell
make
```

## How to run

```shell
./build/host_app
```

