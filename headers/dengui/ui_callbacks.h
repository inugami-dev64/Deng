#ifndef __UI_CALLBACKS_H
#define __UI_CALLBACKS_H

#ifdef __UI_CALLBACKS_CPP
    #include <vector>
    #include <array>
    #include <mutex>
    #include <array>
    #include <stdlib.h>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <das/assets.h>
    
    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>

    #include <utils/font.h>
    #include <utils/collision.h>
    #include <dengui/dengui_win_def.h>
    #include <dengui/dengui_infos.h>
    #include <dengui/vulkan/vulkan_update.h>
    #include <dengui/dengui_window.h>
    #include <dengui/dengui_events.h>
#endif

namespace dengui {
    /* Setup dengui callbacks for use */
    void setupCallbacks (
        dengUtils::StringRasterizer *p_sr,
        dengMath::vec2<deng_ui32_t> deng_window_area
    );

    void helloCallback(WindowElement *p_elem, Events *p_ev);
    void helloCallback(DDMElement *p_elem, Events *p_ev);
    
    /* Spawn help window */
    void ddmHelpCallback(DDMElement *p_elem, Events *p_ev);
    
    /* Basic window handling callbacks */
    void minTriangleCallback(WindowElement *p_cur_elem, Events *p_ev);
    void maxTriangleCallback(WindowElement *p_cur_elem, Events *p_ev);
    void closeBtnCallback(WindowElement *p_cur_elem, Events *p_ev);
}

#endif
