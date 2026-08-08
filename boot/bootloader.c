#include <stdint.h>
#include <stddef.h>

// The bootloader runs after firmware, with DRAM up. Its job: load the kernel and
// device tree into memory, verify the kernel, and jump to it. Still no OS.

extern int verify_signature(const void *image, size_t len, const char *pubkey);
extern void *load_file(const char *path, size_t *out_len);
extern void  console_puts(const char *s);

// The kernel entry point: ARM64 Linux takes the device-tree blob in x0.
typedef void (*kernel_entry_t)(void *dtb);

void boot(const char *kernel_path, const char *dtb_path, const char *kernel_key) {
    size_t klen = 0, dlen = 0;
    void *kernel = load_file(kernel_path, &klen);
    void *dtb    = load_file(dtb_path, &dlen);

    // Chain of trust: do NOT execute an image we cannot verify.
    if (verify_signature(kernel, klen, kernel_key) != 0) {
        console_puts("SECURE BOOT: kernel signature INVALID — refusing to boot\n");
        for (;;) { }               // halt — never jump to an unverified kernel
    }

    console_puts("kernel verified; jumping\n");
    kernel_entry_t entry = (kernel_entry_t)kernel;
    entry(dtb);                    // hand the device tree to the kernel; never returns
}
