#include <stdint.h>

// Earliest code after reset, running from on-chip ROM on the ARM core before
// DRAM or the OS exist. It brings up the minimum to load the next stage.
#define UART_BASE   0x09000000UL   // PL011 UART on this ARM SoC
#define UART_DR     0x00           // data register offset
#define DDR_CTRL    0x10000000UL   // DRAM controller base

static volatile uint32_t *uart = (volatile uint32_t *)(UART_BASE + UART_DR);

// No libc this early: we emit characters straight to the UART register.
static void early_putc(char c) {
    *uart = (uint32_t)c;           // MMIO write — volatile, uncached
}

static void early_puts(const char *s) {
    while (*s) early_putc(*s++);
}

// Bring up DRAM so the next boot stage has memory to load into.
static int ddr_init(void) {
    volatile uint32_t *ddr = (volatile uint32_t *)DDR_CTRL;
    *ddr = 0x1u;                   // start the DRAM controller
    // Poll the "ready" bit — volatile guarantees we re-read the register.
    for (int i = 0; i < 100000; i++) {
        if (*ddr & (1u << 31)) return 0;   // ready
    }
    return -1;                     // timeout: DRAM failed to come up
}

void early_init(void) {
    early_puts("DPU firmware: early init\n");
    if (ddr_init() != 0) {
        early_puts("FATAL: DRAM init failed\n");
        for (;;) { }               // halt — nothing more is safe
    }
    early_puts("DRAM up; handing off to bootloader\n");
    // ... load + verify the bootloader image, then jump to it ...
}
