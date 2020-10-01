#ifndef DENGUI_EVENTS_H
#define DENGUI_EVENTS_H

namespace dengUI {

    class Events {
    private:
        deng::Window *m_p_window;
        std::vector<dengUtils::UIVerticesData> *m_p_vertices_data;
        std::vector<uint16_t> *m_p_indices_data;
        PixelCollision *m_p_pixel_collision;

    private:
        void updateBuffers(ClickInfo *p_clickinfo);

    public:
        Events(deng::Window *p_window, PixelCollision *p_pixel_collision, std::vector<dengUtils::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data);
        void checkForClicks(ClickInfo *p_clickinfo, WindowObjectHandleList *p_handle_list);
    };

};

#endif