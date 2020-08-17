#include "headers/renderer.h"

int main() {
    deng::Window window(1280, 1080, "Sugoi dekai!");
    deng::Renderer rend(window);
    rend.run();
    return 0;
}