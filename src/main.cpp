#include "core/dengcore.h"

int main() {
    deng::Window window(1650, 1280, "Game!");
    deng::Renderer rend(window);
    rend.run();
    return 0;
}