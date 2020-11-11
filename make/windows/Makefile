CC = gcc
CXX = g++
LD = ar
SRC_DIR = ..\..\src
BUILD_DIR = ..\..\build
TARGET = $(BUILD_DIR)\deng\deng.exe
LIBSURFACE_TARGET = libsurface.a
VULKANSDK_VERSION = 1.2.154.1
VULKANSDK_PATH = C:\VulkanSDK\$(VULKANSDK_VERSION)

INCLUDE_PATH = -I $(VULKANSDK_PATH)\Include

LD_PATH = -L $(VULKANSDK_PATH)\Lib \
		  -L $(BUILD_DIR)\deng\lib

MAIN_SRC = $(SRC_DIR)\main\main.cpp
CC_FLAGS = -Wall -m64 -std=c11
CXX_FLAGS = -Wall -m64 -std=c++11

LD_FLAGS = -s -lcomctl32 -lvulkan-1 -lsurface 

MAIN_OBJ = $(BUILD_DIR)\obj\deng\main.cpp.o

CORE_OBJ = $(BUILD_DIR)\obj\deng\camera.cpp.o \
		   $(BUILD_DIR)\obj\deng\renderer_utils.cpp.o \
		   $(BUILD_DIR)\obj\deng\renderer.cpp.o \
		   $(BUILD_DIR)\obj\deng\window.cpp.o 

DENGUI_OBJ = $(BUILD_DIR)\obj\deng\dengui_sprites.cpp.o \
			 $(BUILD_DIR)\obj\deng\dengui_window.cpp.o \
			 $(BUILD_DIR)\obj\deng\css_data_manager.cpp.o \
			 $(BUILD_DIR)\obj\deng\dengui_pixel_collision.cpp.o 

MATH_OBJ = $(BUILD_DIR)\obj\deng\deng_math.cpp.o \
		   $(BUILD_DIR)\obj\deng\events.cpp.o 

UTILITIES_OBJ = $(BUILD_DIR)\obj\deng\object_loader.cpp.o \
				$(BUILD_DIR)\obj\deng\texture_loader.cpp.o \
				$(BUILD_DIR)\obj\deng\file_manager.cpp.o \
				$(BUILD_DIR)\obj\deng\objects_handler.cpp.o \
				$(BUILD_DIR)\obj\deng\grid_generator.cpp.o \
				$(BUILD_DIR)\obj\deng\timer.cpp.o 

OBJ = $(MAIN_OBJ) $(CORE_OBJ) $(DENGUI_OBJ) $(MATH_OBJ) $(UTILITIES_OBJ)

LIBSURFACE_OBJ = $(BUILD_DIR)\obj\libsurface\vulkan_handler.c.o \
				 $(BUILD_DIR)\obj\libsurface\key_translation.c.o \
				 $(BUILD_DIR)\obj\libsurface\key_vector.c.o \
				 $(BUILD_DIR)\obj\libsurface\win32_surface.c.o 

# Linking the engine
$(TARGET): .init_build $(LIBSURFACE_TARGET) $(OBJ)
	$(CXX) -o $(BUILD_DIR)\deng\$(TARGET) $(OBJ) $(LD_PATH) $(LD_FLAGS)
	Xcopy /E ..\..\objects $(BUILD_DIR)\deng\objects\ /D /Y
	Xcopy /E ..\..\textures $(BUILD_DIR)\deng\textures\ /D /Y
	Xcopy /E ..\..\shaders $(BUILD_DIR)\deng\shaders\ /D /Y
	Xcopy /E ..\..\config $(BUILD_DIR)\deng\config\ /D /Y
	Xcopy /E $(SRC_DIR)\dengui\css $(BUILD_DIR)\deng\ui_styling\ /D /Y

.init_build:
	.\init_build.bat $(BUILD_DIR)

# Surface library build
$(LIBSURFACE_TARGET): $(LIBSURFACE_OBJ)
	$(LD) rs $(LIBSURFACE_TARGET) $(LIBSURFACE_OBJ)
	move $(LIBSURFACE_TARGET) $(BUILD_DIR)\deng\lib 

$(BUILD_DIR)\obj\libsurface\vulkan_handler.c.o: $(SRC_DIR)\surface\vulkan_handler.c
	$(CC) -c $(SRC_DIR)\surface\vulkan_handler.c -o $(BUILD_DIR)\obj\libsurface\vulkan_handler.c.o $(INCLUDE_PATH) $(CC_FLAGS)

$(BUILD_DIR)\obj\libsurface\key_vector.c.o: $(SRC_DIR)\surface\key_vector.c
	$(CC) -c $(SRC_DIR)\surface\key_vector.c -o $(BUILD_DIR)\obj\libsurface\key_vector.c.o $(INCLUDE_PATH) $(CC_FLAGS)

$(BUILD_DIR)\obj\libsurface\key_translation.c.o: $(SRC_DIR)\surface\key_translation.c
	$(CC) -c $(SRC_DIR)\surface\key_translation.c -o $(BUILD_DIR)\obj\libsurface\key_translation.c.o $(INCLUDE_PATH) $(CC_FLAGS)

$(BUILD_DIR)\obj\libsurface\win32_surface.c.o: $(SRC_DIR)\surface\win32_surface.c
	$(CC) -c $(SRC_DIR)\surface\win32_surface.c -o $(BUILD_DIR)\obj\libsurface\win32_surface.c.o $(INCLUDE_PATH) $(CC_FLAGS)


