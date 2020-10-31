CC = gcc
LDFLAGS = -lX11 -lXcursor -lvulkan
OBJ = main.o x11_surface.o key_translation.o key_vector.o vulkan_handler.o
TARGET = window

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c

vulkan_handler.o: vulkan_handler.c
	$(CC) -c vulkan_handler.c

key_vector.o: key_vector.c
	$(CC) -c key_vector.c 

x11_surface.o: x11_surface.c
	$(CC) -c x11_surface.c

key_translation.o: key_translation.c
	$(CC) -c key_translation.c

.PHONY: clean
clean:
	rm -rf *.o && rm window

