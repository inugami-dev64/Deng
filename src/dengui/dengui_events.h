#ifndef DENGUI_EVENTS_H
#define DENGUI_EVENTS_H

namespace dengUI {

    class Events {
    private:
        deng::Window *m_p_window;
        std::vector<deng::UIVerticesData> *m_p_vertices_data;
        std::vector<uint16_t> *m_p_indices_data;
        // std::vector<WindowObject> *m_p_window_objects;

    public:
        Events(deng::Window *p_window, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data);
        void updateMouse();
        void minimizeWindow();
    };

};

#endif