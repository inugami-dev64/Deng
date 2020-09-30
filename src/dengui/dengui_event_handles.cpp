#include "../core/deng_core.h"

namespace dengUI {
    WindowObjectHandleList::WindowObjectHandleList(std::vector<WindowObject> *p_window_objects, std::vector<WindowObject*> *p_clickable_objects, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data) {
        this->m_p_window_objects = p_window_objects;
        this->m_p_clickable_objects = p_clickable_objects;
        this->m_p_vertices_data = p_vertices_data;
        this->m_p_indices_data = p_indices_data;

        // execution of all handle setup functions
        this->setupMinimiseHandle();
    }

    WindowObjectHandleList::~WindowObjectHandleList() {
        delete this->m_p_minimise_handle;
    }

    void WindowObjectHandleList::setupMinimiseHandle() {
        this->m_p_minimise_handle = new MinimiseHandle(this->m_p_window_objects, this->m_p_vertices_data, this->m_p_indices_data);
    }

    void WindowObjectHandleList::callClickHandler(const uint32_t &handle_id) {
        // add clickable window object handles into cases
        switch (handle_id)
        {
        case 0:
            this->m_p_minimise_handle->onClick();
            break;
        
        default:
            break;
        }
    }


    MinimiseHandle::MinimiseHandle(std::vector<WindowObject> *p_window_objects, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data) {
        this->m_p_minimise_triangle = deng::HandleData::findElementByDescription(p_window_objects, p_window_objects->size(), "minimising triangle");
        this->m_p_main_window = deng::HandleData::findElementByDescription(p_window_objects, p_window_objects->size(), "main window");
        this->m_p_main_window_borders = deng::HandleData::findElementByDescription(p_window_objects, p_window_objects->size(), "window borders");
        
        this->m_p_vertices_data = p_vertices_data;
        this->m_p_indices_data = p_indices_data;

        this->m_window_size_mode = DENGUI_WINDOW_SIZE_MODE_MAXIMISED;
        this->verifyVertices();
    }

    void MinimiseHandle::verifyVertices() {
        this->m_verified == DENG_TRUE;

        if(this->m_p_minimise_triangle->vertices_bounds.second - this->m_p_minimise_triangle->vertices_bounds.first != 3) {
            ERRME("Failed to verify dengui minimise triangle vertices in minimise handler!");
            ERRME("The reqired amount of vertices is 3, but given is " + std::to_string(this->m_p_minimise_triangle->vertices_bounds.second - this->m_p_minimise_triangle->vertices_bounds.first) + "!");
            this->m_verified == DENG_FALSE;
        }

        if(this->m_p_main_window->vertices_bounds.second - this->m_p_main_window->vertices_bounds.first != 4) {
            ERRME("Failed to verify dengui window vertices in minimise handler!");
            ERRME("The reqired amount of vertices is 3, but given is " + std::to_string(this->m_p_main_window->vertices_bounds.second - this->m_p_main_window->vertices_bounds.first) + "!");
            this->m_verified == DENG_FALSE;
        }

        if(this->m_p_main_window_borders->vertices_bounds.second - this->m_p_main_window_borders->vertices_bounds.first != 8) {
            ERRME("Failed to verify dengui window border vertices in minimise handler!");
            ERRME("The reqired amount of vertices is 3, but given is " + std::to_string(this->m_p_main_window_borders->vertices_bounds.second - this->m_p_main_window_borders->vertices_bounds.first) + "!");
            this->m_verified == DENG_FALSE;
        }
    }

    void MinimiseHandle::onClick() {
        if(this->m_verified) {
            deng::vec3<deng::UIVerticesData> local_original_vertices;
            local_original_vertices.first = (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first];
            local_original_vertices.second = (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first + 1];
            local_original_vertices.third = (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first + 2];

            switch (this->m_window_size_mode)
            {
            case DENGUI_WINDOW_SIZE_MODE_MAXIMISED:
                (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first].position_vec.first = local_original_vertices.third.position_vec.first;
                (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first + 1].position_vec.second = local_original_vertices.third.position_vec.second;
                (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first + 2].position_vec.first = local_original_vertices.first.position_vec.first;
                
                this->m_p_main_window->is_drawn = DENG_FALSE;
                this->m_p_main_window_borders->is_drawn = DENG_FALSE;
                this->m_window_size_mode = DENGUI_WINDOW_SIZE_MODE_MINIMIZED;
                break;

            case DENGUI_WINDOW_SIZE_MODE_MINIMIZED:
                (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first].position_vec.first = local_original_vertices.third.position_vec.first;
                (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first + 1].position_vec.second = local_original_vertices.first.position_vec.second;
                (*this->m_p_vertices_data)[this->m_p_minimise_triangle->vertices_bounds.first + 2].position_vec.first = local_original_vertices.first.position_vec.first;
                
                this->m_p_main_window->is_drawn = DENG_TRUE;
                this->m_p_main_window_borders->is_drawn = DENG_TRUE;
                this->m_window_size_mode = DENGUI_WINDOW_SIZE_MODE_MAXIMISED;
                break;
            
            default:
                break;
            }
        }
    }
}