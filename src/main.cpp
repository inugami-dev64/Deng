#include "headers/renderer.h"

int main() {
    Deng::Window window(1280, 720, "Sugoi dekai!");
    Deng::Renderer rend(window);
    rend.run();
    return 0;
}