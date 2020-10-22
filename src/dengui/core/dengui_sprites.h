#ifndef DENGUI_SPRITES_H
#define DENGUI_SPRITES_H

namespace dengUI {

    struct TriangleBorderCalculationData {
        dengMath::vec2<float> vector_length;
        dengMath::vec2<dengMath::vec2<float>> vector_dimentions;
        dengMath::vec2<dengMath::vec2<float>> border_width_vectors; 
    };

    struct ShapeVerticesDataCreator {
        static void createRectangle(WindowInfo *p_windowinfo, RectangleInfo **pp_rectangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const dengUISizeUnit &size_unit, const int &sequence_id);
        static void createTriangle(WindowInfo *p_windowsinfo, TriangleInfo **pp_triangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const dengUISizeUnit &size_unit, const int sequence_id);
        // static void createCircle(WindowInfo *p_windowinfo, CircleInfo **pp_circle_info, )
    };
}  

#endif