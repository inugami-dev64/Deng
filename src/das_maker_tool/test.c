#include "asset_creator_core.h"

int main(int argc, char *img_file[]) {
    DynamicVertices vertices;
    DynamicIndices indices;
    
    load_model(&vertices, &indices, VERTICES_COLOR_MODE_TEXTURE_MAPPED, "../../objects/box.obj");
    printf("Done!\n");
    return 0;
}