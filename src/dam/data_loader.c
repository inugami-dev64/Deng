#define DAS_EXT_LOADERS
#define DAS_GENERIC_HELPERS
#include "das_core.h"

 FILE *file;

/* Generic helper functions */
void dasCleanCharBuffer(char *buffer, size_t size) {
    for(size_t index = 0; index < size; index++)
        buffer[index] = 0x00;
}

/* Image headers initialisation */
void dasInitBMPimageHeaders(BMPFileHeader *p_file_header, BMPInfoHeader *p_info_header, BMPColorHeader *p_color_header) {
    *p_file_header = (BMPFileHeader) {0x4D42, 0, 0, 0, 0};
    *p_info_header = (BMPInfoHeader) {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    *p_color_header = (BMPColorHeader) {0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000, 0x73524742, {0}};
}

void dasInitTGAimageHeaders(TGATypeHeader *p_type_header, TGAColorMapHeader *p_color_map_header, TGAInfoHeader *p_info_header) {
    *p_type_header = (TGATypeHeader) {0, 0, 0};
    *p_color_map_header = (TGAColorMapHeader) {0, 0, 0};
    *p_info_header = (TGAInfoHeader) {0, 0, 0, 0, 0, 0};
}

/* Image loaders */

// Detect image format
 ImageFormat dasDetectImageFormat(const char *file_name) {
    char *ext = cm_GetFileExtName(file_name);

    if(!strcmp(ext, "tga")) return IMAGE_FORMAT_TGA;
    else if(!strcmp(ext, "bmp")) return IMAGE_FORMAT_BMP;
    else if(!strcmp(ext, "png")) return IMAGE_FORMAT_PNG;
    else if(!strcmp(ext, "jpg")) return IMAGE_FORMAT_JPG; 

    free(ext);
    return IMAGE_FORMAT_UNKNOWN; 
}

// Callback function to get image data
void dasLoadTexture(DENGtexture *p_texture, const char *file_name) {
    ImageFormat format = dasDetectImageFormat(file_name);

    switch (format)
    {
    case IMAGE_FORMAT_BMP:
        printf("Detected BMP texture\n");
        dasLoadBMPimage(p_texture, file_name);
        break;

    case IMAGE_FORMAT_TGA:
        printf("Detected TGA texture\n");
        dasLoadTGAimage(p_texture, file_name);
        break;
    
    default:
        break;
    }
}


/* BMP image */
 void dasLoadBMPimage(DENGtexture *p_texture, const char *file_name) {
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    BMPColorHeader color_header;
    dasInitBMPimageHeaders(&file_header, &info_header, &color_header);
    p_texture->pixel_data.p_pixel_data = (uint8_t*) malloc(sizeof(uint8_t));
    file = fopen(file_name, "rb");

    if(!file) {
        printf("%s%s%s\n", "ERROR: Failed to load texture image file ", file_name, "!");
        return;
    }

    /* BMP image verifications */
    fread(&file_header, sizeof(file_header), 1, file);

    if(file_header.file_type != 0x4D42) {
        printf("%s%s%s\n", "ERROR: Unrecognised or corrupt texture file ", file_name, "!");
        return;
    }

    fread(&info_header, sizeof(info_header), 1, file);

    if(info_header.bit_count == 32 && info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
        fread(&color_header, sizeof(color_header), 1, file);
        int verify_color_data = color_header.alpha_mask == 0xff000000 &&
                                color_header.blue_mask == 0x000000ff &&
                                color_header.green_mask == 0x0000ff00 &&
                                color_header.red_mask == 0x00ff0000 && 
                                color_header.color_space_type == 0x73524742;
        
        if(!verify_color_data) {
            printf("%s%s%s\n", "ERROR: Unexpected color mask format or color space type in file ", file_name, "!");
            return;
        }
    }

    else if(info_header.bit_count == 32 && !(info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader)))) {
        printf("%s%s%s\n", "ERROR: Texture file ", file_name, " doesn't contain bit mask information!");
        return;
    }

    fseek(file, (long) file_header.offset_data, SEEK_SET);

    if(info_header.bit_count == 32) {
        info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
    }

    else if(info_header.bit_count == 24) {
        info_header.size = sizeof(BMPInfoHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    }

    else {
        printf("%s%s%s\n", "ERROR: Unsupported bit count for texture file ", file_name, "!");
        return;
    }

    file_header.file_size = file_header.offset_data;
    p_texture->pixel_data.width = (uint16_t) info_header.width;
    p_texture->pixel_data.height = (uint16_t) info_header.height;

    p_texture->pixel_data.size = info_header.height * info_header.width * 4;
    p_texture->pixel_data.p_pixel_data = (uint8_t*) realloc((void*) p_texture->pixel_data.p_pixel_data, p_texture->pixel_data.size);
    cm_CheckMemoryAlloc((void*) p_texture->pixel_data.p_pixel_data);

    /* No padding */
    if(info_header.bit_count == 32) {
        size_t w_index, data_index;
        int h_index;
        uint8_t tmp_arr[info_header.height][4 * info_header.width];
        
        for(h_index = 0; h_index < (int) info_header.height; h_index++) 
            fread(tmp_arr[(size_t) h_index], 4 * info_header.width * sizeof(uint8_t), 1, file);

        for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--)
            for(w_index = 0; w_index < 4 * info_header.width; w_index++, data_index++)
                p_texture->pixel_data.p_pixel_data[data_index] = tmp_arr[(size_t) h_index][w_index];
    }
    /* With padding */
    else {
        size_t w_index, data_index = 0;
        int h_index;

        long padding_count_per_row = 4 - ((long) info_header.width * (long) info_header.bit_count) / 8 % 4;
        uint8_t tmp_arr[info_header.height][3 * info_header.width];
        
        for(h_index = 0; h_index < (int) info_header.height; h_index++) {
            fread(tmp_arr[(size_t) h_index], 3 * info_header.width * sizeof(uint8_t), 1, file);
            fseek(file, padding_count_per_row, SEEK_CUR);
        }

        for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--) {
            for(w_index = 0; w_index < info_header.width * 3; w_index += 3, data_index += 4) {
                p_texture->pixel_data.p_pixel_data[data_index] = tmp_arr[(size_t) h_index][w_index];
                p_texture->pixel_data.p_pixel_data[data_index + 1] = tmp_arr[(size_t) h_index][w_index + 1];
                p_texture->pixel_data.p_pixel_data[data_index + 2] = tmp_arr[(size_t) h_index][w_index + 2];
                p_texture->pixel_data.p_pixel_data[data_index + 3] = 0xFF;
            }
        }
    }

    fclose(file);
}


