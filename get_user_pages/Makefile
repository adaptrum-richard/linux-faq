BASEINCLUDE ?= /home/richard/work/arch/risc-v/runninglinuxkernel_5.15
get-user-pages-test-objs := get_user_pages_test.o

obj-m:=get-user-pages-test.o
all :
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;
	riscv64-unknown-linux-gnu-gcc -o userspace userspace.c
clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko userspace

