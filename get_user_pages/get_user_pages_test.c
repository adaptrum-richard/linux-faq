#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/mmzone.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/pagemap.h>

#define MYDEVICE_MAJOR 42
#define DEVICE_NAME "simpleuser"
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos);
static struct file_operations my_device_ops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .write = device_write
};

static int device_open(struct inode *inode, struct file *file)
{
    printk ("device_open: %d.%d\n", MAJOR (inode->i_rdev), MINOR (inode->i_rdev));
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    pr_info("device_release: %d.%d\n", MAJOR (inode->i_rdev),MINOR (inode->i_rdev));
    return 0;
}

static ssize_t device_write(struct file *filp, const char __user *buf, size_t count, loff_t * ppos)
{
    int res;
    unsigned long uaddr;
    char addrstr[80];
    struct page *page;
    char *my_page_address;
    unsigned long copied = copy_from_user(addrstr, buf, sizeof(addrstr));
    if (copied != 0)
        pr_err("Told to copy %lu, but only copied %lu\n", count, copied);
    uaddr = simple_strtoul(addrstr, NULL, 0);
    down_read(&current->mm->mmap_lock);
    res = get_user_pages(
        uaddr,
        1, /* Only want one page */
        FOLL_WRITE, /* Do want to write into it */
        &page,
        NULL);
    if(res == 1){
        pr_err("Got page\n");
        my_page_address = kmap(page);
        strcpy (my_page_address, "Hello, is it me you're looking for?\n");
        pr_err("Got address %p and user told me it was 0x%lx\n", my_page_address, uaddr);
        pr_err("Wrote: %s", my_page_address);
        kunmap(page);
    }
    if (!PageReserved(page)){
        SetPageDirty(page);
    } else {
        pr_err("Couldn't get page :(\n");
    }
    up_read(&current->mm->mmap_lock);
    return count;
}


static struct class *mmap_class;

static int __init my_init(void)
{
    int ret;
    ret = register_chrdev (MYDEVICE_MAJOR, DEVICE_NAME, &my_device_ops);
    if (ret < 0) {
        pr_alert("my_init: failed with %d\n", ret);
        return ret;
    }
    mmap_class = class_create (THIS_MODULE, DEVICE_NAME);
    device_create (mmap_class, NULL, MKDEV (MYDEVICE_MAJOR, 0), NULL, DEVICE_NAME);

    return 0;
}

static void __exit my_exit(void)
{
    pr_debug("Goodbye\n");
    device_destroy (mmap_class, MKDEV (MYDEVICE_MAJOR, 0));
    class_destroy (mmap_class);
    unregister_chrdev (MYDEVICE_MAJOR, DEVICE_NAME);
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
