CC:=gcc
obj:=main.c

all: sandbox sandbox.so

sandbox: main.c
	$(CC) -o sandbox $(obj)

sandbox.so: sandbox.c
	$(CC) -o sandbox.so -shared -fPIC sandbox.c -ldl

test:
	$(CC) -o sandbox $(obj)
	$(CC) -o sandbox.so -shared -fPIC sandbox.c -ldl
	$(CC) sample.c -o sample1
	$(CC) sample.c -o sample2 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

clean:
	rm sandbox sandbox.so sample1 sample2
