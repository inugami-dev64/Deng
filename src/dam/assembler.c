#define ASSEMBLER_ONLY
#include "asset_creator_core.h"
static FILE *file;

static const char *get_file_ext_name(const char *file_name) {
    size_t index;
    char *output = malloc((strlen(file_name) + 3) * sizeof(char));

    /* Copy file name chars to output */
    for(index = 0; index < strlen(file_name); index++) 
        output[index] = file_name[index];

    output[index] = '.';
    output[index + 1] = 'd';
    output[index + 2] = 'a';
    output[index + 3] = 's';

    return (const char*) output;
}   

void assemble_das(DynamicVertices *p_vertices, DynamicIndices *p_indices, DynamicPixelData *p_pixel_data, 
const char *file_name, const char *asset_name, const char *description) {
    const char *ext_name = get_file_ext_name(file_name);

    file = fopen(ext_name, "wb");
    assemble_INFO_HDR(asset_name, description);
    assemble_VERT_HDR(p_vertices);
    assemble_INDX_HDR(p_indices);

    if(p_vertices->vertices_type) assemble_TPIX_HDR(p_pixel_data);

    // free(ext_name);
}

static void assemble_INFO_HDR(const char *asset_name, const char *description) {
    uint8_t asset_name_size, desc_size;
    asset_name_size = strlen(asset_name);
    desc_size = strlen(description);
    uint32_t hdr_size = 14 + (uint32_t) (asset_name_size + desc_size);
    
    fwrite(INFO_HEADER_NAME, sizeof(char), 8, file);
    fwrite(&hdr_size, sizeof(uint32_t), 1, file);
    fwrite(&asset_name_size, 1, 1, file);
    fwrite(asset_name, 1, strlen(asset_name), file);
    fwrite(&desc_size, 1, 1, file);
    fwrite(description, 1, strlen(description), file);
}

static void assemble_VERT_HDR(DynamicVertices *p_vertices) {
    uint32_t hdr_size;
    fwrite(VERTICES_HEADER_NAME, 1, strlen(VERTICES_HEADER_NAME), file);
    hdr_size = 17;
    if(p_vertices->vertices_type) {
        hdr_size += (uint32_t) (p_vertices->size * sizeof(VERT_MAPPED));
        fwrite(&hdr_size, sizeof(uint32_t), 1, file);
        fwrite((uint32_t*) &p_vertices->size, sizeof(uint32_t), 1, file);
        fwrite(&p_vertices->vertices_type, sizeof(uint8_t), 1, file);
        fwrite(p_vertices->p_mapped_vertices, sizeof(VERT_MAPPED), p_vertices->size, file); 
    }

    else {
        hdr_size += (p_vertices->size * sizeof(VERT_UNMAPPED));
        fwrite(&hdr_size, sizeof(uint32_t), 1, file);
        fwrite((uint32_t*) &p_vertices->size, sizeof(uint32_t), 1, file);
        fwrite(&p_vertices->vertices_type, sizeof(uint8_t), 1, file);
        // fwrite(p_vertices->p_unmapped_vertices, p_vertices->size * sizeof(VERT_UNMAPPED), 1, file); 
    }
}

static void assemble_INDX_HDR(DynamicIndices *p_indices) {
    uint32_t hdr_size = 16 + (p_indices->size * sizeof(uint32_t));
    uint32_t indices_count = (uint32_t) p_indices->size;
    printf("Indices size is: %d\n", indices_count);
    fwrite(INDICES_HEADER_NAME, strlen(INDICES_HEADER_NAME), 1, file);
    fwrite(&hdr_size, sizeof(uint32_t), 1, file);
    fwrite(&indices_count, sizeof(uint32_t), 1, file);
    fwrite(p_indices->p_indices, p_indices->size * sizeof(uint32_t), 1, file);

}

static void assemble_TPIX_HDR(DynamicPixelData *p_pixel_data) {
    uint32_t hdr_size = 16 + p_pixel_data->size;
    fwrite(TEXTURE_PIXEL_DATA, strlen(TEXTURE_PIXEL_DATA), 1, file);
    fwrite(&hdr_size, sizeof(uint32_t), 1, file);
    fwrite(&p_pixel_data->width, sizeof(uint16_t), 1, file);
    fwrite(&p_pixel_data->height, sizeof(uint16_t), 1, file);
    fwrite(p_pixel_data->p_pixel_data, p_pixel_data->size, 1, file);
}