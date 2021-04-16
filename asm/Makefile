hello.out: hello.o
	ld -m elf_i386 -s -o hello.out hello.o

hello.o: hello.asm
	nasm -f elf hello.asm

clean:
	-rm *.o *.out
