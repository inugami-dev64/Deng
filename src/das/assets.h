#ifndef ASSETS_H
#define ASSETS_H

/* Vertex data structs */
typedef struct deng_ObjVertData {
    float vert_x;
    float vert_y;
    float vert_z;
} deng_ObjVertData;

typedef struct deng_ObjVertData2D {
    float vert_x;
    float vert_y;
} deng_ObjVertData2D;


/* Fragment data structs */
typedef struct deng_ObjTextureData {
    float tex_x;
    float tex_y;
} deng_ObjTextureData;


typedef struct deng_ObjColorData {
    float col_r;
    float col_g;
    float col_b;
    float col_a;
} deng_ObjColorData;


/*********************************************/
/****** General shader vertices structs ******/
/*********************************************/

typedef struct VERT_MAPPED {
    deng_ObjVertData vert_data;
    deng_ObjTextureData tex_data;
} VERT_MAPPED;


typedef struct VERT_UNMAPPED {
    deng_ObjVertData vert_data;
    deng_ObjColorData color_data;
} VERT_UNMAPPED;


typedef struct VERT_MAPPED_2D {
    deng_ObjVertData2D vert_data;
    deng_ObjTextureData tex_data;
} VERT_MAPPED_2D;


typedef struct VERT_UNMAPPED_2D {
    deng_ObjVertData2D vert_data;
    deng_ObjColorData color_data;
} VERT_UNMAPPED_2D;


/* Vertices and indices offsets combined */
typedef struct UNI_OFFSET {
    uint64_t vert_offset;
    uint64_t ind_offset;
} UNI_OFFSET;


/* Vertices data that is allocated on heap */
typedef struct deng_VertDynamic {
    VERT_MAPPED *p_texture_mapped_vert_data;
    VERT_UNMAPPED *p_unmapped_vert_data;

    VERT_MAPPED_2D *p_texture_mapped_vert_data_2d;
    VERT_UNMAPPED_2D *p_unmapped_vert_data_2d;

    /* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
    size_t size;
    uint64_t memory_offset;
} deng_VertDynamic;


/* Indices data that is allocated on heap */
typedef struct deng_IndicesDynamic {
    uint32_t *p_indices;
    size_t size;
    uint64_t memory_offset;
} deng_IndicesDynamic;


/* Pixel data that is allocated on heap */
typedef struct deng_PixelDataDynamic {
    uint8_t *p_pixel_data;
    size_t size;
    uint16_t width;
    uint16_t height;
    uint64_t memory_offset;
} deng_PixelDataDynamic;


/* Specify the asset type and the shader it should use */
typedef enum deng_AssetMode {
    DENG_ASSET_MODE_3D_TEXTURE_MAPPED = 0,
    DENG_ASSET_MODE_3D_UNMAPPED = 1,
    DENG_ASSET_MODE_2D_TEXTURE_MAPPED = 2,
    DENG_ASSET_MODE_2D_UNMAPPED = 3
} deng_AssetMode;


/* Main asset struct */
typedef struct deng_Asset {
    char *name;
    char *id;
    char *description;
    uint64_t time_point; 
    deng_AssetMode asset_mode;
    uint8_t is_shown;
    deng_VertDynamic vertices;
    deng_IndicesDynamic indices;
    
    // For fragment shader
    deng_ObjColorData solid_fill_color;
} deng_Asset;


/* Main texture struct */
typedef struct deng_Texture {
    char *id;
    char *description;
    deng_PixelDataDynamic pixel_data;
} deng_Texture;

#endif
