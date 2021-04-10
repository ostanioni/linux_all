all: lib elf

elf: main.c
	cc -o main.out main.c -ldl
lib: say_hello.c
	cc say_hello.c -shared -fPIC -Wall -o libHello.so 

clean:
	rm main.out libHello.so