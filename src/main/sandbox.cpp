#include "../core/api_core.h"

std::vector<DENGAsset> assets(2);
std::vector<dengMath::ModelMatrix> am(2);
std::vector<DENGTexture> textures(2);

int main() {
    // Create initialiser class objects
    deng::WindowWrap window(1440, 1080, "Game!");
    deng::Renderer rend;
    dengUtils::FontManager fm(nullptr, &window);

    rend.setHints(DENG_RENDERER_HINT_GAME_MODE);

    dengUtils::dengRendStr str;
    str.text = "Hello world!";
    dengError res;
    res = fm.newStr(str, "SourceCodePro-Regular.ttf", 30, {0.0f, 0.0f}, {0x00, 0x00, 0x00});
    if(res != DENG_NO_ERRORS) ERR("Failed to create render string!");

    // Load textures
    dasLoadTexture(&textures[0], "../../textures/statue.tga");
    dasLoadTexture(&textures[1], "../../textures/10x10.bmp");
    
    // Read assets
    dasReadAsset(&assets[0], "../../assets/statue.das", DENG_ASSET_MODE_3D_TEXTURE_MAPPED);
    dasBindTexture(&assets[0], 0);
    am[0].setRotation(180.0f, 0.0f, 0.0f);
    am[0].setTransformation(0.0f, -0.5f, 1.0f);
    am[0].setScale(2.0f, 2.0f, 2.0f);
    dengMath::applyModelMatrix(assets[0], am[0].getModelMatrix());

    // LOGGER
    cm_LogWrite("asset.log", "#entry point", 1);
    std::string content;
    for(size_t index = 0; index < assets[0].vertices.size; index++) {
        content = "{" + std::to_string(assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_x) + "," + std::to_string(assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_y) +
        "," + std::to_string(assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_z) + "}";
        cm_LogWrite("asset.log", content.c_str(), 0); 
    }
    
    dasReadAsset(&assets[1], "../../assets/box.das", DENG_ASSET_MODE_3D_TEXTURE_MAPPED);
    dasBindTexture(&assets[1], 1);
    assets[1].solid_fill_color = {0.6f, 1.0f, 0.0f, 1.0f};
    
    // Submit data
    rend.submitAssets(assets.data(), assets.size());
    rend.submitTextures(textures.data(), textures.size());
    rend.submitRendStr(&str, 1);

    // Run renderer
    rend.initRenderer(&window);    
    rend.run();

    return 0;
}