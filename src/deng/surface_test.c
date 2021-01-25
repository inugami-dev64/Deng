#include "../../headers/deng/deng_surface_core.h"

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

        if(deng_IsKeyActive(p_window, DENG_KEY_W)) printf("%s\n", "Moving forward!");
        if(deng_IsKeyActive(p_window, DENG_KEY_S)) printf("%s\n", "Moving backward!");
        if(deng_IsKeyActive(p_window, DENG_KEY_A)) printf("%s\n", "Moving leftward!");
        if(deng_IsKeyActive(p_window, DENG_KEY_D)) printf("%s\n", "Moving rightward!");
        if(deng_IsKeyActive(p_window, DENG_KEY_E)) {
            printf("%s\n", "Destroying window");
            deng_DestroyWindow(p_window);
            exit(0);
        }

        if(deng_IsMouseBtnActive(p_window, DENG_MOUSE_BTN_1)) printf("%s\n", "LMB!");
        if(deng_IsMouseBtnActive(p_window, DENG_MOUSE_BTN_2)) printf("%s\n", "MMB!");
        if(deng_IsMouseBtnActive(p_window, DENG_MOUSE_BTN_3)) printf("%s\n", "RMB!");

        if(deng_IsKeyActive(p_window, DENG_KEY_ESCAPE) && read_esc && !p_window->virtual_mouse_position.is_enabled) {
            read_esc = 0;
            deng_SetMouseCursorMode(p_window, DENG_MOUSE_MODE_VIRTUAL);
        }

        else if(deng_IsKeyActive(p_window, DENG_KEY_ESCAPE) && read_esc && p_window->virtual_mouse_position.is_enabled) {
            read_esc = 0;
            deng_SetMouseCursorMode(p_window, DENG_MOUSE_MODE_CURSOR_VISIBLE);
        }

        else if(!read_esc) read_esc = deng_IsKeyReleased(p_window, DENG_KEY_ESCAPE);
            
        deng_GetMousePos(p_window, &mouse_x, &mouse_y, false);
    }

    return 0;
}
