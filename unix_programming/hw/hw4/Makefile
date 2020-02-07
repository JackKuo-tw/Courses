CC:=gcc
obj:=webserver

all:http.o webserver.o
	$(CC) -o $(obj) $(obj).o http.o

%.o: %.c
	$(CC) -c $(obj).c http.c

clean:
	rm *.o webserver
