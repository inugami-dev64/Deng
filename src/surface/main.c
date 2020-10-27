#include "deng_surface_core.h"

void main() {
    init_window("Game!", 1600, 1200);
    int is_w, is_a, is_s, is_d, is_lmb, is_mmb, is_rmb;
    int mouse_x, mouse_y;

    while(1) {
        update_window();

        if(is_w = is_key_active(DENG_KEY_W)) printf("%s\n", "Moving forward!");
        if(is_s = is_key_active(DENG_KEY_S)) printf("%s\n", "Moving backward!");
        if(is_a = is_key_active(DENG_KEY_A)) printf("%s\n", "Moving leftward!");
        if(is_d = is_key_active(DENG_KEY_D)) printf("%s\n", "Moving rightward!");

        if(is_lmb = is_mouse_btn_active(DENG_MOUSE_BTN_1)) printf("%s\n", "LMB!");
        if(is_mmb = is_mouse_btn_active(DENG_MOUSE_BTN_2)) printf("%s\n", "MMB!");
        if(is_rmb = is_mouse_btn_active(DENG_MOUSE_BTN_3)) printf("%s\n", "RMB!");
        
        if(is_key_active(DENG_KEY_ESCAPE))
            set_mouse_coords(100, 100);
            
        get_mouse_coords(&mouse_x, &mouse_y);
        printf("Mouse x: %d\n", mouse_x);
        printf("Mouse y: %d\n", mouse_y);
        // if(!is_w && !is_a && !is_s && !is_d) printf("%s\n", "No movement");
    }

    destroy_window();
}