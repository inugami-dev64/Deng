#include "../../core/deng_core.h"

namespace dengUI {

    void ShapeVerticesDataCreator::createRectangle(WindowInfo *p_windowinfo, RectangleInfo **pp_rectangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const int &sequence_id) {
        if((*pp_rectangle_info)->p_vertices != nullptr && (*pp_rectangle_info)->p_indices != nullptr) {
            LOG("rectangle vertices size: " + std::to_string((*pp_rectangle_info)->p_vertices->size()));
            (*(*pp_rectangle_info)->p_vertices)[0].position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X) / 2);
            (*(*pp_rectangle_info)->p_vertices)[0].position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y) / 2);
            (*(*pp_rectangle_info)->p_vertices)[0].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[0].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            (*(*pp_rectangle_info)->p_vertices)[1].position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X));
            (*(*pp_rectangle_info)->p_vertices)[1].position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y) / 2);
            (*(*pp_rectangle_info)->p_vertices)[1].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[1].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            (*(*pp_rectangle_info)->p_vertices)[2].position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X)); 
            (*(*pp_rectangle_info)->p_vertices)[2].position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.second + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y));
            (*(*pp_rectangle_info)->p_vertices)[2].position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;
            (*(*pp_rectangle_info)->p_vertices)[2].color_vec = (*pp_rectangle_info)->rectangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

            (*(*pp_rectangle_info)->p_vertices)[3].position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X) / 2);
            (*(*pp_rectangle_info)->p_vertices)[3].position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_rectangle_info)->rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y));
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

    void ShapeVerticesDataCreator::createTriangle(WindowInfo *p_windowinfo, TriangleInfo **pp_triangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const int sequence_id) {
        if((*pp_triangle_info)->p_vertices != nullptr && (*pp_triangle_info)->p_indices != nullptr && p_windowinfo != nullptr) {
            dengMath::vec4<dengUtils::UIVerticesData> local_surround_rectangle_vertices;

            local_surround_rectangle_vertices.getVectorElement(0)->position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X) / 2);
            local_surround_rectangle_vertices.getVectorElement(0)->position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y) / 2);
            local_surround_rectangle_vertices.getVectorElement(0)->position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            local_surround_rectangle_vertices.getVectorElement(1)->position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X));
            local_surround_rectangle_vertices.getVectorElement(1)->position_vec.second = p_windowinfo->position.second - ((p_windowinfo->origin.second + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y) / 2);
            local_surround_rectangle_vertices.getVectorElement(1)->position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            local_surround_rectangle_vertices.getVectorElement(2)->position_vec.first = p_windowinfo->position.first + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X)); 
            local_surround_rectangle_vertices.getVectorElement(2)->position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.second + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y));
            local_surround_rectangle_vertices.getVectorElement(2)->position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            local_surround_rectangle_vertices.getVectorElement(3)->position_vec.first = p_windowinfo->position.first - ((p_windowinfo->origin.first + 1) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.first, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X) / 2);
            local_surround_rectangle_vertices.getVectorElement(3)->position_vec.second = p_windowinfo->position.second + ((1 - ((p_windowinfo->origin.first + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize((*pp_triangle_info)->surround_rectangle_size.second, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y));
            local_surround_rectangle_vertices.getVectorElement(3)->position_vec.third = (1/sequence_id) * DENGUI_WINDOW_LAYER_MULTIPLIER;

            dengMath::vec3<dengUtils::UIVerticesData> local_triangle_vertices_data;

            for(size_t i = 0; i < (*pp_triangle_info)->point_collision_with_edge_indices.size(); i++) {
                switch (*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(i))
                {
                case 0: 
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.first = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(0))->position_vec.first + (*(*pp_triangle_info)->triangle_point_edge_rel_coords.getVectorElement(i));
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.second = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(0))->position_vec.second;
                    break;
                
                case 1:
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.first = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(1))->position_vec.first;
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.second = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(1))->position_vec.second + (*(*pp_triangle_info)->triangle_point_edge_rel_coords.getVectorElement(i));
                    break;

                case 2:
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.first = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(3))->position_vec.first + (*(*pp_triangle_info)->triangle_point_edge_rel_coords.getVectorElement(i));
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.second = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(3))->position_vec.second;
                    break;

                case 3:
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.first = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(0))->position_vec.first;
                    (*(*pp_triangle_info)->p_vertices)[i].position_vec.second = local_surround_rectangle_vertices.getVectorElement(*(*pp_triangle_info)->point_collision_with_edge_indices.getVectorElement(0))->position_vec.second + (*(*pp_triangle_info)->triangle_point_edge_rel_coords.getVectorElement(i));
                    break;
                default:
                    break;
                }

                (*(*pp_triangle_info)->p_vertices)[i].color_vec = (*pp_triangle_info)->triangle_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
            }

            switch (border_draw_mode)
            {
            case DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER:
                (*(*pp_triangle_info)->p_vertices)[3].position_vec = (*(*pp_triangle_info)->p_vertices)[0].position_vec;
                (*(*pp_triangle_info)->p_vertices)[3].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_triangle_info)->p_vertices)[4].position_vec = (*(*pp_triangle_info)->p_vertices)[1].position_vec;
                (*(*pp_triangle_info)->p_vertices)[4].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                (*(*pp_triangle_info)->p_vertices)[5].position_vec = (*(*pp_triangle_info)->p_vertices)[2].position_vec;
                (*(*pp_triangle_info)->p_vertices)[5].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});

                (*(*pp_triangle_info)->p_vertices)[6].position_vec.first = (*(*pp_triangle_info)->p_vertices)[0].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X); 
                (*(*pp_triangle_info)->p_vertices)[6].position_vec.second = (*(*pp_triangle_info)->p_vertices)[0].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_triangle_info)->p_vertices)[6].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                
                (*(*pp_triangle_info)->p_vertices)[7].position_vec.first = (*(*pp_triangle_info)->p_vertices)[1].position_vec.first + dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_triangle_info)->p_vertices)[7].position_vec.second = (*(*pp_triangle_info)->p_vertices)[1].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_triangle_info)->p_vertices)[7].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                 
                (*(*pp_triangle_info)->p_vertices)[8].position_vec.first = (*(*pp_triangle_info)->p_vertices)[2].position_vec.first - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_X);
                (*(*pp_triangle_info)->p_vertices)[8].position_vec.first = (*(*pp_triangle_info)->p_vertices)[2].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_border_info->p_css_general_border_info->border_width, p_windowinfo->p_window->getSize(), DENG_COORD_AXIS_Y);
                (*(*pp_triangle_info)->p_vertices)[8].color_vec = p_border_info->p_css_general_border_info->border_color >> dengMath::vec4<float>({0.0f, 0.0f, 0.0f, 1.0f});
                break;
            
            default:
                break;
            }
        }
    }
}