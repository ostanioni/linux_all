say_hello: main.o say_hello.o
	cc -o say.out main.o say_hello.o
main.o: main.c say_hello.h
	cc -c -o main.o main.c
say_hello.o: say_hello.c
	cc -c -o say_hello.o say_hello.c
clean:
	rm *.o *.out