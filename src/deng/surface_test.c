#undef __DENG_API_CORE
#include <windows.h>
#include <common/base_types.h>
#include <deng/key_definitions.h>
#include <deng/key_ev.h>
#include <deng/surface_window.h>

int main() {
    deng_SurfaceWindow *p_window = deng_InitVKSurfaceWindow (
        1200, 
        800, 
        "Game!", 
        DENG_WINDOW_MODE_FIXED
    );
    float mouse_x, mouse_y;
    int read_esc = 1;

    while(deng_IsRunning(p_window)) {
        deng_UpdateWindow(p_window);

        // Keyboard keys
        if(deng_FindKeyStatus(DENG_KEY_W, DENG_MOUSE_BTN_UNKNOWN, DENG_INPUT_TYPE_KB, DENG_INPUT_EVENT_TYPE_ACTIVE)) 
            printf("%s\n", "Moving forward!");
        if(deng_FindKeyStatus(DENG_KEY_S, DENG_MOUSE_BTN_UNKNOWN, DENG_INPUT_TYPE_KB, DENG_INPUT_EVENT_TYPE_ACTIVE)) 
            printf("%s\n", "Moving backward!");
        if(deng_FindKeyStatus(DENG_KEY_A, DENG_MOUSE_BTN_UNKNOWN, DENG_INPUT_TYPE_KB, DENG_INPUT_EVENT_TYPE_ACTIVE))
            printf("%s\n", "Moving leftward!");
        if(deng_FindKeyStatus(DENG_KEY_D, DENG_MOUSE_BTN_UNKNOWN, DENG_INPUT_TYPE_KB, DENG_INPUT_EVENT_TYPE_ACTIVE))
            printf("%s\n", "Moving rightward!");
        if(deng_FindKeyStatus(DENG_KEY_E, DENG_MOUSE_BTN_UNKNOWN, DENG_INPUT_TYPE_KB, DENG_INPUT_EVENT_TYPE_ACTIVE)) {
            printf("%s\n", "Destroying window");
            deng_DestroyWindow(p_window);
            exit(0);
        }

        deng_GetMousePos(p_window, &mouse_x, &mouse_y, false);
    }

    return 0;
}
