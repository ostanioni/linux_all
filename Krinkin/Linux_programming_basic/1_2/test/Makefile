all: lib elf

elf: main.c
	cc main.c -ldl -fPIC -g -O0 -o main.out
lib: libShared.c
	cc libShared.c -shared -fPIC -Wall -g -O0 -o libShared.so 

clean:
	rm *.out *.so