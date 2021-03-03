#define __KEY_EV_C
#include <deng/key_ev.h>


void deng_RegisterKeyEvent (
    deng_Key key, 
    deng_MouseButton btn, 
    deng_InputType in_type, 
    deng_InputEventType ev_type
) {
    if(in_type == DENG_INPUT_TYPE_KB) {
        if(key == DENG_KEY_UNKNOWN) 
            return;
        if(ev_type == DENG_INPUT_EVENT_TYPE_ACTIVE) {
            active_keys[key] = true;
            released_keys[key] = false;
        }
        else if(ev_type == DENG_INPUT_EVENT_TYPE_RELEASED) {
            active_keys[key] = false;
            released_keys[key] = true;
        }
    }

    else if(in_type == DENG_INPUT_TYPE_MOUSE) {
        if(btn == DENG_MOUSE_BTN_UNKNOWN)
            return;
        if(ev_type == DENG_INPUT_EVENT_TYPE_ACTIVE) {
            active_btns[btn] = true;
            released_btns[btn] = false;
        }
        else if(ev_type == DENG_INPUT_EVENT_TYPE_RELEASED) {
            active_btns[btn] = false;
            released_btns[btn] = true;
        }
    }
}


/*
 * Find given key or mouse button status from specified event array
 */
deng_bool_t deng_FindKeyStatus (
    deng_Key key, 
    deng_MouseButton btn, 
    deng_InputType in_type, 
    deng_InputEventType ev_type
) {
    deng_bool_t stat = false;
    if(in_type == DENG_INPUT_TYPE_KB) {
        if(key == DENG_KEY_UNKNOWN)
            return false;
        if(ev_type == DENG_INPUT_EVENT_TYPE_ACTIVE)
            stat = active_keys[key];
        else if(ev_type == DENG_INPUT_EVENT_TYPE_RELEASED)
            stat = released_keys[key];
    }
    else if(in_type == DENG_INPUT_TYPE_MOUSE) {
        if(btn == DENG_MOUSE_BTN_UNKNOWN)
            return false;
        if(ev_type == DENG_INPUT_EVENT_TYPE_ACTIVE)
            stat = active_btns[btn];
        else if(ev_type == DENG_INPUT_EVENT_TYPE_RELEASED)
            stat = released_btns[btn];
    }

    return stat;
}


void deng_UnreleaseKeys() {
    memset (
        released_keys, 
        0x00, 
        C_ARR_SIZE(released_keys)
    );
    memset (
        released_btns, 
        0x00, 
        C_ARR_SIZE(released_btns)
    );
}
