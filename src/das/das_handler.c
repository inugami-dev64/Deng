#include <stdlib.h>
#define DAS_EXT_HANDLER
#include "../../headers/das/das_core.h"
 
/* Add .das file extension to the file name */
char *dasGetDASFileName(char *file_name) {
    size_t index;
    char *output = (char*) malloc (
        (strlen(file_name) + 3) * 
        sizeof(char)
    );

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
void dasAssemble (
    deng_Asset *p_asset, 
    const char *out_file
) {
    FILE *file;
    file = fopen (
        out_file, 
        "wb"
    );

    if(!file) {
        fprintf (
            stderr,
            "Failed to open file: %s\n",
            out_file
        );

        exit(EXIT_FAILURE);
    }
    

    dasAssembleINFOHDR (
        p_asset->id,
        p_asset->asset_mode,
        file
    );

    dasAssembleVERTHDR (
        &p_asset->vertices, 
        p_asset->asset_mode,
        file
    );
    
    dasAssembleINDXHDR (
        &p_asset->indices, 
        file
    );

    fclose(file);
}


/* das INFO_HDR assembler */
void dasAssembleINFOHDR (
    char *asset_name, 
    deng_AssetMode v_type, 
    FILE *file
) {
    deng_ui64_t timestamp = (deng_ui64_t) time(NULL);
    deng_ui8_t type = (deng_ui8_t) v_type;
    deng_ui8_t asset_name_size = (deng_ui8_t) strlen(asset_name);
    deng_ui32_t hdr_size = 22 + (deng_ui32_t) asset_name_size;
    
    printf("Timestamp: %ld\n", timestamp);
    fwrite(INFO_HEADER_NAME, sizeof(char), 8, file);
    fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
    fwrite(&timestamp, sizeof(deng_ui64_t), 1, file);
    fwrite(&type, sizeof(deng_ui8_t), 1, file);
    fwrite(&asset_name_size, sizeof(deng_ui8_t), 1, file);
    fwrite(asset_name, sizeof(char), strlen(asset_name), file);
}


/* das VERT_HDR assembler */
void dasAssembleVERTHDR (
    deng_VertDynamic *p_vertices, 
    deng_AssetMode asset_mode,
    FILE *file
) {
    deng_ui32_t hdr_vert_c = (deng_ui32_t) p_vertices->size;
    deng_ui32_t hdr_size = 0;
    
    fwrite(VERTICES_HEADER_NAME, sizeof(char), 8, file);
    switch(asset_mode) 
    {
    case DENG_ASSET_MODE_3D_UNMAPPED:
        hdr_size = 16 + 12 * hdr_vert_c;
        fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
        fwrite(&hdr_vert_c, sizeof(deng_ui32_t), 1, file);
        fwrite (
            p_vertices->p_unmapped_unnormalized_vert,
            sizeof(VERT_UNMAPPED_UNOR),
            p_vertices->size,
            file
        );
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        hdr_size = 16 + 20 * hdr_vert_c;
        fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
        fwrite(&hdr_vert_c, sizeof(deng_ui32_t), 1, file);
        fwrite (
            p_vertices->p_tex_mapped_unnormalized_vert,
            sizeof(VERT_MAPPED_UNOR),
            p_vertices->size,
            file
        );
        break;

    case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        hdr_size = 16 + 24 * hdr_vert_c;
        fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
        fwrite(&hdr_vert_c, sizeof(deng_ui32_t), 1, file);
        fwrite (
            p_vertices->p_unmapped_normalized_vert,
            sizeof(VERT_UNMAPPED_NOR),
            p_vertices->size,
            file
        );
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        printf("Write test\n");
        hdr_size = 16 + 32 * hdr_vert_c;
        fwrite(&hdr_size, sizeof(deng_ui32_t), 1, file);
        fwrite(&hdr_vert_c, sizeof(deng_ui32_t), 1, file);
        fwrite (
            p_vertices->p_tex_mapped_normalized_vert,
            sizeof(VERT_MAPPED_NOR),
            p_vertices->size,
            file
        );
        break;
    default:
        break;
    }
}


/* Write INDX_HDR to asset file */
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


/* das file reader callback function */
void dasReadAsset (
    deng_Asset *p_asset, 
    const char *file_name,
    deng_AssetMode f_asset_mode
) {
    FILE *file;
    file = fopen(file_name, "rb");
    
    /* Check if asset exists and if needed exit */
    if(!file) {
        printf("ERROR: Asset file '%s' does not exist!\n", file_name);
        exit(-1);
    }

    dasReadINFOHDR (
        &p_asset->id, 
        &p_asset->time_point, 
        &p_asset->asset_mode,
        (char*) file_name, 
        file
    );

    dasReadVERTHDR (
        &p_asset->vertices, 
        p_asset->asset_mode,
        f_asset_mode,
        (char*) file_name, 
        file
    );

    if(f_asset_mode != DENG_ASSET_MODE_DONT_CARE)
        p_asset->asset_mode = f_asset_mode;
    
    dasReadINDXHDR (
        &p_asset->indices, 
        (char*) file_name, 
        file
    );

    fclose(file);
}


/* Destroy all allocated assets */
void dasDestroyAssets (
    deng_Asset *assets,
    deng_i32_t asset_c
) {
    // O(n)
    deng_i32_t l_index;
    for(l_index = 0; l_index < asset_c; l_index++) {
        switch (assets[l_index].asset_mode)
        {
        case DENG_ASSET_MODE_2D_UNMAPPED:
            free (assets[l_index].vertices.p_unmapped_vert_data_2d);
            break;

        case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
            free(assets[l_index].vertices.p_tex_mapped_vert_data_2d);
            break;

        case DENG_ASSET_MODE_3D_UNMAPPED:
            free(assets[l_index].vertices.p_unmapped_unnormalized_vert);
            break;

        case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
            free(assets[l_index].vertices.p_unmapped_normalized_vert);
            break;

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
            free(assets[l_index].vertices.p_tex_mapped_unnormalized_vert);
            break;

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
            free(assets[l_index].vertices.p_tex_mapped_normalized_vert);
            break;
        
        default:
            break;
        }
    }
    
    free(assets[l_index].indices.p_indices);
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
    char **p_asset_name, 
    deng_ui64_t *p_timestamp, 
    deng_AssetMode *p_asset_mode,
    char *file_name, 
    FILE *file
) {
    deng_ui32_t hdr_size;
    deng_ui8_t name_size;
    char hdr_name[8];
    size_t res;
    res = fread(hdr_name, 8 * sizeof(char), 1, file);
    
    if(strncmp(hdr_name, INFO_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify INFO_HDR in asset file %s!\n", file_name);
        exit(EXIT_FAILURE);
    }

    res = fread(&hdr_size, sizeof(deng_ui32_t), 1, file);
    res = fread(p_timestamp, sizeof(deng_ui64_t), 1, file);
    
    deng_ui8_t mode = 0;
    res = fread(&mode, sizeof(deng_ui8_t), 1, file);
    *p_asset_mode = (deng_AssetMode) mode;

    res = fread(&name_size, sizeof(deng_ui8_t), 1, file);
    *p_asset_name = (char*) calloc((size_t) name_size + 1, sizeof(char));
    res = fread(*p_asset_name, sizeof(char), (size_t) name_size, file);

    if(!res) {
        perror("Failed to read data from INFO_HDR\n");
        exit(EXIT_FAILURE);
    }
}

void dasReadVERTHDR (
    deng_VertDynamic *p_vertices,
    deng_AssetMode file_asset_mode,
    deng_AssetMode new_asset_mode,
    char *file_name, 
    FILE *file
) {
    char hdr_name[8];
    deng_ui32_t hdr_size;
    deng_ui32_t vert_count;

    size_t res;
    res = fread(hdr_name, sizeof(char), 8, file);

    if(strncmp(hdr_name, VERTICES_HEADER_NAME, 8)) {
        printf("ERROR: Failed to verify VERT_HDR in asset file %s!\n", file_name);
        return;
    }

    res = fread(&hdr_size, sizeof(deng_ui32_t), 1, file);
    res = fread(&vert_count, sizeof(deng_ui32_t), 1, file);
    p_vertices->size = vert_count;

    char file_str[256];
    cm_LogWrite("vert_read.log", "#Entry point", true);

    switch(new_asset_mode)
    {
    case DENG_ASSET_MODE_3D_UNMAPPED:
        p_vertices->p_unmapped_unnormalized_vert = (VERT_UNMAPPED_UNOR*) calloc (
            p_vertices->size,
            sizeof(VERT_UNMAPPED_UNOR)
        );

        switch(file_asset_mode)
        {
        case DENG_ASSET_MODE_3D_UNMAPPED:
            res = fread (
                p_vertices->p_unmapped_unnormalized_vert, 
                sizeof(VERT_UNMAPPED_UNOR), 
                p_vertices->size, 
                file
            );
            break;

        case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED: {
            VERT_UNMAPPED_NOR *tmp_vert = (VERT_UNMAPPED_NOR*) calloc (
                p_vertices->size,
                sizeof(VERT_UNMAPPED_NOR)
            );

            res = fread (
                tmp_vert,
                sizeof(VERT_UNMAPPED_NOR),
                p_vertices->size,
                file
            );

            for(size_t i = 0; i < p_vertices->size; i++) {
                p_vertices->p_unmapped_unnormalized_vert[i].vert_data = tmp_vert[i].vert_data;
                p_vertices->p_unmapped_unnormalized_vert[i].color_data = tmp_vert[i].color_data;
            }

            free(tmp_vert);
            break;
        }

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED: {
            VERT_MAPPED_UNOR *tmp_vert = (VERT_MAPPED_UNOR*) calloc (
                p_vertices->size,
                sizeof(VERT_MAPPED_UNOR)
            );
            
            res = fread (
                tmp_vert,
                sizeof(VERT_MAPPED_UNOR),
                p_vertices->size,
                file
            );

            for(size_t i = 0; i < p_vertices->size; i++) {
                p_vertices->p_unmapped_unnormalized_vert[i].vert_data = tmp_vert[i].vert_data;
                p_vertices->p_unmapped_unnormalized_vert[i].color_data = DEFAULT_ASSET_COLOR;
            }

            free(tmp_vert);
            break;
        }

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED: {
            VERT_MAPPED_NOR *tmp_vert = (VERT_MAPPED_NOR*) calloc (
                p_vertices->size,
                sizeof(VERT_MAPPED_NOR)
            );

            res = fread (
                tmp_vert,
                sizeof(VERT_MAPPED_NOR),
                p_vertices->size,
                file
            );

            for(size_t i = 0; i < p_vertices->size; i++) {
                p_vertices->p_unmapped_unnormalized_vert[i].vert_data = tmp_vert[i].vert_data;
                p_vertices->p_unmapped_unnormalized_vert[i].color_data = DEFAULT_ASSET_COLOR;
            }

            free(tmp_vert);
            break;
        }

        default:
            break;
        }

        break;

    case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        p_vertices->p_unmapped_normalized_vert = (VERT_UNMAPPED_NOR*) calloc (
            p_vertices->size,
            sizeof(VERT_UNMAPPED_NOR)
        );

        switch(file_asset_mode)
        {
        case DENG_ASSET_MODE_3D_UNMAPPED:
        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
            printf("test\n");
            fprintf (
                stderr,
                "Cannot read normalised vertices from non normalised asset file\n"
            );
            exit(EXIT_FAILURE);
            break;

        case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED: {
            res = fread (
                p_vertices->p_unmapped_normalized_vert,
                sizeof(VERT_UNMAPPED_NOR),
                p_vertices->size,
                file
            );
            break;
        }

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED: {
            VERT_MAPPED_NOR *tmp_vert = (VERT_MAPPED_NOR*) calloc (
                p_vertices->size,
                sizeof(VERT_MAPPED_NOR)
            );

            res = fread (
                tmp_vert,
                sizeof(VERT_MAPPED_NOR),
                p_vertices->size,
                file
            );

            for(size_t i = 0; i < p_vertices->size; i++) {
                p_vertices->p_unmapped_normalized_vert[i].vert_data = tmp_vert[i].vert_data;
                p_vertices->p_unmapped_normalized_vert[i].norm_data = tmp_vert[i].norm_data;
                p_vertices->p_unmapped_normalized_vert[i].color_data = DEFAULT_ASSET_COLOR;
            }

            free(tmp_vert);
            break;
        }

        default:
            break;
        }
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        p_vertices->p_tex_mapped_unnormalized_vert = (VERT_MAPPED_UNOR*) calloc (
            p_vertices->size,
            sizeof(VERT_MAPPED_UNOR)
        );

        switch(file_asset_mode)
        {
        case DENG_ASSET_MODE_3D_UNMAPPED:
        case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
            fprintf (
                stderr,
                "Cannot read texture vertices from non texture mapped asset file\n"
            );
            exit(EXIT_FAILURE);
            break;

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED: {
            res = fread (
                p_vertices->p_tex_mapped_unnormalized_vert,
                sizeof(VERT_MAPPED_UNOR),
                p_vertices->size,
                file
            );
            break;
        }

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED: {
            VERT_MAPPED_NOR *tmp_vert = (VERT_MAPPED_NOR*) calloc (
                p_vertices->size,
                sizeof(VERT_MAPPED_NOR)
            );
            
            res = fread (
                tmp_vert,
                sizeof(VERT_MAPPED_NOR),
                p_vertices->size,
                file
            );

            for(size_t i = 0; i < p_vertices->size; i++) {
                printf("Hello test\n");
                sprintf (
                    file_str,
                    "%f,%f,%f | %f,%f | %f,%f,%f",
                    tmp_vert[i].vert_data.vert_x,
                    tmp_vert[i].vert_data.vert_y,
                    tmp_vert[i].vert_data.vert_z,
                    tmp_vert[i].tex_data.tex_x,
                    tmp_vert[i].tex_data.tex_y,
                    tmp_vert[i].norm_data.nor_x,
                    tmp_vert[i].norm_data.nor_y,
                    tmp_vert[i].norm_data.nor_z
                );
                cm_LogWrite("vert_read.log", file_str, false);
                memset(file_str, 0, 256);

                p_vertices->p_tex_mapped_unnormalized_vert[i].vert_data = tmp_vert[i].vert_data;
                p_vertices->p_tex_mapped_unnormalized_vert[i].tex_data = tmp_vert[i].tex_data;
            }

            free(tmp_vert);
            break;
        }

        default:
            break;
        }
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        p_vertices->p_tex_mapped_normalized_vert = (VERT_MAPPED_NOR*) calloc (
            p_vertices->size,
            sizeof(VERT_MAPPED_NOR)
        );

        switch(file_asset_mode)
        {
        case DENG_ASSET_MODE_3D_UNMAPPED:
        case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
            fprintf (
                stderr,
                "Cannot read texture or normal vertices from non texture mapped or non normalised asset file\n"
            );
            exit(EXIT_FAILURE);
            break;

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED: {
            res = fread (
                p_vertices->p_tex_mapped_normalized_vert,
                sizeof(VERT_UNMAPPED_NOR),
                p_vertices->size,
                file
            );
            break;
        }

        default:
            break;
        }
        break;


    case DENG_ASSET_MODE_DONT_CARE:
        switch(file_asset_mode)
        {
        case DENG_ASSET_MODE_3D_UNMAPPED:
            p_vertices->p_unmapped_unnormalized_vert = (VERT_UNMAPPED_UNOR*) calloc (
                p_vertices->size,
                sizeof(VERT_UNMAPPED_UNOR)
            );

            res = fread (
                p_vertices->p_unmapped_unnormalized_vert, 
                sizeof(VERT_UNMAPPED_UNOR), 
                p_vertices->size, 
                file
            );
            break;

        case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
            p_vertices->p_unmapped_normalized_vert = (VERT_UNMAPPED_NOR*) calloc (
                p_vertices->size,
                sizeof(VERT_UNMAPPED_NOR)
            );

            res = fread (
                p_vertices->p_unmapped_normalized_vert,
                sizeof(VERT_MAPPED_NOR),
                p_vertices->size,
                file
            );
            break;
        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
            p_vertices->p_tex_mapped_unnormalized_vert = (VERT_MAPPED_UNOR*) calloc (
                p_vertices->size,
                sizeof(VERT_MAPPED_UNOR)
            );

            res = fread (
                p_vertices->p_tex_mapped_unnormalized_vert,
                sizeof(VERT_MAPPED_UNOR),
                p_vertices->size,
                file
            );
            break;

        case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
            printf("Loading textured normalised vertices\n");
            p_vertices->p_tex_mapped_normalized_vert = (VERT_MAPPED_NOR*) calloc (
                p_vertices->size,
                sizeof(VERT_MAPPED_NOR)
            );

            res = fread (
                p_vertices->p_tex_mapped_normalized_vert,
                sizeof(VERT_MAPPED_NOR),
                p_vertices->size,
                file
            );
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

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
    res = fread(hdr_name, sizeof(char), 8, file);
    printf("%s\n", hdr_name);
    
    if(strncmp(hdr_name, INDICES_HEADER_NAME, 8)) {
        printf (
            "ERROR: Failed to verify INDX_HDR in asset file %s!\n", 
            file_name
        );
        exit(EXIT_FAILURE);
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
