BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
find-vma-for-page-objs := find_vma_for_page.o

obj-m	:=   find-vma-for-page.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;

