#include "../core/api_core.h"
#define SANDBOX_BACKGROUND {0.1411765f, 0.0431372f, 0.2313725f, 1.0f}
#define TRI_INDEX   0
#define CIR_INDEX   1
#define REC_INDEX   2

std::vector<deng_Asset> assets(3);

int main() {
    // Create initialiser class objects
    deng::WindowWrap window(1280, 980, "Game!");
    deng::Renderer rend;

    rend.setHints (
        DENG_RENDERER_HINT_SHOW_FPS_COUNTER |
        DENG_RENDERER_HINT_MSAA_2 |
        DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS | 
        DENG_RENDERER_HINT_MIPMAP_ENABLE,
        &window
    );

    dengUtils::TriangleGenerator tri_gen(window.getSize());
    dengUtils::CircleGenerator cir_gen(window.getSize());
    dengUtils::RectangleGenerator rec_gen(window.getSize());

    assets[TRI_INDEX] = tri_gen.makeAbsTriangleAsset (
        "triangle",
        NULL,
        {-1.0f, 0.0f},
        {0.67, 0.67},
        {-1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {{-1.0f, 1.0f}, {0.0f, -1.0f}, {1.0f, 1.0f}},
        5,
        {0.0f, 0.0f, 0.0f, 1.0f}
    );
    assets[TRI_INDEX].is_shown = true;

    assets[CIR_INDEX] = cir_gen.makeAbsCircleAsset (
        "circle",
        NULL,
        {0.0f, 0.0f},
        0.33f,
        {0.0f, 1.0f, 0.0f, 1.0f},
        5,
        {0.0f, 0.0f, 0.0f, 1.0f}
    );
    assets[CIR_INDEX].is_shown = true;

    assets[REC_INDEX] = rec_gen.makeUnmappedAbsRecAsset (
        "rectangle",
        NULL,
        {0.33f, 0.0f},
        {0.67f, 0.67f},
        {-1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        5,
        {0.0f, 0.0f, 0.0f, 1.0f}
    );
    assets[REC_INDEX].is_shown = true;
    
    rend.submitAssets (
        assets.data(),
        assets.size()
    );

    rend.initRenderer (
        &window,
        DENG_RENDERER_USAGE_GAME_MODE,
        SANDBOX_BACKGROUND
    );

    rend.run();

    return 0;
}
