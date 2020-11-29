#define DAS_HANDLER_ONLY
#include "dam_core.h"
 
/* Add .das file extension to the file name */
char *get_file_ext_name(char *file_name) {
    size_t index;
    char *output = (char*) malloc((strlen(file_name) + 3) * sizeof(char));

    /* Copy file name chars to output */
    for(index = 0; index < strlen(file_name); index++) 
        output[index] = file_name[index];

    output[index] = '.';
    output[index + 1] = 'd';
    output[index + 2] = 'a';
    output[index + 3] = 's';

    return output;
}   

/* das file assembler callback function */
void assemble_das(Asset *p_asset, char *file_name) {
    FILE *file;
    char *ext_name = get_file_ext_name(file_name);

    file = fopen((const char*) ext_name, "wb");
    assemble_INFO_HDR(p_asset->name, p_asset->description, file);
    assemble_VERT_HDR(&p_asset->vertices, file);
    assemble_INDX_HDR(&p_asset->indices, file);

    if(p_asset->vertices.vertices_type) assemble_TPIX_HDR(&p_asset->pixel_data, file);

    fclose(file);
    free(ext_name);
}

/* das INFO_HDR assembler */
void assemble_INFO_HDR(char *asset_name, char *description, FILE *file) {
    uint8_t asset_name_size, desc_size;
    uint64_t time_point = (uint64_t) time(NULL);
    asset_name_size = strlen(asset_name);
    desc_size = strlen(description);
    uint32_t hdr_size = 22 + (uint32_t) (asset_name_size + desc_size);
    
    fwrite(INFO_HEADER_NAME, sizeof(char), 8, file);
    fwrite(&time_point, sizeof(uint64_t), 1, file);
    fwrite(&hdr_size, sizeof(uint32_t), 1, file);
    fwrite(&asset_name_size, 1, 1, file);
    fwrite(asset_name, 1, strlen(asset_name), file);
    fwrite(&desc_size, 1, 1, file);
    fwrite(description, 1, strlen(description), file);
}

void assemble_VERT_HDR(DynamicVertices *p_vertices, FILE *file) {
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
        fwrite(p_vertices->p_unmapped_vertices, sizeof(VERT_UNMAPPED), p_vertices->size, file); 
    }
}

void assemble_INDX_HDR(DynamicIndices *p_indices, FILE *file) {
    uint32_t hdr_size = 16 + (p_indices->size * sizeof(uint32_t));
    uint32_t indices_count = (uint32_t) p_indices->size;
    fwrite(INDICES_HEADER_NAME, strlen(INDICES_HEADER_NAME), 1, file);
    fwrite(&hdr_size, sizeof(uint32_t), 1, file);
    fwrite(&indices_count, sizeof(uint32_t), 1, file);
    fwrite(p_indices->p_indices, p_indices->size * sizeof(uint32_t), 1, file);

}

void assemble_TPIX_HDR(DynamicPixelData *p_pixel_data, FILE *file) {
    uint32_t hdr_size = 16 + p_pixel_data->size;
    fwrite(TEXTURE_PIXEL_NAME, strlen(TEXTURE_PIXEL_NAME), 1, file);
    fwrite(&hdr_size, sizeof(uint32_t), 1, file);
    fwrite(&p_pixel_data->width, sizeof(uint16_t), 1, file);
    fwrite(&p_pixel_data->height, sizeof(uint16_t), 1, file);
    fwrite(p_pixel_data->p_pixel_data, p_pixel_data->size, 1, file);
}


/* das file reader callback function */
void read_das(Asset *p_asset, char *file_name) {
    FILE *file;
    file = fopen(file_name, "rb");
    read_INFO_HDR(&p_asset->name, &p_asset->description, &p_asset->time_point, file_name, file);
    read_VERT_HDR(&p_asset->vertices, file_name, file);
    read_INDX_HDR(&p_asset->indices, file_name, file);
    
    if(p_asset->vertices.vertices_type) read_TPIX_HDR(&p_asset->pixel_data, file_name, file);

    fclose(file);
}

