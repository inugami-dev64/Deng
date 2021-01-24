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
void dasLoadTexture(deng_Texture *p_asset, const char *file_name);
#ifdef DAS_EXT_LOADERS
    ImageFormat dasDetectImageFormat(const char *file_name);
    void dasLoadBMPimage(deng_Texture *p_asset, const char *file_name);
    void dasLoadTGAimage(deng_Texture *p_asset, const char *file_name);
#endif

/* Vertices data loading */
/* Vertices type values define the following: */
    /* 0 - UNMAPPED */
    /* 1 - TEXTURE MAPPED */
void dasLoadModel(deng_Asset *p_asset, const char *file_name);
#ifdef DAS_EXT_LOADERS
    deng_i32_t dasGetIndex(char *buffer, size_t cur_index);
    void dasLoadOBJmodelVertices(deng_ObjVertData **pp_vert_data, size_t *p_vert_data_size, deng_ObjTextureData **pp_texture_data, size_t *p_tex_data_size, long file_size);
    void dasLoadOBJindices(deng_ui32_t **pp_vert_indices, size_t *p_vert_indices_size, deng_ui32_t **pp_tex_vert_indices, size_t *p_tex_vert_indices_size, long file_size);
    void dasIndexUnmappedVertices(deng_ObjVertData **pp_vert_data, size_t vert_size, deng_ui32_t **pp_indices, size_t indices_size, deng_Asset *p_asset);
    
    void dasIndexTexMappedVertices(deng_ObjVertData **pp_vert_data, size_t vert_size, deng_ObjTextureData **pp_tex_data, size_t tex_size, deng_ui32_t **pp_vert_indices, size_t vert_indices_size, deng_ui32_t **pp_tex_indices, 
    size_t tex_indices_size, deng_Asset *p_asset);
#endif

#ifdef DAS_GENERIC_HELPERS
    char *dasGetFileExt(const char *file_name);
    void dasCheckForMemAllocErr(void *mem_addr);
    void dasCleanCharBuffer(char *buffer, size_t size);
#endif

#endif