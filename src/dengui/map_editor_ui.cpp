#define __DENG_USE_GUI
#include "../../headers/deng/api_core.h"

namespace dengui {
    MapEditor::MapEditor (
        EventInfo &ev_info, 
        dengUtils::StringRasterizer *p_sr
    ) {
        m_p_ev = new Events(ev_info);
        
        // Create main side window for map editor
        WindowInfo wi;
        wi.fl_b = DENG_MAP_UI_MAIN_WINDOW_FLAGS;
        wi.id = "DENG map editor";
        wi.pc = DENG_MAP_UI_PRIMARY_COLOR;
        wi.sc = DENG_MAP_UI_SECONDARY_COLOR;
        wi.tc = DENG_MAP_UI_TERTIARY_COLOR;
        wi.pos = {0.0f, 0.0f};
        wi.size = {0.6f, 1.0f};
        wi.wt = DENG_MAP_UI_MAIN_WINDOW_TYPE;
        wi.border = DENG_MAP_UI_BORDER;

        beginWindow (
            &m_p_main_win,
            &wi,
            ev_info.deng_window_area,
            p_sr,
            m_p_ev,
            false
        );
    }


    /* Event handler class instance getter */
    Events *MapEditor::getEv() { return m_p_ev; }
}
