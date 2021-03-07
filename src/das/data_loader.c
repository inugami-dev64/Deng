#define __DATA_LOADER_C
#include <das/data_loader.h>

static FILE *file;

/* Generic helper functions */
void dasCleanCharBuffer(char *buffer, size_t size) {
    for(size_t index = 0; index < size; index++)
        buffer[index] = 0x00;
}

/* Image headers initialisation */
void dasInitBMPimageHeaders (
    BMPFileHeader *p_file_header, 
    BMPInfoHeader *p_info_header, 
    BMPColorHeader *p_color_header
) {
    *p_file_header = (BMPFileHeader) { 
        0x4D42, 
        0, 0, 0, 0
    };

    *p_info_header = (BMPInfoHeader) {
        0, 0, 0, 1, 
        0, 0, 0, 0, 
        0, 0, 0
    };

    *p_color_header = (BMPColorHeader) {
        0x00ff0000, 
        0x0000ff00, 
        0x000000ff, 
        0xff000000, 
        0x73524742, 
        {0}
    };
}

void dasInitTGAimageHeaders (
    TGATypeHeader *p_type_header, 
    TGAColorMapHeader *p_color_map_header, 
    TGAInfoHeader *p_info_header
) {
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
void dasLoadTexture(deng_Texture *p_texture, const char *file_name) {
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


/*
 * Read raw bitmap data from file
 * This function expects the file to be uncompressed and color depth either 24 bits or 32 bits
 */
void dasReadBitmap (
    FILE* file,
    deng_ui8_t bit_c,
    deng_bool_t vert_re,
    deng_Texture *p_tex
) {
    deng_ui8_t *tmp_pix;
    size_t offset = 0, offseta;
    deng_i32_t i, k;
    size_t j;

    switch (bit_c)
    {
    case 24:
        offset = 3 * p_tex->pixel_data.width * sizeof(deng_ui8_t);
        offseta = 4 * p_tex->pixel_data.width * sizeof(deng_ui8_t);

        tmp_pix = (deng_ui8_t*)calloc(
            p_tex->pixel_data.height * offset,
            sizeof(deng_ui8_t)
        );

        fread(
            tmp_pix,
            offset,
            p_tex->pixel_data.height,
            file
        );
        
        // Check if pixel reaarangement should be done (bottom - left to top - left)
        printf("Texture size: %d, %d\n", p_tex->pixel_data.width, p_tex->pixel_data.height);
        if (vert_re) {
            for (i = (deng_i32_t) p_tex->pixel_data.height - 1, k = 0; i >= 0; i--, k++) {
                for (j = 0; j < p_tex->pixel_data.width; j++) {
                    *(p_tex->pixel_data.p_pixel_data + k * offseta + j * 4) = *(tmp_pix + i * offset + j * 3);
                    *(p_tex->pixel_data.p_pixel_data + k * offseta + j * 4 + 1) = *(tmp_pix + i * offset + j * 3 + 1);
                    *(p_tex->pixel_data.p_pixel_data + k * offseta + j * 4 + 2) = *(tmp_pix + i * offset + j * 3 + 2);
                    *(p_tex->pixel_data.p_pixel_data + k * offseta + j * 4 + 3) = 0xff;
                }
            }
        }

        else {
            for (i = 0; i < (deng_i32_t)p_tex->pixel_data.height; i++) {
                for (j = 0; j < p_tex->pixel_data.width; j++) {
                    *(p_tex->pixel_data.p_pixel_data + i * offseta + j * 4) = *(tmp_pix + i * offset + j * 3);
                    *(p_tex->pixel_data.p_pixel_data + i * offseta + j * 4 + 1) = *(tmp_pix + i * offset + j * 3 + 1);
                    *(p_tex->pixel_data.p_pixel_data + i * offseta + j * 4 + 2) = *(tmp_pix + i * offset + j * 3 + 2);
                    *(p_tex->pixel_data.p_pixel_data + i * offseta + j * 4 + 3) = 0xff;
                }
            }
        }

        free(tmp_pix);
        break;

    case 32:
        offset = 4 * p_tex->pixel_data.width * sizeof(deng_ui8_t);
           
        if (!vert_re) {
            fread (
                p_tex->pixel_data.p_pixel_data, 
                sizeof(deng_ui8_t), 
                p_tex->pixel_data.size, 
                file
            );
        }

        else {
            tmp_pix = (deng_ui8_t*)calloc(
                p_tex->pixel_data.height,
                offset
            );

            fread(
                tmp_pix,
                offset,
                p_tex->pixel_data.height,
                file
            );

            for (i = p_tex->pixel_data.height; i >= 0; i--)
                memcpy(p_tex->pixel_data.p_pixel_data, tmp_pix + i * offset, offset);
        }

        break;

    default:
        printf("Invalid bit count\n");
        break;
    }

    // LOG PIXELS 
    /*cm_OpenLogger("frag.log");
    cm_LogWrite("frag.log", "#ENTRY POINT");
    char buf[2048];
    for (j = 0; j < p_tex->pixel_data.size - 4; j += 4) {
        sprintf(
            buf,
            "PIX: %d, %d, %d, %d",
            p_tex->pixel_data.p_pixel_data[j],
            p_tex->pixel_data.p_pixel_data[j + 1],
            p_tex->pixel_data.p_pixel_data[j + 2],
            p_tex->pixel_data.p_pixel_data[j + 3]
        );
        cm_LogWrite(buf);
    }
    cm_CloseLogger();*/
}


/* BMP image */
 void dasLoadBMPimage(deng_Texture *p_texture, const char *file_name) {
    size_t res;
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    BMPColorHeader color_header;
    dasInitBMPimageHeaders (
        &file_header, 
        &info_header, 
        &color_header
    );
    file = fopen(file_name, "rb");

    if(!file) {
        printf("%s%s%s\n", "ERROR: Failed to load texture image file ", file_name, "!");
        return;
    }

    /* BMP image verifications */
    res = fread (
        &file_header, 
        sizeof(file_header), 
        1, 
        file
    );

    if(file_header.file_type != 0x4D42) {
        printf("%s%s%s\n", "ERROR: Unrecognised or corrupt texture file ", file_name, "!");
        return;
    }

    res = fread (
        &info_header, 
        sizeof(info_header), 
        1, 
        file
    );

    if(info_header.bit_count == 32 && info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
        res = fread(&color_header, sizeof(color_header), 1, file);
        deng_bool_t verify_color_data = color_header.alpha_mask == 0xff000000 &&
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
    p_texture->pixel_data.width = (deng_ui16_t) info_header.width;
    p_texture->pixel_data.height = (deng_ui16_t) info_header.height;

    p_texture->pixel_data.size = info_header.height * info_header.width * 4;
    p_texture->pixel_data.p_pixel_data = (deng_ui8_t*) calloc ( 
        p_texture->pixel_data.size,
        sizeof(deng_ui8_t)
    );

    /* No padding */
    if(info_header.bit_count == 32) {
        size_t w_index, data_index;
        int h_index;
        deng_ui8_t *tmp_arr; 
        
        // Allocate memory for tmp_arr[y][x]
        size_t height_offset = info_header.width * 4;
        tmp_arr = (deng_ui8_t*) calloc (
            info_header.height,
            sizeof(deng_ui8_t) * height_offset
        );
        
        for(h_index = 0; h_index < (int) info_header.height; h_index++) 
            res = fread (
                tmp_arr[(size_t) h_index], 
                4 * info_header.width * sizeof(deng_ui8_t), 
                1, 
                file
           );

        // Realign pixels to top - bottom format
        for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--)
            for(w_index = 0; w_index < 4 * info_header.width; w_index++, data_index++)
                p_texture->pixel_data.p_pixel_data[data_index] = (tmp_arr + h_index * height_offset + w_index);
        
        free(tmp_arr);
    }
    /* With padding */
    else {
        size_t w_index, data_index = 0;
        int h_index;

        long padding_count_per_row = 4 - ((long) info_header.width * (long) info_header.bit_count) / 8 % 4;
        deng_ui8_t *tmp_arr;
        
        // Allocate memory for tmp_arr[y][x]
        size_t height_offset = info_header.width * 3;
        tmp_arr = (deng_ui8_t*)calloc(
            info_header.height,
            sizeof(deng_ui8_t) * height_offset
        );
        
        for(h_index = 0; h_index < (int) info_header.height; h_index++) {
            res = fread(tmp_arr[(size_t) h_index], 3 * info_header.width * sizeof(deng_ui8_t), 1, file);
            fseek(file, padding_count_per_row, SEEK_CUR);
        }

        for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--) {
            for(w_index = 0; w_index < info_header.width * 3; w_index += 3, data_index += 4) {
                p_texture->pixel_data.p_pixel_data[data_index] = *(tmp_arr + h_index * height_offset + w_index);
                p_texture->pixel_data.p_pixel_data[data_index + 1] = *(tmp_arr + h_index * height_offset + w_index + 1);
                p_texture->pixel_data.p_pixel_data[data_index + 2] = *(tmp_arr + h_index * height_offset + w_index + 2);
                p_texture->pixel_data.p_pixel_data[data_index + 3] = 0xFF;
            }
        }
    }

    fclose(file);

    if(!res) {
        printf("Failed to read from image file: %s\n", file_name);
        exit(-1);
    }
}


/* 
 * Load TGA image into deng_Texture
 */
 void dasLoadTGAimage(deng_Texture *p_asset, const char *file_name) {
    size_t res;
    TGATypeHeader type_header;
    TGAColorMapHeader color_header;
    TGAInfoHeader info_header;
    dasInitTGAimageHeaders(&type_header, &color_header, &info_header);
    p_asset->pixel_data.p_pixel_data = (deng_ui8_t*) malloc(sizeof(deng_ui8_t));

    file = fopen(file_name, "rb");
    if(!file) {
        printf("%s%s%s\n", "Failed to load texture image file ", file_name, "!");
        return;
    }

    res = fread((void*) &type_header, sizeof(type_header), 1, file);

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

    res = fread((void*) &color_header, sizeof(color_header), 1, file);
    res = fread((void*) &info_header, sizeof(info_header), 1, file);
    
    p_asset->pixel_data.width = info_header.width;
    p_asset->pixel_data.height = info_header.height;
    p_asset->pixel_data.size = (size_t) (info_header.height * info_header.width * 4);
    printf("Texture size: %ld\n", p_asset->pixel_data.size);
    p_asset->pixel_data.p_pixel_data = (deng_ui8_t*) calloc(p_asset->pixel_data.size, sizeof(deng_ui8_t));
    
    if (info_header.y_origin != p_asset->pixel_data.height)
        dasReadBitmap(
            file,
            info_header.bit_count,
            true,
            p_asset
        );
    else dasReadBitmap(
        file,
        info_header.bit_count,
        false,
        p_asset
    );

    p_asset->pixel_data.width = info_header.width;
    p_asset->pixel_data.height = info_header.height;

    fclose(file);

    if(!res) {
        printf("Failed to read image file: %s\n", file_name);
        exit(-1);
    }
}

/**************************************/
/***** Wavefront 3D model loading *****/
/**************************************/

// Callback function for loading 3d model data
void dasLoadModel (
    deng_Asset *p_asset, 
    char *file_name
) {
    file = fopen(file_name, "rb");
    if(!file) 
        printf (
            "ERROR: Failed to open model file: %s\n", 
            file_name
        );

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Vertices data variables
    deng_ObjVertData *p_vertices_data = NULL;
    size_t vertices_size = 0;
    deng_ObjTextureData *p_texture_data = NULL;
    size_t texture_size = 0;
    deng_ObjNormalData *p_norm_data = NULL;
    size_t norm_size = 0;

    // Indices data variables
    IndexSet *indices = NULL;
    size_t indices_c = 0;

    char *file_ext = cm_GetFileExtName(file_name);
    if(!(file_ext && !strcmp(file_ext, "obj"))) {
        printf("Incorrect object file format!\n");
        return;
    }

    // Load all verices from obj model file
    dasLoadOBJmodelVertices (
        &p_vertices_data, 
        &vertices_size, 
        &p_texture_data, 
        &texture_size, 
        &p_norm_data,
        &norm_size,
        file_name,
        file_size
    );

    // Load all indices from obj model file
    switch(p_asset->asset_mode) 
    {
    case DENG_ASSET_MODE_3D_UNMAPPED:
        dasLoadOBJindices (
            &indices, 
            false,
            false,
            &indices_c, 
            file_name,
            file_size
        );
        break;

    case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        dasLoadOBJindices (
            &indices, 
            false,
            true,
            &indices_c, 
            file_name,
            file_size
        );
        break;
    
    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        dasLoadOBJindices (
            &indices, 
            true,
            false,
            &indices_c, 
            file_name,
            file_size
        );
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        dasLoadOBJindices (
            &indices, 
            true,
            true,
            &indices_c, 
            file_name,
            file_size
        );
        break;

    default:
        break;
    }

    dasMakeIndexedVertices (
        p_asset, 
        indices, 
        indices_c, 
        p_vertices_data, 
        p_texture_data, 
        p_norm_data
    );
}

/* OBJ file format */
// Read vertices information from the file
void dasLoadOBJmodelVertices (
    deng_ObjVertData **pp_vert_data, 
    size_t *p_vert_size, 
    deng_ObjTextureData **pp_texture_data, 
    size_t *p_tex_size, 
    deng_ObjNormalData **pp_norm_data,
    size_t *p_norm_size,
    char *file_name,
    long file_size
) {
    size_t index = 0;
    size_t res = 0;
    printf("Loading vertices...\n");
    unsigned char prev_tmp = 0x00, tmp = 0x00, next_tmp = 0x00;

    char x_buffer[32] = { 0 };
    char y_buffer[32] = { 0 };
    char z_buffer[32] = { 0 };

    memset(x_buffer, 0, 32);
    memset(y_buffer, 0, 32);
    memset(z_buffer, 0, 32);

    
    *pp_vert_data = (deng_ObjVertData*) calloc (
        1, 
        sizeof(deng_ObjVertData)
    );
    
    *pp_texture_data = (deng_ObjTextureData*) calloc (
        1, 
        sizeof(deng_ObjTextureData)
    );

    *pp_norm_data = (deng_ObjNormalData*) calloc (
        1,
        sizeof(deng_ObjNormalData)
    );
    
    *p_vert_size = 0;
    *p_tex_size = 0;
    *p_norm_size = 0;

    // LOGGING
    /*
     *char file_str[128];
     *cm_LogWrite("vert.log", "#Entry point", true);
     */
    // Find first vertex instance in obj file
    prev_tmp = 0x00;
    tmp = 0x00;
    next_tmp = 0x00;
    while(!(tmp == 'v' && next_tmp == 0x20) && ftell(file) < file_size) {
        prev_tmp = tmp;
        tmp = next_tmp;
        res = fread(&next_tmp, sizeof(next_tmp), 1, file);
        if(!res) CORRUPTION_OBJ_ERR(file_name);
    }

    fseek(file, -1L, SEEK_CUR);

    /* Find all vertices coordinates in .obj file */
    while
    (
        (prev_tmp == 0x0A || prev_tmp == 0x0D || prev_tmp == 0x20) && 
        tmp == 'v' && 
        next_tmp == 0x20 && 
        ftell(file) < file_size
    ) {
        /* Get x coordinate data */
        tmp = next_tmp;
        if(tmp != 0x20) CORRUPTION_OBJ_ERR(file_name);
        while(tmp == 0x20) 
            res = fread(&tmp, sizeof(tmp), 1, file);
        
        if(!res) CORRUPTION_OBJ_ERR(file_name);

        x_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x20) x_buffer[index] = tmp;
        }

        while(tmp == 0x20) res = fread(&tmp, sizeof(tmp), 1, file);

        /* Get y coordinate data */
        y_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x20) y_buffer[index] = tmp;
        }

        while(tmp == 0x20) res = fread(&tmp, sizeof(tmp), 1, file);

        /* Get z coordinate data */
        z_buffer[0] = tmp;
        for(index = 1; tmp != 0x0A && tmp != 0x0D && tmp != 0x20; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x0A && tmp != 0x0D && tmp != 0x20) 
                z_buffer[index] = tmp;
        }

        while(tmp == 0x20 || tmp == 0x0A || tmp == 0x0D) {
            prev_tmp = tmp;
            res = fread(&tmp, sizeof(tmp), 1, file); 
            if(!res) CORRUPTION_OBJ_ERR(file_name);
        }

        res = fread(&next_tmp, sizeof(next_tmp), 1, file);
        if(!res) CORRUPTION_OBJ_ERR(file_name);

        (*p_vert_size)++;
        *pp_vert_data = (deng_ObjVertData*) realloc (
            *pp_vert_data, 
            (*p_vert_size) * sizeof(deng_ObjVertData)
        );
        cm_CheckMemoryAlloc(*pp_vert_data);

        (*pp_vert_data)[(*p_vert_size) - 1] = (deng_ObjVertData) {
            (deng_vec_t) atof(x_buffer), 
            (deng_vec_t) atof(y_buffer), 
            (deng_vec_t) atof(z_buffer)
        };
        
        memset(x_buffer, 0, 32);
        memset(y_buffer, 0, 32);
        memset(z_buffer, 0, 32);
    }
    
    // Find the beginning of texture mapping coordinate declarations
    fseek(file, -3L, SEEK_CUR);
    prev_tmp = 0x00;
    tmp = 0x00;
    next_tmp = 0x00;
    while(!(prev_tmp == 'v' && tmp == 't' && next_tmp == 0x20) && ftell(file) < file_size) {
        prev_tmp = tmp;
        tmp = next_tmp;
        res = fread(&next_tmp, sizeof(next_tmp), 1, file);
        if(!res) CORRUPTION_OBJ_ERR(file_name);
    }

    /* Find all texture map coordinate data in .obj file */
    while
    (
        prev_tmp == 'v' &&
        tmp == 't' && 
        next_tmp == 0x20 && 
        ftell(file) < file_size
    ) {
        /* Get x coordinate data */
        tmp = next_tmp;
        if(tmp != 0x20) CORRUPTION_OBJ_ERR(file_name);
        while(tmp == 0x20) 
            res = fread(&tmp, sizeof(tmp), 1, file);

        if(!res) CORRUPTION_OBJ_ERR(file_name);

        x_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x20) x_buffer[index] = tmp;
        }

        while(tmp == 0x20) res = fread(&tmp, sizeof(tmp), 1, file);

        /* Get y coordinate data */
        y_buffer[0] = tmp;
        for(index = 1; tmp != 0x0A && tmp != 0x0D && tmp != 0x20 && ftell(file) < file_size; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x0A && tmp != 0x0D && tmp != 0x20) 
                y_buffer[index] = tmp;
        }

        while((tmp == 0x0A || tmp == 0x0D || tmp == 0x20) && ftell(file) < file_size)
            res = fread(&tmp, sizeof(tmp), 1, file);

        prev_tmp = tmp;
        res = fread(&tmp, sizeof(tmp), 1, file);
        res = fread(&next_tmp, sizeof(next_tmp), 1, file);

        (*p_tex_size)++;
        deng_ObjTextureData *tmp = (deng_ObjTextureData*) realloc (
            *pp_texture_data, 
            (*p_tex_size) * sizeof(deng_ObjTextureData)
        );
        cm_CheckMemoryAlloc(tmp);
        (*pp_texture_data) = tmp;
        (*pp_texture_data)[(*p_tex_size) - 1] = (deng_ObjTextureData) {
            (deng_vec_t) atof(x_buffer), 
            1.0f - (deng_vec_t) atof(y_buffer)
        }; 

        // LOGGING
        /*
         *sprintf (
         *    file_str,
         *    "vt %f,%f",
         *    (*pp_texture_data)[(*p_tex_size) - 1].tex_x,
         *    (*pp_texture_data)[(*p_tex_size) - 1].tex_y
         *);
         *cm_LogWrite("vert.log", file_str, false);
         *memset(file_str, 0, 128);
         */

        memset(x_buffer, 0, 32);
        memset(y_buffer, 0, 32);
    }

    fseek(file, -4L, SEEK_CUR);
    // Find the beginning of texture mapping coordinate declarations
    prev_tmp = 0x00;
    tmp = 0x00;
    next_tmp = 0x00;
    while(!(prev_tmp == 'v' && tmp == 'n' && next_tmp == 0x20) && ftell(file) < file_size) {
        prev_tmp = tmp;
        tmp = next_tmp;
        res = fread(&next_tmp, sizeof(next_tmp), 1, file);
        if(!res) CORRUPTION_OBJ_ERR(file_name);
    }

    // Read all vertex normals
    while
    (
        prev_tmp == 'v' &&
        tmp == 'n' && 
        next_tmp == 0x20 && 
        ftell(file) < file_size
    ) {
        // Get x coordinate data 
        tmp = next_tmp;
        if(tmp != 0x20) CORRUPTION_OBJ_ERR(file_name);
        while(tmp == 0x20) 
            res = fread(&tmp, sizeof(tmp), 1, file);
        
        if(!res) CORRUPTION_OBJ_ERR(file_name);

        // Get x coordinate data
        x_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x20) x_buffer[index] = tmp;
        }

        while(tmp == 0x20) res = fread(&tmp, sizeof(tmp), 1, file);

        // Get y coordinate data 
        y_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x20) y_buffer[index] = tmp;
        }

        while(tmp == 0x20)
            res = fread(&tmp, sizeof(tmp), 1, file);

        if(!res) CORRUPTION_OBJ_ERR(file_name);

        // Get z coordinate data
        z_buffer[0] = tmp;
        for(index = 1; tmp != 0x20 && tmp != 0x0A && tmp != 0x0D && ftell(file) < file_size; index++) {
            res = fread(&tmp, sizeof(tmp), 1, file);
            if(!res) CORRUPTION_OBJ_ERR(file_name);
            if(tmp != 0x20 && tmp != 0x0A && tmp != 0x0D)
                z_buffer[index] = tmp;
        }

        while((tmp == 0x0A || tmp == 0x0D || tmp == 0x20) && ftell(file) < file_size)
            res = fread(&tmp, sizeof(tmp), 1, file);
        
        if(!res) CORRUPTION_OBJ_ERR(file_name);
        
        prev_tmp = tmp;
        res = fread(&tmp, sizeof(tmp), 1, file);
        res = fread(&next_tmp, sizeof(next_tmp), 1, file);
        if(!res) CORRUPTION_OBJ_ERR(file_name);

        (*p_norm_size)++;
        deng_ObjNormalData *tmp = (deng_ObjNormalData*) realloc (
            *pp_norm_data, 
            (*p_norm_size) * sizeof(deng_ObjNormalData)
        );

        cm_CheckMemoryAlloc(tmp);
        (*pp_norm_data) = tmp;
        (*pp_norm_data)[(*p_norm_size) - 1] = (deng_ObjNormalData) {
            (deng_vec_t) atof(x_buffer), 
            (deng_vec_t) atof(y_buffer),
            (deng_vec_t) atof(z_buffer)
        }; 
        
        // LOGGING
        /*
         *sprintf (
         *    file_str,
         *    "vn %f,%f,%f",
         *    (*pp_norm_data)[(*p_norm_size) - 1].nor_x,
         *    (*pp_norm_data)[(*p_norm_size) - 1].nor_y,
         *    (*pp_norm_data)[(*p_norm_size) - 1].nor_z
         *);
         *cm_LogWrite("vert.log", file_str, false);
         *memset(file_str, 0, 128);
         */

        memset(x_buffer, 0, 32);
        memset(y_buffer, 0, 32);
        memset(z_buffer, 0, 32);
    }
}


