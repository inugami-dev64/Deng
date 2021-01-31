#ifndef DATA_H
#define DATA_H

/* BMP image headers */
#pragma pack(1)
typedef struct BMPFileHeader {
    deng_ui16_t file_type;
    deng_ui32_t file_size;
    deng_ui16_t reserved1;
    deng_ui16_t reserved2;
    deng_ui32_t offset_data;
} BMPFileHeader;

typedef struct BMPInfoHeader {
    deng_ui32_t size;
    deng_i32_t width;
    deng_i32_t height;

    deng_ui16_t planes;
    deng_ui16_t bit_count;
    deng_ui32_t compression;
    deng_ui32_t size_image;
    deng_i32_t x_pixels_per_meter;
    deng_i32_t y_pixels_per_meter; 
    deng_ui32_t colors_used;
    deng_ui32_t colors_important;
} BMPInfoHeader;

typedef struct BMPColorHeader {
    deng_ui32_t red_mask;
    deng_ui32_t green_mask;
    deng_ui32_t blue_mask;
    deng_ui32_t alpha_mask;
    deng_ui32_t color_space_type;
    deng_ui32_t unused[16];
} BMPColorHeader;
#pragma pack()

/* TGA image headers */
#pragma pack(1)
typedef struct TGATypeHeader {
    deng_ui8_t id_lenght;
    deng_ui8_t colormap_type;
    deng_ui8_t image_type;
} TGATypeHeader;

typedef struct TGAColorMapHeader {
    deng_ui16_t first_colormap_index;
    deng_ui16_t colormap_length;
    deng_ui8_t entity_size;
} TGAColorMapHeader;

typedef struct TGAInfoHeader {
    deng_ui16_t x_origin;
    deng_ui16_t y_origin;
    deng_ui16_t width;
    deng_ui16_t height;
    deng_ui8_t bit_count;
    deng_ui8_t image_descriptor;
} TGAInfoHeader;
#pragma pack()

void das_InitBMPImageHeaders (
    BMPFileHeader *p_file_header, 
    BMPInfoHeader *p_info_header, 
    BMPColorHeader *p_color_header
);

void das_InitTGAImageHeaders (
    TGATypeHeader *p_type_header, 
    TGAColorMapHeader *p_color_map_header, 
    TGAInfoHeader *p_info_header
);
#endif
