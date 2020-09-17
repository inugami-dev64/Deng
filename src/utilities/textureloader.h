#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

namespace deng {

    //bmp format
    #pragma pack(1)
    struct BMPFileHeader {
        uint16_t file_type{0x4D42};
        uint32_t file_size{0};
        uint16_t reserved1{0};
        uint16_t reserved2{0};
        uint32_t offset_data{0};
    };

    struct BMPInfoHeader {
        uint32_t size{0};
        int32_t width{0};
        int32_t height{0};

        uint16_t planes{1};
        uint16_t bit_count{0};
        uint32_t compression{0};
        uint32_t size_image{0};
        int32_t x_pixels_per_meter{0};
        int32_t y_pixels_per_meter{0}; 
        uint32_t colors_used {0};
        uint32_t colors_important{0};
    };

    struct BMPColorHeader {
        uint32_t red_mask{0x00ff0000};
        uint32_t green_mask{0x0000ff00};
        uint32_t blue_mask{0x000000ff};
        uint32_t alpha_mask{0xff000000};
        uint32_t color_space_type{0x73524742};
        uint32_t unused[16]{0}; 
    };

    #pragma pack()

    class TextureLoaderBMP {
    private:
        BMPFileHeader m_file_header;
        BMPInfoHeader m_info_header;
        BMPColorHeader m_color_header;
        std::vector<std::vector<uint8_t>> pixel_data;

    private:
        bool checkColorData();

    public:
        TextureLoaderBMP(const std::string &file_name);
        void getTextureDetails(uint32_t *p_texture_width, uint32_t *p_texture_height, VkDeviceSize *texture_size, std::vector<uint8_t> &texture_pixel_data);
    };

    // tga format
    #pragma pack(1)
    struct TGATypeHeader {
        uint8_t id_lenght{0};
        uint8_t colormap_type{0};
        uint8_t image_type{0};
    };

    struct TGAColorMapHeader {
        uint16_t first_colormap_index{0};
        uint16_t colormap_length{0};
        uint8_t entity_size{0};
    };

    struct TGAInfoHeader {
        uint16_t x_origin{0};
        uint16_t y_origin{0};
        uint16_t width{0};
        uint16_t height{0};
        uint8_t bit_count{0};
        uint8_t image_descriptor{0};
    };
    #pragma pack()

    class TextureLoaderTGA {
    private:
        TGATypeHeader m_type_header;
        TGAColorMapHeader m_colormap_header;
        TGAInfoHeader m_info_header;
        std::vector<std::vector<uint8_t>> m_pixeldata;

    public:
        TextureLoaderTGA(const std::string &file_name);
        void getTextureDetails(uint32_t *p_texture_width, uint32_t *p_texture_height, VkDeviceSize *p_texture_size, std::vector<uint8_t> &texture_pixel_data);
    };

    dengTextureFormat getTexFileFormat(const std::string &texture_filepath);
}



#endif