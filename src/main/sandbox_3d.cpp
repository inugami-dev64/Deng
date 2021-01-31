#include "../../headers/deng/api_core.h"
#define SANDBOX_BACKGROUND {0.1411765f, 0.0431372f, 0.2313725f, 1.0f}

int main() {
    deng::WindowWrap window(1280, 980, "Game!");
    deng::Renderer rend(&window);

    rend.setHints (
        DENG_RENDERER_HINT_SHOW_FPS_COUNTER |
        DENG_RENDERER_HINT_MSAA_2 |
        DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS |
        DENG_RENDERER_HINT_MIPMAP_ENABLE
    );
    
    deng_Asset viking_room;
    dasReadAsset(&viking_room, "../../assets/viking_norm.das", DENG_ASSET_MODE_DONT_CARE);
    viking_room.asset_mode = DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED;
    viking_room.tex_id = (char*) "viking";
    viking_room.id = (char*) "viking_room";
    viking_room.is_shown = true;

    deng_Texture viking_tex;
    dasLoadTexture(&viking_tex, "../../textures/viking_room.tga");
    viking_tex.id = (char*) "viking";

    rend.submitAssets(&viking_room, 1);
    rend.submitTextures(&viking_tex, 1);

    rend.initRenderer (
        DENG_RENDERER_USAGE_GAME_MODE, 
        SANDBOX_BACKGROUND
    );

    rend.run();
    return 0;
}