# Engine core objects
$(BUILD_DIR)\obj\deng\camera.cpp.o: $(SRC_DIR)\core\camera.cpp
	$(CXX) -c $(SRC_DIR)\core\camera.cpp -o $(BUILD_DIR)\obj\deng\camera.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS) 

$(BUILD_DIR)\obj\deng\renderer_utils.cpp.o: $(SRC_DIR)\core\renderer_utils.cpp
	$(CXX) -c $(SRC_DIR)\core\renderer_utils.cpp -o $(BUILD_DIR)\obj\deng\renderer_utils.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

$(BUILD_DIR)\obj\deng\renderer.cpp.o: $(SRC_DIR)\core\renderer.cpp
	$(CXX) -c $(SRC_DIR)\core\renderer.cpp -o $(BUILD_DIR)\obj\deng\renderer.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\window.cpp.o: $(SRC_DIR)\core\window.cpp
	$(CXX) -c $(SRC_DIR)\core\window.cpp -o $(BUILD_DIR)\obj\deng\window.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)


# DENGUI objects
$(BUILD_DIR)\obj\deng\dengui_sprites.cpp.o: $(SRC_DIR)\dengui\core\dengui_sprites.cpp
	$(CXX) -c $(SRC_DIR)\dengui\core\dengui_sprites.cpp -o $(BUILD_DIR)\obj\deng\dengui_sprites.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

$(BUILD_DIR)\obj\deng\dengui_window.cpp.o: $(SRC_DIR)\dengui\core\dengui_window.cpp
	$(CXX) -c $(SRC_DIR)\dengui\core\dengui_window.cpp -o $(BUILD_DIR)\obj\deng\dengui_window.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\css_data_manager.cpp.o: $(SRC_DIR)\dengui\css_engine\css_data_manager.cpp
	$(CXX) -c $(SRC_DIR)\dengui\css_engine\css_data_manager.cpp -o $(BUILD_DIR)\obj\deng\css_data_manager.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
# $(BUILD_DIR)\obj\deng\dengui_event_handles.cpp.o: $(SRC_DIR)\dengui\events\dengui_event_handles.cpp
# 	$(CXX) -c $(SRC_DIR)\dengui\events\dengui_event_handles.cpp -o $(BUILD_DIR)\obj\deng\ui_event_handles.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
# $(BUILD_DIR)\obj\deng\dengui_events.cpp.o: $(SRC_DIR)\dengui\events\dengui_events.cpp
# 	$(CXX) -c $(SRC_DIR)\dengui\events\dengui_events.cpp -o $(BUILD_DIR)\obj\deng\ui_events.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\dengui_pixel_collision.cpp.o: $(SRC_DIR)\dengui\pixel_perfect_collision\dengui_pixel_collision.cpp
	$(CXX) -c $(SRC_DIR)\dengui\pixel_perfect_collision\dengui_pixel_collision.cpp -o $(BUILD_DIR)\obj\deng\dengui_pixel_collision.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)


# Math objects
$(BUILD_DIR)\obj\deng\deng_math.cpp.o: $(SRC_DIR)\maths\deng_math.cpp	
	$(CXX) -c $(SRC_DIR)\maths\deng_math.cpp -o $(BUILD_DIR)\obj\deng\deng_math.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\events.cpp.o: $(SRC_DIR)\maths\events.cpp
	$(CXX) -c $(SRC_DIR)\maths\events.cpp -o $(BUILD_DIR)\obj\deng\events.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)


# Utilities objects
$(BUILD_DIR)\obj\deng\object_loader.cpp.o: $(SRC_DIR)\utilities\assets_handler\object_loader.cpp
	$(CXX) -c $(SRC_DIR)\utilities\assets_handler\object_loader.cpp -o $(BUILD_DIR)\obj\deng\object_loader.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\texture_loader.cpp.o: $(SRC_DIR)\utilities\assets_handler\texture_loader.cpp
	$(CXX) -c $(SRC_DIR)\utilities\assets_handler\texture_loader.cpp -o $(BUILD_DIR)\obj\deng\texture_loader.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

$(BUILD_DIR)\obj\deng\file_manager.cpp.o: $(SRC_DIR)\utilities\data_handler\file_manager.cpp
	$(CXX) -c $(SRC_DIR)\utilities\data_handler\file_manager.cpp -o $(BUILD_DIR)\obj\deng\file_manager.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\objects_handler.cpp.o: $(SRC_DIR)\utilities\data_handler\objects_handler.cpp
	$(CXX) -c $(SRC_DIR)\utilities\data_handler\objects_handler.cpp -o $(BUILD_DIR)\obj\deng\objects_handler.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\grid_generator.cpp.o: $(SRC_DIR)\utilities\map\grid_generator.cpp
	$(CXX) -c $(SRC_DIR)\utilities\map\grid_generator.cpp -o $(BUILD_DIR)\obj\deng\grid_generator.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)\obj\deng\timer.cpp.o: $(SRC_DIR)\utilities\timer\timer.cpp
	$(CXX) -c $(SRC_DIR)\utilities\timer\timer.cpp -o $(BUILD_DIR)\obj\deng\timer.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)


# Main object file
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) -c $(MAIN_SRC) -o $(MAIN_OBJ) $(INCLUDE_PATH) $(CXX_FLAGS)

.PHONY: clean clean_exec
clean:
	rd /s $(BUILD_DIR)\obj

clean_exec:
	rd /s $(BUILD_DIR)\deng 