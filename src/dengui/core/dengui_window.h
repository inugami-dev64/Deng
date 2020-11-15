#ifndef DENGUI_H
#define DENGUI_H

namespace dengUI {

    class Window {
    private:
        std::vector<dengUtils::UnmappedVerticesData> m_vertices_data;
        std::vector<uint16_t> m_indices_data;
        WindowInfo m_windowinfo;
        BufferInfo m_bufferinfo;
        ClickInfo m_clickinfo;

        PixelCollision *m_p_collision;
        // Events *m_p_events;
        // WindowObjectHandleList *m_p_handler_list;
        dengCSS::CSSDataHandler *m_p_css_data_handler;
        BaseWindowVertices m_base_window_vertices;

    private: 
        void createBaseWindow(dengCSS::CSSHeadData *p_title_css_data, dengCSS::CSSBodyData *p_body_css_data, dengCSS::CSSGenericObjectData *p_minimise_triangle_css_data);
        void createBuffers();
        void sortClickableObjects();

        void getWindowVertices(const dengUIWindowVerticesType &vertices_type, dengCSS::CSSHeadData *p_css_title_data, dengCSS::CSSBodyData *p_css_body_data, dengCSS::CSSGenericObjectData *p_generic_object_data);

    public:
        Window(WindowInfo &windowinfo, BufferInfo &bufferinfo);
        // void getClickableObjects(std::vector<WindowObject*> *p_clickable_objects);
        ~Window();
        void update();
    };
}

#endif