BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
zone-and-page-objs := zone_and_page.o

obj-m:=zone-and-page.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;

