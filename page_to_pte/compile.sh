#!/bin/sh
export ARCH=riscv
export CROSS_COMPILE=riscv64-linux-gnu-

if [ "$1" = "clean" ]; then
	make clean
	rm test
else
	make
	riscv64-unknown-linux-gnu-gcc test.c -o test
fi
