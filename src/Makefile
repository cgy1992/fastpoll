CC=gcc
CCFLAGS=-Wall -Werror -Wextra --std=c11 -D_GNU_SOURCE
LDFLAGS=-lm -lpthread -lfcgi
APP=fastpoll
PID=/var/run/$(APP).pid

all: main.o
	$(CC) -o ../bin/$(APP) main.o $(LDFLAGS)

main.o: main.c
	$(CC) -c -o main.o $(CCFLAGS) main.c

clean:
	rm -rf *.o

# ---------
# debugging

start:
	spawn-fcgi -p 9000 -a 127.0.0.1 -f ../bin/$(APP) -P $(PID)

stop:
	kill -9 $$(cat $(PID))
	
