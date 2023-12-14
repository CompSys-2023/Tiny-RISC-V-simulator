# GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 -O

all: sim
rebuild: clean all

# sim uses simulate
sim: *.c 
	$(GCC) *.c -o sim 

%.riscv: testing_files/%.c testing_files/lib.c Makefile
	/opt/riscv-32v/bin/riscv32-unknown-elf-gcc -march=rv32im -mabi=ilp32 -fno-tree-loop-distribute-patterns -mno-relax -O0 $< testing_files/lib.c -static -nostartfiles -nostdlib -o $@

%.riscv: testing_files/%.s Makefile
	/opt/riscv-32v/bin/riscv32-unknown-elf-gcc -march=rv32im -mabi=ilp32 -fno-tree-loop-distribute-patterns -mno-relax -O0 $< -static -nostartfiles -nostdlib -o $@

%.dis: %.riscv Makefile
	/opt/riscv-32v/bin/riscv32-unknown-elf-objdump -s -w $< > $@
	/opt/riscv-32v/bin/riscv32-unknown-elf-objdump -S $< >> $@

zip: ../src.zip

../src.zip: clean
	cd .. && zip -r src.zip src/Makefile src/*.c src/*.h

clean:
	rm -rf *.o sim  vgcore*
