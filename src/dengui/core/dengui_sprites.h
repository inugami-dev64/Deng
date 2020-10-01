#ifndef DENGUI_SPRITES_H
#define DENGUI_SPRITES_H

namespace dengUI {

    struct ShapeVerticesDataCreator {
        static void getRectangleVertices(RectangleInfo *p_rectangle_info, WindowObject *p_window_object);
        static void getTriangleVertices(TriangleInfo *p_triangle_info, WindowObject *p_window_objects);
    };
}  

#endif