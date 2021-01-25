#ifndef DAS_HANDLER_H
#define INFO_HEADER_NAME "INFO_HDR"
#define VERTICES_HEADER_NAME "VERT_HDR"
#define INDICES_HEADER_NAME "INDX_HDR"
#define TEXTURE_PIXEL_NAME "TPIX_HDR"


/* All structs of subflags that can be used with main flags */
/* These structs contain oly raw text data and the data needs to be proccessed later */ 
// All local/assemble flags
typedef struct Frag {
    char *color_data[4];
    char *tex_path;
} Frag;

/* das assembler functions */
#ifdef DAS_EXT_HANDLER
    // Generic file extension name adder
    char *dasGetFileExtName(char *file_name); 
    void dasAssembleINFOHDR(char *asset_name, char *description, FILE *file);
    void dasAssembleVERTHDR(deng_VertDynamic *p_vertices, FILE *file);
    void dasAssembleINDXHDR(deng_IndicesDynamic *p_indices, FILE *file);
#endif

/* Callback function for creating .das binary asset file */
void dasAssemble(deng_Asset *passet, const char *filename);

/* das reader functions */
#ifdef DAS_EXT_HANDLER
    void dasReadINFOHDR(char **assetname, char **description, deng_ui64_t *ptimepoint, char *filename, FILE *file);
    void dasReadVERTHDR(deng_VertDynamic *pvertices, char *filename, FILE *file);
    void dasReadINDXHDR(deng_IndicesDynamic *pindices, char *filename, FILE *file);
#endif

/* Callback function for reading .das binary asset file */
void dasReadAsset(deng_Asset *p_asset, const char *file_name, deng_AssetMode tex_mode);
void dasDestroyAssets(deng_Asset *assets, deng_i32_t asset_c);
void dasDestroyTextures(deng_Texture *textures, deng_i32_t tex_c);
#endif