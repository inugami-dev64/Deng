#include "../core/deng_core.h" 

namespace dengUI {
    
    Events::Events(deng::Window *p_window, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data) {
        this->m_p_window = p_window;
        this->m_p_vertices_data = p_vertices_data;
        this->m_p_indices_data = p_indices_data;
    }

    void Events::minimizeWindow() {
        return;
    }
};