/* TGA image */
 void dasLoadTGAimage(DENGtexture *p_asset, const char *file_name) {
    TGATypeHeader type_header;
    TGAColorMapHeader color_header;
    TGAInfoHeader info_header;
    dasInitTGAimageHeaders(&type_header, &color_header, &info_header);
    p_asset->pixel_data.p_pixel_data = (uint8_t*) malloc(sizeof(uint8_t));

    file = fopen(file_name, "rb");
    if(!file) {
        printf("%s%s%s\n", "Failed to load texture image file ", file_name, "!");
        return;
    }

    fread((void*) &type_header, sizeof(type_header), 1, file);

    printf("Read image type is: %d\n", type_header.image_type);
    // Check the tga image type and exit if image is not supported
    switch (type_header.image_type)
    {
    case 0:
        printf("%s\n", "ERROR: No texture image data available!");
        return;

    case 1:
        printf("%s\n", "ERROR: Color-mapped tga image is not supported!");
        return;
    
    case 3:
        printf("%s\n", "ERROR: Grayscale tga image is not supported!");
        return;

    case 9:
        printf("%s\n", "ERROR: RLE color-mapped tga image is not supported!");
        return;
    
    case 10:
        printf("%s\n", "ERROR: RLE true-color tga image is not supported!");
        return;  
    
    case 11:
        printf("%s\n", "ERROR: RLE grayscale tga image is not supported!");
        return;
    
    default:
        break;
    }

    fread((void*) &color_header, sizeof(color_header), 1, file);
    fread((void*) &info_header, sizeof(info_header), 1, file);
    
    p_asset->pixel_data.size = (size_t) (info_header.height * info_header.width * 4);
    p_asset->pixel_data.p_pixel_data = (uint8_t*) realloc(p_asset->pixel_data.p_pixel_data, p_asset->pixel_data.size);
    cm_CheckMemoryAlloc(p_asset->pixel_data.p_pixel_data);
    
    switch (info_header.bit_count)
    {
    case 32: {
        /* Check if pixel row rearrangement has to be done */
        if(info_header.y_origin != info_header.height) {
            size_t data_index, w_index;
            int h_index;
            uint8_t tmp_arr[info_header.height][info_header.width * 4];

            for(h_index = 0; h_index < (int) info_header.height; h_index++)
                fread(tmp_arr[(size_t) h_index], 4 * info_header.width * sizeof(uint8_t), 1, file);

            for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--) {
                for(w_index = 0; w_index < info_header.width * 4; w_index++, data_index++)
                    p_asset->pixel_data.p_pixel_data[data_index] = tmp_arr[(int) h_index][w_index];
            }
        }

        else 
            fread(p_asset->pixel_data.p_pixel_data, p_asset->pixel_data.size * sizeof(uint8_t), 1, file);
        break;
    }

    case 24: {
        if(info_header.y_origin != info_header.height) {
            size_t data_index, w_index; 
            int h_index;
            uint8_t tmp_arr[info_header.height][info_header.width * 3];

            for(h_index = 0; h_index < info_header.height; h_index++)
                fread(tmp_arr[(size_t) h_index], 3 * info_header.width * sizeof(uint8_t), 1, file);

            for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--) {
                for(w_index = 0; w_index < (int) info_header.width * 3; w_index += 3, data_index += 4) {
                    p_asset->pixel_data.p_pixel_data[data_index] = tmp_arr[(size_t) h_index][(size_t) w_index];
                    p_asset->pixel_data.p_pixel_data[data_index + 1] = tmp_arr[(size_t) h_index][(size_t) w_index + 1];
                    p_asset->pixel_data.p_pixel_data[data_index + 2] = tmp_arr[(size_t) h_index][(size_t) w_index + 2];
                    p_asset->pixel_data.p_pixel_data[data_index + 3] = 0xFF;
                }
            }
        }

        else {
            size_t tmp_index, data_index;
            uint8_t tmp_arr[info_header.height * info_header.width * 3];
            fread(tmp_arr, 3 * info_header.width * info_header.height * sizeof(uint8_t), 1, file);

            for(tmp_index = 0, data_index = 0; data_index < p_asset->pixel_data.size; data_index += 4, tmp_index += 3) {
                p_asset->pixel_data.p_pixel_data[data_index] = tmp_arr[tmp_index];
                p_asset->pixel_data.p_pixel_data[data_index + 1] = tmp_arr[tmp_index + 1];
                p_asset->pixel_data.p_pixel_data[data_index + 2] = tmp_arr[tmp_index + 2];
                p_asset->pixel_data.p_pixel_data[data_index + 3] = 0xFF;
            }
        }
    }
    
    default:
        break;
    }

    p_asset->pixel_data.width = info_header.width;
    p_asset->pixel_data.height = info_header.height;

    fclose(file);
}