/* Read obj file indices information */
void dasLoadOBJindices (
    IndexSet **p_indices,
    deng_bool_t read_tex_indices,
    deng_bool_t read_norm_indices,
    size_t *p_indices_c, 
    char *file_name,
    long file_size
) {
    size_t l_index, prev_index;
    size_t res;
    printf("Loading indices...\n");
    
    *p_indices_c = 0;
    (*p_indices) = (IndexSet*) calloc (
        1,
        sizeof(IndexSet)
    );

    // Find first index declaration
    char beg_ch = 0x00;
    char prev_ch = 0x00;
    deng_ui8_t is_indexed = false;
    fseek(file, -4L, SEEK_CUR);

    while(ftell(file) < file_size) {
        res = fread(&beg_ch, 1, 1, file);
        if(beg_ch == 'f' && (prev_ch == 0x0A || prev_ch == 0x0D)) {
            is_indexed = true;
            break;
        }

        prev_ch = beg_ch;
    }
    
    // Check for file corruption error
    if(!is_indexed) CORRUPTION_OBJ_ERR(file_name);

    // From now on char buffer will be used to access file data
    char *buffer = (char*) calloc (
        file_size - ftell(file) + 1,
        sizeof(char)
    );

    res = fread (
        buffer,
        sizeof(char),
        file_size - (ftell(file) - 1),
        file
    );

    if(!res) {
        printf("Failed to read indices from model file\n");
        exit(EXIT_FAILURE);
    }
    
    char *vert_i_ch;
    char *tex_i_ch;
    char *norm_i_ch;

    // LOGGING
    /*
     *char file_str[128];
     *cm_LogWrite("ind_read.log", "#Entry point", true);
     */

    for(l_index = 0; l_index < strlen(buffer);) {
        // Skip white spaces and index declaration
        while
        (
            (buffer[l_index] == 0x20 || 
            buffer[l_index] == 0x0A || 
            buffer[l_index] == 0x0D ||
            buffer[l_index] == 'f') &&
            l_index < strlen(buffer)
        ) l_index++;

        if(l_index == strlen(buffer)) break;
        prev_index = l_index;

        // Find first '/'
        while(buffer[l_index] != '/') l_index++;
        vert_i_ch = (char*) calloc (
            l_index - prev_index,
            sizeof(char)
        );

        strncpy (
            vert_i_ch,
            buffer + prev_index,
            l_index - prev_index
        );
        l_index++;
        
        // Find second '/'
        prev_index = l_index;
        while(buffer[l_index] != '/') l_index++;
        tex_i_ch = (char*) calloc (
            l_index - prev_index,
            sizeof(char)
        );

        strncpy (
            tex_i_ch,
            buffer + prev_index,
            l_index - prev_index
        );
        l_index++;

        // Find whitespace or newline
        prev_index = l_index;
        while(buffer[l_index] != 0x20 && buffer[l_index] != 0x0A)
            l_index++;
        
        norm_i_ch = (char*) calloc (
            l_index - prev_index,
            sizeof(char)
        );

        strncpy (
            norm_i_ch,
            buffer + prev_index,
            l_index - prev_index
        );
        
        // Push indices to indexset instance
        (*p_indices_c)++;
        IndexSet *tmp = (IndexSet*) realloc (
            *p_indices,
            (*p_indices_c) * sizeof(IndexSet)
        );

        cm_CheckMemoryAlloc(tmp);
        (*p_indices) = tmp;
        /*memset((*p_indices) + ((*p_indices_c) - 1), 0, sizeof(IndexSet));*/
        (*p_indices)[(*p_indices_c) - 1].vert = (deng_ui32_t) atoi(vert_i_ch);
        if(read_tex_indices)
            (*p_indices)[(*p_indices_c) - 1].tex = (deng_ui32_t) atoi(tex_i_ch);
        else
            (*p_indices)[(*p_indices_c) - 1].tex = 0;

        if(read_norm_indices)
            (*p_indices)[(*p_indices_c) - 1].norm = (deng_ui32_t) atoi(norm_i_ch);
        else
            (*p_indices)[(*p_indices_c) - 1].norm = 0;

        // LOGGING
        /*
         *sprintf (
         *    file_str,
         *    "f %d/%d/%d",
         *    (*p_indices)[(*p_indices_c) - 1].vert,
         *    (*p_indices)[(*p_indices_c) - 1].tex,
         *    (*p_indices)[(*p_indices_c) - 1].norm
         *);
         *cm_LogWrite("ind_read.log", file_str, false);
         *memset(file_str, 0, 128);
         */

        free(vert_i_ch);
        free(tex_i_ch);
        free(norm_i_ch);
    }

    free(buffer);
}


