#ifndef DENGUI_SPRITES_H
#define DENGUI_SPRITES_H

namespace dengUI {

    struct WindowObject {
        deng::vec2<size_t> vertices_bounds;
        deng::vec2<size_t> indices_bounds;
        dengUIWindowLayerSequenceId sequence_id;
        const char *description;
        dengBool is_clickable;
        void(Events::*p_click_handler_func)();

    };

    struct ShapeVerticesDataCreator {
        static void getRectangleVertices(RectangleInfo *p_rectangle_info, WindowObject *p_window_object);
        static void getTriangleVertices(TriangleInfo *p_triangle_info, WindowObject *p_window_objects);
    };
}  

#endif