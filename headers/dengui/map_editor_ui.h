#ifndef __MAP_EDITOR_UI_H
#define __MAP_EDITOR_UI_H

#ifdef __MAP_EDITOR_UI_CPP
    #include <stdlib.h>
    #include <vector>
    #include <array>
    #include <string>
    #include <mutex>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>

    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>

    #include <utils/font.h>
    #include <utils/shapes.h>
    #include <utils/collision.h>
    #include <dengui/dengui.h>
    #include <dengui/ui_callbacks.h>

    // Main editor window
    #define DENG_MAP_UI_MAIN_WINDOW_FLAGS       DENGUI_WINDOW_FLAG_NO_MOVE | \
                                                DENGUI_WINDOW_FLAG_NO_RESIZE 

    #define DENG_MAP_UI_MAIN_WINDOW_TYPE        dengui::WINDOW_TYPE_STATIC_RIGHT

    // Asset windows
    #define DENG_MAP_UI_ASSET_WINDOW_FLAGS      DENGUI_WINDOW_FLAG_ALWAYS_ON_TOP | \
                                                DENGUI_WINDOW_FLAG_NO_RESIZE
    #define DENG_MAP_UI_ASSET_WINDOW_TYPE       dengui::WINDOW_TYPE_FLOATING

    // Texture windows
    #define DENG_MAP_UI_TEXTURE_WINDOW_FLAGS    DENGUI_WINDOW_FLAG_ALWAYS_ON_TOP | \
                                                DENGUI_WINDOW_FLAG_NO_RESIZE

    #define DENG_MAP_UI_TEXTURE_WINDOW_TYPE     dengui::WINDOW_TYPE_FLOATING

    // Default color scheme
    #define DENG_MAP_UI_PRIMARY_COLOR           DENGUI_DEFAULT_PRIMARY_COLOR
    #define DENG_MAP_UI_SECONDARY_COLOR         DENGUI_DEFAULT_SECONDARY_COLOR
    #define DENG_MAP_UI_TERTIARY_COLOR          DENGUI_DEFAULT_TERTIARY_COLOR

    // Default border mode
    #define DENG_MAP_UI_BORDER                  dengui::BORDER_MEDIUM
#endif

namespace dengui {
    class MapEditor {
    private:
        Events *m_p_ev = NULL;
        Window *m_p_main_win = NULL;
        std::vector<Window*> m_assets_windows;
        std::vector<Window*> m_texture_windows;
        dengMath::vec2<deng_ui32_t> m_win_size;
        dengUtils::StringRasterizer *m_p_sr = NULL;
        ChildPushButton *m_p_pb = NULL;
        ChildContainer *m_p_cont = NULL;

    private:
        std::vector<DropDownElementInfo> getDDMElemInfos();
        void genTestObjects();
    
    public:
        MapEditor (
            EventInfo &ev_info,
            dengUtils::StringRasterizer *p_sr,
            dengMath::vec2<deng_ui32_t> window_size
        );

        ~MapEditor();

    // Getters
    public:
        Events *getEv();
    };
}

#endif
