#include "../../core/deng_core.h"

namespace dengUtils {
    GridManager::GridManager(dengUtils::SpecifiedObject *grid, const float &max_distance, float *p_grid_lenght, float *p_grid_width, float *p_color_r, float *p_color_g, float *p_color_b) {
        this->m_grid = grid;
        this->m_max_vertex_buffer_distance = max_distance;
        this->m_p_grid_lenght = p_grid_lenght;
        this->m_p_grid_width = p_grid_width;
        this->m_p_grid_color_r = p_color_r;
        this->m_p_grid_color_g = p_color_g;
        this->m_p_grid_color_b = p_color_b;

        this->m_grid->p_index_data = new std::vector<uint32_t>;
    }

    GridManager::~GridManager() {
        delete this->m_grid->p_index_data;
    }

    dengMath::vec2<float*> GridManager::getOrdereredVertexDataElements(const uint32_t index, const dengCoordinateAxisType &axis) {
        dengMath::vec2<float*> local_elements;
        switch (axis)
        {
        case DENG_COORD_AXIS_X:
            local_elements.first = &this->m_grid->vertex_data[index].position_vec.third;
            local_elements.second = &this->m_grid->vertex_data[index].position_vec.first;
            break;

        case DENG_COORD_AXIS_Z:
            local_elements.first = &this->m_grid->vertex_data[index].position_vec.first;
            local_elements.second = &this->m_grid->vertex_data[index].position_vec.third;
            break;
        
        default:
            break;
        }

        return local_elements;
    }

    void GridManager::generateAxisLineVertices(dengMath::vec2<int32_t> &min_vertices, dengMath::vec2<int32_t> &max_vertices, const dengCoordinateAxisType &axis, int32_t &index) {
        // first is min and second is max
        dengMath::vec2<int32_t*> local_vertices;

        float *local_grid_dimention_size;

        switch (axis)
        {
        case DENG_COORD_AXIS_X:
            local_vertices.first = &min_vertices.first;
            local_vertices.second = &max_vertices.first;
            local_grid_dimention_size = this->m_p_grid_width;
            break;

        case DENG_COORD_AXIS_Z:
            local_vertices.first = &min_vertices.second;
            local_vertices.second = &max_vertices.second;
            local_grid_dimention_size = this->m_p_grid_lenght;
            break;
        
        default:
            break;
        }

        for(int32_t line_index = (*local_vertices.first); line_index < (*local_vertices.second); line_index++) {
            for(int32_t i = 0; i < 3; i++, index++) {
                switch (i)
                {
                case 0:
                    if(axis == DENG_COORD_AXIS_Z) {
                        *this->getOrdereredVertexDataElements(index, axis).first = *local_vertices.first;
                        this->m_grid->vertex_data[index].position_vec.second = 0.0f;
                        *this->getOrdereredVertexDataElements(index, axis).second = line_index * (*local_grid_dimention_size);
                    }

                    else if(axis == DENG_COORD_AXIS_X) {
                        *this->getOrdereredVertexDataElements(index, axis).first = *local_vertices.second;
                        this->m_grid->vertex_data[index].position_vec.second = 0.0f;
                        *this->getOrdereredVertexDataElements(index, axis).second = line_index * (*local_grid_dimention_size);
                    }
                    break;

                case 1:
                    if(axis == DENG_COORD_AXIS_Z) {
                        *this->getOrdereredVertexDataElements(index, axis).first = *local_vertices.second;
                        this->m_grid->vertex_data[index].position_vec.second = 0.0f;
                        *this->getOrdereredVertexDataElements(index, axis).second = line_index * (*local_grid_dimention_size);
                    }
                    
                    else if(axis == DENG_COORD_AXIS_X) {
                        *this->getOrdereredVertexDataElements(index, axis).first = *local_vertices.first;
                        this->m_grid->vertex_data[index].position_vec.second = 0.0f;
                        *this->getOrdereredVertexDataElements(index, axis).second = line_index * (*local_grid_dimention_size);
                    }
                    break;

                case 2:
                    *this->getOrdereredVertexDataElements(index, axis).first = *local_vertices.first;
                    this->m_grid->vertex_data[index].position_vec.second = 0.0f;
                    *this->getOrdereredVertexDataElements(index, axis).second = line_index * (*local_grid_dimention_size) + DENG_GRID_TRIANGLE_THICKNESS;
                    break;

                default:
                    break;
                }

                this->m_grid->vertex_data[index].color_vec = {*this->m_p_grid_color_r, *this->m_p_grid_color_g, *this->m_p_grid_color_b};
            }
        }
    }

    /* vertices only need to be updated when the camera has moved */
    void GridManager::generateVertices(dengMath::vec4<float> camera_position) {
        // first represents x-axis and second represents z-axis
        dengMath::vec2<int32_t> local_min_vertices; 
        local_min_vertices.first = static_cast<int32_t>(-this->m_max_vertex_buffer_distance / *this->m_p_grid_width);
        local_min_vertices.second = static_cast<int32_t>(-this->m_max_vertex_buffer_distance / *this->m_p_grid_lenght);

        dengMath::vec2<int32_t> local_max_vertices;
        local_max_vertices.first = static_cast<int32_t>(this->m_max_vertex_buffer_distance / *this->m_p_grid_width);
        local_max_vertices.second = static_cast<int32_t>(this->m_max_vertex_buffer_distance / *this->m_p_grid_lenght);
        this->m_grid->vertex_data.resize(((abs(local_min_vertices.first) + abs(local_max_vertices.first)) * 3) + ((abs(local_min_vertices.second) + abs(local_max_vertices.second)) * 3) );
        
        int32_t index = 0;
        this->generateAxisLineVertices(local_min_vertices, local_max_vertices, DENG_COORD_AXIS_Z, index);
        this->generateAxisLineVertices(local_min_vertices, local_max_vertices, DENG_COORD_AXIS_X, index);
    }
}