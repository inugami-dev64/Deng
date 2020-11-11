#include "../core/deng_core.h"

int main() {
    deng::Window window(1200, 800, "DENG");
    deng::Renderer rend(window);
    rend.run();
    
    return 0;
}