/* 3D model loading */
// Callback function for loading 3d model data
void dasLoadModel(DENGasset *p_asset, const char *file_name) {
    file = fopen(file_name, "rb");
    if(!file) printf("ERROR: Failed to open model file: %s\n", file_name);

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Vertices data variables
    OBJVerticesData *p_vertices_data = NULL;
    size_t vertices_size = 0;
    OBJTextureData *p_texture_data = NULL;
    size_t texture_size = 0;

    // Indices data variables
    uint32_t *p_vertices_indices = NULL;
    size_t vertices_indices_size = 0;
    uint32_t *p_tex_vert_indices = NULL;
    size_t tex_vert_indices_size = 0;

    char *file_ext = cm_GetFileExtName(file_name);
    if(!(file_ext && !strcmp(file_ext, "obj"))) {
        printf("Incorrect object file format!\n");
        return;
    }

    dasLoadOBJmodelVertices(&p_vertices_data, &vertices_size, &p_texture_data, &texture_size, file_size);
    dasLoadOBJindices(&p_vertices_indices, &vertices_indices_size, &p_tex_vert_indices, &tex_vert_indices_size, file_size);

    dasIndexTexMappedVertices(&p_vertices_data, vertices_size, &p_texture_data, texture_size, &p_vertices_indices, vertices_indices_size, &p_tex_vert_indices,
    tex_vert_indices_size, p_asset);
    p_asset->tex_mode = 1;
    
    fclose(file);
}


