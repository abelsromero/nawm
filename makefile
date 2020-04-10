.PHONY: all clean

SOURCE = main.c
TARGET = main

build:
	gcc -g $(SOURCE) -lX11 -o $(TARGET)
#   gcc -g $(SOURCE) -lxdo -lX11 -o $(TARGET)

clean:
	rm -f $(TARGET)
