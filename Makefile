CC=`which gcc`
TPL_GEN=`which python3` lib/gctpl/gctpl.py
RM=`which rm`
SPWNCGI=`which spawn-fcgi`
KILL=`which kill`
SUDO=`which sudo`

CFLAGS= --std=c11 -D_GNU_SOURCE -I/usr/include/mysql -I./inc -O3
LDFLAGS=-lm -lpthread -lfcgi -lmysqlclient

APP=fastpoll
CWD=$(shell pwd)
OUT=$(CWD)/bin/$(APP)
PID=$(CWD)/run/$(APP).pid
SOCK=$(CWD)/run/$(APP).sock

SRC=$(shell find src -name '*.[c]')
OBJ=$(SRC:%.c=%.o)

TPL_IN=$(shell find src -name '*.td')
TPL_OUT=src/template_def

.PHONY: clean tpl start stop

all: tpl app

app: $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LDFLAGS)

tpl:
	bin/tplgen.php
	
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
	#$(CC) $(CFLAGS) -S $^ -o $@.S -masm=intel

clean:
	$(RM) -f $(OBJ)

# ---------
# debugging
 
refresh: stop clean all start 

start:
	$(SPWNCGI) -s $(SOCK) -P $(PID) -- $(OUT) $(SOCK)
 
stop:
	$(KILL) -9 $(shell cat $(PID))
	$(RM) $(PID)
