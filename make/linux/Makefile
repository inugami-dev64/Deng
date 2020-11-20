CC = gcc
CXX = g++
LD = ar
SRC_DIR = ../../src
BUILD_DIR = ../../build
TARGET = $(BUILD_DIR)/deng/deng
LIBSURFACE_TARGET = libsurface.a

LD_PATH = -L $(BUILD_DIR)/deng/lib

CC_FLAGS = -Wall -m64
CXX_FLAGS = -Wall -m64 -std=c++11
LD_FLAGS = -lvulkan -lpthread -lsurface -lX11 -lXcursor -ldl 

MAIN_SRC = $(SRC_DIR)/main/main.cpp
MAIN_OBJ = $(BUILD_DIR)/obj/deng/main.cpp.o

CORE_OBJ = $(BUILD_DIR)/obj/deng/camera.cpp.o \
		   $(BUILD_DIR)/obj/deng/renderer_utils.cpp.o \
		   $(BUILD_DIR)/obj/deng/renderer.cpp.o \
		   $(BUILD_DIR)/obj/deng/window.cpp.o 

# DENGUI_OBJ = $(BUILD_DIR)/obj/deng/dengui_sprites.cpp.o \
# 			 $(BUILD_DIR)/obj/deng/dengui_window.cpp.o \
# 			 $(BUILD_DIR)/obj/deng/css_data_manager.cpp.o \
# 			 $(BUILD_DIR)/obj/deng/dengui_pixel_collision.cpp.o \
# 			 $(BUILD_DIR)/obj/deng/dengui_font_loader.cpp.o 

MATH_OBJ = $(BUILD_DIR)/obj/deng/deng_math.cpp.o \
		   $(BUILD_DIR)/obj/deng/events.cpp.o 

UTILITIES_OBJ = $(BUILD_DIR)/obj/deng/object_loader.cpp.o \
				$(BUILD_DIR)/obj/deng/texture_loader.cpp.o \
				$(BUILD_DIR)/obj/deng/file_manager.cpp.o \
				$(BUILD_DIR)/obj/deng/timer.cpp.o
				# $(BUILD_DIR)/obj/deng/grid_generator.cpp.o 

OBJ = $(MAIN_OBJ) $(CORE_OBJ) $(MATH_OBJ) $(UTILITIES_OBJ)

LIBSURFACE_OBJ = $(BUILD_DIR)/obj/libsurface/vulkan_handler.c.o \
				 $(BUILD_DIR)/obj/libsurface/key_translation.c.o \
				 $(BUILD_DIR)/obj/libsurface/key_vector.c.o \
				 $(BUILD_DIR)/obj/libsurface/x11_surface.c.o

$(TARGET): .init_build $(LIBSURFACE_TARGET) $(OBJ)
	$(CXX) -o $(BUILD_DIR)/deng/$(TARGET) $(OBJ) $(LD_PATH) $(LD_FLAGS)
	cp -r ../../objects $(BUILD_DIR)/deng/
	cp -r ../../textures $(BUILD_DIR)/deng/
	cp -r ../../shaders $(BUILD_DIR)/deng/
	cp -r ../../config $(BUILD_DIR)/deng/
	cp -r $(SRC_DIR)/dengui/css $(BUILD_DIR)/deng/ui_styling
	cp -r $(SRC_DIR)/surface/xcursor $(BUILD_DIR)/deng/
	cp -r ../../fonts $(BUILD_DIR)/deng

.init_build:
	chmod +x init_build.sh
	./init_build.sh $(BUILD_DIR)

# Surface library build
$(LIBSURFACE_TARGET): $(LIBSURFACE_OBJ)
	$(LD) rs $(LIBSURFACE_TARGET) $(LIBSURFACE_OBJ)
	mv $(LIBSURFACE_TARGET) $(BUILD_DIR)/deng/lib/

$(BUILD_DIR)/obj/libsurface/vulkan_handler.c.o: $(SRC_DIR)/surface/vulkan_handler.c
	$(CC) -c $(SRC_DIR)/surface/vulkan_handler.c -o $(BUILD_DIR)/obj/libsurface/vulkan_handler.c.o $(CC_FLAGS)

$(BUILD_DIR)/obj/libsurface/key_vector.c.o: $(SRC_DIR)/surface/key_vector.c
	$(CC) -c $(SRC_DIR)/surface/key_vector.c -o $(BUILD_DIR)/obj/libsurface/key_vector.c.o $(CC_FLAGS)

$(BUILD_DIR)/obj/libsurface/key_translation.c.o: $(SRC_DIR)/surface/key_translation.c
	$(CC) -c $(SRC_DIR)/surface/key_translation.c -o $(BUILD_DIR)/obj/libsurface/key_translation.c.o $(CC_FLAGS)

$(BUILD_DIR)/obj/libsurface/x11_surface.c.o: $(SRC_DIR)/surface/x11_surface.c
	$(CC) -c $(SRC_DIR)/surface/x11_surface.c -o $(BUILD_DIR)/obj/libsurface/x11_surface.c.o $(CC_FLAGS)


# Engine core objects
$(BUILD_DIR)/obj/deng/camera.cpp.o: $(SRC_DIR)/core/camera.cpp
	$(CXX) -c $(SRC_DIR)/core/camera.cpp -o $(BUILD_DIR)/obj/deng/camera.cpp.o $(CXX_FLAGS) 

