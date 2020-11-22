#include "asset_creator_core.h"

static FILE *file;

/* Static method declarations */
static void clean_buffer(char *buffer, size_t size);
static void index_vertices(DynamicVertices *p_vertices, VerticesColorProps color_props);
static void load_OBJ_model_vertices(OBJVerticesData **pp_vert_data, size_t *p_vert_data_size, OBJTextureData **pp_texture_data, size_t *p_tex_data_size, long file_size);
static void load_OBJ_indices(uint32_t **pp_vert_indices, size_t *p_vert_indices_size, uint32_t **pp_tex_vert_indices, size_t *p_tex_vert_indices_size, long file_size);

void init_BMP_image_headers(BMPFileHeader *p_file_header, BMPInfoHeader *p_info_header, BMPColorHeader *p_color_header) {
    *p_file_header = (BMPFileHeader) {0x4D42, 0, 0, 0, 0};
    *p_info_header = (BMPInfoHeader) {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    *p_color_header = (BMPColorHeader) {0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000, 0x73524742, 0};
}

void init_TGA_image_headers(TGATypeHeader *p_type_header, TGAColorMapHeader *p_color_map_header, TGAInfoHeader *p_info_header) {
    *p_type_header = (TGATypeHeader) {0, 0, 0};
    *p_color_map_header = (TGAColorMapHeader) {0, 0, 0};
    *p_info_header = (TGAInfoHeader) {0, 0, 0, 0, 0, 0};
}

static void clean_buffer(char *buffer, size_t size) {
    for(size_t index = 0; index < size; index++)
        buffer[index] = 0x00;
}

/* Image loaders */
ImageFormat detect_image_format(const char *file_name) {
    char file_format[3];
    size_t img_size = strlen(file_name);

    for(size_t index = img_size - 3, count = 0; index < img_size; index++, count++)
        file_format[count] = file_name[index];

    if(!strcmp(file_format, "tga")) return IMAGE_FORMAT_TGA;
    else if(!strcmp(file_format, "bmp")) return IMAGE_FORMAT_BMP;

    return IMAGE_FORMAT_UNKNOWN; 
}

/* TGA image */
void load_TGA_image(DynamicPixelData *p_pixel_data, const char *file_name) {
    size_t index;
    TGATypeHeader type_header;
    TGAColorMapHeader color_header;
    TGAInfoHeader info_header;
    init_TGA_image_headers(&type_header, &color_header, &info_header);

    file = fopen(file_name, "rb");
    if(!file) {
        printf("%s%s%s\n", "Failed to load texture image file ", file_name, "!");
        return;
    }

    fread((void*) &type_header, sizeof(type_header), 1, file);

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
    resize_pixel_data_arr(p_pixel_data, (size_t) (info_header.height * info_header.width * 4));

    
    switch (info_header.bit_count)
    {
    case 32: {
        if(info_header.y_origin != info_header.height) {
            size_t data_index, w_index;
            int h_index;
            uint8_t tmp_arr[info_header.height][info_header.width * 4];

            for(h_index = 0; h_index < (int) info_header.height; h_index++)
                fread(tmp_arr[(size_t) h_index], 4 * info_header.width * sizeof(uint8_t), 1, file);

            for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--) {
                for(w_index = 0; w_index < info_header.width * 4; w_index++, data_index++)
                    p_pixel_data->p_pixel_data[data_index] = tmp_arr[(int) h_index][w_index];
            }
        }

        else 
            fread(p_pixel_data->p_pixel_data, p_pixel_data->size * sizeof(uint8_t), 1, file);
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
                    p_pixel_data->p_pixel_data[data_index] = tmp_arr[(size_t) h_index][(size_t) w_index];
                    p_pixel_data->p_pixel_data[data_index + 1] = tmp_arr[(size_t) h_index][(size_t) w_index + 1];
                    p_pixel_data->p_pixel_data[data_index + 2] = tmp_arr[(size_t) h_index][(size_t) w_index + 2];
                    p_pixel_data->p_pixel_data[data_index + 3] = 0xFF;
                }
            }
        }

        else {
            size_t tmp_index, data_index;
            uint8_t tmp_arr[info_header.height * info_header.width * 3];
            fread(tmp_arr, 3 * info_header.width * info_header.height * sizeof(uint8_t), 1, file);

            for(tmp_index = 0, data_index = 0; data_index < p_pixel_data->size; data_index += 4, tmp_index += 3) {
                p_pixel_data->p_pixel_data[data_index] = tmp_arr[tmp_index];
                p_pixel_data->p_pixel_data[data_index + 1] = tmp_arr[tmp_index + 1];
                p_pixel_data->p_pixel_data[data_index + 2] = tmp_arr[tmp_index + 2];
                p_pixel_data->p_pixel_data[data_index + 3] = 0xFF;
            }
        }
    }
    
    default:
        break;
    }

    fclose(file);
}

