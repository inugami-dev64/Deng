CC = gcc
OBJ = das_handler.c.o \
	  data_loader.c.o \
	  common.c.o \
	  dam.c.o \

TARGET = dam

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

das_handler.c.o: das_handler.c
	$(CC) -c das_handler.c -o das_handler.c.o

dam.c.o: dam.c
	$(CC) -c dam.c -o dam.c.o

data_loader.c.o: data_loader.c
	$(CC) -c data_loader.c -o data_loader.c.o

common.c.o: ../common/common.c
	$(CC) -c ../common/common.c -o common.c.o

.PHONY: clean
clean:
	rm -rf *.o dam