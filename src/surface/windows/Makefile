CC = gcc
VULKANVERSION = 1.2.154.1

LDFLAGS = -s -lcomctl32 -lvulkan-1 -Wl,--subsystem,windows
CCFLAGS = -D UNICODE -D _UNICODE -Wall
INCLUDEFLAGS = -I C:\VulkanSDK\$(VULKANVERSION)\Include 
LDPATH = -L C:\VulkanSDK\$(VULKANVERSION)\Lib
OBJ = win32_surface.o main.o key_vector.o key_translation.o
TARGET = window.exe

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDPATH) $(LDFLAGS)

win32_surface.o: win32_surface.c
	$(CC) $(CCFLAGS) -c win32_surface.c $(INCLUDEFLAGS)

main.o: main.c
	$(CC) $(CCFLAGS) -c main.c $(INCLUDEFLAGS)

key_vector.o: key_vector.c
	$(CC) $(CCFLAGS) -c key_vector.c $(INCLUDEFLAGS)

key_translation.o: key_translation.c
	$(CC) $(CCFLAGS) -c key_translation.c $(INCLUDEFLAGS)

.PHONY: clean

clean:
	del $(OBJ) $(TARGET)
