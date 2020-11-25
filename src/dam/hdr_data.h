#ifndef DATA_H
#define DATA_H

#define VERT_TYPE_TEXTURE_MAPPED
#define VERT_TYPE_UNMAPPED

/* DAS file format headers */
typedef struct INFO_HDR {
    char *hdr_name;
    uint32_t hdr_size;
    uint8_t name_size;
    char *asset_name;
    uint8_t desc_size;
    char *desc;
} INFO_HDR;

typedef struct VERT_HDR {
    char *hdr_name;
    uint32_t hdr_size;
    uint32_t vert_count;
    uint8_t vert_type;
    DynamicVertices vertices_data;
} VERT_HDR;

typedef struct INDX_HDR {
    char *hdr_name;
    uint32_t hdr_size;
    uint32_t indices_count;
    DynamicIndices indices_data;
} INDX_HDR;

typedef struct TPIX_HDR {
    char *hdr_name;
    uint32_t hdr_size;
    uint16_t img_width;
    uint16_t img_height;
    DynamicPixelData pixel_data; 
} TPIX_HDR;


/* BMP image headers */
#pragma pack(1)
typedef struct BMPFileHeader {
    uint16_t file_type;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset_data;
} BMPFileHeader;

typedef struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;

    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t size_image;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter; 
    uint32_t colors_used;
    uint32_t colors_important;
} BMPInfoHeader;

typedef struct BMPColorHeader {
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t alpha_mask;
    uint32_t color_space_type;
    uint32_t unused[16];
} BMPColorHeader;
#pragma pack()

/* TGA image headers */
#pragma pack(1)
typedef struct TGATypeHeader {
    uint8_t id_lenght;
    uint8_t colormap_type;
    uint8_t image_type;
} TGATypeHeader;

typedef struct TGAColorMapHeader {
    uint16_t first_colormap_index;
    uint16_t colormap_length;
    uint8_t entity_size;
} TGAColorMapHeader;

typedef struct TGAInfoHeader {
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t bit_count;
    uint8_t image_descriptor;
} TGAInfoHeader;
#pragma pack()

void init_BMP_image_headers(BMPFileHeader *p_file_header, BMPInfoHeader *p_info_header, BMPColorHeader *p_color_header);
void init_TGA_image_headers(TGATypeHeader *p_type_header, TGAColorMapHeader *p_color_map_header, TGAInfoHeader *p_info_header);

#endif