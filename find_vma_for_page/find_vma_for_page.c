#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
static int pid = 0;
module_param(pid, int, 0);
/*
1. 测试环境 linux-5.15 riscv64 qemu
2. 本测试代码需要重新编译内核，
    需要使用EXPORT_SYMBOL_GPL导出page_lock_anon_vma_read，
    rmap_walk，follow_page三个函数
3. 编译
    riscv64-unknown-linux-gnu-gcc test.c //编译应用程序
    sh compile.sh //遍历编译驱动
4. 先运行./a.out， 在测试程序中，父进程会调用malloc分配66*4KiB的内存，
    对33KiB的内存进行赋值。然后调用fork，在父和子进程中打印出ptr的地址，然后睡眠。

5. 安装驱动程序，insmod find-vma-for-page.ko pid=父进程的pid

6. 观察在堆分配的66KiB,发现堆空间分配的物理内存是在子进程和父进程的vma同时映射的。
*/
static bool show_share_vma(struct page *page, struct vm_area_struct *vma,
      		unsigned long addr, void *arg)
{
    struct mm_struct *mm = vma->vm_mm;
    struct task_struct *task;
    for_each_process(task){
        if(task->mm == mm && task->pid == (pid+1))
            printk(KERN_INFO "\t\tchild, pid:%d, name:%s, vma[0x%lx-0x%lx]\n",
                task->pid, task->comm, vma->vm_start, vma->vm_end);
    }   
    return true;
}
static void find_vma_for_page(struct page *page)
{
    /*page_lock_anon_vma_read，rmap_walk
        需要再内核中导出符号
    */
    struct rmap_walk_control rwc = {
        .arg = NULL,
        .rmap_one = show_share_vma,
        .anon_lock = page_lock_anon_vma_read,
    };
    rmap_walk(page, &rwc);
}

static int __init my_init(void)
{
    struct pid * kpid;
    struct task_struct *task;
    struct mm_struct *mm;
    struct page *page = NULL;
    struct vm_area_struct *vma;
    unsigned long addr;
    char task_name[TASK_COMM_LEN] = {0};
    int counts;
    kpid = find_get_pid(pid);
    task = get_pid_task(kpid, PIDTYPE_PID);
    mm = task->mm;
    printk("task name = %s , task pid = %d\n", get_task_comm(task_name, task), task->pid);
    vma = mm->mmap;
    down_read(&mm->mmap_lock);
    for(; vma; vma = vma->vm_next){
        counts = 0;
        printk("parent vma[0x%lx-0x%lx]\n", vma->vm_start, vma->vm_end);
        for(addr = vma->vm_start; addr < vma->vm_end; addr+=PAGE_SIZE){
            page = follow_page(vma, addr, FOLL_GET);/*follow_page需要再内核中导出符号*/
            if(page){
                printk("\tpage addr: 0x%lx, index: 0x%lx, counts:%d\n", 
                     (unsigned long)page/sizeof(*page), page->index, ++counts);
                find_vma_for_page(page);
		put_page(page);
            }
        }
    }
    up_read(&mm->mmap_lock);
    return 0;
}

static void __exit my_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
