BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
slab-test-objs := slab_test.o

obj-m:=slab_test.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;

