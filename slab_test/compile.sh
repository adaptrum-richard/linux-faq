#!/bin/sh
export ARCH=riscv
export CROSS_COMPILE=riscv64-linux-gnu-

if [ "$1" = "clean" ]; then
	make clean
else
	make
fi
