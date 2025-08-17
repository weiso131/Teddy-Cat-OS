# Teddy-Cat-OS

## Introduction
This project is a **learning-oriented operating system**, targeting a **32-bit RISC-V architecture** and runnable on a `virt` machine.  

It is based on the tutorial [**“OS in 1000 Lines”**](https://operating-system-in-1000-lines.vercel.app/en/), serving as the starting point for learning.  
Many parts of the code are adapted or directly carried over from that project.

## Related Links
- [Development Log (Traditional Chinese)](https://hackmd.io/@weiso131/teddy-cat-os)

## Environment Setup
```bash
sudo apt update && sudo apt install -y clang lld qemu-system-riscv32 curl

git clone https://github.com/weiso131/Teddy-Cat-OS
cd Teddy-Cat-OS
curl -LO https://github.com/qemu/qemu/raw/v8.0.4/pc-bios/opensbi-riscv32-generic-fw_dynamic.bin
```
