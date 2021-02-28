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
#include "../common/base_types.h"
#include "../common/common.h"

// Local dependencies
#include "forward_dec.h"
#include "err_def.h"
#include "deng_surface_core.h"
#include "../das/das_core.h"
#include "deng_math.h"

#include "../utils/timer.h"
#include "window.h"
#include "camera.h"

#include "../utils/shapes.h"
#include "../utils/collision.h"
#include "rend_helpers.h"
#include "../utils/font.h"
#include "../utils/grid_generator.h"

// Editor UI classes
#include "../dengui/dengui.h"
#include "../dengui/ui_callbacks.h"
#include "../dengui/map_editor_ui.h"
#include "renderer.h"

#endif
