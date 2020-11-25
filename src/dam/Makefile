CC = gcc
OBJ = loader.c.o \
	  assembler.c.o \
	  main.c.o \

TARGET = dam

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

loader.c.o: loader.c
	$(CC) -c loader.c -o loader.c.o

main.c.o: main.c
	$(CC) -c main.c -o main.c.o

assembler.c.o: assembler.c
	$(CC) -c assembler.c -o assembler.c.o

.PHONY: clean
clean:
	rm -rf *.o dam