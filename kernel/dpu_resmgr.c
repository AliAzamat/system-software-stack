#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/ioctl.h>
#include <linux/mutex.h>
#include <linux/init.h>

#define DEVICE_NAME "dpu_resmgr"
#define DPU_ENGINE_PHYS 0x40000000UL
#define ENGINE_SIZE     0x1000

#define REG_STATUS     0x00
#define REG_THROUGHPUT 0x08
#define REG_QUEUES     0x0C

#define DPU_IOC_MAGIC   'R'
#define DPU_ALLOC_QUEUE _IOR(DPU_IOC_MAGIC, 1, int)
#define DPU_FREE_QUEUE  _IOW(DPU_IOC_MAGIC, 2, int)

static int major;
static void __iomem *engine;
static DEFINE_MUTEX(rm_lock);
static unsigned long queue_bitmap;
static int n_queues;

// Read the live packets/sec counter straight from an engine register.
static u32 dpu_throughput(void) {
    return ioread32(engine + REG_THROUGHPUT);
}

static int alloc_queue(void) {
    int q;
    mutex_lock(&rm_lock);
    for (q = 0; q < n_queues; q++) {
        if (!(queue_bitmap & (1UL << q))) {
            queue_bitmap |= (1UL << q);
            mutex_unlock(&rm_lock);
            return q;
        }
    }
    mutex_unlock(&rm_lock);
    return -EBUSY;
}

static void free_queue(int q) {
    mutex_lock(&rm_lock);
    queue_bitmap &= ~(1UL << q);
    mutex_unlock(&rm_lock);
}

static long dpu_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    int q;
    switch (cmd) {
    case DPU_ALLOC_QUEUE:
        q = alloc_queue();
        if (q < 0) return q;
        if (copy_to_user((int __user *)arg, &q, sizeof(q)))
            return -EFAULT;
        return 0;
    case DPU_FREE_QUEUE:
        if (copy_from_user(&q, (int __user *)arg, sizeof(q)))
            return -EFAULT;
        if (q < 0 || q >= n_queues) return -EINVAL;
        free_queue(q);
        return 0;
    default:
        return -ENOTTY;
    }
}

// The telemetry plane: read() returns current throughput as text, one line.
static ssize_t dpu_read(struct file *f, char __user *ubuf, size_t len, loff_t *off) {
    char line[32];
    int n;
    if (*off > 0) return 0;                                   // one-shot: EOF after one read
    n = scnprintf(line, sizeof(line), "%u\n", dpu_throughput());
    if (copy_to_user(ubuf, line, n)) return -EFAULT;          // safe copy out
    *off += n;
    return n;
}

static const struct file_operations dpu_fops = {
    .owner          = THIS_MODULE,
    .read           = dpu_read,
    .unlocked_ioctl = dpu_ioctl,
};

static int __init dpu_init(void) {
    engine = ioremap(DPU_ENGINE_PHYS, ENGINE_SIZE);
    if (!engine) {
        return -ENOMEM;
    }
    n_queues = ioread32(engine + REG_QUEUES);
    major = register_chrdev(0, DEVICE_NAME, &dpu_fops);
    if (major < 0) {
        iounmap(engine);
        return major;
    }
    printk(KERN_INFO "dpu_resmgr: %d queues, major=%d\n", n_queues, major);
    return 0;
}

static void __exit dpu_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    iounmap(engine);
    printk(KERN_INFO "dpu_resmgr: unloaded\n");
}

module_init(dpu_init);
module_exit(dpu_exit);
MODULE_LICENSE("GPL");