$(BUILD_DIR)/obj/deng/renderer_utils.cpp.o: $(SRC_DIR)/core/renderer_utils.cpp
	$(CXX) -c $(SRC_DIR)/core/renderer_utils.cpp -o $(BUILD_DIR)/obj/deng/renderer_utils.cpp.o $(CXX_FLAGS)

$(BUILD_DIR)/obj/deng/renderer.cpp.o: $(SRC_DIR)/core/renderer.cpp
	$(CXX) -c $(SRC_DIR)/core/renderer.cpp -o $(BUILD_DIR)/obj/deng/renderer.cpp.o $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/window.cpp.o: $(SRC_DIR)/core/window.cpp
	$(CXX) -c $(SRC_DIR)/core/window.cpp -o $(BUILD_DIR)/obj/deng/window.cpp.o $(CXX_FLAGS)


# DENGUI objects
$(BUILD_DIR)/obj/deng/dengui_sprites.cpp.o: $(SRC_DIR)/dengui/core/dengui_sprites.cpp
	$(CXX) -c $(SRC_DIR)/dengui/core/dengui_sprites.cpp -o $(BUILD_DIR)/obj/deng/dengui_sprites.cpp.o $(CXX_FLAGS)

$(BUILD_DIR)/obj/deng/dengui_window.cpp.o: $(SRC_DIR)/dengui/core/dengui_window.cpp
	$(CXX) -c $(SRC_DIR)/dengui/core/dengui_window.cpp -o $(BUILD_DIR)/obj/deng/dengui_window.cpp.o $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/css_data_manager.cpp.o: $(SRC_DIR)/dengui/css_engine/css_data_manager.cpp
	$(CXX) -c $(SRC_DIR)/dengui/css_engine/css_data_manager.cpp -o $(BUILD_DIR)/obj/deng/css_data_manager.cpp.o $(CXX_FLAGS)
	
# $(BUILD_DIR)/obj/deng/dengui_event_handles.cpp.o: $(SRC_DIR)/dengui/events/dengui_event_handles.cpp
# 	$(CXX) -c $(SRC_DIR)/dengui/events/dengui_event_handles.cpp -o $(BUILD_DIR)/obj/deng/ui_event_handles.cpp.o  $(CXX_FLAGS)
	
# $(BUILD_DIR)/obj/deng/dengui_events.cpp.o: $(SRC_DIR)/dengui/events/dengui_events.cpp
# 	$(CXX) -c $(SRC_DIR)/dengui/events/dengui_events.cpp -o $(BUILD_DIR)/obj/deng/ui_events.cpp.o  $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/dengui_pixel_collision.cpp.o: $(SRC_DIR)/dengui/pixel_perfect_collision/dengui_pixel_collision.cpp
	$(CXX) -c $(SRC_DIR)/dengui/pixel_perfect_collision/dengui_pixel_collision.cpp -o $(BUILD_DIR)/obj/deng/dengui_pixel_collision.cpp.o $(CXX_FLAGS)

$(BUILD_DIR)/obj/deng/dengui_font_loader.cpp.o: $(SRC_DIR)/dengui/core/dengui_font_loader.cpp
	$(CXX) -c $(SRC_DIR)/dengui/core/dengui_font_loader.cpp -o $(BUILD_DIR)/obj/deng/dengui_font_loader.cpp.o $(CXX_FLAGS)


# Math objects
$(BUILD_DIR)/obj/deng/deng_math.cpp.o: $(SRC_DIR)/maths/deng_math.cpp	
	$(CXX) -c $(SRC_DIR)/maths/deng_math.cpp -o $(BUILD_DIR)/obj/deng/deng_math.cpp.o $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/events.cpp.o: $(SRC_DIR)/maths/events.cpp
	$(CXX) -c $(SRC_DIR)/maths/events.cpp -o $(BUILD_DIR)/obj/deng/events.cpp.o $(CXX_FLAGS)


# Utilities objects
$(BUILD_DIR)/obj/deng/object_loader.cpp.o: $(SRC_DIR)/utilities/assets_handler/object_loader.cpp
	$(CXX) -c $(SRC_DIR)/utilities/assets_handler/object_loader.cpp -o $(BUILD_DIR)/obj/deng/object_loader.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/texture_loader.cpp.o: $(SRC_DIR)/utilities/assets_handler/texture_loader.cpp
	$(CXX) -c $(SRC_DIR)/utilities/assets_handler/texture_loader.cpp -o $(BUILD_DIR)/obj/deng/texture_loader.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

$(BUILD_DIR)/obj/deng/file_manager.cpp.o: $(SRC_DIR)/utilities/data_handler/file_manager.cpp
	$(CXX) -c $(SRC_DIR)/utilities/data_handler/file_manager.cpp -o $(BUILD_DIR)/obj/deng/file_manager.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
		
$(BUILD_DIR)/obj/deng/grid_generator.cpp.o: $(SRC_DIR)/utilities/map/grid_generator.cpp
	$(CXX) -c $(SRC_DIR)/utilities/map/grid_generator.cpp -o $(BUILD_DIR)/obj/deng/grid_generator.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/timer.cpp.o: $(SRC_DIR)/utilities/timer/timer.cpp
	$(CXX) -c $(SRC_DIR)/utilities/timer/timer.cpp -o $(BUILD_DIR)/obj/deng/timer.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)


# Main object file
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) -c $(MAIN_SRC) -o $(MAIN_OBJ) $(CXX_FLAGS)


.PHONY: clean clean_exec
clean:
	rm -rf $(BUILD_DIR)/obj

clean_exec:
	rm -rf $(BUILD_DIR)/deng 
