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
#ifdef DAS_HANDLER_ONLY
    // Generic file extension name adder
    char *get_file_ext_name(char *file_name); 
    void assemble_INFO_HDR(char *asset_name, char *description, FILE *file);
    void assemble_VERT_HDR(DynamicVertices *p_vertices, FILE *file);
    void assemble_INDX_HDR(DynamicIndices *p_indices, FILE *file);
    void assemble_TPIX_HDR(DynamicPixelData *p_pixel_data, FILE *file);
#endif
/* Callback function for creating .das binary asset file */
void assemble_das(Asset *p_asset, char *file_name);

/* das reader functions */
#ifdef DAS_HANDLER_ONLY
    void read_INFO_HDR(char **asset_name, char **description, uint64_t *p_time_point, char *file_name, FILE *file);
    void read_VERT_HDR(DynamicVertices *p_vertices, char *file_name, FILE *file);
    void read_INDX_HDR(DynamicIndices *p_indices, char *file_name, FILE *file);
    void read_TPIX_HDR(DynamicPixelData *p_pixel_data, char *file_name, FILE *file);
#endif
/* Callback function for reading .das binary asset file */
void read_das(Asset *p_asset, char *file_name);

#endif