# Build the firmware stub and compile the device tree source (.dts) into the
# binary blob (.dtb) the bootloader hands to the kernel.
CROSS_COMPILE ?= aarch64-linux-gnu-
CC  := $(CROSS_COMPILE)gcc
DTC := dtc

all: build/early_init.o build/dpu.dtb

build/early_init.o: early_init.c
	$(CC) -ffreestanding -nostdlib -c $< -o $@

# dtc compiles the human-readable .dts into the .dtb the bootloader loads.
build/dpu.dtb: ../boot/dpu.dts
	$(DTC) -I dts -O dtb -o $@ $<

clean:
	rm -f build/*.o build/*.dtb
