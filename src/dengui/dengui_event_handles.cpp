#include "../core/deng_core.h"

namespace dengUI {
    MinimiseHandle::MinimiseHandle(SpriteInfo &triangle_sprite_info, SpriteInfo &window_sprite_info, SpriteInfo &window_border_sprite_info) {
        this->m_triangle_sprite_info = triangle_sprite_info;
        this->m_window_sprite_info = window_sprite_info;
        this->m_window_border_sprite_info = window_border_sprite_info;
        this->m_window_size_mode = DENGUI_WINDOW_SIZE_MODE_MAXIMISED;
        this->verifyVertices();
    }

    void MinimiseHandle::verifyVertices() {
        this->m_verified == DENG_TRUE;

        if(this->m_triangle_sprite_info.vertices_bounds.second - this->m_triangle_sprite_info.vertices_bounds.first != 3) {
            ERRME("Failed to verify dengui minimise triangle vertices in minimise handler!");
            this->m_verified == DENG_FALSE;
        }

        if(this->m_window_sprite_info.vertices_bounds.second - this->m_window_sprite_info.vertices_bounds.first != 4) {
            ERRME("Failed to verify dengui window vertices in minimise handler!");
            this->m_verified == DENG_FALSE;
        }

        if(this->m_window_border_sprite_info.vertices_bounds.second - this->m_window_border_sprite_info.vertices_bounds.first != 8) {
            ERRME("Failed to verify dengui window border vertices in minimise handler!");
            this->m_verified == DENG_FALSE;
        }
    }

    void MinimiseHandle::onClick() {
        if(this->m_verified) {
            deng::vec3<deng::UIVerticesData> local_original_vertices;
            local_original_vertices.first = (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first];
            local_original_vertices.second = (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first + 1];
            local_original_vertices.third = (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first + 2];

            switch (this->m_window_size_mode)
            {
            case DENGUI_WINDOW_SIZE_MODE_MAXIMISED:
                (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first].position_vec.first = local_original_vertices.third.position_vec.first;
                (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first + 1].position_vec.second = local_original_vertices.third.position_vec.second;
                (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first + 2].position_vec.first = local_original_vertices.first.position_vec.first;
                
                this->m_window_sprite_info.p_window_object->is_drawn = DENG_FALSE;
                this->m_window_border_sprite_info.p_window_object->is_drawn = DENG_FALSE;
                this->m_window_size_mode = DENGUI_WINDOW_SIZE_MODE_MINIMIZED;
                break;

            case DENGUI_WINDOW_SIZE_MODE_MINIMIZED:
                (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first].position_vec.first = local_original_vertices.third.position_vec.first;
                (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first + 1].position_vec.second = local_original_vertices.first.position_vec.second;
                (*this->m_triangle_sprite_info.p_vertices_data)[this->m_triangle_sprite_info.vertices_bounds.first + 2].position_vec.first = local_original_vertices.first.position_vec.first;
                
                this->m_window_sprite_info.p_window_object->is_drawn = DENG_TRUE;
                this->m_window_border_sprite_info.p_window_object->is_drawn = DENG_TRUE;
                this->m_window_size_mode = DENGUI_WINDOW_SIZE_MODE_MAXIMISED;
                break;
            
            default:
                break;
            }
        }
    }


}