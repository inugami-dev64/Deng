#include "../core/api_core.h"
#define SANDBOX_BACKGROUND {0.1411765f, 0.0431372f, 0.2313725f, 1.0f}

std::vector<DENGAsset> assets(1);
std::vector<dengMath::ModelMatrix> am(2);
std::vector<DENGTexture> textures(2);

int main() {
    // Create initialiser class objects
    deng::WindowWrap window(1440, 1080, "Game!");
    deng::Renderer rend;
    
    // dengUtils::FontManager fm("", &window);
    // dengUtils::bitmapStr hello_world;
    // hello_world.text = "HELLO WORLD!";

    // fm.newPxStr (
    //     hello_world,
    //     DEFAULT_FONT_FILE,
    //     40,
    //     {0.5f, 0.5f},
    //     {0xff, 0xff, 0xff}
    // );

    rend.setHints (
        DENG_RENDERER_HINT_GAME_MODE | 
        DENG_RENDERER_HINT_SHOW_FPS_COUNTER
    );

    // Load textures
    rend.submitTexture("textures/brick.tga", "brick");
    rend.submitTexture("textures/viking_room.tga", "viking_room");

    // Read assets
    dasReadAsset(&assets[0], "assets/viking_room.das", DENG_ASSET_MODE_3D_TEXTURE_MAPPED);
    assets[0].tex_id = (char*) "viking_room";
    
    am[0].setRotation(0.0f, 0.0f, 0.0f);        
    am[0].setTransformation(0.0f, -0.5f, 1.0f);
    am[0].setScale(2.0f, 2.0f, 2.0f);
    dengMath::applyModelMatrix(assets[0], am[0].getModelMatrix());
    
    // dasReadAsset(&assets[1], "../../assets/box.das", DENG_ASSET_MODE_3D_TEXTURE_MAPPED);
    // dasBindTexture(&assets[1], 1);
    
    // LOGGER
    cm_LogWrite("asset.log", "#entry point", 1);
    std::string content;
    // Vertices
    for(size_t index = 0; index < assets[0].vertices.size; index++) {
        content = "{" + std::to_string(assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_x) + "," + 
        std::to_string(assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_y) +
        "," + std::to_string(assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_z) + "}";
        cm_LogWrite("asset.log", content.c_str(), 0); 
    }
    // Indices
    cm_LogWrite("asset.log", "\n", 0);
    for(size_t index = 0; index < assets[0].indices.size; index++) {
        content = std::to_string(assets[0].indices.p_indices[index]);
        cm_LogWrite("asset.log", content.c_str(), 0); 
    }

    // Submit data
    rend.submitAssets(assets.data(), assets.size());
    // rend.submitRendStr(&hello_world, 1);

    // Run renderer
    rend.initRenderer (
        &window, 
        DENG_RENDERER_USAGE_MAP_EDITOR,
        SANDBOX_BACKGROUND
    );    
    rend.run();

    return 0;
}