# Top-level build: orchestrate firmware, the kernel module, and the device tree
# into the build/ directory. One `make` produces every artifact the CI ships.
ARCH ?= arm64
CROSS_COMPILE ?= aarch64-linux-gnu-
export ARCH CROSS_COMPILE

all: firmware kernel dtb

firmware:
	$(MAKE) -C firmware

kernel:
	$(MAKE) -C kernel

dtb:
	dtc -I dts -O dtb -o build/dpu.dtb boot/dpu.dts

clean:
	$(MAKE) -C firmware clean
	$(MAKE) -C kernel clean
	rm -f build/*.dtb

.PHONY: all firmware kernel dtb clean
