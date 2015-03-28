CC=gcc
CCFLAGS=-Wall -Werror -Wextra -std=c11
LDFLAGS=-lm -lpthread -lfcgi
APP=fsp

all: main.o
	$(CC) -o ../bin/$(APP) main.o $(LDFLAGS)

main.o: main.c
	$(CC) -c -o main.o $(CCFLAGS) main.c

clean:
	rm -rf *.o

start:
	spawn-fcgi -p 9000 -a 127.0.0.1 -f ../bin/$(APP)
	