/* BMP image */
void load_BMP_image(DynamicPixelData *p_pixel_data, const char *file_name) {
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    BMPColorHeader color_header;
    init_BMP_image_headers(&file_header, &info_header, &color_header);
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

    else if(info_header.bit_count == 32 && !info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
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

    resize_pixel_data_arr(p_pixel_data, info_header.height * info_header.width * 4);
    
    /* No padding */
    if(info_header.bit_count == 32) {
        size_t w_index, data_index;
        int h_index;
        uint8_t tmp_arr[info_header.height][4 * info_header.width];
        
        for(h_index = 0; h_index < (int) info_header.height; h_index++) 
            fread(tmp_arr[(size_t) h_index], 4 * info_header.width * sizeof(uint8_t), 1, file);

        for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--)
            for(w_index = 0; w_index < 4 * info_header.width; w_index++, data_index++)
                p_pixel_data->p_pixel_data[data_index] = tmp_arr[(size_t) h_index][w_index];
    }
    /* With padding */
    else {
        size_t w_index, data_index = 0;
        int h_index;

        long padding_count_per_row = 4 - ((long) info_header.width * (long) info_header.bit_count) / 8 % 4;
        printf("Padding count is %ld\n", padding_count_per_row);
        uint8_t tmp_arr[info_header.height][3 * info_header.width];
        printf("Height: %d\n", info_header.height);
        printf("Width: %d\n", info_header.width);
        
        for(h_index = 0; h_index < (int) info_header.height; h_index++) {
            fread(tmp_arr[(size_t) h_index], 3 * info_header.width * sizeof(uint8_t), 1, file);
            fseek(file, padding_count_per_row, SEEK_CUR);
        }

        for(h_index = (int) info_header.height - 1, data_index = 0; h_index >= 0; h_index--) {
            for(w_index = 0; w_index < info_header.width * 3; w_index += 3, data_index += 4) {
                p_pixel_data->p_pixel_data[data_index] = tmp_arr[(size_t) h_index][w_index];
                p_pixel_data->p_pixel_data[data_index + 1] = tmp_arr[(size_t) h_index][w_index + 1];
                p_pixel_data->p_pixel_data[data_index + 2] = tmp_arr[(size_t) h_index][w_index + 2];
                p_pixel_data->p_pixel_data[data_index + 3] = 0xFF;
            }
        }
    }

    fclose(file);
}


/* 3D model loading */

