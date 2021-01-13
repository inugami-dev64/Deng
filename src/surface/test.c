#include "deng_surface_core.h"

int main() {
    deng_SurfaceWindow *p_window = deng_InitVKSurfaceWindow(1200, 800, "Game!", DENG_WINDOW_MODE_FIXED);
    int is_w, is_a, is_s, is_d, is_lmb, is_mmb, is_rmb;
    float mouse_x, mouse_y;
    int read_esc = 1;

    while(deng_IsRunning(p_window)) {
        deng_UpdateWindow(p_window);

        if((is_w = deng_IsKeyActive(p_window, DENG_KEY_W))) printf("%s\n", "Moving forward!");
        if((is_s = deng_IsKeyActive(p_window, DENG_KEY_S))) printf("%s\n", "Moving backward!");
        if((is_a = deng_IsKeyActive(p_window, DENG_KEY_A))) printf("%s\n", "Moving leftward!");
        if((is_d = deng_IsKeyActive(p_window, DENG_KEY_D))) printf("%s\n", "Moving rightward!");

        if((is_lmb = deng_IsMouseBtnActive(p_window, DENG_MOUSE_BTN_1))) printf("%s\n", "LMB!");
        if((is_mmb = deng_IsMouseBtnActive(p_window, DENG_MOUSE_BTN_2))) printf("%s\n", "MMB!");
        if((is_rmb = deng_IsMouseBtnActive(p_window, DENG_MOUSE_BTN_3))) printf("%s\n", "RMB!");

        if(deng_IsKeyActive(p_window, DENG_KEY_ESCAPE) && read_esc && !p_window->virtual_mouse_position.is_enabled) {
            read_esc = 0;
            deng_SetMouseCursorMode(p_window, DENG_HIDE_CURSOR);
        }

        else if(deng_IsKeyActive(p_window, DENG_KEY_ESCAPE) && read_esc && p_window->virtual_mouse_position.is_enabled) {
            read_esc = 0;
            deng_SetMouseCursorMode(p_window, DENG_SHOW_CURSOR);
        }

        else if(!read_esc) read_esc = deng_IsKeyReleased(p_window, DENG_KEY_ESCAPE);
            
        deng_GetMousePos(p_window, &mouse_x, &mouse_y, false);
        // printf("Mouse x: %f", mouse_x);
        // printf("Mouse y: %f\n", mouse_y);
        // if(!is_w && !is_a && !is_s && !is_d) printf("%s\n", "No movement");
    }

    deng_DestroyWindow(p_window);
    return 0;
}