#define DAS_EXT_HANDLER
#include "das_core.h"
 
/* Add .das file extension to the file name */
char *dasGetFileExtName(char *file_name) {
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
void dasAssemble(deng_Asset *p_asset, const char *file_name) {
    FILE *file;
    file = fopen(file_name, "wb");
    if(!file) {
        fprintf (
            stderr,
            "Failed to open file: %s\n",
            file_name
        );

        exit(-1);
    }
    
    dasAssembleINFOHDR(p_asset->name, p_asset->description, file);
    dasAssembleVERTHDR(&p_asset->vertices, file);
    dasAssembleINDXHDR(&p_asset->indices, file);

    fclose(file);
}


/* das INFO_HDR assembler */
void dasAssembleINFOHDR (
    char *asset_name, 
    char *description, 
    FILE *file
) {
    deng_ui8_t asset_name_size, desc_size;
    deng_ui64_t time_point = (deng_ui64_t) time(NULL);
    asset_name_size = strlen(asset_name);
    desc_size = strlen(description);
    deng_ui32_t hdr_size = 22 + (deng_ui32_t) (asset_name_size + desc_size);
    
    fwrite(INFO_HEADER_NAME, sizeof(char), 8, file);
    fwrite(&time_point, sizeof(deng_ui64_t), 1, file);
    fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
    fwrite(&asset_name_size, 1, 1, file);
    fwrite(asset_name, 1, strlen(asset_name), file);
    fwrite(&desc_size, 1, 1, file);
    fwrite(description, 1, strlen(description), file);
}


/* das VERT_HDR assembler */
void dasAssembleVERTHDR (
    deng_VertDynamic *p_vertices, 
    FILE *file
) {
    deng_ui32_t hdr_size, vert_size;
    fwrite(VERTICES_HEADER_NAME, 1, strlen(VERTICES_HEADER_NAME), file);
    hdr_size = 17;
    vert_size = (deng_ui32_t) p_vertices->size;
    hdr_size += (deng_ui32_t) (p_vertices->size * sizeof(VERT_MAPPED));

    fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
    fwrite(&vert_size, sizeof(deng_ui32_t), 1, file);
    fwrite(p_vertices->p_texture_mapped_vert_data, sizeof(VERT_MAPPED), p_vertices->size, file); 
}

void dasAssembleINDXHDR (
    deng_IndicesDynamic *p_indices, 
    FILE *file
) {
    deng_ui32_t hdr_size = 16 + (p_indices->size * sizeof(deng_ui32_t));
    deng_ui32_t indices_count = (deng_ui32_t) p_indices->size;
    fwrite(INDICES_HEADER_NAME, strlen(INDICES_HEADER_NAME), 1, file);
    fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
    fwrite(&indices_count, sizeof(deng_ui32_t), 1, file);
    fwrite(p_indices->p_indices, p_indices->size * sizeof(deng_ui32_t), 1, file);
}

void dasAssembleTPIXHDR (
    deng_PixelDataDynamic *p_pixel_data, 
    FILE *file
) {
    deng_ui32_t hdr_size = 16 + p_pixel_data->size;
    fwrite(TEXTURE_PIXEL_NAME, strlen(TEXTURE_PIXEL_NAME), 1, file);
    fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
    fwrite(&p_pixel_data->width, sizeof(deng_ui16_t), 1, file);
    fwrite(&p_pixel_data->height, sizeof(deng_ui16_t), 1, file);
    fwrite(p_pixel_data->p_pixel_data, p_pixel_data->size, 1, file);
}


/* das file reader callback function */
void dasReadAsset (
    deng_Asset *p_asset, 
    const char *file_name, 
    deng_AssetMode asset_mode
) {
    FILE *file;
    file = fopen(file_name, "rb");
    
    /* Check if asset exists and if needed exit */
    if(!file) {
        printf("ERROR: Asset file '%s' does not exist!\n", file_name);
        exit(-1);
    }

    size_t index;
    deng_VertDynamic tmp_vert;
    p_asset->asset_mode = asset_mode;
    
    dasReadINFOHDR (
        &p_asset->name, 
        &p_asset->description, 
        &p_asset->time_point, 
        (char*) file_name, 
        file
    );

    switch (asset_mode)
    {
    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        dasReadVERTHDR (
            &p_asset->vertices, 
            (char*) file_name, 
            file
        );
        break;
    
    case DENG_ASSET_MODE_3D_UNMAPPED:
        dasReadVERTHDR(&tmp_vert, (char*) file_name, file);
        p_asset->vertices.size = tmp_vert.size;
        p_asset->vertices.p_unmapped_vert_data = (VERT_UNMAPPED*) malloc (
            p_asset->vertices.size * sizeof(VERT_UNMAPPED)
        );

        // Populate asset unmapped vertices
        for(index = 0; index < p_asset->vertices.size; index++)
            p_asset->vertices.p_unmapped_vert_data[index].vert_data = tmp_vert.p_texture_mapped_vert_data[index].vert_data;
        
        break;

    case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
        dasReadVERTHDR(&tmp_vert, (char*) file_name, file);
        p_asset->vertices.size = tmp_vert.size;
        p_asset->vertices.p_texture_mapped_vert_data_2d = (VERT_MAPPED_2D*) malloc(p_asset->vertices.size * sizeof(VERT_MAPPED_2D));

        // Populate 2D asset textured vertices
        for(index = 0; index < p_asset->vertices.size; index++) {
            p_asset->vertices.p_texture_mapped_vert_data_2d[index].vert_data.vert_x = 
            tmp_vert.p_texture_mapped_vert_data[index].vert_data.vert_x;
            p_asset->vertices.p_texture_mapped_vert_data_2d[index].vert_data.vert_y =
            tmp_vert.p_texture_mapped_vert_data[index].vert_data.vert_y;
            p_asset->vertices.p_texture_mapped_vert_data_2d[index].tex_data = tmp_vert.p_texture_mapped_vert_data[index].tex_data;  
        }
        break;

    case DENG_ASSET_MODE_2D_UNMAPPED:
        dasReadVERTHDR(&tmp_vert, (char*) file_name, file);
        p_asset->vertices.size = tmp_vert.size;
        p_asset->vertices.p_unmapped_vert_data_2d = (VERT_UNMAPPED_2D*) malloc (
            p_asset->vertices.size * 
            sizeof(VERT_UNMAPPED_2D)
        );
        
        // Populate 2D asset unmapped vertices
        for(index = 0; index < p_asset->vertices.size; index++) {
            p_asset->vertices.p_unmapped_vert_data_2d[index].vert_data.vert_x = 
            tmp_vert.p_texture_mapped_vert_data[index].vert_data.vert_x;
            p_asset->vertices.p_unmapped_vert_data_2d[index].vert_data.vert_y = 
            tmp_vert.p_texture_mapped_vert_data[index].vert_data.vert_y;
        }
        break;


    default:
        break;
    }
    
    dasReadINDXHDR(&p_asset->indices, (char*) file_name, file);
    fclose(file);
}


/* Destroy all allocated assets */
void dasDestroyAssets (
    deng_Asset *assets,
    deng_i32_t asset_c
) {
    deng_i32_t l_index;
    for(l_index = 0; l_index < asset_c; l_index++) {
        switch (assets[l_index].asset_mode)
        {
        case DENG_ASSET_MODE_2D_UNMAPPED:
            free(assets[l_index].vertices.p_unmapped_vert_data_2d);
            break;
        
        case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
            free(assets[l_index].vertices.p_texture_mapped_vert_data_2d);
            break;

        case DENG_ASSET_MODE_3D_UNMAPPED:
            free(assets[l_index].vertices.p_unmapped_vert_data);
            break;

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
            free(assets[l_index].vertices.p_texture_mapped_vert_data);
            break;
        
        default:
            break;
        }
    }

    if(asset_c) free(assets);
}


/* Destroy all allocated textures */
void dasDestroyTextures (
    deng_Texture *textures,
    deng_i32_t tex_c
) {
    deng_i32_t l_index;
    for(l_index = 0; l_index < tex_c; l_index++) 
        free(textures[l_index].pixel_data.p_pixel_data);

    if(tex_c) free(textures);
}


/* Read das INFO_HDR information */
void dasReadINFOHDR (
    char **asset_name, 
    char **description, 
    deng_ui64_t *p_time_point, 
    char *file_name, 
    FILE *file
) {
    deng_ui32_t hdr_size;
    deng_ui8_t name_size, desc_size;
    char hdr_name[8];
    size_t res;
    res = fread(hdr_name, 8 * sizeof(char), 1, file);
    
    if(strncmp(hdr_name, INFO_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify INFO_HDR in asset file %s!\n", file_name);
        return;
    }

    res = fread(p_time_point, sizeof(deng_ui64_t), 1, file);
    res = fread(&hdr_size, sizeof(deng_ui32_t), 1, file);

    res = fread(&name_size, sizeof(deng_ui8_t), 1, file);
    *asset_name = (char*) calloc((size_t) name_size, sizeof(char));
    res = fread(*asset_name, sizeof(char), (size_t) name_size, file);

    res = fread(&desc_size, sizeof(deng_ui8_t), 1, file);
    *description = (char*) calloc((size_t) desc_size, sizeof(char));
    res = fread(*description, sizeof(char), (size_t) desc_size, file);

    if(!res) {
        perror("Failed to read INFO_HDR\n");
        exit(-1);
    }
}

void dasReadVERTHDR (
    deng_VertDynamic *p_vertices, 
    char *file_name, 
    FILE *file
) {
    char hdr_name[8];
    deng_ui32_t hdr_size;
    deng_ui32_t vert_count;

    size_t res;
    res = fread(hdr_name, 8, 1, file);

    if(strncmp(hdr_name, VERTICES_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify VERT_HDR in asset file %s!\n", file_name);
        return;
    }

    res = fread(&hdr_size, sizeof(deng_ui32_t), 1, file);
    res = fread(&vert_count, sizeof(deng_ui32_t), 1, file);
    p_vertices->size = vert_count;

    p_vertices->p_texture_mapped_vert_data = (VERT_MAPPED*) malloc (
        p_vertices->size * sizeof(VERT_MAPPED)
    );

    res = fread (
        p_vertices->p_texture_mapped_vert_data, 
        sizeof(VERT_MAPPED), 
        p_vertices->size, 
        file
    );

    if(!res) {
        perror("Failed to read VERT_HDR\n");
        exit(-1);
    }
}

void dasReadINDXHDR (
    deng_IndicesDynamic *p_indices, 
    char *file_name, 
    FILE *file
) {
    char hdr_name[8];
    deng_ui32_t hdr_size, indices_count;

    size_t res;
    res = fread(hdr_name, 8, 1, file);
    printf("%s\n", hdr_name);
    
    if(strncmp(hdr_name, INDICES_HEADER_NAME, 8)) {
        printf (
            "ERROR: Failed to verify INDX_HDR in asset file %s!\n", 
            file_name
        );
        return;
    }

    res = fread(&hdr_size, sizeof(deng_ui32_t), 1, file);
    res = fread(&indices_count, sizeof(deng_ui32_t), 1, file);
    
    p_indices->size = indices_count;
    p_indices->p_indices = (deng_ui32_t*) malloc (
        p_indices->size * sizeof(deng_ui32_t)
    );
    res = fread(p_indices->p_indices, sizeof(deng_ui32_t), p_indices->size, file);

    if(!res) {
        perror("Failed to read INDX_HDR\n");
        exit(-1);
    }
}