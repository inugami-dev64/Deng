#ifndef ASSETS_H
#define ASSETS_H

/* Vertex data structs */
typedef struct deng_ObjVertData {
    deng_vec_t vert_x;
    deng_vec_t vert_y;
    deng_vec_t vert_z;
} deng_ObjVertData;

typedef struct deng_ObjVertData2D {
    deng_vec_t vert_x;
    deng_vec_t vert_y;
} deng_ObjVertData2D;


/* Fragment data structs */
typedef struct deng_ObjTextureData {
    deng_vec_t tex_x;
    deng_vec_t tex_y;
} deng_ObjTextureData;


typedef struct deng_ObjNormalData {
    deng_vec_t nor_x;
    deng_vec_t nor_y;
    deng_vec_t nor_z;
} deng_ObjNormalData;


typedef struct deng_ObjColorData {
    deng_vec_t col_r;
    deng_vec_t col_g;
    deng_vec_t col_b;
    deng_vec_t col_a;
} deng_ObjColorData;



/*********************************************/
/****** General shader vertices structs ******/
/*********************************************/

typedef struct VERT_MAPPED_UNOR {
    deng_ObjVertData vert_data;
    deng_ObjTextureData tex_data;
} VERT_MAPPED_UNOR;


typedef struct VERT_UNMAPPED_UNOR {
    deng_ObjVertData vert_data;
    deng_ObjColorData color_data;
} VERT_UNMAPPED_UNOR;

typedef struct VERT_MAPPED_NOR {
    deng_ObjVertData vert_data;
    deng_ObjTextureData tex_data;
    deng_ObjNormalData norm_data;
} VERT_MAPPED_NOR;


typedef struct VERT_UNMAPPED_NOR {
    deng_ObjVertData vert_data;
    deng_ObjColorData color_data;
    deng_ObjNormalData norm_data;
} VERT_UNMAPPED_NOR;


// 2D vertices data
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
    deng_ui64_t vert_offset;
    deng_ui64_t ind_offset;
} UNI_OFFSET;


/* Vertices data that is allocated on heap */
typedef struct deng_VertDynamic {
    VERT_MAPPED_UNOR *p_tex_mapped_unnormalized_vert;
    VERT_UNMAPPED_UNOR *p_unmapped_unnormalized_vert;
    VERT_MAPPED_NOR *p_tex_mapped_normalized_vert;
    VERT_UNMAPPED_NOR *p_unmapped_normalized_vert;

    VERT_MAPPED_2D *p_tex_mapped_vert_data_2d;
    VERT_UNMAPPED_2D *p_unmapped_vert_data_2d;

    size_t size;
    deng_ui64_t memory_offset;
} deng_VertDynamic;


/* Indices data that is allocated on heap */
typedef struct deng_IndicesDynamic {
    deng_ui32_t *p_indices;
    size_t size;
    deng_ui64_t memory_offset;
} deng_IndicesDynamic;


/* Pixel data that is allocated on heap */
typedef struct deng_PixelDataDynamic {
    deng_ui8_t *p_pixel_data;
    size_t size;
    deng_ui16_t width;
    deng_ui16_t height;
    deng_ui64_t memory_offset;
} deng_PixelDataDynamic;


/* Specify the asset type and the shader it should use */
typedef enum deng_AssetMode {
    DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED    = 0,
    DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED          = 1,
    DENG_ASSET_MODE_3D_TEXTURE_MAPPED               = 2,
    DENG_ASSET_MODE_3D_UNMAPPED                     = 3,
    DENG_ASSET_MODE_2D_TEXTURE_MAPPED               = 4,
    DENG_ASSET_MODE_2D_UNMAPPED                     = 5,
    DENG_ASSET_MODE_DONT_CARE                       = 6
} deng_AssetMode;


/* Main asset struct */
typedef struct deng_Asset {
    char *id;
    char *tex_id;
    char *description;
    deng_ui64_t time_point; 
    deng_AssetMode asset_mode;
    deng_ui8_t is_shown;
    deng_VertDynamic vertices;
    deng_IndicesDynamic indices;
} deng_Asset;


/* Main texture struct */
typedef struct deng_Texture {
    char *id;
    char *description;
    deng_PixelDataDynamic pixel_data;
} deng_Texture;

#endif
