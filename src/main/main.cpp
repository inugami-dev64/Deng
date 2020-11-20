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

    // index = model_handler.loadModel("statue", "Statue of liberty", "objects/obj1.obj", {0.0f, 0.7f, 0.0f}, frag_info);
    // model_handler.initModelMatrix(index, &generic_model_matrix); DENG_PIPELINE_TYPE_TEXTURE_MAPPED;
    dengUtils::FragInfo frag_info;
    frag_info.pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;
    frag_info.texture_file = "textures/obj1.tga";

    dengUtils::ModelHandler model_handler(0);

    LOG("seg test!");
    index = model_handler.loadModel("box", "Simple box", "objects/obj1.obj", {0.5f, 0.7f, 0.0f}, frag_info);
    model_handler.initModelMatrix(index, &generic_model_matrix);

    model_handler.getModels(&game_objects);

    rend.initObjects(&game_objects);
    rend.run();
    
    return 0;
}