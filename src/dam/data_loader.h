#ifndef LOADER_H
#define LOADER_H

typedef struct OBJVerticesData {
    float vert_x;
    float vert_y;
    float vert_z;
} OBJVerticesData;

typedef struct OBJTextureData {
    float tex_x;
    float tex_y;
} OBJTextureData;

typedef struct OBJColorData {
    float col_r;
    float col_g;
    float col_b;
    float col_a;
} OBJColorData;

typedef struct VERT_UNMAPPED {
    OBJVerticesData vert_data;
    OBJColorData color_data;
} VERT_UNMAPPED;

typedef struct VERT_MAPPED {
    OBJVerticesData vert_data;
    OBJTextureData tex_data;
} VERT_MAPPED;

typedef struct DynamicVertices {
    VERT_MAPPED *p_mapped_vertices;
    VERT_UNMAPPED *p_unmapped_vertices;
    /* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
    uint8_t vertices_type;
    size_t size;
} DynamicVertices;

typedef struct DynamicIndices {
    uint32_t *p_indices;
    size_t size;
} DynamicIndices;

typedef struct DynamicPixelData {
    uint8_t *p_pixel_data;
    size_t size;
    uint16_t width;
    uint16_t height;
} DynamicPixelData;

typedef struct Asset {
    char *name;
    char *description;
    uint64_t time_point; 
    DynamicVertices vertices;
    DynamicIndices indices;
    DynamicPixelData pixel_data;
} Asset;

typedef struct AssetAssemblyInfo {
    int frag_mode;
    OBJColorData color_data;
    
} AssetAssemblyInfo;

typedef enum ImageFormat {
    IMAGE_FORMAT_BMP = 0,
    IMAGE_FORMAT_TGA = 1,
    IMAGE_FORMAT_PNG = 2,
    IMAGE_FORMAT_JPG = 3,
    IMAGE_FORMAT_UNKNOWN = 4
} ImageFormat;

/* Image loading */

void load_image(Asset *p_asset, const char *file_name);
#ifdef LOADER_ONLY
    static ImageFormat detect_image_format(const char *file_name);
    static void load_BMP_image(Asset *p_asset, const char *file_name);
    static void load_TGA_image(Asset *p_asset, const char *file_name);
#endif

/* Vertices data loading */
/* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
void load_model(Asset *p_asset, const char *file_name, OBJColorData *p_color_data, int color_mode);
#ifdef LOADER_ONLY
    static void load_OBJ_model_vertices(OBJVerticesData **pp_vert_data, size_t *p_vert_data_size, OBJTextureData **pp_texture_data, size_t *p_tex_data_size, long file_size);
    static void load_OBJ_indices(uint32_t **pp_vert_indices, size_t *p_vert_indices_size, uint32_t **pp_tex_vert_indices, size_t *p_tex_vert_indices_size, long file_size);
    static void index_unmapped_vertices(OBJVerticesData **pp_vert_data, size_t vert_size, uint32_t **pp_indices, size_t indices_size, OBJColorData *p_color_data, Asset *p_asset);
    
    static void index_tex_mapped_vertices(OBJVerticesData **pp_vert_data, size_t vert_size, OBJTextureData **pp_tex_data, size_t tex_size, uint32_t **pp_vert_indices, size_t vert_indices_size, uint32_t **pp_tex_indices, 
    size_t tex_indices_size, Asset *p_asset);
#endif

void clean_buffer(char *buffer, size_t size);
void check_for_mem_alloc_err(void *mem_addr);

#endif