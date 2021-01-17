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
#include "../common/common.h"


typedef deng_ui32_t deng_CameraUniformFlagBits;
#define DENG_CAMERA_UNIFORM_ORTHOGRAPHIC_CAMERA_MODE_3D 0x01u
#define DENG_CAMERA_UNIFORM_PERSPECTIVE_CAMERA_MODE_3D  0x02u
#define DENG_CAMERA_UNIFORM_ORTHOGRAPHIC_CAMERA_MODE_2D 0x04u
#define DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_2D           0x08u
#define DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_3D           0x10u

/* Hints for controlling window with API calls */
typedef deng_ui32_t deng_WindowHintBits;
#define DENG_WINDOW_HINT_FULL_SCREEN                    0x01u
#define DENG_WINDOW_HINT_WINDOWED                       0x02u
#define DENG_WINDOW_HINT_NO_MOUSE_LOCK                  0x04u
#define DENG_WINDOW_HINT_NO_CURSOR_HIDE                 0x08u

/* Hints for controlling renderer features */
typedef deng_ui32_t deng_RendererHintBits;
#define DENG_RENDERER_HINT_ENABLE_VSYNC                     0x0001u
#define DENG_RENDERER_HINT_SHOW_FPS_COUNTER                 0x0002u
#define DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS         0x0004u
#define DENG_RENDERER_HINT_MSAA_MAX_HARDWARE_SUPPORTED      0x0008u
#define DENG_RENDERER_HINT_MSAA_64                          0x0010u
#define DENG_RENDERER_HINT_MSAA_32                          0x0020u
#define DENG_RENDERER_HINT_MSAA_16                          0x0040u
#define DENG_RENDERER_HINT_MSAA_8                           0x0080u
#define DENG_RENDERER_HINT_MSAA_4                           0x0100u
#define DENG_RENDERER_HINT_MSAA_2                           0x0200u
#define DENG_RENDERER_HINT_MSAA_1                           0x0400u
#define DENG_RENDERER_HINT_MIPMAP_ENABLE                    0x0800u


/* Coordinate axis specifier */
enum deng_CoordinateAxisType {
    DENG_COORD_AXIS_UNDEFINED = -1,
    DENG_COORD_AXIS_X = 0,
    DENG_COORD_AXIS_Y = 1,
    DENG_COORD_AXIS_Z = 2
};

/* Movement mode specifier */
enum deng_MovementEvent {
    DENG_MOVEMENT_NONE = -1,
    DENG_MOVEMENT_FORWARD = 0,
    DENG_MOVEMENT_BACKWARD = 1,
    DENG_MOVEMENT_RIGHTWARD = 2,
    DENG_MOVEMENT_LEFTWARD = 3,
    DENG_MOVEMENT_UPWARD = 4,
    DENG_MOVEMENT_DOWNWARD = 5
};

/* Pipeline type specifier */
enum deng_PipelineType {
    DENG_PIPELINE_TYPE_UNMAPPED_3D = 0,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D = 1,
    DENG_PIPELINE_TYPE_UNMAPPED_2D = 2,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D = 3
};


/* User input specifiers */
enum deng_InputMode {
    DENG_INPUT_NONMOVEMENT = 0,
    DENG_INPUT_MOVEMENT = 1
};

/* Triangle angle specifier */
enum deng_TriangleAngleType {
    DENG_TRIANGLE_ANGLE_ALPHA = 0,
    DENG_TRIANGLE_ANGLE_BETA = 1,
    DENG_TRIANGLE_ANGLE_GAMMA = 2
};

/* Renderer usage mode */
enum deng_RendererUsageMode {
    DENG_RENDERER_USAGE_ASSET_EDITOR = 0,
    DENG_RENDERER_USAGE_MAP_EDITOR = 1,
    DENG_RENDERER_USAGE_GAME_MODE = 2,
};



// Local dependencies
#include "forward_dec.h"
#include "err_def.h"
#include "../surface/deng_surface_core.h"
#include "../das/das_core.h"
#include "../maths/deng_math.h"

#include "../utilities/timer/timer.h"
#include "window.h"
#include "camera.h"

#include "../shapes/shapes.h"
#include "../maths/events.h"
#include "rend_helpers.h"
#include "../utilities/font.h"
#include "../dengui/main/dengui.h"

// Editor UI classes
#include "../dengui/editor_uis/map_editor_ui.h"
#include "renderer.h"

#endif