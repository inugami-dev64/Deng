CC = gcc
LDFLAGS = -lX11 -lXcursor -lvulkan
OBJ = ../test.o ../x11_surface.o ../key_translation.o ../key_vector.o ../vulkan_handler.o
TARGET = ../window

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

../test.o: ../test.c
	$(CC) -c ../test.c -o ../test.o

../vulkan_handler.o: ../vulkan_handler.c
	$(CC) -c ../vulkan_handler.c -o ../vulkan_handler.o

../key_vector.o: ../key_vector.c
	$(CC) -c ../key_vector.c -o ../key_vector.o

../x11_surface.o: ../x11_surface.c
	$(CC) -c ../x11_surface.c -o ../x11_surface.o

../key_translation.o: ../key_translation.c
	$(CC) -c ../key_translation.c -o ../key_translation.o

.PHONY: clean
clean:
	rm -rf ../*.o && rm ../window

