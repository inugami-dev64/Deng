#include "headers/renderer.h"

namespace deng {
    GridManager::GridManager(const float &max_distance, const float &grid_height, const float &grid_width, const float &color_r, const float &color_g, const float &color_b, const float &color_a) {
        this->m_max_vertex_buffer_distance = max_distance;
        this->m_grid_height = grid_height;
        this->m_grid_width = grid_width;
        this->m_grid_color_r = color_r;
        this->m_grid_color_g = color_g;
        this->m_grid_color_b = color_b;
        this->m_grid_color_a = color_a;
    }

    /* vertices only need to be updated when the camera has moved */
    void GridManager::generateVertices(vec3<float> camera_position) {
        int32_t local_min_vertices_x = static_cast<int32_t>(-this->m_max_vertex_buffer_distance / this->m_grid_width);
        int32_t local_min_vertices_z = static_cast<int32_t>(-this->m_max_vertex_buffer_distance / this->m_grid_height);

        int32_t local_max_vertices_x = static_cast<int32_t>(this->m_max_vertex_buffer_distance / this->m_grid_width);
        int32_t local_max_vertices_z = static_cast<int32_t>(this->m_max_vertex_buffer_distance / this->m_grid_height);
        this->m_vertices.resize((abs(local_min_vertices_x) + abs(local_max_vertices_x)) * (abs(local_min_vertices_z) + abs(local_max_vertices_z)));

        for(int32_t zI = local_min_vertices_z, vert_index = 0; zI <= local_max_vertices_z; zI += m_grid_height) {
            for(int32_t xI = local_min_vertices_x; xI <= local_max_vertices_x; xI += m_grid_width, vert_index++) {
                this->m_vertices[vert_index] = {{static_cast<float>(xI) * m_grid_width, 0.0f, static_cast<float>(zI) * m_grid_height}, {this->m_grid_color_r, this->m_grid_color_g, this->m_grid_color_b, this->m_grid_color_a}}; 
            }
        }
    }

    void GridManager::updateVertices(vec3<float> camera_position) {
        this->m_vertices.clear();
        this->generateVertices(camera_position);
    }
}