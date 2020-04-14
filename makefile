CC = gcc
ODIR = build
MAIN = main.c
TARGET = nawm

.PHONY: all clean

log.o:
	$(CC) -std=c99 -c log.c/src/log.c -o $(ODIR)/log.o

build: target log.o
	$(CC) -std=c18 -g $(MAIN) -Ilog.c/src -lxdo -lX11 -lXinerama $(ODIR)/log.o -o $(ODIR)/$(TARGET)

target:
	mkdir -p build

clean:
	rm -rf $(ODIR)
