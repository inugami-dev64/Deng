#include "core/deng_core.h"

int main() {
    deng::Window window(1600, 1200, "Game!");
    deng::Renderer rend(window);
    rend.run();
    
    return 0;
}