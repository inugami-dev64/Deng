#include "../core/deng_core.h" 

namespace dengUI {
    
    Events::Events(deng::Window *p_window, PixelCollision *p_pixel_collision, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data) {
        this->m_p_window = p_window;
        this->m_p_pixel_collision = p_pixel_collision;
        this->m_p_vertices_data = p_vertices_data;
        this->m_p_indices_data = p_indices_data;
    }

    void Events::minimizeWindow(deng::vec2<size_t> &triangle_vertices_bounds, deng::vec2<size_t> &triangle_indices_bounds) {
        (*this->m_p_vertices_data)[triangle_vertices_bounds.first].position_vec.second = (*this->m_p_vertices_data)[triangle_vertices_bounds.first + 2].position_vec.second;
        (*this->m_p_vertices_data)[triangle_vertices_bounds.first + 1].position_vec.second = (*this->m_p_vertices_data)[triangle_vertices_bounds.first + 2].position_vec.second;
        (*this->m_p_vertices_data)[triangle_vertices_bounds.first + 2].position_vec.second = (*this->m_p_vertices_data)[triangle_vertices_bounds.first].position_vec.first;
    }

    void Events::checkForClicks(ClickInfo *p_clickinfo) {
        if(this->m_p_window->getInputMode() == DENG_INPUT_NONMOVEMENT) {
            deng::vec2<double> local_mouse_positions;

            glfwGetCursorPos(this->m_p_window->getWindow(), &local_mouse_positions.first, &local_mouse_positions.second);
            if(this->m_p_pixel_collision->isCollided(p_clickinfo->description, {static_cast<size_t>(local_mouse_positions.first), static_cast<size_t>(local_mouse_positions.second)})) {
                int local_mouse_button_status = glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_1);
                
                switch (local_mouse_button_status)
                {
                case GLFW_PRESS: {
                    if(!(*p_clickinfo->p_is_clicked)) {
                        LOG("Clicked on triangle!");
                        *p_clickinfo->p_is_clicked = DENG_TRUE;
                    }
                    break;
                }

                case GLFW_RELEASE:
                    if(*p_clickinfo->p_is_clicked) *p_clickinfo->p_is_clicked = DENG_FALSE;
                    break;
                
                default:
                    break;
                }
            }    
        }
    }
};