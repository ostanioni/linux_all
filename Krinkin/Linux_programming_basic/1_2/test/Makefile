# readelf -d solution
flags=-Wall -g3 -O0 -Wl,-rpath,.
all: lib elf

elf: solution.c
	cc solution.c -ldl $(flags) -o solution
lib: libShared.c
	cc libShared.c -shared -fPIC $(flags)  -o libShared.so 

clean:
	rm libShared.so solution