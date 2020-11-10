#include "../../core/deng_core.h"

namespace dengUI {

    void ShapeVerticesDataCreator::createRectangle(WindowInfo *p_windowinfo, RectangleInfo **pp_rectangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const dengUISizeUnit &size_unit, const int &sequence_id) {
        if((*pp_rectangle_info)->p_vertices != nullptr && (*pp_rectangle_info)->p_indices != nullptr) {
            dengMath::vec2<float> local_rectangle_size;
            switch (size_unit)
            {
            case DENGUI_SIZE_UNIT_VECTOR_UNITS:
                local_rectangle_size.first = (*pp_rectangle_info)->rectangle_size.first;
                local_rectangle_size.second = (*pp_rectangle_info)->rectangle_size.second; 
                break;

            case DENGUI_SIZE_UNIT_PIXELS:
                local_rectangle_size.first = dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                local_rectangle_size.second = dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                break;
            
            default:
                break;
            }
            LOG("rectangle vertices size: " + std::to_string((*pp_rectangle_info)->p_vertices->size()));
            (*(*pp_rectangle_info)->p_vertices)[0].position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * local_rectangle_size.first / 2);
            (*(*pp_rectangle_info)->p_vertices)[0].position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * local_rectangle_size.second / 2);
            (*(*pp_rectangle_info)->p_vertices)[0].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[0].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            (*(*pp_rectangle_info)->p_vertices)[1].position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * local_rectangle_size.first);
            (*(*pp_rectangle_info)->p_vertices)[1].position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * local_rectangle_size.second / 2);
            (*(*pp_rectangle_info)->p_vertices)[1].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[1].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            (*(*pp_rectangle_info)->p_vertices)[2].position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * local_rectangle_size.first); 
            (*(*pp_rectangle_info)->p_vertices)[2].position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.second + 1) / 2)) * local_rectangle_size.second);
            (*(*pp_rectangle_info)->p_vertices)[2].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[2].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            (*(*pp_rectangle_info)->p_vertices)[3].position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * local_rectangle_size.first / 2);
            (*(*pp_rectangle_info)->p_vertices)[3].position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * local_rectangle_size.second);
            (*(*pp_rectangle_info)->p_vertices)[3].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[3].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            *(*pp_rectangle_info)->p_indices = {0, 1, 2, 2, 3, 0};
            
            std::vector<uint16_t> local_new_indices;
            switch (border_draw_mode)
            {
            case DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER:
                (*(*pp_rectangle_info)->p_vertices)[4].position_vec = (*(*pp_rectangle_info)->p_vertices)[0].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[4].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_rectangle_info)->p_vertices)[5].position_vec = (*(*pp_rectangle_info)->p_vertices)[1].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[5].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_rectangle_info)->p_vertices)[6].position_vec = (*(*pp_rectangle_info)->p_vertices)[2].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[6].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_rectangle_info)->p_vertices)[7].position_vec = (*(*pp_rectangle_info)->p_vertices)[3].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[7].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[8].position_vec.first = (*(*pp_rectangle_info)->p_vertices)[0].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[8].position_vec.second = (*(*pp_rectangle_info)->p_vertices)[0].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices)[8].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[8].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[9].position_vec.first = (*(*pp_rectangle_info)->p_vertices)[1].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[9].position_vec.second = (*(*pp_rectangle_info)->p_vertices)[1].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices)[9].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[9].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[10].position_vec.first = (*(*pp_rectangle_info)->p_vertices)[2].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[10].position_vec.second = (*(*pp_rectangle_info)->p_vertices)[2].position_vec.second + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices)[10].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[10].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[11].position_vec.first = (*(*pp_rectangle_info)->p_vertices)[3].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[11].position_vec.second = (*(*pp_rectangle_info)->p_vertices)[3].position_vec.second + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices)[11].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[11].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                
                local_new_indices = {static_cast<uint16_t>(8), static_cast<uint16_t>(9), static_cast<uint16_t>(5), 
                                     static_cast<uint16_t>(5), static_cast<uint16_t>(4), static_cast<uint16_t>(8),
                                     static_cast<uint16_t>(5), static_cast<uint16_t>(9), static_cast<uint16_t>(10),
                                     static_cast<uint16_t>(10), static_cast<uint16_t>(6), static_cast<uint16_t>(5),
                                     static_cast<uint16_t>(7), static_cast<uint16_t>(6), static_cast<uint16_t>(10),
                                     static_cast<uint16_t>(10), static_cast<uint16_t>(11), static_cast<uint16_t>(7),
                                     static_cast<uint16_t>(8), static_cast<uint16_t>(4), static_cast<uint16_t>(7), 
                                     static_cast<uint16_t>(7), static_cast<uint16_t>(11), static_cast<uint16_t>(8)};
                break;

            case DENGUI_BORDER_DRAW_MODE_SPECIFIED_BORDER:
                // top border
                (*(*pp_rectangle_info)->p_vertices )[4].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_left_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices )[4].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_top_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices )[4].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices )[4].color_vec = p_border_info->p_css_specified_border_info->border_top_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices )[5].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[1].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_right_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices )[5].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[1].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_top_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices )[5].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                ( *(*pp_rectangle_info)->p_vertices)[5].color_vec = p_border_info->p_css_specified_border_info->border_top_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                ( *(*pp_rectangle_info)->p_vertices)[6].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[1].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_right_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                ( *(*pp_rectangle_info)->p_vertices)[6].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[1].position_vec.second;
                ( *(*pp_rectangle_info)->p_vertices)[6].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                ( *(*pp_rectangle_info)->p_vertices)[6].color_vec = p_border_info->p_css_specified_border_info->border_top_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                ( *(*pp_rectangle_info)->p_vertices)[7].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_left_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                ( *(*pp_rectangle_info)->p_vertices)[7].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec.second;
                ( *(*pp_rectangle_info)->p_vertices)[7].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                ( *(*pp_rectangle_info)->p_vertices)[7].color_vec = p_border_info->p_css_specified_border_info->border_top_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                // right border
                (*(*pp_rectangle_info)->p_vertices)[8].position_vec = (*(*pp_rectangle_info)->p_vertices )[1].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[8].color_vec = p_border_info->p_css_specified_border_info->border_right_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 2.0f});

                (*(*pp_rectangle_info)->p_vertices)[9].position_vec.first = (*(*pp_rectangle_info)->p_vertices )[1].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_right_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[9].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[1].position_vec.second;
                (*(*pp_rectangle_info)->p_vertices)[9].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices )[9].color_vec = p_border_info->p_css_specified_border_info->border_right_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 2.0f});
                
                (*(*pp_rectangle_info)->p_vertices)[10].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_right_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[10].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec.second;
                (*(*pp_rectangle_info)->p_vertices)[10].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[10].color_vec = p_border_info->p_css_specified_border_info->border_right_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[11].position_vec = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[11].color_vec = p_border_info->p_css_specified_border_info->border_right_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                //bottom border
                (*(*pp_rectangle_info)->p_vertices)[12].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_left_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[12].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec.second;
                (*(*pp_rectangle_info)->p_vertices)[12].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[12].color_vec = p_border_info->p_css_specified_border_info->border_bottom_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[13].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_right_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[13].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec.second;
                (*(*pp_rectangle_info)->p_vertices)[13].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[13].color_vec = p_border_info->p_css_specified_border_info->border_bottom_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[14].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_right_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[14].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[2].position_vec.second + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_bottom_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices)[14].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[14].color_vec = p_border_info->p_css_specified_border_info->border_bottom_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[15].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_left_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[15].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec.second + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_bottom_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_rectangle_info)->p_vertices)[15].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[15].color_vec = p_border_info->p_css_specified_border_info->border_bottom_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                //left border 
                (*(*pp_rectangle_info)->p_vertices)[16].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_left_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[16].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec.second;
                (*(*pp_rectangle_info)->p_vertices)[16].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[16].color_vec = p_border_info->p_css_specified_border_info->border_left_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                
                (*(*pp_rectangle_info)->p_vertices)[17].position_vec = ( *(*pp_rectangle_info)->p_vertices)[0].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[17].color_vec = p_border_info->p_css_specified_border_info->border_left_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                
                (*(*pp_rectangle_info)->p_vertices)[18].position_vec = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec;
                (*(*pp_rectangle_info)->p_vertices)[18].color_vec = p_border_info->p_css_specified_border_info->border_left_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_rectangle_info)->p_vertices)[19].position_vec.first = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_specified_border_info->border_left_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_rectangle_info)->p_vertices)[19].position_vec.second = ( *(*pp_rectangle_info)->p_vertices)[3].position_vec.second;
                (*(*pp_rectangle_info)->p_vertices)[19].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_rectangle_info)->p_vertices)[19].color_vec = p_border_info->p_css_specified_border_info->border_left_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                local_new_indices = {static_cast<uint16_t>(4), static_cast<uint16_t>(5), static_cast<uint16_t>(6), 
                                     static_cast<uint16_t>(6), static_cast<uint16_t>(7), static_cast<uint16_t>(4),
                                     static_cast<uint16_t>(8), static_cast<uint16_t>(9), static_cast<uint16_t>(10),
                                     static_cast<uint16_t>(10), static_cast<uint16_t>(11), static_cast<uint16_t>(8),
                                     static_cast<uint16_t>(12), static_cast<uint16_t>(13), static_cast<uint16_t>(14),
                                     static_cast<uint16_t>(14), static_cast<uint16_t>(15), static_cast<uint16_t>(12),
                                     static_cast<uint16_t>(16), static_cast<uint16_t>(17), static_cast<uint16_t>(18), 
                                     static_cast<uint16_t>(18), static_cast<uint16_t>(19), static_cast<uint16_t>(16)};
                break;

            case DENGUI_BORDER_DRAW_MODE_NO_BORDER:
                return;
                break;

            default:
                break;
            }

            (*pp_rectangle_info)->p_indices->insert((*pp_rectangle_info)->p_indices->end(), local_new_indices.begin(), local_new_indices.end());
        }
    }

    void ShapeVerticesDataCreator::createTriangle(WindowInfo *p_windowinfo, TriangleInfo **pp_triangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const dengUISizeUnit &size_unit, const int sequence_id) {
        if((*pp_triangle_info)->p_vertices != nullptr && (*pp_triangle_info)->p_indices != nullptr && p_windowinfo != nullptr) {
            dengMath::vec2<float> local_rectangle_size;
            switch (size_unit)
            {
            case DENGUI_SIZE_UNIT_VECTOR_UNITS:
                local_rectangle_size.first = (*pp_triangle_info)->surround_rectangle_size.first;
                local_rectangle_size.second = (*pp_triangle_info)->surround_rectangle_size.second;
                break;

            case DENGUI_SIZE_UNIT_PIXELS:
                local_rectangle_size.first = dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                local_rectangle_size.second = dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                break;
            
            default:
                break;
            }
            size_t index, vertices_rel_index;
            dengMath::vec4<dengUtils::UIVerticesData> local_surround_rectangle_vertices;

            local_surround_rectangle_vertices.first.position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * local_rectangle_size.first / 2) + dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->margin_data.margin_left, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
            local_surround_rectangle_vertices.first.position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * local_rectangle_size.second / 2) + dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->margin_data.margin_top, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);

            local_surround_rectangle_vertices.second.position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * local_rectangle_size.first);
            local_surround_rectangle_vertices.second.position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * local_rectangle_size.second / 2) + dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->margin_data.margin_top, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);

            local_surround_rectangle_vertices.third.position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * local_rectangle_size.first); 
            local_surround_rectangle_vertices.third.position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.second + 1) / 2)) * local_rectangle_size.second) - dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->margin_data.margin_bottom, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);

            local_surround_rectangle_vertices.fourth.position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * local_rectangle_size.first / 2) + dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->margin_data.margin_left, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
            local_surround_rectangle_vertices.fourth.position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * local_rectangle_size.second) - dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->margin_data.margin_bottom, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);

            for(index = 0; index < (*pp_triangle_info)->vertices_relativity_infos.size(); index++) {
                vertices_rel_index = (*pp_triangle_info)->vertices_relativity_infos.getVectorElement(index)->rectangle_vertex_index;

                switch ((*pp_triangle_info)->vertices_relativity_infos.getVectorElement(index)->rectangle_axis_type)
                {
                case DENG_COORD_AXIS_X:
                    (*(*pp_triangle_info)->p_vertices)[index].position_vec.first = local_surround_rectangle_vertices.getVectorElement(vertices_rel_index)->position_vec.first + (((*pp_triangle_info)->vertices_relativity_infos.getVectorElement(index)->rectangle_vertex_distance + 1) / 2 * (*pp_triangle_info)->surround_rectangle_size.first);
                    (*(*pp_triangle_info)->p_vertices)[index].position_vec.second = local_surround_rectangle_vertices.getVectorElement(vertices_rel_index)->position_vec.second;
                    break;
                
                case DENG_COORD_AXIS_Y:
                    (*(*pp_triangle_info)->p_vertices)[index].position_vec.first = local_surround_rectangle_vertices.getVectorElement(vertices_rel_index)->position_vec.first;
                    (*(*pp_triangle_info)->p_vertices)[index].position_vec.second = local_surround_rectangle_vertices.getVectorElement(vertices_rel_index)->position_vec.second + (((*pp_triangle_info)->vertices_relativity_infos.getVectorElement(index)->rectangle_vertex_distance + 1) / 2 * (*pp_triangle_info)->surround_rectangle_size.second);
                    break;
                
                default:
                    break;
                }

                (*(*pp_triangle_info)->p_vertices)[index].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                (*(*pp_triangle_info)->p_vertices)[index].color_vec = (*pp_triangle_info)->triangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                LOG("Minimise triangle vertices are: {" + std::to_string((*(*pp_triangle_info)->p_vertices)[index].position_vec.first) + ";" + std::to_string((*(*pp_triangle_info)->p_vertices)[index].position_vec.second) + ";" + std::to_string((*(*pp_triangle_info)->p_vertices)[index].position_vec.third) + "}");
            }

            (*(*pp_triangle_info)->p_indices) = {0, 1, 2};

            switch (border_draw_mode)
            {
            case DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER: {
                std::vector<uint16_t> local_border_indices;
                (*(*pp_triangle_info)->p_vertices)[3].position_vec = (*(*pp_triangle_info)->p_vertices)[0].position_vec;
                (*(*pp_triangle_info)->p_vertices)[3].position_vec = (*(*pp_triangle_info)->p_vertices)[0].position_vec;
                (*(*pp_triangle_info)->p_vertices)[3].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_triangle_info)->p_vertices)[4].position_vec = (*(*pp_triangle_info)->p_vertices)[1].position_vec;
                (*(*pp_triangle_info)->p_vertices)[4].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_triangle_info)->p_vertices)[5].position_vec = (*(*pp_triangle_info)->p_vertices)[2].position_vec;
                (*(*pp_triangle_info)->p_vertices)[5].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                std::array<TriangleBorderCalculationData, 3> local_triangle_border_calc_info;
                size_t index, l_index1, l_index2;

                for(index = 0; index < local_triangle_border_calc_info.size(); index++) {
                    switch (index)
                    {
                    case 0:
                        l_index1 = 1, l_index2 = 2;
                        break;
                    
                    case 1:
                        l_index1 = 0, l_index2 = 2;
                        break;

                    case 2:
                        l_index1 = 0, l_index2 = 1;
                        break;
                    
                    default:
                        break;
                    }
                    
                    local_triangle_border_calc_info[index].vector_dimentions.first.first = (*(*pp_triangle_info)->p_vertices)[l_index1].position_vec.first - (*(*pp_triangle_info)->p_vertices)[index].position_vec.first;
                    local_triangle_border_calc_info[index].vector_dimentions.first.second = (*(*pp_triangle_info)->p_vertices)[l_index1].position_vec.second - (*(*pp_triangle_info)->p_vertices)[index].position_vec.second;
                    local_triangle_border_calc_info[index].vector_length.first = static_cast<float>(sqrt(static_cast<double>(dengMath::exp(local_triangle_border_calc_info[index].vector_dimentions.first.first, 2) + dengMath::exp(local_triangle_border_calc_info[index].vector_dimentions.first.second, 2))));

                    local_triangle_border_calc_info[index].vector_dimentions.second.first = (*(*pp_triangle_info)->p_vertices)[l_index2].position_vec.first - (*(*pp_triangle_info)->p_vertices)[index].position_vec.first;
                    local_triangle_border_calc_info[index].vector_dimentions.second.second = (*(*pp_triangle_info)->p_vertices)[l_index2].position_vec.second - (*(*pp_triangle_info)->p_vertices)[index].position_vec.second;
                    local_triangle_border_calc_info[index].vector_length.second = static_cast<float>(sqrt(static_cast<double>(dengMath::exp(local_triangle_border_calc_info[index].vector_dimentions.second.first, 2) + dengMath::exp(local_triangle_border_calc_info[index].vector_dimentions.second.second, 2))));
                
                    local_triangle_border_calc_info[index].border_width_vectors.first.first = (local_triangle_border_calc_info[index].vector_dimentions.first.first / local_triangle_border_calc_info[index].vector_length.first) * dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                    local_triangle_border_calc_info[index].border_width_vectors.first.second = (local_triangle_border_calc_info[index].vector_dimentions.first.second / local_triangle_border_calc_info[index].vector_length.first) * dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                    local_triangle_border_calc_info[index].border_width_vectors.second.first = (local_triangle_border_calc_info[index].vector_dimentions.second.first / local_triangle_border_calc_info[index].vector_length.second) * dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                    local_triangle_border_calc_info[index].border_width_vectors.second.second = (local_triangle_border_calc_info[index].vector_dimentions.second.second / local_triangle_border_calc_info[index].vector_length.second) * dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                    
                    (*(*pp_triangle_info)->p_vertices)[index + 6].position_vec.first = -(local_triangle_border_calc_info[index].border_width_vectors.first.first + local_triangle_border_calc_info[index].border_width_vectors.second.first) + (*(*pp_triangle_info)->p_vertices)[index].position_vec.first; 
                    (*(*pp_triangle_info)->p_vertices)[index + 6].position_vec.second = -(local_triangle_border_calc_info[index].border_width_vectors.first.second + local_triangle_border_calc_info[index].border_width_vectors.second.second) + + (*(*pp_triangle_info)->p_vertices)[index].position_vec.second;
                    (*(*pp_triangle_info)->p_vertices)[index + 6].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
                    (*(*pp_triangle_info)->p_vertices)[index + 6].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                    LOG("Triangle border vertices are: {" + std::to_string((*(*pp_triangle_info)->p_vertices)[index + 6].position_vec.first) + ";" + std::to_string((*(*pp_triangle_info)->p_vertices)[index + 6].position_vec.second) + "}");
                }
                
                local_border_indices = {static_cast<uint16_t>(6), static_cast<uint16_t>(7), static_cast<uint16_t>(3),
                                        static_cast<uint16_t>(7), static_cast<uint16_t>(4), static_cast<uint16_t>(3),
                                        static_cast<uint16_t>(4), static_cast<uint16_t>(7), static_cast<uint16_t>(8),
                                        static_cast<uint16_t>(8), static_cast<uint16_t>(5), static_cast<uint16_t>(4),
                                        static_cast<uint16_t>(6), static_cast<uint16_t>(3), static_cast<uint16_t>(8),
                                        static_cast<uint16_t>(8), static_cast<uint16_t>(3), static_cast<uint16_t>(5)};

                (*pp_triangle_info)->p_indices->insert((*pp_triangle_info)->p_indices->end(), local_border_indices.begin(), local_border_indices.end());
                break;
            }
            
            default:
                break;
            }
        }
    }
}