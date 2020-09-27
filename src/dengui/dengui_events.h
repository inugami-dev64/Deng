#ifndef DENGUI_EVENTS_H
#define DENGUI_EVENTS_H

namespace dengUI {

    class Events {
    private:
        deng::Window *m_p_window;
        std::vector<deng::UIVerticesData> *m_p_vertices_data;
        std::vector<uint16_t> *m_p_indices_data;
        PixelCollision *m_p_pixel_collision;

    public:
        Events(deng::Window *p_window, PixelCollision *p_pixel_collision, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data);
        void checkForClicks(ClickInfo *p_clickinfo);
        void minimizeWindow(deng::vec2<size_t> &triangle_vertices_bounds, deng::vec2<size_t> &triangle_indices_bounds);
    };

};

#endif