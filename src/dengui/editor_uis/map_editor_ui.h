#ifndef GUI_BINDINGS_H
#define GUI_BINDINGS_H

#ifdef __DENG_USE_GUI
    // Main editor window
    #define DENG_MAP_UI_MAIN_WINDOW_FLAGS       DENGUI_WINDOW_FLAG_NO_MOVE | \
                                                DENGUI_WINDOW_FLAG_NO_RESIZE | \
                                                DENGUI_WINDOW_FLAG_NO_CLOSE
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
    #define DENG_MAP_UI_BORDER                  dengui::WINDOW_BORDER_LIGHT
#endif

namespace dengui {
    class MapEditor {
    private:
        Events *m_p_ev;
        Window *m_p_main_win;
        std::vector<Window*> m_assets_windows;
        std::vector<Window*> m_texture_windows;
    
    public:
        MapEditor (
            EventInfo &ev_info,
            dengUtils::FontManager *p_fm
        );

    // Getters
    public:
        Events *getEv();

    };
}

#endif