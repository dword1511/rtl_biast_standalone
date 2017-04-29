PROGRAM = rtl_biast

CFLAGS += -O2 -g -Wall -Werror -W
LDLIBS  = $(shell $(CC) $(CFLAGS) --print-file-name=librtlsdr.a) $(shell pkg-config --libs libusb-1.0)
CC      = gcc
PREFIX ?= /usr/local

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o

.PHONY: clean install uninstall

clean:
	rm -f $(PROGRAM) *.o

install:
	install -m 0755 $(PROGRAM) $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/$(PROGRAM)