void read_INFO_HDR(char **asset_name, char **description, uint64_t *p_time_point, char *file_name, FILE *file) {
    uint32_t hdr_size;
    uint8_t name_size, desc_size;
    char hdr_name[8];
    fread(hdr_name, 8 * sizeof(char), 1, file);
    
    if(strncmp(hdr_name, INFO_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify INFO_HDR in asset file %s!\n", file_name);
        return;
    }

    fread(p_time_point, sizeof(uint64_t), 1, file);
    fread(&hdr_size, sizeof(uint32_t), 1, file);

    fread(&name_size, sizeof(uint8_t), 1, file);
    *asset_name = (char*) calloc((size_t) name_size, sizeof(char));
    fread(*asset_name, sizeof(char), (size_t) name_size, file);

    fread(&desc_size, sizeof(uint8_t), 1, file);
    *description = (char*) calloc((size_t) desc_size, sizeof(char));
    fread(*description, sizeof(char), (size_t) desc_size, file);
}

void read_VERT_HDR(DynamicVertices *p_vertices, char *file_name, FILE *file) {
    char hdr_name[8];
    uint32_t hdr_size;
    uint32_t vert_count;

    fread(hdr_name, 8, 1, file);
    printf("%s\n", hdr_name);

    if(strncmp(hdr_name, VERTICES_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify VERT_HDR in asset file %s!\n", file_name);
        return;
    }

    fread(&hdr_size, sizeof(uint32_t), 1, file);
    fread(&vert_count, sizeof(uint32_t), 1, file);
    fread(&p_vertices->vertices_type, sizeof(uint8_t), 1, file);
    p_vertices->size = vert_count;

    if(p_vertices->vertices_type) {
        p_vertices->p_mapped_vertices = (VERT_MAPPED*) malloc(p_vertices->size * sizeof(VERT_MAPPED));
        fread(p_vertices->p_mapped_vertices, sizeof(VERT_MAPPED), p_vertices->size, file);
    }

    else {
        p_vertices->p_unmapped_vertices = (VERT_UNMAPPED*) malloc(p_vertices->size * sizeof(VERT_UNMAPPED));
        fread(p_vertices->p_unmapped_vertices, sizeof(VERT_UNMAPPED), p_vertices->size, file);
    }
}

void read_INDX_HDR(DynamicIndices *p_indices, char *file_name, FILE *file) {
    char hdr_name[8];
    uint32_t hdr_size, indices_count;

    fread(hdr_name, 8, 1, file);
    printf("%s\n", hdr_name);
    
    if(strncmp(hdr_name, INDICES_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify INDX_HDR in asset file %s!\n", file_name);
        return;
    }

    fread(&hdr_size, sizeof(uint32_t), 1, file);
    fread(&indices_count, sizeof(uint32_t), 1, file);
    
    p_indices->size = indices_count;
    p_indices->p_indices = (uint32_t*) malloc(p_indices->size * sizeof(uint32_t));
    fread(p_indices->p_indices, sizeof(uint32_t), p_indices->size, file);
}

void read_TPIX_HDR(DynamicPixelData *p_pixel_data, char *file_name, FILE *file) {
    char hdr_name[8];
    uint32_t hdr_size;
    
    fread(hdr_name, 8, 1, file);
    printf("%s\n", hdr_name);

    if(strncmp(hdr_name, TEXTURE_PIXEL_NAME, 8)) {
        printf("ERROR: Failed to verify TPIX_HDR in asset file %s!\n", file_name);
        return;
    }

    fread(&hdr_size, sizeof(uint32_t), 1, file);
    fread(&p_pixel_data->width, sizeof(uint16_t), 1, file);
    fread(&p_pixel_data->height, sizeof(uint16_t), 1, file);

    p_pixel_data->size = p_pixel_data->width * p_pixel_data->height * 4;
    p_pixel_data->p_pixel_data = (uint8_t*) malloc(sizeof(uint8_t) * p_pixel_data->size);
    fread(p_pixel_data->p_pixel_data, 1, p_pixel_data->size, file);
}