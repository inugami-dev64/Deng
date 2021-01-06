#ifndef API_CORE_H
#define API_CORE_H

/*  BEFORE EVERY GIT COMMIT SET GENERIC_DEBUG VALUE TO 0!!!     */
/*  Debug will need to be defined by the programmer and not by the libarary */ 
#define __GENERIC_DEBUG 1
#define __DENG_API_CORE

/*  please do not enable these at the same time, 
    otherwise you won't be able to debug anything */
#define __CAMERA_LOCATION_DEBUG 0
#define __CAMERA_MOUSE_DEBUG 0

#define ERR(x) throw std::runtime_error(std::string("ERROR: ") + x)
#define WARNME(x) std::cout << "WARNING: " << x << std::endl

#if __GENERIC_DEBUG
    #define LOG(x) std::cout << "LOG: " << x << std::endl
    #define WARNMEDB(x) std::cout << "WARNING: " << x << std::endl
#else
    #define LOG(x)
    #define ERRMEDB(x)
#endif

#include <vulkan/vulkan.h>

#ifdef __linux__
    #include <dirent.h> 
#endif

#ifdef _WIN32
    #include "../win32_dirent_dep/dirent.h"
#endif

#include <cmath>
#include <chrono>
#include <type_traits>
#include <vector>
#include <string>
#include <string.h>
#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>


/* Universal deng error return type */ 
enum dengError {
    DENG_NO_ERRORS = 0,
    DENG_ERROR_TYPE_GENERAL_ERROR = 1,
    DENG_ERROR_TYPE_GENERAL_THIRD_PARTY_LIB_CALLBACK_ERROR = 2,
    DENG_ERROR_TYPE_INVALID_PATH = 3,
    DENG_ERROR_TYPE_INVALID_ASSET_DATA = 4,
    DENG_ERROR_TYPE_INVALID_TEXTURE_DATA = 5
};

/* Coordinate axis specifier */
enum dengCoordinateAxisType {
    DENG_COORD_AXIS_UNDEFINED = -1,
    DENG_COORD_AXIS_X = 0,
    DENG_COORD_AXIS_Y = 1,
    DENG_COORD_AXIS_Z = 2
};

/* Movement mode specifier */
enum dengMovementEvent {
    DENG_MOVEMENT_NONE = -1,
    DENG_MOVEMENT_FORWARD = 0,
    DENG_MOVEMENT_BACKWARD = 1,
    DENG_MOVEMENT_RIGHTWARD = 2,
    DENG_MOVEMENT_LEFTWARD = 3,
    DENG_MOVEMENT_UPWARD = 4,
    DENG_MOVEMENT_DOWNWARD = 5
};

typedef uint32_t dengCameraUniformFlagBits;
#define DENG_CAMERA_UNIFORM_ORTHOGRAPHIC_CAMERA_MODE_3D 0x00000001u
#define DENG_CAMERA_UNIFORM_PERSPECTIVE_CAMERA_MODE_3D 0x00000002u
#define DENG_CAMERA_UNIFORM_ORTHOGRAPHIC_CAMERA_MODE_2D 0x00000004u
#define DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_2D 0x00000008u
#define DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_3D 0x00000010u

/* Pipeline type specifier */
enum dengPipelineType {
    DENG_PIPELINE_TYPE_UNMAPPED_3D = 0,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D = 1,
    DENG_PIPELINE_TYPE_UNMAPPED_2D = 2,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D = 3
};

/* Coordinate mode specifier */
enum dengCoordinateMode {
    DENG_COORDINATE_MODE_DEFAULT = 0,
    DENG_COORDINATE_MODE_REVERSE = 1
};

/* Coordinate type specifier */
enum dengCoordinateType {
    DENG_VERTEX_COORD = 0,
    DENG_VERTEX_TEXTURE_COORD = 1,
    DENG_VERTEX_NORMAL_COORD = 2
};

/* Image type specifier */
enum dengImageType {
    DENG_IMAGE_TYPE_TEXTURE = 0,
    DENG_IMAGE_TYPE_DEPTH = 1
};

/* User input specifiers */
enum dengInputMode {
    DENG_INPUT_NONMOVEMENT = 0,
    DENG_INPUT_MOVEMENT = 1
};

/* Triangle angle specifier */
enum dengTriangleAngleType {
    DENG_TRIANGLE_ANGLE_ALPHA = 0,
    DENG_TRIANGLE_ANGLE_BETA = 1,
    DENG_TRIANGLE_ANGLE_GAMMA = 2
};

/* Renderer usage mode */
enum dengRendererUsageMode {
    DENG_RENDERER_USAGE_ASSET_EDITOR = 0,
    DENG_RENDERER_USAGE_MAP_EDITOR = 1,
    DENG_RENDERER_USAGE_GAME_MODE = 2,
};

/* Hints for controlling window with API calls */
typedef uint32_t dengWindowHintBits;
#define DENG_WINDOW_HINT_FULL_SCREEN 0x00000001u
#define DENG_WINDOW_HINT_WINDOWED 0x00000002u
#define DENG_WINDOW_HINT_NO_MOUSE_LOCK 0x00000004u
#define DENG_WINDOW_HINT_NO_CURSOR_HIDE 0x00000008u

/* Hints for controlling renderer with API calls */
typedef uint32_t dengRendererHintBits;
#define DENG_RENDERER_HINT_ASSET_EDITOR 0x00000001u
#define DENG_RENDERER_HINT_MAP_EDITOR 0x00000002u
#define DENG_RENDERER_HINT_GAME_MODE 0x00000004u
#define DENG_RENDERER_HINT_ENABLE_VSYNC 0x00000008u
#define DENG_RENDERER_HINT_SHOW_FPS_COUNTER 0x00000010u
#define DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS 0x00000020u

// local dependencies
#include "../surface/deng_surface_core.h"
#include "../das/das_core.h"
#include "../maths/deng_math.h"

#include "../utilities/timer/timer.h"
#include "window.h"
#include "camera.h"

#include "../maths/events.h"
#include "rend_helpers.h"
#include "../utilities/font.h"
#include "../dengui/main/dengui.h"

#include "renderer.h"

#endif