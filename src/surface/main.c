#include "deng_surface_core.h"

void main() {
    init_window("Game!", 1600, 1200);

    while(1) {
        update_window();
        if(is_key_active(DENG_KEY_W)) printf("%s\n", "Moving forward!");
        if(is_key_active(DENG_KEY_S)) printf("%s\n", "Moving backward!");
        if(is_key_active(DENG_KEY_A)) printf("%s\n", "Moving leftward!");
        if(is_key_active(DENG_KEY_D)) printf("%s\n", "Moving rightward!");
    }

    destroy_window();
}