/* OBJ file format */
// Read vertices information from the file
 void dasLoadOBJmodelVertices(OBJVerticesData **pp_vert_data, size_t *p_vert_size, OBJTextureData **pp_texture_data, size_t *p_tex_size, long file_size) {
    size_t index;
    unsigned char prev_tmp = 0x00, tmp = 0x00, next_tmp = 0x00;
    char x_buffer[12], y_buffer[12], z_buffer[12];
    dasCleanCharBuffer(x_buffer, 12);
    dasCleanCharBuffer(y_buffer, 12);
    dasCleanCharBuffer(z_buffer, 12);
    
    *pp_vert_data = (OBJVerticesData*) malloc(sizeof(OBJVerticesData));
    *pp_texture_data = (OBJTextureData*) malloc(sizeof(OBJTextureData));

    // According to ASCII table 0x0A = \n; 0x20 = ' '; 0x76 = 'v' 
    /* Skip the useless metadata and find the nearest vertex value */ 
    while(!(prev_tmp == 0x0A && tmp == 0x76 && next_tmp == 0x20) && ftell(file) < file_size) {
        prev_tmp = tmp;
        tmp = next_tmp;
        fread(&next_tmp, sizeof(next_tmp), 1, file);
    }

    /* Find all vertices coordinates in .obj file */
    while(prev_tmp == 0x0A && tmp == 0x76 && next_tmp == 0x20 && ftell(file) < file_size) {
        /* Get x coordinate data */
        for(index = 0; tmp != 0x20; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x20) x_buffer[index] = tmp;
        }

        while(tmp == 0x20) fread(&tmp, sizeof(tmp), 1, file);

        /* Get y coordinate data */
        y_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x20) y_buffer[index] = tmp;
        }

        while(tmp == 0x20) fread(&tmp, sizeof(tmp), 1, file);

        /* Get z coordinate data */
        z_buffer[0] = tmp;
        for(index = 1; tmp != 0x0A; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x0A) z_buffer[index] = tmp;
        }

        while(tmp == 0x0A) {
            prev_tmp = tmp;
            fread(&tmp, sizeof(tmp), 1, file); 
        }

        fread(&next_tmp, sizeof(next_tmp), 1, file);

        (*p_vert_size)++;
        *pp_vert_data = (OBJVerticesData*) realloc(*pp_vert_data, (*p_vert_size) * sizeof(OBJVerticesData));
        cm_CheckMemoryAlloc(*pp_vert_data);
        (*pp_vert_data)[(*p_vert_size) - 1] = (OBJVerticesData) {(float) atof(x_buffer), (float) atof(y_buffer), (float) atof(z_buffer)};

        dasCleanCharBuffer(x_buffer, 12);
        dasCleanCharBuffer(y_buffer, 12);
        dasCleanCharBuffer(z_buffer, 12);
    }

    /* Find all texture map coordinate data in .obj file */
    while(prev_tmp == 0x0A && tmp == 0x76 && next_tmp == 0x74 && ftell(file) < file_size) {
        /* Get x coordinate data */
        fseek(file, 1L, SEEK_CUR);
        for(index = 0; tmp != 0x20; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x20) x_buffer[index] = tmp;
        }

        while(tmp == 0x20) fread(&tmp, sizeof(tmp), 1, file);

        /* Get y coordinate data */
        y_buffer[0] = tmp;
        for(index = 1; tmp != 0x0A && ftell(file) < file_size; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x0A) y_buffer[index] = tmp;
        }

        while(tmp == 0x0A) {
            prev_tmp = tmp;
            fread(&tmp, sizeof(tmp), 1, file);
        }

        fread(&next_tmp, sizeof(next_tmp), 1, file);

        (*p_tex_size)++;
        *pp_texture_data = (OBJTextureData*) realloc(*pp_texture_data, (*p_tex_size) * sizeof(OBJTextureData));
        cm_CheckMemoryAlloc(*pp_texture_data);
        (*pp_texture_data)[(*p_tex_size) - 1] = (OBJTextureData) {(float) atof(x_buffer), (float) atof(y_buffer)}; 
        
        dasCleanCharBuffer(x_buffer, 12);
        dasCleanCharBuffer(y_buffer, 12);
    }
    // Position of object's file stream as right now is at the beginning of vertex normals reading 
}


