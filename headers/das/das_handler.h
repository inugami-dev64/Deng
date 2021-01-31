#ifndef DAS_HANDLER_H

#define INFO_HEADER_NAME "INFO_HDR"
#define VERTICES_HEADER_NAME "VERT_HDR"
#define INDICES_HEADER_NAME "INDX_HDR"

/* das assembler functions */
#ifdef DAS_EXT_HANDLER
    // Generic file extension name adder
    char *dasGetDASFileName(char *file_name); 
    void dasAssembleINFOHDR (
        char *asset_name, 
        deng_AssetMode asset_mode, 
        FILE *file
    );

    void dasAssembleVERTHDR (
        deng_VertDynamic *p_vertices, 
        deng_AssetMode asset_mode, 
        FILE *file
    );

    void dasAssembleINDXHDR (
        deng_IndicesDynamic *p_indices, 
        FILE *file
    );
#endif

/* Callback function for creating .das binary asset file */
void dasAssemble (
    deng_Asset *p_asset, 
    const char *out_file
);

/* das reader functions */
#ifdef DAS_EXT_HANDLER
    #define CORRUPTION_DAS_ERR(x) fprintf(stderr, "Corrupted DENG asset file '%s'\n", x), \
                                  exit(EXIT_FAILURE)
    void dasReadINFOHDR (
        char **p_asset_name, 
        deng_ui64_t *p_timestamp, 
        deng_AssetMode *p_asset_mode,
        char *file_name, 
        FILE *file
    );
    
    void dasReadVERTHDR (
        deng_VertDynamic *p_vertices, 
        deng_AssetMode file_asset_mode,
        deng_AssetMode new_asset_mode,
        char *file_name, 
        FILE *file
    );

    void dasReadINDXHDR (
        deng_IndicesDynamic *p_indices, 
        char *filename, 
        FILE *file
    );
#endif

/* Callback function for reading .das binary asset file */
void dasReadAsset (
    deng_Asset *p_asset, 
    const char *file_name,
    deng_AssetMode f_asset_mode
);

void dasDestroyAssets (
    deng_Asset *assets, 
    deng_i32_t asset_c
);

void dasDestroyTextures (
    deng_Texture *textures, 
    deng_i32_t tex_c
);
#endif
