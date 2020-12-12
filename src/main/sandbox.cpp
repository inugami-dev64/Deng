#include "../core/api_core.h"

std::vector<DENGAsset> assets(2);
std::vector<DENGTexture> textures(2);

int main() {
    // Create new window
    deng::WindowWrap window(1440, 1080, "Game!");
    
    deng::Renderer rend;
    rend.setHints(DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS | DENG_RENDERER_HINT_SHOW_FPS_COUNTER);


    // dasReadDAS(&assets[0], "../../assets/statue.das");
    dasLoadTexture(&textures[0], "../../textures/statue.tga");
    dasLoadTexture(&textures[1], "../../textures/brick.tga");

    dasReadAsset(&assets[0], "../../assets/statue.das", DENG_ASSET_MODE_3D_TEXTURE_MAPPED);
    dasBindTexture(&assets[0], 0);

    dasReadAsset(&assets[1], "../../assets/box.das", DENG_ASSET_MODE_3D_UNMAPPED);
    // dasBindTexture(&assets[1], 0);
    
    assets[1].solid_fill_color = {0.6f, 1.0f, 0.0f, 1.0f};

    rend.submitAssets(assets.data(), assets.size());
    rend.submitTextures(textures.data(), textures.size());

    rend.initRenderer(&window);    
    rend.run();

    return 0;
}