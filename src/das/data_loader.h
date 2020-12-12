#ifndef LOADER_H
#define LOADER_H

typedef struct OBJVerticesData {
    float vert_x;
    float vert_y;
    float vert_z;
} OBJVerticesData;

typedef struct OBJVerticesData2D {
    float vert_x;
    float vert_y;
} OBJVerticesData2D;

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


typedef struct VERT_MAPPED {
    OBJVerticesData vert_data;
    OBJTextureData tex_data;
} VERT_MAPPED;


typedef struct VERT_UNMAPPED {
    OBJVerticesData vert_data;
    OBJColorData color_data;
} VERT_UNMAPPED;


typedef struct VERT_MAPPED_2D {
    OBJVerticesData2D vert_data;
    OBJTextureData tex_data;
} VERT_MAPPED_2D;


typedef struct VERT_UNAMPPED_2D {
    OBJVerticesData2D vert_data;
    OBJColorData color_data;
} VERT_UNMAPPED_2D;


typedef struct DynamicVertices {
    VERT_MAPPED *p_texture_mapped_vert_data;
    OBJVerticesData *p_unmapped_vert_data;

    VERT_MAPPED_2D *p_texture_mapped_vert_data_2d;
    OBJVerticesData2D *p_unmapped_vert_data_2d;

    /* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
    size_t size;
    uint64_t memory_offset;
} DynamicVertices;

typedef struct DynamicIndices {
    uint32_t *p_indices;
    size_t size;
    uint64_t memory_offset;
} DynamicIndices;

typedef struct DynamicPixelData {
    uint8_t *p_pixel_data;
    size_t size;
    uint16_t width;
    uint16_t height;
    uint64_t memory_offset;
} DynamicPixelData;

typedef enum AssetMode {
    DENG_ASSET_MODE_3D_TEXTURE_MAPPED = 0,
    DENG_ASSET_MODE_3D_UNMAPPED = 1,
    DENG_ASSET_MODE_2D_TEXTURE_MAPPED = 2,
    DENG_ASSET_MODE_2D_UNMAPPED = 3
} AssetMode;

typedef struct DENGAsset {
    char *name;
    char *description;
    uint64_t time_point; 
    AssetMode asset_mode;
    DynamicVertices vertices;
    DynamicIndices indices;
    
    // For fragment shader
    OBJColorData solid_fill_color;
    size_t fragment_index;
} DENGAsset;


typedef struct DENGTexture {
    char *name;
    char *descritpion;
    DynamicPixelData pixel_data;
} DENGTexture;

typedef enum ImageFormat {
    IMAGE_FORMAT_BMP = 0,
    IMAGE_FORMAT_TGA = 1,
    IMAGE_FORMAT_PNG = 2,
    IMAGE_FORMAT_JPG = 3,
    IMAGE_FORMAT_UNKNOWN = 4
} ImageFormat;

/* Image loading */
void dasLoadTexture(DENGTexture *p_asset, const char *file_name);
#ifdef DAS_EXT_LOADERS
    ImageFormat dasDetectImageFormat(const char *file_name);
    void dasLoadBMPimage(DENGTexture *p_asset, const char *file_name);
    void dasLoadTGAimage(DENGTexture *p_asset, const char *file_name);
#endif

/* Vertices data loading */
/* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
void dasLoadModel(DENGAsset *p_asset, const char *file_name);
#ifdef DAS_EXT_LOADERS
    void dasLoadOBJmodelVertices(OBJVerticesData **pp_vert_data, size_t *p_vert_data_size, OBJTextureData **pp_texture_data, size_t *p_tex_data_size, long file_size);
    void dasLoadOBJindices(uint32_t **pp_vert_indices, size_t *p_vert_indices_size, uint32_t **pp_tex_vert_indices, size_t *p_tex_vert_indices_size, long file_size);
    void dasIndexUnmappedVertices(OBJVerticesData **pp_vert_data, size_t vert_size, uint32_t **pp_indices, size_t indices_size, DENGAsset *p_asset);
    
    void dasIndexTexMappedVertices(OBJVerticesData **pp_vert_data, size_t vert_size, OBJTextureData **pp_tex_data, size_t tex_size, uint32_t **pp_vert_indices, size_t vert_indices_size, uint32_t **pp_tex_indices, 
    size_t tex_indices_size, DENGAsset *p_asset);
#endif

#ifdef DAS_GENERIC_HELPERS
    char *dasGetFileExt(const char *file_name);
    void dasCheckForMemAllocErr(void *mem_addr);
    void dasCleanCharBuffer(char *buffer, size_t size);
#endif

#endif