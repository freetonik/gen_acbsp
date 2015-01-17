CC=g++
CFLAGS= -g

mainmake: main.c allinc.h
	 g++ $(CFLAGS) main.c

clean:
	rm -f main a.out
