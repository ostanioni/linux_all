all: lib elf

elf: main.c
	cc -o main.out main.c -ldl
lib: libShared.c
	cc libShared.c -shared -fPIC -Wall -o libShared.so 

clean:
	rm *.out *.so