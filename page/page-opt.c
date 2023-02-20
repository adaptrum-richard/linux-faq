#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/mm.h>
#include <linux/pagemap.h>


static int __init my_init(void)
{
	struct page *page;
	unsigned long vaddr = __get_free_pages(GFP_ATOMIC, 0);
	printk("vaddr: 0x%lx\n", vaddr);
	page = virt_to_page((void*)vaddr);
	printk("page: 0x%lx to pfn = 0x%lx, pfn to phys: 0x%lx, pfn to page: 0x%lx\n", 
		(unsigned long)page, page_to_pfn(page), 
		(unsigned long)__pfn_to_phys(page_to_pfn(page)),
		(unsigned long)pfn_to_page(page_to_pfn(page)));
	

	free_pages(vaddr, 0);	
	return 0;
}

static void __exit my_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
