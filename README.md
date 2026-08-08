# Embedded DPU System Software Stack

The capstone of the journey. Build the software stack for a high-performance embedded networking/compute platform (a DPU). Initialize hardware through a bootloader and firmware handoff, describe the board with a device tree, load a kernel driver that manages networking and compute resources under lock, expose an ioctl control plane and a telemetry monitor to user space, enforce a secure-boot configuration workflow, and wrap it all in a reproducible build/test/deploy pipeline (Kbuild, Makefile, Dockerfile, Jenkinsfile, RUNBOOK). Optimized for throughput, reliability, and low-level hardware interaction.

Built step-by-step with [KhwajaLabs Build](https://khwajalabs.com).

## Stack
- C
- Embedded Linux
- ARM
- Bootloader
- UEFI
- Device Tree
- Kernel Driver
- Python
- Docker
- Jenkins
- Secure Boot
