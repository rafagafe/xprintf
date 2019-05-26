
CC = gcc
CFLAGS = -g -O3 -std=gnu99 -Wall

src  = $(wildcard *.c)
src += $(wildcard ../*.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

.PHONY: build all clean

build: xprintf-test

all: clean build

clean::
	rm -rf *.d $(dep)
	rm -rf *.o $(obj)
	rm -rf xprintf-test

test: xprintf-test
	./xprintf-test

xprintf-test: $(obj)
	gcc $(CFLAGS) -o $@ $^

-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@
