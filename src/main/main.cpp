#include "../core/deng_core.h"

int main() {
    std::vector<DENGasset> assets;
    std::vector<DENGtexture> textures; 
    assets.resize(1);
    textures.resize(1);
    
    deng::WindowWrap window(1440, 1080, "Game!");
    deng::Renderer rend(window);

    // dasReadDAS(&assets[0], "../../assets/statue.das");
    dasReadDAS(&assets[0], "../../assets/box.das", 0);
    assets[0].solid_fill_color = {1.0f, 1.0f, 1.0f, 1.0f};
    // dasLoadTexture(&textures[0], "../../textures/obj1.bmp");

    // size_t index;
    // for(index = 0; index < assets[0].vertices.size; index++)
        // printf("v: %f;%f;%f\n", assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_x, 
        // assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_y, assets[0].vertices.p_texture_mapped_vert_data[index].vert_data.vert_z);

    // printf("\n");
    // for(index = 0; index < assets[0].vertices.size; index++)
        // printf("t: %f;%f\n", assets[0].vertices.p_texture_mapped_vert_data[index].tex_data.tex_x, 
        // assets[0].vertices.p_texture_mapped_vert_data[index].tex_data.tex_y);
    

    rend.submitAssets(&assets);
    // rend.submitTextures(&textures);

    rend.run();

    return 0;
}