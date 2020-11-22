#include "asset_creator_core.h"

void init_mapped_vertices_arr(DynamicVertices *p_vertices) {
    p_vertices->size = 0;
    p_vertices->p_mapped_vertices = (VERT_MAPPED*) malloc(sizeof(VERT_MAPPED));
    p_vertices->p_unmapped_vertices = NULL;
    p_vertices->vertices_type = 1;
}

void init_unmapped_vertices_arr(DynamicVertices *p_vertices) {
    p_vertices->size = 0;
    p_vertices->p_mapped_vertices = NULL;
    p_vertices->p_unmapped_vertices = (VERT_UNMAPPED*) malloc(sizeof(VERT_UNMAPPED));
    p_vertices->vertices_type = 0;
}

void init_indices_arr(DynamicIndices *p_indices) {
    p_indices->size = 0;
    p_indices->p_indices = (uint32_t*) malloc(sizeof(uint32_t));
}

void init_pixel_data_arr(DynamicPixelData *p_pixel_data) {
    p_pixel_data->size = 0;
    p_pixel_data->p_pixel_data = (uint8_t*) malloc(sizeof(uint8_t));
}

void push_mapped_vertices_to_arr(DynamicVertices *p_vertices, VERT_MAPPED data) { 
    p_vertices->size++;
    p_vertices->p_mapped_vertices = (VERT_MAPPED*) realloc(p_vertices->p_mapped_vertices, p_vertices->size * sizeof(VERT_MAPPED));
    p_vertices->p_mapped_vertices[p_vertices->size - 1] = data;
}

void push_unmapped_vertices_to_arr(DynamicVertices *p_vertices, VERT_UNMAPPED data) {
    p_vertices->size++;
    p_vertices->p_unmapped_vertices = (VERT_UNMAPPED*) realloc(p_vertices->p_unmapped_vertices, p_vertices->size * sizeof(VERT_UNMAPPED));
    p_vertices->p_unmapped_vertices[p_vertices->size - 1] = data;
}

void push_indices_to_arr(DynamicIndices *p_indices, uint32_t data) {
    p_indices->size++;
    p_indices->p_indices = (uint32_t*) realloc(p_indices->p_indices, p_indices->size * sizeof(uint32_t));
    p_indices->p_indices[p_indices->size - 1] = data;
}

void push_pixel_data_to_arr(DynamicPixelData *p_pixel_data, uint8_t data) {
    p_pixel_data->size++;
    p_pixel_data->p_pixel_data = (uint8_t*) realloc(p_pixel_data->p_pixel_data, p_pixel_data->size * sizeof(uint8_t));
    p_pixel_data->p_pixel_data[p_pixel_data->size - 1] = data;
}

void resize_mapped_vertices_arr(DynamicVertices *p_vertices, size_t size) {
    p_vertices->size = size;
    p_vertices->p_mapped_vertices = (VERT_MAPPED*) realloc(p_vertices->p_mapped_vertices, p_vertices->size * sizeof(VERT_MAPPED));
}

void resize_unmapped_vertices_arr(DynamicVertices *p_vertices, size_t size) {
    p_vertices->size = size;
    p_vertices->p_unmapped_vertices = (VERT_UNMAPPED*) realloc(p_vertices->p_unmapped_vertices, p_vertices->size * sizeof(VERT_UNMAPPED));
}

void resize_indices_arr(DynamicIndices *p_indices, size_t size) {
    p_indices->size = size;
    p_indices->p_indices = (uint32_t*) realloc(p_indices->p_indices, p_indices->size * sizeof(uint32_t));
}

void resize_pixel_data_arr(DynamicPixelData *p_pixel_data, size_t size) {
    p_pixel_data->size = size;
    p_pixel_data->p_pixel_data = (uint8_t*) realloc(p_pixel_data->p_pixel_data, p_pixel_data->size * sizeof(uint8_t));
}