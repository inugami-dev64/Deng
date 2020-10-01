#include "../../core/deng_core.h"

namespace dengUI {

    void ShapeVerticesDataCreator::getRectangleVertices(RectangleInfo *p_rectangle_info, WindowObject *p_window_object) {
        if(p_rectangle_info != nullptr && p_window_object != nullptr) {
            size_t local_vertices_size = p_rectangle_info->p_vertices_data->size();
            size_t local_indices_size = p_rectangle_info->p_indices_data->size();
            p_window_object->vertices_bounds.first = local_vertices_size;
            p_window_object->indices_bounds.first = local_indices_size;

            std::vector<uint16_t> local_new_indices;

            switch (p_rectangle_info->enable_fill)
            {
            case DENG_TRUE: {
                p_rectangle_info->p_vertices_data->resize(local_vertices_size + 4);
                
                (*p_rectangle_info->p_vertices_data)[local_vertices_size].position_vec.first = p_rectangle_info->p_vertices->first.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size].position_vec.second = p_rectangle_info->p_vertices->first.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 1].position_vec.first = p_rectangle_info->p_vertices->second.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 1].position_vec.second = p_rectangle_info->p_vertices->second.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 1].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 2].position_vec.first = p_rectangle_info->p_vertices->third.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 2].position_vec.second = p_rectangle_info->p_vertices->third.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 2].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 3].position_vec.first = p_rectangle_info->p_vertices->fourth.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 3].position_vec.second = p_rectangle_info->p_vertices->fourth.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 3].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                local_new_indices = {static_cast<uint16_t>(local_vertices_size), static_cast<uint16_t>(local_vertices_size + 1), static_cast<uint16_t>(local_vertices_size + 2),
                                     static_cast<uint16_t>(local_vertices_size + 2), static_cast<uint16_t>(local_vertices_size + 3), static_cast<uint16_t>(local_vertices_size)};
                break;
            }

            case DENG_FALSE: {
                p_rectangle_info->p_vertices_data->resize(local_vertices_size + 8);

                (*p_rectangle_info->p_vertices_data)[local_vertices_size].position_vec.first = p_rectangle_info->p_vertices->first.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size].position_vec.second = p_rectangle_info->p_vertices->first.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 1].position_vec.first = p_rectangle_info->p_vertices->second.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 1].position_vec.second = p_rectangle_info->p_vertices->second.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 1].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 2].position_vec.first = p_rectangle_info->p_vertices->third.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 2].position_vec.second = p_rectangle_info->p_vertices->third.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 2].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 3].position_vec.first = p_rectangle_info->p_vertices->fourth.first;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 3].position_vec.second = p_rectangle_info->p_vertices->fourth.second;
                (*p_rectangle_info->p_vertices_data)[local_vertices_size + 3].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

                float local_distance_from_corner_point = (p_rectangle_info->border_thickness / sin(dengMath::Math::degToRad(45)));
                dengMath::vec2<float> local_positions;

                switch (p_rectangle_info->border_offset_mode)
                {
                case DENGUI_NON_FILLED_BORDER_OFFSET_MODE_IN: {
                    for(size_t i = 4, angle = 135; i < 8; i++, angle += 90) {
                        local_positions = dengMath::Math::getCartesianCoordsPoint({(*p_rectangle_info->p_vertices_data)[local_vertices_size + (i - 4)].position_vec.first, (*p_rectangle_info->p_vertices_data)[local_vertices_size + (i - 4)].position_vec.second}, angle, local_distance_from_corner_point, DENG_TRUE);
                        (*p_rectangle_info->p_vertices_data)[local_vertices_size + i].position_vec.first = local_positions.first;
                        (*p_rectangle_info->p_vertices_data)[local_vertices_size + i].position_vec.second = local_positions.second;
                        (*p_rectangle_info->p_vertices_data)[local_vertices_size + i].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                    }
                    break;
                }

                case DENGUI_NON_FILLED_BORDER_OFFSET_MODE_OUT: {
                    for(size_t i = 4, angle = 315; i < 8; i++, angle += 90) {
                        local_positions = dengMath::Math::getCartesianCoordsPoint({(*p_rectangle_info->p_vertices_data)[local_vertices_size + (i - 4)].position_vec.first, (*p_rectangle_info->p_vertices_data)[local_vertices_size + (i - 4)].position_vec.second}, angle, local_distance_from_corner_point, DENG_TRUE);
                        (*p_rectangle_info->p_vertices_data)[local_vertices_size + i].position_vec.first = local_positions.first;
                        (*p_rectangle_info->p_vertices_data)[local_vertices_size + i].position_vec.second = local_positions.second;
                        (*p_rectangle_info->p_vertices_data)[local_vertices_size + i].position_vec.third = (1/p_rectangle_info->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                    }
                }
                default:
                    break;
                }

                local_new_indices = {static_cast<uint16_t>(local_vertices_size), static_cast<uint16_t>(local_vertices_size + 1), static_cast<uint16_t>(local_vertices_size + 5), 
                                     static_cast<uint16_t>(local_vertices_size + 5), static_cast<uint16_t>(local_vertices_size + 4), static_cast<uint16_t>(local_vertices_size),
                                     static_cast<uint16_t>(local_vertices_size), static_cast<uint16_t>(local_vertices_size + 4), static_cast<uint16_t>(local_vertices_size + 7),
                                     static_cast<uint16_t>(local_vertices_size + 7), static_cast<uint16_t>(local_vertices_size + 3), static_cast<uint16_t>(local_vertices_size),
                                     static_cast<uint16_t>(local_vertices_size + 7), static_cast<uint16_t>(local_vertices_size + 6), static_cast<uint16_t>(local_vertices_size + 2),
                                     static_cast<uint16_t>(local_vertices_size + 2), static_cast<uint16_t>(local_vertices_size + 3), static_cast<uint16_t>(local_vertices_size + 7),
                                     static_cast<uint16_t>(local_vertices_size + 5), static_cast<uint16_t>(local_vertices_size + 1), static_cast<uint16_t>(local_vertices_size + 2), 
                                     static_cast<uint16_t>(local_vertices_size + 2), static_cast<uint16_t>(local_vertices_size + 6), static_cast<uint16_t>(local_vertices_size + 5)};
            }
            
            default:
                break;
            }

            p_window_object->vertices_bounds.second = p_rectangle_info->p_vertices_data->size();
            p_rectangle_info->p_indices_data->insert(p_rectangle_info->p_indices_data->end(), local_new_indices.begin(), local_new_indices.end());
            p_window_object->indices_bounds.second = p_rectangle_info->p_indices_data->size();
            p_window_object->sequence_id = p_rectangle_info->sequence_id;

            for(size_t i = local_vertices_size; i < p_rectangle_info->p_vertices_data->size(); i++) 
                (*p_rectangle_info->p_vertices_data)[i].color_vec = *p_rectangle_info->p_color;
        }
    }

    void ShapeVerticesDataCreator::getTriangleVertices(TriangleInfo *p_triangleinfo, WindowObject *p_window_object) {
        
        if(p_triangleinfo != nullptr && p_window_object != nullptr) {
            size_t local_vertices_size = p_triangleinfo->p_vertices_data->size();
            size_t local_indices_size = p_triangleinfo->p_indices_data->size();
            p_window_object->vertices_bounds.first = local_vertices_size;
            p_window_object->indices_bounds.first = local_indices_size;

            p_triangleinfo->p_vertices_data->resize(local_vertices_size + 3);
            p_triangleinfo->p_indices_data->resize(local_indices_size + 3);

            p_window_object->vertices_bounds.second = p_triangleinfo->p_vertices_data->size();
            p_window_object->indices_bounds.second = p_triangleinfo->p_indices_data->size();
            p_window_object->sequence_id = p_triangleinfo->sequence_id;

            (*p_triangleinfo->p_vertices_data)[local_vertices_size].position_vec.first = p_triangleinfo->p_vertices->first.first;
            (*p_triangleinfo->p_vertices_data)[local_vertices_size].position_vec.second = p_triangleinfo->p_vertices->first.second;
            (*p_triangleinfo->p_vertices_data)[local_vertices_size].position_vec.third = (1/p_triangleinfo->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            (*p_triangleinfo->p_vertices_data)[local_vertices_size + 1].position_vec.first = p_triangleinfo->p_vertices->second.first;
            (*p_triangleinfo->p_vertices_data)[local_vertices_size + 1].position_vec.second = p_triangleinfo->p_vertices->second.second;
            (*p_triangleinfo->p_vertices_data)[local_vertices_size + 1].position_vec.third = (1/p_triangleinfo->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            (*p_triangleinfo->p_vertices_data)[local_vertices_size + 2].position_vec.first = p_triangleinfo->p_vertices->third.first;
            (*p_triangleinfo->p_vertices_data)[local_vertices_size + 2].position_vec.second = p_triangleinfo->p_vertices->third.second;
            (*p_triangleinfo->p_vertices_data)[local_vertices_size + 2].position_vec.third = (1/p_triangleinfo->sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            (*p_triangleinfo->p_indices_data)[local_indices_size] = static_cast<uint16_t>(local_vertices_size);
            (*p_triangleinfo->p_indices_data)[local_indices_size + 1] = static_cast<uint16_t>(local_vertices_size + 1);
            (*p_triangleinfo->p_indices_data)[local_indices_size + 2] = static_cast<uint16_t>(local_vertices_size + 2);

            for(size_t i = local_vertices_size; i < p_triangleinfo->p_vertices_data->size(); i++) 
                (*p_triangleinfo->p_vertices_data)[i].color_vec = *p_triangleinfo->p_color;
        }


    }
}