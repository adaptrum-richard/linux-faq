#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/mmzone.h>

static struct kmem_cache *fcache;
static void *buf;

static int __init my_init(void)
{
    fcache = kmem_cache_create("my_object", 20, 8, 0 , NULL);
    buf = kmem_cache_zalloc(fcache, GFP_KERNEL);
    return 0;
}

static void __exit my_exit(void)
{
    kmem_cache_free(fcache, buf);
    kmem_cache_destroy(fcache);
}
/*
断点设置：b kmem_cache_create if (size == 20 && align == 8)
*/
MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
