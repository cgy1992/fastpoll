CC=`which gcc`
TPL_GEN=`which python3` lib/gctpl/gctpl.py
RM=`which rm`
SPWNCGI=`which spawn-fcgi`
KILL=`which kill`
SUDO=`which sudo`

CFLAGS=-Wall -Werror -Wextra -Winline --std=c11 -D_GNU_SOURCE -I/usr/include/mysql -I./inc
LDFLAGS=-lm -lpthread -lfcgi -lmysqlclient

APP=fastpoll
CWD=$(shell readlink -f .)
OUT=$(CWD)/bin/$(APP)
PID=$(CWD)/run/$(APP).pid
SOCK=$(CWD)/run/$(APP).sock

SRC=$(shell find src -name '*.[c]')
OBJ=$(SRC:%.c=%.o)

TPL_IN=$(shell find src -name '*.td')
TPL_OUT=src/template_def

.PHONY: clean tpl start stop

#all: tpl app
all: app

app: $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LDFLAGS)

tpl:
	$(TPL_GEN) -l fsp.h fcgi_stdio.h -c -p fsp_ -u constant -i $(TPL_IN) -o $(TPL_OUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) -f $(OBJ)

# ---------
# debugging
 
start:
	$(SPWNCGI) -s $(SOCK) -f $(OUT) -P $(PID)
 
stop:
	$(KILL) -9 $(shell cat $(PID))
	$(RM) $(PID)