/* Read obj file indices information */
 void dasLoadOBJindices(uint32_t **pp_vert_indices, size_t *p_vert_indices_size, uint32_t **pp_tex_vert_indices, size_t *p_tex_vert_indices_size, long file_size) {
    size_t index;
    unsigned char prev_tmp = 0x00, tmp = 0x00, next_tmp = 0x00;
    char vert_index_buffer[32], tex_vert_buffer[32];
    dasCleanCharBuffer(vert_index_buffer, 32);
    dasCleanCharBuffer(tex_vert_buffer, 32);

    *pp_vert_indices = (uint32_t*) malloc(sizeof(uint32_t));
    *pp_tex_vert_indices = (uint32_t*) malloc(sizeof(uint32_t));

    /* Skip all vertex normals */
    while(!(prev_tmp == 0x0A && tmp == 0x66 && next_tmp == 0x20) && ftell(file) < file_size) {
        prev_tmp = tmp;
        tmp = next_tmp;
        fread(&next_tmp, sizeof(next_tmp), 1, file);
    }

    // According to ASCII table 0x2F = '/' 
    while(prev_tmp == 0x0A && tmp == 0x66 && next_tmp == 0x20 && ftell(file) < file_size) {
        // I faces
        for(size_t n = 0; n < 3; n++) {
            /* Read vertex index value */
            for(index = 0; tmp != 0x2F; index++) {
                fread(&tmp, sizeof(tmp), 1, file);
                if(tmp != 0x2F) vert_index_buffer[index] = tmp;
            } 

            tmp = 0x00;

            /* Read texture vertex value */
            for(index = 0; tmp != 0x2F; index++) {
                fread(&tmp, sizeof(tmp), 1, file);
                if(tmp != 0x2F) tex_vert_buffer[index] = tmp;
            }

            /* Skip vertex normal indices */
            while((tmp != 0x20 && tmp != 0x0A) && ftell(file) < file_size) fread(&tmp, sizeof(tmp), 1, file);
            /* Skip spaces */
            while((tmp == 0x20 || tmp == 0x0A) && ftell(file) < file_size) {
                if(n == 2) prev_tmp = tmp;
                fread(&tmp, sizeof(tmp), 1, file);
            }

            if(n != 2) fseek(file, -1L, SEEK_CUR);   
            else fread(&next_tmp, sizeof(next_tmp), 1, file);

            (*p_vert_indices_size)++;
            (*p_tex_vert_indices_size)++;
            *pp_vert_indices = (uint32_t*) realloc(*pp_vert_indices, (*p_vert_indices_size) * sizeof(uint32_t));
            cm_CheckMemoryAlloc(*pp_vert_indices);
            *pp_tex_vert_indices = (uint32_t*) realloc(*pp_tex_vert_indices, (*p_tex_vert_indices_size) * sizeof(uint32_t)); 
            cm_CheckMemoryAlloc(*pp_tex_vert_indices);
            (*pp_vert_indices)[(*p_vert_indices_size) - 1] = (uint32_t) atoi(vert_index_buffer) - 1;
            (*pp_tex_vert_indices)[(*p_tex_vert_indices_size) - 1] = (uint32_t) atoi(tex_vert_buffer) - 1;

            dasCleanCharBuffer(vert_index_buffer, 32);
            dasCleanCharBuffer(tex_vert_buffer, 32);
        }
    }
}


