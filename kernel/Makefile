# Drive Kbuild for our module. KDIR points at the target kernel's build tree
# (its headers + Kbuild). ARCH/CROSS_COMPILE select the ARM target so this
# builds on an x86 host for the aarch64 DPU.
KDIR ?= /lib/modules/$(shell uname -r)/build
ARCH ?= arm64
CROSS_COMPILE ?= aarch64-linux-gnu-

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

# A placeholder check target the CI 'Test' stage invokes.
check:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) C=1 modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
