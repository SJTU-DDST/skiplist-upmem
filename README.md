# SKIPLIST-UPMEM

在upmem中实现skiplist数据结构。

<img src="https://user-images.githubusercontent.com/83715643/188350201-0fce7b2d-df4a-4d9d-8ac9-a3b00d11e02d.png" alt="image-20220905103607298"  />

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

# Improvement

- mram allocator 设计过于简单，仅仅可以分配内存，不可以释放内存，需要设计动态内存分配器。
- dpu仅仅使用了单线程，每个DPU可用24线程，其实需要12线程以上才能拉满性能
