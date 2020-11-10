#ifndef DENG_CORE_H
#define DENG_CORE_H

/*  BEFORE EVERY GIT COMMIT SET GENERIC_DEBUG VALUE TO 0!!!     */
#define DENG_VULKAN
#define GENERIC_DEBUG 0

/*  please do not enable these at the same time, 
    otherwise you won't be able to debug anything */
#define CAMERA_LOCATION_DEBUG 0
#define CAMERA_MOUSE_DEBUG 0

#define ERR(x) throw std::runtime_error(x)

#ifdef _WIN32
    #define TERM_ERRME_MODE ""
    #define TERM_LOG_MODE ""
    #define TERM_END "\n"
#else
    #define TERM_ERRME_MODE "\033[1;31m"
    #define TERM_LOG_MODE "\033[1;34m"
    #define TERM_END "\033[0m\n"
#endif
#define ERRME(x) std::cout << TERM_ERRME_MODE << x << TERM_END 

#if GENERIC_DEBUG
    #define LOG(x) std::cout << TERM_LOG_MODE << x << TERM_END
    #define ERRMEDB(x) std::cout << TERM_ERRME_MODE << x << TERM_END;
    const bool enable_validation_layers = true;
#else
    #define LOG(x)
    #define ERRMEDB(x)
    const bool enable_validation_layers = false;
#endif
#define DISABLE_DENGUI 0

enum dengPipelineDrawMode {
    DENG_PIPELINE_DRAW_MODE_LINEAR = 0,
    DENG_PIPELINE_DRAW_MODE_INDEXED = 1
};

enum dengBool {
    DENG_FALSE = 0,
    DENG_TRUE = 1
};

enum dengCoordinateAxisType {
    DENG_COORD_AXIS_UNDEFINED = -1,
    DENG_COORD_AXIS_X = 0,
    DENG_COORD_AXIS_Y = 1,
    DENG_COORD_AXIS_Z = 2
};

enum dengMovementEvent {
    DENG_MOVEMENT_NONE = -1,
    DENG_MOVEMENT_FORWARD = 0,
    DENG_MOVEMENT_BACKWARD = 1,
    DENG_MOVEMENT_RIGHTWARD = 2,
    DENG_MOVEMENT_LEFTWARD = 3,
    DENG_MOVEMENT_UPWARD = 4,
    DENG_MOVEMENT_DOWNWARD = 5
};

enum dengWriteMode {
    DENG_WRITEMODE_REWRITE = 0,
    DENG_WRITEMODE_FROM_END = 1
};

enum dengFolderContentsReadMode {
    DENG_FOLDER_CONTENTS_READ_MODE_FILES_ONLY = 0,
    DENG_FOLDER_CONTENTS_READ_MODE_FOLDERS_ONLY = 1,
    DENG_FOLDER_CONTENTS_READ_MODE_FILES_AND_FOLDERS = 2,
    DENG_FOLDER_CONTENTS_READ_MODE_RECURSIVE = 3
};

enum dengFolderContentsSortingMode {
    DENG_FOLDER_CONTENTS_SORT_STYLE_DONT_CARE = -1,
    DENG_FOLDER_CONTENTS_SORT_STYLE_ALL_ALPHABETICAL = 0,
    DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FOLDERS_FIRST = 1,
    DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FILES_FIRST = 2,
    DENG_FOLDER_CONTENTS_SORT_STYLE_ALL_REVERSE_ALPHABETICAL = 3,
    DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FOLDERS_FIRST = 4,
    DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FILES_FIRST = 5
};

enum dengPipelineType {
    DENG_PIPELINE_TYPE_OBJECT_BASED = 0,
    DENG_PIPELINE_TYPE_SPECIFIED = 1,
    DENG_PIPELINE_TYPE_UI = 2
};

enum dengCoordinateMode {
    DENG_COORDINATE_MODE_DEFAULT = 0,
    DENG_COORDINATE_MODE_REVERSE = 1
};

enum dengCoordinateType {
    DENG_VERTEX_COORD = 0,
    DENG_VERTEX_TEXTURE_COORD = 1,
    DENG_VERTEX_NORMAL_COORD = 2
};

enum dengImageType {
    DENG_IMAGE_TYPE_TEXTURE = 0,
    DENG_IMAGE_TYPE_DEPTH = 1
};

enum dengTextureFormat {
    DENG_TEXTURE_FORMAT_BMP = 0,
    DENG_TEXTURE_FORMAT_TGA = 1,
    DENG_TEXTURE_FORMAT_PNG = 2,
    DENG_TEXTURE_FORMAT_JPG = 3,
    DENG_TEXTURE_FORMAT_UNKNOWN = 4
};

enum dengInputMode {
    DENG_INPUT_NONMOVEMENT = 0,
    DENG_INPUT_MOVEMENT = 1
};

enum dengTriangleAngleType {
    DENG_TRIANGLE_ANGLE_ALPHA = 0,
    DENG_TRIANGLE_ANGLE_BETA = 1,
    DENG_TRIANGLE_ANGLE_GAMMA = 2
};

enum dengNumeralType {
    DENG_NUMERAL_TYPE_FLOAT = 0,
    DENG_NUMERAL_TYPE_DOUBLE = 1,
    DENG_NUMERAL_TYPE_NON_FLOATING = 2,
    DENG_NUMERAL_TYPE_BOOL = 3,
    DENG_NUMERAL_TYPE_DONT_CARE = 4
};


// external dependencies
#ifdef _WIN32
    #include "../win32_dirent_dep/dirent.h"
#else 
    #include <dirent.h>
#endif

#include <vulkan/vulkan.h>
#include <cmath>
#include <chrono>
#include <type_traits>
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <unordered_map>
#include <typeinfo>

typedef void(*BufferCreateFunc)(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceSize *p_size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkBuffer *p_buffer, VkDeviceMemory *p_buffer_memory, size_t *p_buffer_index);
typedef void(*BufferMemoryPopulateFunc)(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceSize *p_size, const void *p_src_data, VkBuffer *p_buffer, VkDeviceMemory *p_buffer_memory);
typedef void(*BufferCopyFunc)(VkDevice *p_device, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkBuffer *p_src_buffer, VkBuffer *p_dst_buffer, VkDeviceSize *p_size);

// local dependencies
#include "../surface/deng_surface_core.h"
#include "../maths/deng_math.h"
#include "../scripting/parsing.h"
#include "../utilities/data_handler/typename_finder.h"
#include "../utilities/timer/timer.h"
#include "window.h"
#include "camera.h"
#include "../utilities/data_handler/objects_handler.h"
#include "../utilities/map/grid_generator.h"
#include "../maths/events.h"
#include "../utilities/data_handler/files.h"
#include "renderer_utils.h"
#include "../utilities/assets_handler/texture_loader.h"
#include "../utilities/assets_handler/object_loader.h"
#include "../dengui/core/dengui_core.h"
#include "renderer.h"

#endif