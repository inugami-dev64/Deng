#ifndef DENG_CORE_H
#define DENG_CORE_H

/*  BEFORE EVERY GIT COMMIT SET GENERIC_DEBUG VALUE TO 0!!!     */
/*  DENGUI and grid have been totally removed from the engine */
#define DENG_VULKAN
#define GENERIC_DEBUG 1

/*  please do not enable these at the same time, 
    otherwise you won't be able to debug anything */
#define CAMERA_LOCATION_DEBUG 1
#define CAMERA_MOUSE_DEBUG 0

#define ERR(x) throw std::runtime_error(std::string("ERROR: ") + x)
#define WARNME(x) std::cout << "WARNING: " << x << std::endl

#if GENERIC_DEBUG
    
    #define LOG(x) std::cout << "LOG: " << x << std::endl
    #define WARNMEDB(x) std::cout << "WARNING: " << x << std::endl
    const bool enable_validation_layers = true;
#else
    #define LOG(x)
    #define ERRMEDB(x)
    const bool enable_validation_layers = false;
#endif

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

/* Pipeline type specifier */
enum dengPipelineType {
    DENG_PIPELINE_TYPE_UNMAPPED = 0,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED = 1,
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
enum dengWindowHint {
    DENG_WINDOW_HINT_FULL_SCREEN = 2,
    DENG_WINDOW_HINT_NO_MOUSE_LOCK = 4,
    DENG_WINDOW_HINT_NO_CURSOR_HIDE = 8
};

/* Hints for controlling renderer with API calls */
enum dengRendererHint {
    DENG_RENDERER_HINT_ASSET_EDITOR = 1,
    DENG_RENDERER_HINT_MAP_EDITOR = 2,
    DENG_RENDERER_HINT_GAME_MODE = 4,
    DENG_RENDERER_HINT_ENABLE_VSYNC = 8,
    DENG_RENDERER_HINT_SHOW_FPS_COUNTER = 16
};

#include <vulkan/vulkan.h>
#include <ft2build.h>
#include FT_FREETYPE_H

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
#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>

// local dependencies
#include "../surface/deng_surface_core.h"
#include "../das/das_core.h"
#include "../maths/deng_math.h"
#include "../utilities/timer/timer.h"
#include "window.h"
#include "camera.h"
// #include "../utilities/map/grid_generator.h"
#include "../maths/events.h"
#include "rend_helpers.h"
#include "../utilities/rend_utils.h"
// #include "../dengui/core/dengui_core.h"
#include "renderer.h"

#endif