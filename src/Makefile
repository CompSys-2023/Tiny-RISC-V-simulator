# GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 -O

all: sim
rebuild: clean all

# sim uses simulate
sim: *.c 
	$(GCC) *.c -o sim 

zip: ../src.zip

../src.zip: clean
	cd .. && zip -r src.zip src/Makefile src/*.c src/*.h

%.riscv: %.c lib.c Makefile
	/usr/local/opt/riscv-gnu-toolchain/bin/riscv64-unknown-elf-gcc  -march=rv32im -mabi=ilp32 -fno-tree-loop-distribute-patterns -mno-relax -O1 $< lib.c -static -nostartfiles -nostdlib -o $@

%.dis: %.riscv Makefile
	/usr/local/opt/riscv-gnu-toolchain/bin/riscv64-unknown-elf-objdump -s -w $< > $@
	/usr/local/opt/riscv-gnu-toolchain/bin/riscv64-unknown-elf-objdump -S $< >> $@

clean:
	rm -rf *.o sim  vgcore*
