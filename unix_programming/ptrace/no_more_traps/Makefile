CC	= gcc
CXX	= g++
CFLAGS	= -Wall -g

PROGS	=  autodbg

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $<

autodbg: autodbg.o ptools.o
	$(CXX) -o $@ $^

%: %.o
	$(CC) -o $@ $^

clean:
	rm -f *.o *~ $(PROGS)
