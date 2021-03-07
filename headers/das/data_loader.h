#ifndef __DATA_LOADER_H
#define __DATA_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

#define CORRUPTION_OBJ_ERR(x) fprintf(stderr, "Corrupted Wavefront OBJ file '%s'\n", x), \
                              exit(EXIT_FAILURE)
#define DEFAULT_ASSET_COLOR (deng_ObjColorData) {0.7f, 0.7f, 0.7f, 1.0f}



typedef enum ImageFormat {
    IMAGE_FORMAT_BMP = 0,
    IMAGE_FORMAT_TGA = 1,
    IMAGE_FORMAT_PNG = 2,
    IMAGE_FORMAT_JPG = 3,
    IMAGE_FORMAT_UNKNOWN = 4
} ImageFormat;


#ifdef __DATA_LOADER_C
    #include <stdlib.h> // size_t, malloc(), calloc()
    #include <stdio.h> // FILE, ftell(), fread()..
    #include <string.h> // memset()
    #include <common/base_types.h>
    #include <common/common.h>
    #include <common/hashmap.h>
    #include <das/hdr_data.h>
    #include <das/assets.h>

    ImageFormat dasDetectImageFormat(const char *file_name);
    /*
     * Read raw bitmap data from file
     * This function expects the file to be uncompressed and
     */
    void dasReadBitmap (
        FILE *file,
        deng_ui8_t bit_c,
        deng_bool_t vert_re,
        deng_Texture* p_tex
    );

    void dasLoadBMPimage (
        deng_Texture *p_asset, 
        const char *file_name
    );
    
    void dasLoadTGAimage (
        deng_Texture *p_asset, 
        const char *file_name
    );

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


/* Image loading */
void dasLoadTexture (
    deng_Texture *p_asset, 
    const char *file_name
);


/* Vertices data loading */
/* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
void dasLoadModel (
    deng_Asset *p_asset, 
    char *file_name
);

#ifdef DAS_GENERIC_HELPERS
    char *dasGetFileExt(const char *file_name);
    void dasCheckForMemAllocErr(void *mem_addr);
    void dasCleanCharBuffer(char *buffer, size_t size);
#endif



#ifdef __cplusplus
    }
#endif

#endif
