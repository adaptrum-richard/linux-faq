BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
page-to-pte-objs := page_to_pte.o

obj-m	:=   page-to-pte.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;

