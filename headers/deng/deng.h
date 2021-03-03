#ifndef __DENG_H
#define __DENG_H

namespace std {}
#ifdef DENG_ENABLE_STD_INCL
#endif

#ifdef DENG_ENABLE_STD_INCL
    #include <vector>
    #include <array>
    #include <mutex>
#endif

#include <vulkan/vulkan.h>
#include <common/base_types.h>
#include <das/assets.h>
#include <das/das_handler.h>
#include <das/data_loader.h>

#include <deng/forward_dec.h>
#include <deng/deng_math.h>
#include <deng/surface_window.h>

#include <utils/grid_generator.h>
#include <utils/collision.h>
#include <utils/timer.h>
#include <utils/font.h>

#include <deng/window.h>
#include <deng/camera.h>

// Vulkan renderer includes
#include <deng/vulkan/vulkan_qm.h>
#include <deng/vulkan/vulkan_sd.h>
#include <deng/vulkan/vulkan_resources.h>
#include <deng/vulkan/vulkan_handler.h>
#include <deng/vulkan/vulkan_rend_infos.h>
#include <deng/vulkan/vulkan_renderer.h>


#include <dengui/dengui_win_def.h>
#include <dengui/dengui_infos.h>
#include <dengui/dengui_child.h>
#include <dengui/map_editor_ui.h>
#include <deng/renderer.h>

#include <dengui/dengui_win_def.h>
#include <dengui/dengui_infos.h>
#include <dengui/vulkan/vulkan_update.h>
#include <dengui/dengui_window.h>
#include <dengui/dengui_events.h>

#endif
