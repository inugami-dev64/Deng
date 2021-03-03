#ifndef BASE_TYPES_H
#define BASE_TYPES_H
#include <stdint.h>

/* Main deng integer types */
typedef uint64_t deng_ui64_t;
typedef int64_t deng_i64_t;

typedef uint32_t deng_ui32_t;
typedef int32_t deng_i32_t;

typedef uint16_t deng_ui16_t;
typedef int16_t deng_i16_t;

typedef uint8_t deng_ui8_t;
typedef int8_t deng_i8_t;

typedef float deng_f32_t;
typedef double deng_f64_t;

typedef uint8_t deng_bool_t;

/* Size types */
typedef double deng_px_t;
typedef float deng_vec_t;
#define true 1
#define false 0

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
typedef deng_ui16_t deng_RendererHintBits;
#define DENG_RENDERER_HINT_API_VULKAN                       0x0001u
#define DENG_RENDERER_HINT_API_OPENGL                       0x0002u
#define DENG_RENDERER_HINT_ENABLE_VSYNC                     0x0004u
#define DENG_RENDERER_HINT_SHOW_FPS_COUNTER                 0x0008u
#define DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS         0x0010u
#define DENG_RENDERER_HINT_MSAA_MAX_HARDWARE_SUPPORTED      0x0020u
#define DENG_RENDERER_HINT_MSAA_64                          0x0040u
#define DENG_RENDERER_HINT_MSAA_32                          0x0080u
#define DENG_RENDERER_HINT_MSAA_16                          0x0100u
#define DENG_RENDERER_HINT_MSAA_8                           0x0200u
#define DENG_RENDERER_HINT_MSAA_4                           0x0400u
#define DENG_RENDERER_HINT_MSAA_2                           0x0800u
#define DENG_RENDERER_HINT_MSAA_1                           0x1000u
#define DENG_RENDERER_HINT_MIPMAP_ENABLE                    0x2000u
#define DENG_RENDERER_HINT_CUSTOM_LOOP                      0x4000u


/* Coordinate axis specifier */
typedef enum deng_CoordinateAxisType {
    DENG_COORD_AXIS_UNDEFINED = -1,
    DENG_COORD_AXIS_X = 0,
    DENG_COORD_AXIS_Y = 1,
    DENG_COORD_AXIS_Z = 2
} deng_CoordinateAxisType;


/* Movement mode specifier */
typedef enum deng_MovementEvent {
    DENG_MOVEMENT_NONE = -1,
    DENG_MOVEMENT_FORWARD = 0,
    DENG_MOVEMENT_BACKWARD = 1,
    DENG_MOVEMENT_RIGHTWARD = 2,
    DENG_MOVEMENT_LEFTWARD = 3,
    DENG_MOVEMENT_UPWARD = 4,
    DENG_MOVEMENT_DOWNWARD = 5
} deng_MovementEvent;


/* Pipeline type specifier */
typedef enum deng_PipelineType {
    DENG_PIPELINE_TYPE_UNMAPPED_3D = 0,
    DENG_PIPELINE_TYPE_UNMAPPED_3D_NORM = 1,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D = 2,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D_NORM = 3,
    DENG_PIPELINE_TYPE_UNMAPPED_2D = 4,
    DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D = 5
} deng_PipelineType;


/* Triangle angle specifier */
typedef enum deng_TriangleAngleType {
    DENG_TRIANGLE_ANGLE_ALPHA = 0,
    DENG_TRIANGLE_ANGLE_BETA = 1,
    DENG_TRIANGLE_ANGLE_GAMMA = 2
} deng_TriangleAngleType;


/* Renderer usage mode */
typedef enum deng_RendererUsageMode {
    DENG_RENDERER_USAGE_ASSET_EDITOR = 0,
    DENG_RENDERER_USAGE_MAP_EDITOR = 1,
    DENG_RENDERER_USAGE_GAME_MODE = 2,
} deng_RendererUsageMode;


/* Camera type specifier */
typedef enum deng_CameraType {
    DENG_CAMERA_FPP     = 0,
    DENG_CAMERA_EDITOR  = 1
} deng_CameraType;


/* Editor camera event specifier */
typedef enum deng_EditorCameraEvent {
    DENG_EDITOR_CAMERA_NONE         = -1,
    DENG_EDITOR_CAMERA_Z_MOV_IN     = 0,
    DENG_EDITOR_CAMERA_Z_MOV_OUT    = 1,
    DENG_EDITOR_CAMERA_MOUSE_ROTATE = 2
} deng_EditorCameraEvent;


/* 
 * Specify input device type for action lookup
 */
typedef enum deng_InputType {
    DENG_INPUT_TYPE_MOUSE   = 0,
    DENG_INPUT_TYPE_KB      = 1
} deng_InputType;


/*
 * Specify input event type for lookup
 */
typedef enum deng_InputEventType {
    DENG_INPUT_EVENT_TYPE_ACTIVE    = 0,
    DENG_INPUT_EVENT_TYPE_RELEASED  = 1
} deng_InputEventType;


#define DENG_KEY_PRESS_INTERVAL         200 // ms
#define DENG_MOVEMENT_INTERVAL          17 // ms

#define DENG_CAMERA_BASE_SPEED_X        0.015
#define DENG_CAMERA_BASE_SPEED_Y        0.015
#define DENG_CAMERA_BASE_SPEED_Z        -0.015

#define DENG_FPP_CAMERA_DEFAULT_POS_X   0.0f
#define DENG_FPP_CAMERA_DEFAULT_POS_Y   0.5f
#define DENG_FPP_CAMERA_DEFAULT_POS_Z   0.5f

// World coordinates
#define DENG_EDITOR_CAMERA_DEFAULT_POS_X    0.0f
#define DENG_EDITOR_CAMERA_DEFAULT_POS_Y    0.0f
#define DENG_EDITOR_CAMERA_DEFAULT_POS_Z    3.0f
#define DENG_EDITOR_CAMERA_DEFAULT_X_ROT    dengMath::Conversion::degToRad(-30.0f)
#define DENG_EDITOR_CAMERA_DEFAULT_Y_ROT    dengMath::Conversion::degToRad(30.0f)
#endif
