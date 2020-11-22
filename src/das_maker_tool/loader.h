#ifndef LOADER_H
#define LOADER_H

typedef enum ImageFormat {
    IMAGE_FORMAT_BMP = 0,
    IMAGE_FORMAT_TGA = 1,
    IMAGE_FORMAT_UNKNOWN = 2
} ImageFormat;

typedef enum VerticesColorProps {
    VERTICES_COLOR_MODE_UNMAPPED = 0,
    VERTICES_COLOR_MODE_TEXTURE_MAPPED = 1 
} VerticesColorProps;

/* Image loading */
ImageFormat detect_image_format(const char *file_name);
void load_TGA_image(DynamicPixelData *p_pixel_data, const char *file_name);
void load_BMP_image(DynamicPixelData *p_pixel_data, const char *file_name);

typedef struct OBJVerticesData {
    float vert_x;
    float vert_y;
    float vert_z;
} OBJVerticesData;

typedef struct OBJTextureData {
    float tex_x;
    float tex_y;
} OBJTextureData;

/* Vertices data loading */
void load_model(DynamicVertices *p_vertices, DynamicIndices *p_indices, VerticesColorProps props, const char *file_name);

#endif