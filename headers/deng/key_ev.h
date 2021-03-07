#ifndef __KEY_EV_H
#define __KEY_EV_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KEY_EV_C
    #include <stdlib.h> // malloc()
    #include <math.h>

    // DEBUGGING PURPOSES
    #include <common/base_types.h>
    #include <common/common.h>
    #ifdef __DENG_API_CORE
        #include <vulkan/vulkan.h>
    #endif
    #ifdef __linux__
        #include <X11/XKBlib.h>
    #endif

    #ifdef _WIN32
        #include <Windows.h>
    #endif

    #include <deng/key_definitions.h>
    #include <deng/surface_window.h>


    /*
     * Key event registry arrays
     */
    deng_bool_t active_keys[DENG_KEY_COUNT] = {0};
    deng_bool_t released_keys[DENG_KEY_COUNT] = {0};
    deng_bool_t active_btns[DENG_MOUSE_BTN_COUNT] = {0};
    deng_bool_t released_btns[DENG_MOUSE_BTN_COUNT] = {0};
#endif




void deng_RegisterKeyEvent (
    deng_Key key, 
    deng_MouseButton btn, 
    deng_InputType in_type, 
    deng_InputEventType ev_type
);


void deng_UnreleaseKeys();


/*
 * Find given key or mouse button status from specified event array
 */
deng_bool_t deng_FindKeyStatus (
    deng_Key key, 
    deng_MouseButton btn, 
    deng_InputType in_type, 
    deng_InputEventType ev_type
);

#ifdef __cplusplus
}
#endif

#endif

