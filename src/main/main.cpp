#include "../core/deng_core.h"

int main() {
    deng::Window window(1200, 800, "DENG");
    deng::Renderer rend(window);
    
    size_t index;
    std::vector<dengUtils::GameObject> game_objects;
    dengMath::ModelMatrix generic_model_matrix;
    generic_model_matrix.setRotation(180, 0, 0);
    generic_model_matrix.setScale(2, 2, 2);
    generic_model_matrix.setTransformation(0, 0, 0);
    
    dengUtils::FragInfo frag_info;
    frag_info.pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;
    frag_info.texture_file = "textures/obj1.tga";

    dengUtils::ModelHandler model_handler(0);
    index = model_handler.loadModel("statue", "Statue of liberty", "objects/obj1.obj", {0.0f, 0.7f, 0.0f}, frag_info);
    model_handler.initModelMatrix(index, &generic_model_matrix);

    // index = model_handler.loadModel("box", "Simple box", "objects/box.obj", {0.5f, 0.7f, 0.0f}, frag_info);
    // model_handler.initModelMatrix(index, &generic_model_matrix);

    model_handler.getModels(&game_objects);
    
    // game_objects[0].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;
// 
    // game_objects[0].indices_data.resize(6);
    // game_objects[0].indices_data[0] = 0, game_objects[0].indices_data[1] = 1, game_objects[0].indices_data[2] = 2;
    // game_objects[0].indices_data[3] = 2, game_objects[0].indices_data[4] = 3, game_objects[0].indices_data[5] = 0;
// 
    // game_objects[0].indices_buffer_memory_offset = sizeof(dengUtils::TextureMappedVerticesData) * 4;
// 
    // game_objects[0].p_texture_mapped_vertices_data = new std::vector<dengUtils::TextureMappedVerticesData>;
    // game_objects[0].p_texture_mapped_vertices_data->resize(4);
// 
    // (*game_objects[0].p_texture_mapped_vertices_data)[0] = {{-0.5f, -0.5f, 0.0f}, {-1.0f, -1.0f}};    
    // (*game_objects[0].p_texture_mapped_vertices_data)[1] = {{0.5f, -0.5f, 0.0f}, {1.0f, -1.0f}};
    // (*game_objects[0].p_texture_mapped_vertices_data)[2] = {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}};
    // (*game_objects[0].p_texture_mapped_vertices_data)[3] = {{-0.5f, 0.5f, 0.0f}, {-1.0f, 1.0f}};
// 
    // game_objects[0].vertices_buffer_memory_offset = 0;
    // 
    // game_objects[0].p_raw_texture_data = new dengUtils::RawTextureData;
    // dengUtils::TextureLoaderTGA tex_loader("textures/obj1.tga");
    // tex_loader.getTextureDetails(game_objects[0].p_raw_texture_data);

    rend.initObjects(&game_objects);
    rend.run();
    
    return 0;
}