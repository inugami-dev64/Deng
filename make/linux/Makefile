CC = gcc
CXX = g++
LD = ar
SRC_DIR = ../../src
BUILD_DIR = ../../build
TARGET = deng
DAM_TARGET = $(BUILD_DIR)/deng/dam
LIBSURFACE_TARGET = $(BUILD_DIR)/deng/lib/libsurface.a
LIBDAS_TARGET = $(BUILD_DIR)/deng/lib/libdas.so
LIBCOMMON_TARGET = $(BUILD_DIR)/deng/lib/libcommon.so

LD_PATH = -L $(BUILD_DIR)/deng/lib

INCLUDE_PATH = -I /usr/include/freetype2 \
			   -I /usr/include/freetype2/freetype

CC_FLAGS = -Wall -m64
CXX_FLAGS = -Wall -m64 -std=c++11
LD_FLAGS = -lvulkan -lsurface -lX11 -lXcursor -ldl -ldas -lcommon -lfreetype

# Main engine with sample object loading
MAIN_SRC = $(SRC_DIR)/main/sandbox.cpp
MAIN_OBJ = $(BUILD_DIR)/obj/deng/sandbox.cpp.o

CORE_OBJ = $(BUILD_DIR)/obj/deng/camera.cpp.o \
		   $(BUILD_DIR)/obj/deng/rend_helpers.cpp.o \
		   $(BUILD_DIR)/obj/deng/renderer.cpp.o \
		   $(BUILD_DIR)/obj/deng/window.cpp.o 

MATH_OBJ = $(BUILD_DIR)/obj/deng/deng_math.cpp.o \
		   $(BUILD_DIR)/obj/deng/events.cpp.o 

UTILITIES_OBJ = $(BUILD_DIR)/obj/deng/timer.cpp.o \
				$(BUILD_DIR)/obj/deng/font.cpp.o \
				$(BUILD_DIR)/obj/deng/common.c.o
				# $(BUILD_DIR)/obj/deng/grid_generator.cpp.o 

OBJ = $(MAIN_OBJ) $(CORE_OBJ) $(MATH_OBJ) $(UTILITIES_OBJ)

# Surface library (static)
LIBSURFACE_OBJ = $(BUILD_DIR)/obj/libsurface/vulkan_handler.c.o \
				 $(BUILD_DIR)/obj/libsurface/key_translation.c.o \
				 $(BUILD_DIR)/obj/libsurface/key_vector.c.o \
				 $(BUILD_DIR)/obj/libsurface/x11_surface.c.o

# DENG asset file (das) format library (shared)
LIBDAS_OBJ = $(BUILD_DIR)/obj/libdas/das_handler.c.o \
			 $(BUILD_DIR)/obj/libdas/data_loader.c.o

# Common functionality library (shared)
LIBCOMMON_OBJ = $(BUILD_DIR)/obj/libcommon/common.c.o

DAM_OBJ = $(BUILD_DIR)/obj/dam/dam.c.o


$(TARGET): .init_build $(LIBDAS_TARGET) $(LIBCOMMON_TARGET) $(DAM_TARGET) $(LIBDAS_TARGET) $(LIBSURFACE_TARGET) $(OBJ)
	$(CXX) -o $(BUILD_DIR)/deng/$(TARGET) $(OBJ) $(LD_PATH) $(LD_FLAGS)
	cp -r ../../objects $(BUILD_DIR)/deng/
	cp -r ../../textures $(BUILD_DIR)/deng/
	cp -r ../../shaders $(BUILD_DIR)/deng/shaders
	cp -r $(SRC_DIR)/dengui/css $(BUILD_DIR)/deng/ui_styling
	cp -r $(SRC_DIR)/surface/xcursor $(BUILD_DIR)/deng/

.init_build:
	chmod +x init_build.sh
	./init_build.sh $(BUILD_DIR)

# DENG common library build
$(LIBCOMMON_TARGET): .init_build $(LIBCOMMON_OBJ)
	$(CC) -shared -o $(LIBCOMMON_TARGET) $(LIBCOMMON_OBJ)  

$(BUILD_DIR)/obj/libcommon/common.c.o: $(SRC_DIR)/common/common.c
	$(CC) -c $(SRC_DIR)/common/common.c -o $(BUILD_DIR)/obj/libcommon/common.c.o -fPIC


# libdas library build
$(LIBDAS_TARGET): .init_build $(LIBDAS_OBJ) $(LIBCOMMON_TARGET)
	$(CC) -shared -o $(LIBDAS_TARGET) $(LIBDAS_OBJ)

$(DAM_TARGET): .init_build $(LIBDAS_TARGET) $(DAM_OBJ) $(LIBCOMMON_TARGET)
	$(CC) -o $(DAM_TARGET) $(DAM_OBJ) $(LD_PATH) -ldas -lcommon

$(DAM_OBJ): .init_build $(SRC_DIR)/das/dam.c
	$(CC) -c $(SRC_DIR)/das/dam.c -o $(DAM_OBJ) $(CC_FLAGS) $(INCLUDE_PATH) -fPIC

$(BUILD_DIR)/obj/libdas/das_handler.c.o: .init_build $(SRC_DIR)/das/das_handler.c
	$(CC) -c $(SRC_DIR)/das/das_handler.c -o $(BUILD_DIR)/obj/libdas/das_handler.c.o $(CC_FLAGS) $(INCLUDE_PATH) -fPIC

$(BUILD_DIR)/obj/libdas/data_loader.c.o: .init_build $(SRC_DIR)/das/data_loader.c
	$(CC) -c $(SRC_DIR)/das/data_loader.c -o $(BUILD_DIR)/obj/libdas/data_loader.c.o $(CC_FLAGS) $(INCLUDE_PATH) -fPIC


