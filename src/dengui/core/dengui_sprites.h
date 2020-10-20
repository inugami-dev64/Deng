#ifndef DENGUI_SPRITES_H
#define DENGUI_SPRITES_H

namespace dengUI {

    struct ShapeVerticesDataCreator {
        static void createRectangle(WindowInfo *p_windowinfo, RectangleInfo **pp_rectangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const int &sequence_id);
        static void createTriangle(WindowInfo *p_windowsinfo, TriangleInfo **pp_triangle_info, BorderInfo *p_border_info, const dengUIBorderDrawMode &border_draw_mode, const int sequence_id);
    };
}  

#endif