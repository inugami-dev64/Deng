#include "headers/renderer.h"

int main() {
    Deng::Window window(1280, 1080, "Sugoi dekai!");
    Deng::Renderer rend(window);
    rend.run();
    return 0;
}