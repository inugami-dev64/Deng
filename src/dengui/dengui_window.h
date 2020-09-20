#ifndef DENGUI_H
#define DENGUI_H

namespace dengUI {
    
    class Window {
    private:
        std::vector<deng::UIVerticesData> m_vertices_data;
        std::vector<uint16_t> m_indices_data;
        WindowInfo m_windowinfo;
        WindowBorderInfo m_borderinfo;
        BufferInfo m_bufferinfo;
        std::vector<WindowObject> m_window_objects_info;

        Events *m_p_events;

    private: 
        void createBaseWindow();
        void createBuffers();

    public:
        Window(WindowInfo &windowinfo, WindowBorderInfo borderinfo, BufferInfo &bufferinfo);
        ~Window();
        void update();
    };
}

#endif