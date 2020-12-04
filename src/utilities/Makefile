CXX = g++
OBJ = rend_utils.cpp.o \
	  utils_test.cpp.o 

INCLUDE_PATH = -I /usr/include/freetype2 \
			   -I /usr/include/freetype2/freetype

LD_PATH = -L /usr/lib/x86_64-linux-gnu/ \
		  -L ~/GIT/deng/build/deng/lib

LD_FLAGS = -lfreetype -lcommon

CXX_FLAGS = -Wall -O3

TARGET = font_test

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LD_PATH) -o $(TARGET) $(LD_FLAGS) $(CXX_FLAGS)

rend_utils.cpp.o: rend_utils.cpp
	$(CXX) -c rend_utils.cpp -o rend_utils.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

utils_test.cpp.o: utils_test.cpp
	$(CXX) -c utils_test.cpp -o utils_test.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET)
