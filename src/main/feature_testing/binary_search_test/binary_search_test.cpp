#include "../../../core/deng_core.h"

int main() {
    dengUtils::ObjLoader obj_loader("objects/obj1", DENG_COORDINATE_MODE_DEFAULT);
    dengUtils::TextureLoaderTGA tex_loader("textures/obj1.tga");
    dengUtils::GameObject game_asset;
    game_asset.pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;

    obj_loader.getObjVertices(&game_asset);

}