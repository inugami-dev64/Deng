#include "../../headers/deng/api_core.h"
#define SANDBOX_BACKGROUND {0.1411765f, 0.0431372f, 0.2313725f, 1.0f}

int main() {
    deng::WindowWrap window(1280, 980, "Game!");
    deng::Renderer rend(&window);

    rend.setHints (
        DENG_RENDERER_HINT_SHOW_FPS_COUNTER |
        DENG_RENDERER_HINT_MSAA_4 |
        DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS |
        DENG_RENDERER_HINT_MIPMAP_ENABLE
    );
    
    std::array<deng_Asset, 2> assets;
    dasReadAsset(&assets[0], "../../assets/viking_norm.das", DENG_ASSET_MODE_DONT_CARE);
    assets[0].tex_id = (char*) "viking";
    assets[0].id = (char*) "viking_room";
    assets[0].is_shown = true;

    deng_Texture viking_tex;
    dasLoadTexture(&viking_tex, "../../textures/viking_room.tga");
    viking_tex.id = (char*) "viking";
    
    dengUtils::GridGenerator grid_gen(DENG_DEFAULT_GRID_COLOR, 50.0f, 150);
    assets[1] = grid_gen.getGrid();
    assets[1].is_shown = true;

    rend.submitAssets(assets.data(), assets.size());
    rend.submitTextures(&viking_tex, 1);

    rend.initRenderer (
        DENG_RENDERER_USAGE_MAP_EDITOR, 
        SANDBOX_BACKGROUND
    );

    rend.run();
    return 0;
}
