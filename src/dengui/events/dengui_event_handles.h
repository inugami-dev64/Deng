#ifndef DENGUI_EVENT_HANDLES_H
#define DENGUI_EVENT_HANDLES_H

namespace dengUI {

    class MinimiseHandle {
    private:
        WindowObject *m_p_minimise_triangle;
        WindowObject *m_p_main_window;
        WindowObject *m_p_main_window_borders;

        std::vector<dengUtils::UIVerticesData> *m_p_vertices_data;
        std::vector<uint16_t> *m_p_indices_data;

        dengUIWindowSizeMode m_window_size_mode;
        
        dengBool m_verified;
    private:
        void verifyVertices();

    public:
        MinimiseHandle(std::vector<WindowObject> *p_window_objects, std::vector<dengUtils::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data);
        void onClick();
    };

    class WindowObjectHandleList {
    private:
        std::vector<WindowObject> *m_p_window_objects;
        std::vector<WindowObject*> *m_p_clickable_objects;
        std::vector<dengUtils::UIVerticesData> *m_p_vertices_data;
        std::vector<uint16_t> *m_p_indices_data;
        
        // add all click handles here
        MinimiseHandle *m_p_minimise_handle;

    private:
        // add all handle construction functions here
        void setupMinimiseHandle();

    public:
        WindowObjectHandleList(std::vector<WindowObject> *p_window_objects, std::vector<WindowObject*> *p_clickable_objects, std::vector<dengUtils::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data);
        ~WindowObjectHandleList();
        void callClickHandler(const uint32_t &handle_id);

    };
}

#endif