// load_model() function might need to be rearranged below load_OBJ_model_vertices() and load_OBJ_indices()
void load_model(DynamicVertices *p_vertices, DynamicIndices *p_indices, VerticesColorProps props, const char *file_name) {
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


    load_OBJ_model_vertices(&p_vertices_data, &vertices_size, &p_texture_data, &texture_size, file_size);
    load_OBJ_indices(&p_vertices_indices, &vertices_indices_size, &p_tex_vert_indices, &tex_vert_indices_size, file_size);

    fclose(file);

    /* DEBUGGING */
    char *out_x, *out_y, *out_z;
    file = fopen("output.log", "w");

    for(size_t index = 0; index < vertices_size; index++) {
        out_x = calloc(12, sizeof(char));
        out_y = calloc(12, sizeof(char));
        out_z = calloc(12, sizeof(char));
        sprintf(out_x, "%f ", p_vertices_data[index].vert_x);
        sprintf(out_y, "%f ", p_vertices_data[index].vert_y);
        sprintf(out_z, "%f\n", p_vertices_data[index].vert_z);
        fwrite(out_x, 12, 1, file);
        fwrite(out_y, 12, 1, file);
        fwrite(out_z, 12, 1, file);
        free(out_x);
        free(out_y);
        free(out_z);
    }

    fclose(file);
}

/* OBJ file format */
// Read vertices information from the file
static void load_OBJ_model_vertices(OBJVerticesData **pp_vert_data, size_t *p_vert_size, OBJTextureData **pp_texture_data, size_t *p_tex_size, long file_size) {
    size_t index;
    unsigned char prev_tmp = 0x00, tmp = 0x00, next_tmp = 0x00;
    char x_buffer[12], y_buffer[12], z_buffer[12];
    
    *pp_texture_data = (OBJVerticesData*) malloc(sizeof(OBJVerticesData));
    *pp_vert_data = (OBJTextureData*) malloc(sizeof(OBJTextureData));

    // According to ASCII table 0x0A = \n; 0x20 = ' '; 0x76 = 'v' 
    /* Skip the useless metadata and find the nearest vertex value */ 
    while(prev_tmp != 0x0A && tmp != 0x76 && next_tmp != 0x20 && ftell(file) < file_size) {
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

        while(tmp != 0x20) fread(&tmp, sizeof(tmp), 1, file);

        /* Get y coordinate data */
        y_buffer[0] = tmp;
        for(index = 1; tmp != 0x20; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x20) y_buffer[index] = tmp;
        }

        while(tmp != 0x20) fread(&tmp, sizeof(tmp), 1, file);

        /* Get z coordinate data */
        z_buffer[0] = tmp;
        for(index = 1; tmp != 0x0A; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x0A) z_buffer[index] = tmp;
        }

        prev_tmp = tmp;
        fread(&tmp, sizeof(tmp), 1, file);
        fread(&next_tmp, sizeof(next_tmp), 1, file);

        *pp_vert_data = (OBJVerticesData*) realloc(*pp_vert_data, ((*p_vert_size)++) * sizeof(OBJVerticesData));
        *pp_vert_data[(*p_vert_size) - 1] = (OBJVerticesData) {(float) atof(x_buffer), (float) atof(y_buffer), (float) atof(z_buffer)};
    }

    clean_buffer(x_buffer, 12);
    clean_buffer(y_buffer, 12);
    clean_buffer(z_buffer, 12);

    /* Find all texture map coordinate data in .obj file */
    while(prev_tmp == 0x0A && tmp == 0x76 && next_tmp == 0x74 && ftell(file) < file_size) {
        /* Get x coordinate data */
        for(index = 0; tmp != 0x20; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x20) x_buffer[index] = tmp;
        }

        while(tmp != 0x20 && ftell(file) < file_size) fread(&tmp, sizeof(tmp), 1, file);

        /* Get y coordinate data */
        for(index = 0; tmp != 0x0A && ftell(file) < file_size; index++) {
            fread(&tmp, sizeof(tmp), 1, SEEK_CUR);
            if(tmp != 0x0A) y_buffer[index] = tmp;
        }

        while(tmp == 0x0A && ftell(file) < file_size) fread(&tmp, sizeof(tmp), 1, file);

        fseek(file, -1L, SEEK_CUR);
        fread(&prev_tmp, sizeof(prev_tmp), 1, file);

        fread(&tmp, sizeof(tmp), 1, file);
        fread(&next_tmp, sizeof(next_tmp), 1, file);
        
        fseek(file, 1L, SEEK_CUR);

        *pp_texture_data = (OBJTextureData*) realloc(*pp_texture_data, ((*p_tex_size)++) * sizeof(OBJTextureData));
        *pp_texture_data[(*p_tex_size) - 1] = (OBJTextureData) {(float) atof(x_buffer), (float) atof(y_buffer)}; 
    }

    // Position of object's file stream as right now is at the beginning of vertex normals reading 
}

