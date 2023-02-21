#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/mmzone.h>

/*需要EXPORT_SYMBOL_GPL next_zone 和 first_online_pgdat*/
static void traversing_zone(void)
{
    struct zone *zone;
    for_each_zone(zone){
        printk("zone name:%s, manager pfn [0x%lx - 0x%lx],"
        "managed_pages: %lu, spanned_pages: %lu, present_pages: %lu\n", 
        zone->name,zone->zone_start_pfn, zone_end_pfn(zone),
        zone_managed_pages(zone), zone->spanned_pages, zone->present_pages);
    }
}

static void show_page_to_zone(void)
{
    unsigned long vaddr = __get_free_page(GFP_KERNEL);
    struct page *page;
    struct zone *zone;
    if(vaddr){
        page = virt_to_page(vaddr);
        zone = page_zone(page);
        printk("pfn: 0x%lx, "
            "zone name:%s, manager pfn [0x%lx - 0x%lx],"
            "managed_pages: %lu, spanned_pages: %lu, present_pages: %lu\n", 
            page_to_pfn(page),
            zone->name,zone->zone_start_pfn, zone_end_pfn(zone),
            zone_managed_pages(zone), zone->spanned_pages, zone->present_pages);
    }
}

static int __init my_init(void)
{
    traversing_zone();
    show_page_to_zone();
    return 0;
}
/*
测试结果：
[   44.404384] zone_and_page: loading out-of-tree module taints kernel.
[   44.415174] zone name:DMA32, manager pfn [0x80200 - 0xc0000],managed_pages: 251503, spanned_pages: 261632, present_pages: 261632
[   44.415516] zone name:Normal, manager pfn [0x0 - 0x0],managed_pages: 0, spanned_pages: 0, present_pages: 0
[   44.415750] zone name:Movable, manager pfn [0x0 - 0x0],managed_pages: 0, spanned_pages: 0, present_pages: 0
[   44.416159] pfn: 0x85019, zone name:DMA32, manager pfn [0x80200 - 0xc0000],managed_pages: 251503, spanned_pages: 261632, present_pages: 261632
*/

static void __exit my_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