void dasMakeIndexedVertices (
    deng_Asset *p_asset,
    IndexSet *indices,
    size_t indices_c,
    deng_ObjVertData *p_vert_data,
    deng_ObjTextureData *p_tex_data,
    deng_ObjNormalData *p_norm_data
) {
    p_asset->indices.size = indices_c;
    p_asset->indices.p_indices = (deng_ui32_t*) calloc (
        indices_c,
        sizeof(deng_ui32_t)
    );

    Hashmap hm;
    newHashmap(&hm, indices_c);

    size_t l_index, r_index = 0;
    
    // Allocate initial values for vertices
    switch(p_asset->asset_mode)
    {
    case DENG_ASSET_MODE_3D_UNMAPPED:
        p_asset->vertices.p_unmapped_unnormalized_vert = (VERT_UNMAPPED_UNOR*) calloc (
            1,
            sizeof(VERT_UNMAPPED_UNOR)
        );
        break;

    case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        p_asset->vertices.p_unmapped_normalized_vert = (VERT_UNMAPPED_NOR*) calloc (
            1,
            sizeof(VERT_UNMAPPED_NOR)
        );
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        p_asset->vertices.p_tex_mapped_unnormalized_vert = (VERT_MAPPED_UNOR*) calloc (
            1,
            sizeof(VERT_MAPPED_UNOR)
        );
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        p_asset->vertices.p_tex_mapped_normalized_vert = (VERT_MAPPED_NOR*) calloc (
            1,
            sizeof(VERT_MAPPED_NOR)
        );
        break;

    default:
        break;
    }

    // LOGGING
    /*
     *char file_str[256];
     *cm_LogWrite("vert_write.log", "#Entry point", true);
     */

    for(l_index = 0; l_index < indices_c; l_index++) {
        if(!findValue(&hm, &indices[l_index], sizeof(IndexSet))) {
            // Allocate memory for vertices
            r_index++;
            pushToHashmap (
                &hm, 
                &indices[l_index], 
                sizeof(IndexSet), 
                (void*) r_index
            );
            
            switch(p_asset->asset_mode)
            {
            case DENG_ASSET_MODE_3D_UNMAPPED: {
                VERT_UNMAPPED_UNOR *tmp = (VERT_UNMAPPED_UNOR*) realloc (
                    p_asset->vertices.p_unmapped_unnormalized_vert,
                    r_index * sizeof(VERT_UNMAPPED_UNOR)
                );
                cm_CheckMemoryAlloc(tmp);

                p_asset->vertices.p_unmapped_unnormalized_vert = tmp;
                p_asset->vertices.p_unmapped_unnormalized_vert[r_index - 1].vert_data = p_vert_data[indices[l_index].vert - 1];
                p_asset->vertices.p_unmapped_unnormalized_vert[r_index - 1].color_data = DEFAULT_ASSET_COLOR;
                break;
            }

            case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED: {
                VERT_UNMAPPED_NOR *tmp = (VERT_UNMAPPED_NOR*) realloc (
                    p_asset->vertices.p_unmapped_normalized_vert,
                    r_index * sizeof(VERT_UNMAPPED_NOR)
                );
                cm_CheckMemoryAlloc(tmp);
                
                p_asset->vertices.p_unmapped_normalized_vert = tmp;
                p_asset->vertices.p_unmapped_normalized_vert[r_index - 1].vert_data = p_vert_data[indices[l_index].vert - 1];
                p_asset->vertices.p_unmapped_normalized_vert[r_index - 1].norm_data = p_norm_data[indices[l_index].norm - 1];
                p_asset->vertices.p_unmapped_normalized_vert[r_index - 1].color_data = DEFAULT_ASSET_COLOR;
                break;
            }

            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED: {
                VERT_MAPPED_UNOR *tmp = (VERT_MAPPED_UNOR*) realloc (
                    p_asset->vertices.p_tex_mapped_unnormalized_vert,
                    r_index * sizeof(VERT_MAPPED_UNOR)
                );
                cm_CheckMemoryAlloc(tmp);

                p_asset->vertices.p_tex_mapped_unnormalized_vert = tmp;
                p_asset->vertices.p_tex_mapped_unnormalized_vert[r_index - 1].vert_data = p_vert_data[indices[l_index].vert - 1];
                p_asset->vertices.p_tex_mapped_unnormalized_vert[r_index - 1].tex_data = p_tex_data[indices[l_index].tex - 1];
                break;
            }

            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED: {
                VERT_MAPPED_NOR *tmp = (VERT_MAPPED_NOR*) realloc (
                    p_asset->vertices.p_tex_mapped_normalized_vert,
                    r_index * sizeof(VERT_MAPPED_NOR)
                );
                cm_CheckMemoryAlloc(tmp);

                p_asset->vertices.p_tex_mapped_normalized_vert = tmp;
                p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].vert_data = p_vert_data[indices[l_index].vert - 1];
                p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].tex_data = p_tex_data[indices[l_index].tex - 1];
                p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].norm_data = p_norm_data[indices[l_index].norm - 1];

                // LOGGING
                /*
                 *sprintf (
                 *    file_str,
                 *    "%f,%f,%f | %f,%f | %f,%f,%f",
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].vert_data.vert_x,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].vert_data.vert_y,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].vert_data.vert_z,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].tex_data.tex_x,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].tex_data.tex_y,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].norm_data.nor_x,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].norm_data.nor_y,
                 *    p_asset->vertices.p_tex_mapped_normalized_vert[r_index - 1].norm_data.nor_z
                 *);
                 *cm_LogWrite("vert_write.log", file_str, false);
                 *memset(file_str, 0, 256);
                 */
                break;
            }

            default:
                break;
            }
        }
    }
    
    p_asset->vertices.size = r_index;
    void *p_in;
    for(l_index = 0; l_index < indices_c; l_index++) {
        if(!(p_in = findValue(&hm, &indices[l_index], sizeof(IndexSet)))) {
            printf("Hashmap error occured, this should not happen :(\n");
            exit(EXIT_FAILURE);
        }

        p_asset->indices.p_indices[l_index] = (deng_ui32_t) ((deng_ui64_t) p_in - 1);
    }

    free(hm.map_data);
}
