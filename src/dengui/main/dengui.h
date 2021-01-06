#ifndef DENGUI_H
#define DENGUI_H

#ifndef __DENG_API_CORE
    #include <thread>
    #include <string>
    #include <stdlib.h>
    #include <vector>
    #include <chrono>
    #include <thread>
    #include <mutex>
    #include <array>
    #include "../../maths/deng_math.h"
    #include "../../das/data_loader.h"
    #include "../../surface/deng_surface_core.h"
    #include "../../utilities/font.h"
#endif

// Window description flags
typedef unsigned char denguiWindowFlagBits; 

#include "gui_events.h"
#include "gui_window.h"
#include "../map_editor/map_windows.h"

#endif