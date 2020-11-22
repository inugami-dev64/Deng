CC = gcc
OBJ = dyn_arr.c.o \
	  loader.c.o \
	  test.c.o \

TARGET = image_loader

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

dyn_arr.c.o: dyn_arr.c
	$(CC) -c dyn_arr.c -o dyn_arr.c.o

loader.c.o: loader.c
	$(CC) -c loader.c -o loader.c.o

test.c.o: test.c
	$(CC) -c test.c -o test.c.o

.PHONY: clean
clean:
	rm -rf *.o image_loader