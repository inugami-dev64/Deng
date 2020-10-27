CC = gcc
LDFLAGS = -lX11 -lXcursor
OBJ = main.o x11_surface.o key_translation.o
TARGET = window

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c

x11_surface.o: x11_surface.c
	$(CC) -c x11_surface.c

key_translation.o: key_translation.c
	$(CC) -c key_translation.c

.PHONY: clean
clean:
	rm -rf *.o

