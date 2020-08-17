#include <linux/init.h>
#include <linux/module.h>
#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "gahodrv_ioctl_basic.h"

#define MAX_BUFFER_SIZE 16

typedef struct gahoDrv_t
{
    int test_value;
    uint8_t buffer[MAX_BUFFER_SIZE];
} gahoDrv_t;

gahoDrv_t *gahoDrv = NULL;

static int gahoDrv_open(struct inode *inode, struct file *filp)
{
    printk("<1> Gaho Driver: open\n");
    gahoDrv = kmalloc(sizeof(gahoDrv_t), GFP_KERNEL);
    if (gahoDrv == NULL)
    {
        printk("<1> Gaho Driver: cannot alloc gahoDrv\n");
        return -ENOMEM;
    }
    memset(gahoDrv, 0x00, sizeof(gahoDrv_t));
    return 0;
}

static int gahoDrv_close(struct inode *inode, struct file *filp)
{
    printk("<1> Gaho Driver: close\n");
    if (gahoDrv != NULL)
    {
        kfree(gahoDrv);
        gahoDrv = NULL;
    }
    return 0;
}

static ssize_t gahoDrv_read(struct file *filp, char *buf, size_t size, loff_t *f_pos)
{
    size_t len;
    if (gahoDrv == NULL)
    {
        printk("<1> Gaho Driver: gahoDrv is empty\n");
        return -EFAULT;
    }
    len = (size > MAX_BUFFER_SIZE) ? MAX_BUFFER_SIZE : size;
    printk("<1> Gaho Driver: read (size=%zu)\n", size);
    if (copy_to_user(buf, gahoDrv->buffer, len))
    {
        printk("<1> Gaho Driver: copy the user space fail\n");
        return -EFAULT;
    }
    return len;
}

static ssize_t gahoDrv_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos)
{
    size_t len;
    if (gahoDrv == NULL)
    {
        printk("<1> Gaho Driver: gahoDrv is empty\n");
        return -EFAULT;
    }

    len = (size > MAX_BUFFER_SIZE) ? MAX_BUFFER_SIZE : size;
    printk("<1> Gaho Driver: write (size=%zu)\n", len);
    if (copy_from_user(gahoDrv->buffer, buf, len) != 0)
    {
        printk("<1> Gaho Driver: copy the user space fail\n");
        return -EFAULT;
    }
    return len;
}

static long gahoDrv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret;
    if (gahoDrv == NULL)
    {
        printk("<1> Gaho Driver: gahoDrv is empty\n");
        return -EFAULT;
    }

    if (_IOC_TYPE(cmd) != GAHODRV_IOC_MAGIC || _IOC_NR(cmd) > GAHODRV_IOC_MAXNR)
        return -ENOTTY;

    switch (cmd)
    {
    case GAHODRV_IOC_SET_PARAM1:
        if (copy_from_user((void *)&gahoDrv->test_value, (const void __user *)arg, sizeof(int)))
            ret = -EFAULT;
        printk("<1> Gaho Driver: gahoDrv_IOC_SET_PARAM1 (%d)\n", gahoDrv->test_value);
        break;
    case GAHODRV_IOC_GET_PARAM1:
        printk("<1> Gaho Driver: gahoDrv_IOC_GET_PARAM1 (%d)\n", gahoDrv->test_value);
        if (copy_to_user((void __user *)arg, (void *)&gahoDrv->test_value, sizeof(int)))
            ret = -EFAULT;
        break;
    default:
        ret = -EFAULT;
        break;
    }
    return ret;
}

#ifdef CONFIG_COMPAT
static long gahoDrv_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return gahoDrv_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
}
#endif

static struct file_operations gahoDrv_fops = {
    .owner = THIS_MODULE,
    .open = gahoDrv_open,
    .release = gahoDrv_close,
    .read = gahoDrv_read,
    .write = gahoDrv_write,
    .unlocked_ioctl = gahoDrv_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = gahoDrv_compat_ioctl,
#endif
};

#define gahoDrv_MAJOR 60
#define gahoDrv_NAME "gaho_driver"

static int gaho_driver_init(void)
{
    int ret;
    printk("<1> Gaho Driver: init\n");
    /* Register character device */
    ret = register_chrdev(gahoDrv_MAJOR, gahoDrv_NAME, &gahoDrv_fops);
    if (ret < 0)
    {
        printk("<1> Gaho Driver: Failed to register character device\n");
        return ret;
    }
    return 0;
}

static void gaho_driver_exit(void)
{
    printk("<1> Gaho Driver: exit\n");
    /* Unregister character device */
    unregister_chrdev(gahoDrv_MAJOR, gahoDrv_NAME);
}

module_init(gaho_driver_init);
module_exit(gaho_driver_exit);
