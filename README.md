# Embedded DPU System Software Stack

The software stack for a high-performance embedded networking/compute platform (a DPU). Initialized hardware through a bootloader and firmware handoff, described the board with a device tree, loaded a kernel driver that manages networking and compute resources under lock, exposed an ioctl control plane and a telemetry monitor to user space, enforced a secure-boot configuration workflow, and wrapped it all in a reproducible build/test/deploy pipeline (Kbuild, Makefile, Dockerfile, Jenkinsfile, RUNBOOK). Optimized for throughput, reliability, and low-level hardware interaction.

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
