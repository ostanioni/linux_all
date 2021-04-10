# gcc -o ... -Wl,-rpath .
# readelf -d solution
flags=-Wall -g3 -O0
all: lib elf

elf: solution.c
	cc $^ -ldl $(flags) -o solution
lib: libShared.c
	cc $^ -shared -fPIC $(flags)  -o libShared.so 

clean:
	rm *.out *.so solution