#ifndef DENGUI_CORE_H
#define DENGUI_CORE_H

#define DENGUI_WINDOW_LAYER_MULTIPLIER 0.001f
#define DENGUI_MINIMIZE_TRIANGLE_OFFSET 0.01f

namespace dengUI {
    enum dengUINonFilledBorderOffsetMode {
        DENGUI_NON_FILLED_BORDER_OFFSET_MODE_IN = 0,
        DENGUI_NON_FILLED_BORDER_OFFSET_MODE_OUT = 1
    };

    enum dengUIWindowObjectShape {
        DENGUI_WINDOW_OBJECT_SHAPE_TRIANGLE = 0,
        DENGUI_WINDOW_OBJECT_SHAPE_RECTANGLE = 1
    };

    enum dengUIPixelBoundariesMode {
        DENGUI_PIXEL_BOUNDARIES_MODE_MINIMUM = 0,
        DENGUI_PIXEL_BOUNDARIES_MODE_MAXIMUM = 1
    };

    enum dengUIWindowSizeMode {
        DENGUI_WINDOW_SIZE_MODE_MAXIMISED = 0,
        DENGUI_WINDOW_SIZE_MODE_MINIMIZED = 1
    };

    enum dengUIWindowVerticesType {
        DENGUI_WINDOW_VERTICES_TYPE_HEAD = 0,
        DENGUI_WINDOW_VERTICES_TYPE_BODY = 1,
        DENGUI_WINDOW_VERTICES_TYPE_TRIANGLE = 2
    };

    enum dengUISpriteType {
        DENGUI_SPRITE_TYPE_TRIANGLE = 0,
        DENGUI_SPRITE_TYPE_RECTANGLE = 1
    };

    enum dengUIWindowObjectHandlePtrUsage {
        DENGUI_WINDOW_OBJECT_HANDLE_PTR_USAGE_CALL = 0,
        DENGUI_WINDOW_OBJECT_HANDLE_PTR_USAGE_SET_CALLER = 1
    };

    enum dengUIBorderDrawMode {
        DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER = 0,
        DENGUI_BORDER_DRAW_MODE_SPECIFIED_BORDER = 1,
        DENGUI_BORDER_DRAW_MODE_NO_BORDER = 2
    };

    enum dengUIWindowAssetType {
        DENGUI_WINDOW_ASSET_TYPE_BUTTON = 0,
        DENGUI_WINDOW_ASSET_TYPE_SLIDER = 1,
        DENGUI_WINDOW_ASSET_TYPE_SUB_WINDOW = 2,
        DENGUI_WINDOW_ASSET_TYPE_TEXT_ENTRY_BOX = 3,
        DENGUI_WINDOW_ASSET_TYPE_TEXT_LABEL = 4
    };

    enum dengUISizeUnit {
        DENGUI_SIZE_UNIT_VECTOR_UNITS = 0,
        DENGUI_SIZE_UNIT_PIXELS = 1
    };
}

#include "../css_engine/css_engine_core.h"
#include "dengui_infos.h"
#include "dengui_font_loader.h"
#include "../pixel_perfect_collision/dengui_pixel_collision.h"
// #include "../events/dengui_event_handles.h"
// #include "../events/dengui_events.h"
#include "dengui_sprites.h"
#include "dengui_window.h"


#endif