// Index unmapped vertices
 void dasIndexUnmappedVertices(OBJVerticesData **pp_vert_data, size_t vert_size, uint32_t **pp_indices, size_t indices_size, DENGasset *p_asset) {
    p_asset->vertices.p_unmapped_vert_data = (OBJVerticesData*) malloc(vert_size * sizeof(OBJVerticesData));
    p_asset->vertices.size = vert_size;
    
    p_asset->indices.p_indices = (uint32_t*) malloc(indices_size * sizeof(uint32_t));
    p_asset->indices.size = indices_size;

    size_t index;
    for(index = 0; index < vert_size; index++)
        p_asset->vertices.p_unmapped_vert_data[index] = (*pp_vert_data)[index];

    for(index = 0; index < indices_size; index++)
        p_asset->indices.p_indices[index] = (*pp_indices)[index]; 
}


// Index texture mapped vertices
 void dasIndexTexMappedVertices(OBJVerticesData **pp_vert_data, size_t vert_size, OBJTextureData **pp_tex_data, size_t tex_size, uint32_t **pp_vert_indices, size_t vert_indices_size, uint32_t **pp_tex_indices, 
size_t tex_indices_size, DENGasset *p_asset) {
    size_t l_index, r_index;
    int is_found;
    p_asset->vertices.p_texture_mapped_vert_data = (VERT_MAPPED*) malloc(sizeof(VERT_MAPPED));
    p_asset->vertices.size = 0;

    p_asset->indices.p_indices = (uint32_t*) malloc(vert_indices_size * sizeof(uint32_t));
    p_asset->indices.size = vert_indices_size; 

    VERT_MAPPED *p_tmp_vert = (VERT_MAPPED*) malloc(vert_indices_size * sizeof(VERT_MAPPED));

    for(l_index = 0; l_index < vert_indices_size; l_index++) {
        // printf("Hello!\n");
        p_tmp_vert[l_index].vert_data = (*pp_vert_data)[(size_t) (*pp_vert_indices)[l_index]];
        p_tmp_vert[l_index].tex_data = (*pp_tex_data)[(size_t) (*pp_tex_indices)[l_index]];
    }

    for(l_index = 0; l_index < vert_indices_size; l_index++) {
        // Iterate through output array to find equal value with p_tmp_vert values
        is_found = false;
        for(r_index = 0; r_index < p_asset->vertices.size; r_index++) {
            if(p_asset->vertices.p_texture_mapped_vert_data[r_index].vert_data.vert_x == p_tmp_vert[l_index].vert_data.vert_x &&
               p_asset->vertices.p_texture_mapped_vert_data[r_index].vert_data.vert_y == p_tmp_vert[l_index].vert_data.vert_y &&
               p_asset->vertices.p_texture_mapped_vert_data[r_index].vert_data.vert_z == p_tmp_vert[l_index].vert_data.vert_z &&
               p_asset->vertices.p_texture_mapped_vert_data[r_index].tex_data.tex_x == p_tmp_vert[l_index].tex_data.tex_x && 
               p_asset->vertices.p_texture_mapped_vert_data[r_index].tex_data.tex_y == p_tmp_vert[l_index].tex_data.tex_y)  {
                is_found = true;
                break;
            }
        }

        // Check if previous vertex instance is found and then index it
        if(!is_found) {
            p_asset->vertices.size++;
            p_asset->vertices.p_texture_mapped_vert_data = (VERT_MAPPED*) realloc((void*) p_asset->vertices.p_texture_mapped_vert_data, p_asset->vertices.size * sizeof(VERT_MAPPED));
            cm_CheckMemoryAlloc((void*) p_asset->vertices.p_texture_mapped_vert_data);
            
            p_asset->vertices.p_texture_mapped_vert_data[p_asset->vertices.size - 1] = p_tmp_vert[l_index];
            p_asset->indices.p_indices[l_index] = p_asset->vertices.size - 1;
        }

        else p_asset->indices.p_indices[l_index] = (uint32_t) r_index;
    }   

    free(p_tmp_vert); 
}