#include "asset_creator_core.h"

int main(int argc, char *argv[]) {
    
    DynamicVertices vertices;
    DynamicIndices indices;
    DynamicPixelData pixel_data;
    
    load_model(&vertices, &indices, "../../objects/box.obj", NULL, 1);
    load_image(&pixel_data, "../../textures/obj1.bmp");

    printf("Done loading!\n");
    printf("Now creating asset file...\n");

    assemble_das(&vertices, &indices, &pixel_data, "test", "Statue of liberty", "Statue of Liberty from New York");

    free(vertices.p_mapped_vertices);
    free(indices.p_indices);
    free(pixel_data.p_pixel_data);
    return 0;
}