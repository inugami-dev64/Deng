/*  BEFORE EVERY GIT COMMIT SET DEBUG VALUE TO 0!!!     */

#define DEBUG 1
#define ERR(x) throw std::runtime_error(x)
#define ERRME(x) std::cout << "\033[1;31m" << x << "\033[0m\n" 

#if DEBUG
    #define LOG(x) std::cout << "\033[1;34m" << x << "\033[0m\n"
    const bool enable_validation_layers = true;
#else
    #define LOG(x)
    const bool enable_validation_layers = false;
#endif



namespace deng {

    enum dengPipelineDrawMode {
        DENG_PIPELINE_DRAW_MODE_LINEAR = 0,
        DENG_PIPELINE_DRAW_MODE_INDEXED = 1
    };

    enum dengBool {
        DENG_FALSE = 0,
        DENG_TRUE = 1
    };

    enum dengCoordinateAxisType {
        DENG_X = 0,
        DENG_Y = 1,
        DENG_Z = 2
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

    enum dengBufferType {
        DENG_BUFFER_TYPE_STAGING = 0,
        DENG_BUFFER_TYPE_VERTEX = 1,
        DENG_BUFFER_TYPE_UNIFORM = 2,
        DENG_BUFFER_TYPE_GRID = 3,
        DENG_BUFFER_TYPE_WINDOW = 4,
        DENG_BUFFER_TYPE_WINDOW_INDICES = 5,
    };

    enum ImageType {
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
}

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
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
#include <typeinfo>

#include "../maths/dengmath.h"
#include "../scripting/parsing.h"
#include "../utilities/timer.h"
#include "window.h"
#include "camera.h"
#include "../maths/objectshandler.h"
#include "../utilities/gridgenerator.h"
#include "../maths/events.h"
#include "../utilities/files.h"
#include "rendererutils.h"
#include "../utilities/textureloader.h"
#include "../utilities/objloader.h"
#include "../dengui/dengui.h"
#include "renderer.h"