// Read indices information 
static void load_OBJ_indices(uint32_t **pp_vert_indices, size_t *p_vert_indices_size, uint32_t **pp_tex_vert_indices, size_t *p_tex_vert_indices_size, long file_size) {
    size_t index;
    unsigned char prev_tmp = 0x00, tmp = 0x00, next_tmp = 0x00;
    char vert_index_buffer[32], tex_vert_buffer[32];

    *pp_vert_indices = (uint32_t*) malloc(sizeof(uint32_t));
    *pp_tex_vert_indices = (uint32_t*) malloc(*pp_tex_vert_indices);

    /* Skip all vertex normals */
    while(prev_tmp != 0x0A && tmp != 0x66 && next_tmp != 0x20 && ftell(file) < file_size) {
        prev_tmp = tmp;
        tmp = next_tmp;
        fread(&next_tmp, sizeof(next_tmp), 1, file);
    }

    // According to ASCII table 0x2F = '/' 
    while(prev_tmp == 0x0A && tmp == 0x66 && next_tmp == 0x20 && ftell(file) < file_size) {
        // I faces
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
        while(tmp != 0x20) fread(&tmp, sizeof(tmp), 1, file);
        /* Skip spaces */
        while(tmp == 0x20) fread(&tmp, sizeof(tmp), 1, file);

        *pp_vert_indices = (uint32_t*) realloc(*pp_vert_indices, ((*p_vert_indices_size)++) * sizeof(uint32_t));
        *pp_tex_vert_indices = (uint32_t*) realloc(*pp_tex_vert_indices, ((*p_tex_vert_indices_size)++) * sizeof(uint32_t)); 
        
        // II faces
        tmp = 0x00;

        for(index = 0; tmp != 0x2F; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x2F) vert_index_buffer[index] = tmp;
        }

        tmp = 0x00;

        for(index = 0; tmp != 0x2F; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x2F) tex_vert_buffer[index] = tmp;
        }

        while(tmp != 0x20) fread(&tmp, sizeof(tmp), 1, file);
        while(tmp == 0x20) fread(&tmp, sizeof(tmp), 1, file);

        *pp_vert_indices = (uint32_t*) realloc(*pp_vert_indices, ((*p_vert_indices_size)++) * sizeof(uint32_t));
        *pp_tex_vert_indices = (uint32_t*) realloc(*pp_tex_vert_indices, ((*p_tex_vert_indices_size)++) * sizeof(uint32_t));

        // III faces
        tmp = 0x00;

        for(index = 0; tmp != 0x2F; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x2F) vert_index_buffer[index] = tmp;
        }

        tmp = 0x00;

        for(index = 0; tmp != 0x2F; index++) {
            fread(&tmp, sizeof(tmp), 1, file);
            if(tmp != 0x2F) tex_vert_buffer[index] = tmp;
        }

        while(tmp != 0x0A && ftell(file) < file_size) fread(&tmp, sizeof(tmp), 1, file);
        while(tmp == 0x0A && ftell(file) < file_size) fread(&tmp, sizeof(tmp), 1, file);

        *pp_vert_indices = (uint32_t*) realloc(*pp_vert_indices, ((*p_vert_indices_size)++) * sizeof(uint32_t));
        *pp_tex_vert_indices = (uint32_t*) realloc(*pp_tex_vert_indices, ((*p_tex_vert_indices_size)++) * sizeof(uint32_t));

        fseek(file, -1L, SEEK_CUR);
        fread(&prev_tmp, sizeof(prev_tmp), 1, file);

        fread(&tmp, sizeof(tmp), 1, file);
        fread(&next_tmp, sizeof(next_tmp), 1, file);
    }
}