BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
page-test-objs := page-opt.o

obj-m	:=  page-test.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;

