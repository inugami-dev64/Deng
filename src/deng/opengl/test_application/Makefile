CXX = g++
CC = gcc
CXX_FLAGS = -O3 -g
OBJ = ./test.cpp.o \
	  ./window.cpp.o \
	  ./glad.c.o \
	  ./x11_surface.c.o \
	  ./key_ev.c.o \
	  ./key_translation.c.o \
	  ./vk_ext.c.o

TARGET = test_app
INCL_PATH = -I ../../../../headers -I ../../../../modules/glad/include
LD = -lglfw -lGL -ldl -lGLU -lX11 -lXcursor -lvulkan



$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LD)

./test.cpp.o: test.cpp
	$(CXX) -c test.cpp -o test.cpp.o $(INCL_PATH) $(CXX_FLAGS)

./window.cpp.o: ../../window.cpp
	$(CXX) -c ../../window.cpp -o ./window.cpp.o $(INCL_PATH) $(CXX_FLAGS)

./glad.c.o: ../../../../modules/glad/src/glad.c
	$(CC) -c ../../../../modules/glad/src/glad.c -o ./glad.c.o $(INCL_PATH) $(CXX_FLAGS)

./x11_surface.c.o: ../../surface/x11_surface.c
	$(CC) -c ../../surface/x11_surface.c -o x11_surface.c.o $(INCL_PATH) $(CXX_FLAGS)

./key_ev.c.o: ../../surface/key_ev.c
	$(CC) -c ../../surface/key_ev.c -o key_ev.c.o $(INCL_PATH) $(CXX_FLAGS)

./key_translation.c.o: ../../surface/key_translation.c
	$(CC) -c ../../surface/key_translation.c -o key_translation.c.o $(INCL_PATH) $(CXX_FLAGS)

./vk_ext.c.o: ../../vulkan/surface.c
	$(CC) -c ../../vulkan/surface.c -o vk_ext.c.o $(INCL_PATH) $(CXX_FLAGS)


.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