# Surface library build
$(LIBSURFACE_TARGET): $(LIBSURFACE_OBJ)
	$(LD) rs $(LIBSURFACE_TARGET) $(LIBSURFACE_OBJ)

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
	$(CXX) -c $(SRC_DIR)/core/camera.cpp -o $(BUILD_DIR)/obj/deng/camera.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH) 

$(BUILD_DIR)/obj/deng/rend_helpers.cpp.o: $(SRC_DIR)/core/rend_helpers.cpp
	$(CXX) -c $(SRC_DIR)/core/rend_helpers.cpp -o $(BUILD_DIR)/obj/deng/rend_helpers.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)

$(BUILD_DIR)/obj/deng/renderer.cpp.o: $(SRC_DIR)/core/renderer.cpp
	$(CXX) -c $(SRC_DIR)/core/renderer.cpp -o $(BUILD_DIR)/obj/deng/renderer.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)
	
$(BUILD_DIR)/obj/deng/window.cpp.o: $(SRC_DIR)/core/window.cpp
	$(CXX) -c $(SRC_DIR)/core/window.cpp -o $(BUILD_DIR)/obj/deng/window.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)


# DENGUI objects
$(BUILD_DIR)/obj/deng/dengui_sprites.cpp.o: $(SRC_DIR)/dengui/core/dengui_sprites.cpp
	$(CXX) -c $(SRC_DIR)/dengui/core/dengui_sprites.cpp -o $(BUILD_DIR)/obj/deng/dengui_sprites.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)

$(BUILD_DIR)/obj/deng/dengui_window.cpp.o: $(SRC_DIR)/dengui/core/dengui_window.cpp
	$(CXX) -c $(SRC_DIR)/dengui/core/dengui_window.cpp -o $(BUILD_DIR)/obj/deng/dengui_window.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)
	
$(BUILD_DIR)/obj/deng/css_data_manager.cpp.o: $(SRC_DIR)/dengui/css_engine/css_data_manager.cpp
	$(CXX) -c $(SRC_DIR)/dengui/css_engine/css_data_manager.cpp -o $(BUILD_DIR)/obj/deng/css_data_manager.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)
	
# $(BUILD_DIR)/obj/deng/dengui_event_handles.cpp.o: $(SRC_DIR)/dengui/events/dengui_event_handles.cpp
# 	$(CXX) -c $(SRC_DIR)/dengui/events/dengui_event_handles.cpp -o $(BUILD_DIR)/obj/deng/ui_event_handles.cpp.o  $(CXX_FLAGS)
	
# $(BUILD_DIR)/obj/deng/dengui_events.cpp.o: $(SRC_DIR)/dengui/events/dengui_events.cpp
# 	$(CXX) -c $(SRC_DIR)/dengui/events/dengui_events.cpp -o $(BUILD_DIR)/obj/deng/ui_events.cpp.o  $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/dengui_pixel_collision.cpp.o: $(SRC_DIR)/dengui/pixel_perfect_collision/dengui_pixel_collision.cpp
	$(CXX) -c $(SRC_DIR)/dengui/pixel_perfect_collision/dengui_pixel_collision.cpp -o $(BUILD_DIR)/obj/deng/dengui_pixel_collision.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)

$(BUILD_DIR)/obj/deng/dengui_font_loader.cpp.o: $(SRC_DIR)/dengui/core/dengui_font_loader.cpp
	$(CXX) -c $(SRC_DIR)/dengui/core/dengui_font_loader.cpp -o $(BUILD_DIR)/obj/deng/dengui_font_loader.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)


# Math objects
$(BUILD_DIR)/obj/deng/deng_math.cpp.o: $(SRC_DIR)/maths/deng_math.cpp	
	$(CXX) -c $(SRC_DIR)/maths/deng_math.cpp -o $(BUILD_DIR)/obj/deng/deng_math.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)
	
$(BUILD_DIR)/obj/deng/events.cpp.o: $(SRC_DIR)/maths/events.cpp
	$(CXX) -c $(SRC_DIR)/maths/events.cpp -o $(BUILD_DIR)/obj/deng/events.cpp.o $(CXX_FLAGS) $(INCLUDE_PATH)


# Utilities objects		
$(BUILD_DIR)/obj/deng/grid_generator.cpp.o: $(SRC_DIR)/utilities/map/grid_generator.cpp
	$(CXX) -c $(SRC_DIR)/utilities/map/grid_generator.cpp -o $(BUILD_DIR)/obj/deng/grid_generator.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)
	
$(BUILD_DIR)/obj/deng/timer.cpp.o: $(SRC_DIR)/utilities/timer/timer.cpp
	$(CXX) -c $(SRC_DIR)/utilities/timer/timer.cpp -o $(BUILD_DIR)/obj/deng/timer.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

$(BUILD_DIR)/obj/deng/font.cpp.o: $(SRC_DIR)/utilities/font.cpp
	$(CXX) -c $(SRC_DIR)/utilities/font.cpp -o $(BUILD_DIR)/obj/deng/font.cpp.o $(INCLUDE_PATH) $(CXX_FLAGS)

$(BUILD_DIR)/obj/deng/common.c.o: $(SRC_DIR)/common/common.c
	$(CC) -c $(SRC_DIR)/common/common.c -o $(BUILD_DIR)/obj/deng/common.c.o $(INCLUDE_PATH) $(CC_FLAGS)


# Main object file
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) -c $(MAIN_SRC) -o $(MAIN_OBJ) $(CXX_FLAGS) $(INCLUDE_PATH)


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

