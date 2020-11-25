#ifndef ASSEMBLER_H
#define INFO_HEADER_NAME "INFO_HDR"
#define VERTICES_HEADER_NAME "VERT_HDR"
#define INDICES_HEADER_NAME "INDX_HDR"
#define TEXTURE_PIXEL_DATA "TPIX_HDR"

#ifdef ASSEMBLER_ONLY
    static const char *get_file_ext_name(const char *file_name);
    static void assemble_INFO_HDR(const char *asset_name, const char *description);
    static void assemble_VERT_HDR(DynamicVertices *p_vertices);
    static void assemble_INDX_HDR(DynamicIndices *p_indices);
    static void assemble_TPIX_HDR(DynamicPixelData *p_pixel_data);
#endif
/* Callback function for creating .das binary asset file */
void assemble_das(DynamicVertices *p_vertices, DynamicIndices *p_indices, DynamicPixelData *p_pixel_data, 
const char *file_name, const char *asset_name, const char *description);

#endif