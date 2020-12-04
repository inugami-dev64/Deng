#include "../core/deng_core.h"

int main() {
    std::vector<DENGasset> assets;
    std::vector<DENGtexture> textures; 
    assets.resize(1);
    textures.resize(1);
    
    deng::WindowWrap window(1440, 1080, "Game!");
    deng::Renderer rend(window);

    // dasReadDAS(&assets[0], "../../assets/statue.das");
    dasReadDAS(&assets[0], "../../assets/statue.das", 1);
    assets[0].texture_index = 0;

    dasLoadTexture(&textures[0], "../../textures/obj1.tga");
    rend.submitAssets(&assets);
    rend.submitTextures(&textures);

    rend.run();

    return 0;
}