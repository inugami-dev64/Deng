#ifndef DYN_ARR_H
#define DYN_ARR_H

typedef struct VERT_UNMAPPED {
    float coord_x, coord_y, coord_z;
    float col_r, col_g, col_b, col_a;
} VERT_UNMAPPED;

typedef struct VERT_MAPPED {
    float coord_x, coord_y, coord_z;
    float tex_x, tex_y, tex_z;
} VERT_MAPPED;

typedef struct DynamicVertices {
    VERT_MAPPED *p_mapped_vertices;
    VERT_UNMAPPED *p_unmapped_vertices;
    uint8_t vertices_type;
    size_t size;
} DynamicVertices;

typedef struct DynamicIndices {
    uint32_t *p_indices;
    size_t size;
} DynamicIndices;

typedef struct DynamicPixelData {
    uint8_t *p_pixel_data;
    size_t size;
} DynamicPixelData;

void init_mapped_vertices_arr(DynamicVertices *p_vertices);
void init_unmapped_vertices_arr(DynamicVertices *p_vertices);
void init_indices_arr(DynamicIndices *p_indices);
void init_pixel_data_arr(DynamicPixelData *p_pixel_data);

void push_mapped_vertices_to_arr(DynamicVertices *p_vertices, VERT_MAPPED data);
void push_unmapped_vertices_to_arr(DynamicVertices *p_vertices, VERT_UNMAPPED data);
void push_indices_to_arr(DynamicIndices *p_indices, uint32_t data);
void push_pixel_data_to_arr(DynamicPixelData *p_pixel_data, uint8_t data);

void resize_mapped_vertices_arr(DynamicVertices *p_vertices, size_t size);
void resize_unmapped_vertices_arr(DynamicVertices *p_vertices, size_t size);
void resize_indices_arr(DynamicIndices *p_indices, size_t size);
void resize_pixel_data_arr(DynamicPixelData *p_pixel_data, size_t size);

#endif