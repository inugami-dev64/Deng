#ifndef LOADER_H
#define LOADER_H

typedef enum ImageFormat {
    IMAGE_FORMAT_BMP = 0,
    IMAGE_FORMAT_TGA = 1,
    IMAGE_FORMAT_PNG = 2,
    IMAGE_FORMAT_JPG = 3,
    IMAGE_FORMAT_UNKNOWN = 4
} ImageFormat;

/* Image loading */
void dasLoadTexture (
    deng_Texture *p_asset, 
    const char *file_name
);

#define DEFAULT_ASSET_COLOR (deng_ObjColorData) {0.7f, 0.7f, 0.7f, 1.0f}
#ifdef DAS_EXT_LOADERS
    ImageFormat dasDetectImageFormat(const char *file_name);
    void dasLoadBMPimage (
        deng_Texture *p_asset, 
        const char *file_name
    );
    
    void dasLoadTGAimage (
        deng_Texture *p_asset, 
        const char *file_name
    );
#endif

/* Vertices data loading */
/* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
void dasLoadModel (
    deng_Asset *p_asset, 
    char *file_name
);
#ifdef DAS_EXT_LOADERS
    #define CORRUPTION_OBJ_ERR(x) fprintf(stderr, "Corrupted Wavefront OBJ file '%s'\n", x), \
                                  exit(EXIT_FAILURE)
    typedef struct IndexSet {
        deng_ui32_t vert;
        deng_ui32_t tex;
        deng_ui32_t norm;
    } IndexSet;


    void dasLoadOBJmodelVertices (
        deng_ObjVertData **pp_vert_data, 
        size_t *p_vert_data_size, 
        deng_ObjTextureData **pp_texture_data, 
        size_t *p_tex_data_size, 
        deng_ObjNormalData **pp_norm_data,
        size_t *p_norm_data_size,
        char *file_name,
        long file_size
    );

    void dasLoadOBJindices (
        IndexSet **p_indices,
        deng_bool_t read_tex_indices,
        deng_bool_t read_norm_indices,
        size_t *p_indices_c, 
        char *file_name,
        long file_size
    );
    
    /* Create new indices for already indexed vertices */
    void dasMakeIndexedVertices (
        deng_Asset *p_asset,
        IndexSet *indices,
        size_t indices_c,
        deng_ObjVertData *p_vert_data,
        deng_ObjTextureData *p_tex_data,
        deng_ObjNormalData *p_norm_data
    );
#endif

#ifdef DAS_GENERIC_HELPERS
    char *dasGetFileExt(const char *file_name);
    void dasCheckForMemAllocErr(void *mem_addr);
    void dasCleanCharBuffer(char *buffer, size_t size);
#endif

#endif
