BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
rb-test-objs := rb_test.o

obj-m:=rb-test.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;

