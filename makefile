.PHONY: all clean

SOURCE = main.c
TARGET = nawm

build:
	gcc -g $(SOURCE) -lX11 -lXinerama -o $(TARGET)

clean:
	rm -f $(TARGET)
