#include "headers/renderer.h"

namespace deng {
    TextureLoaderBMP::TextureLoaderBMP(const std::string &file_name) {
        std::ifstream texture_file{file_name.c_str(), std::fstream::binary};
        if(texture_file) {
            texture_file.read((char*) &this->m_file_header, sizeof(this->m_file_header));
            if(this->m_file_header.file_type != 0x4D42) {
                ERRME("Unrecognised texture file format!"); 
                return;
            }
            
            texture_file.read((char*) &this->m_info_header, sizeof(this->m_info_header));

            if(this->m_info_header.bit_count == 32) {
                if(this->m_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                    texture_file.read((char*) &this->m_color_header, sizeof(this->m_color_header));
                    if(!this->checkColorData()) { 
                        ERRME("Unexpected color mask format or color space type!");
                        return;
                    }
                }
                else {
                    ERRME("File doesn't contain bit mask information!");
                    ERRME("Unrecognised file format!");
                    return;
                }
            }

            texture_file.seekg(this->m_file_header.offset_data, texture_file.beg);

            if(this->m_info_header.bit_count == 32) {
                this->m_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                this->m_file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            }

            else if(this->m_info_header.bit_count == 24) {
                LOG("Detected texture with 24bit format!");
                this->m_info_header.size = sizeof(BMPInfoHeader);
                this->m_file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
            }

            else {
                ERRME("Detected image bit count of " + std::to_string(this->m_info_header.bit_count) + "bits !");
                ERRME("Only 24 or 32bit bitmap images are allowed!");
                return;
            }

            this->m_file_header.file_size = m_file_header.offset_data;
            if(this->m_info_header.height < 0) {
                ERRME("Only texture files with bottom left origin are supported!");
                return;
            }

            this->pixel_data.resize(this->m_info_header.height);

            // without padding
            if(this->m_info_header.width * this->m_info_header.bit_count / 8 % 4 == 0) {
                for(int32_t hI = 0; hI < this->m_info_header.height; hI++) {
                    this->pixel_data[hI].resize(this->m_info_header.width * this->m_info_header.bit_count / 8);
                    texture_file.read((char*) this->pixel_data[hI].data(), this->m_info_header.width * this->m_info_header.bit_count / 8);
                }
            }

            // with padding
            else {
                std::vector<uint8_t> padding_count_per_row(4 - (this->m_info_header.width * this->m_info_header.bit_count / 8) % 4);
                for(int32_t hI = 0; hI < this->m_info_header.height; hI++) {
                    this->pixel_data[hI].resize(this->m_info_header.width * this->m_info_header.bit_count / 8);
                    texture_file.read((char*) this->pixel_data[hI].data(), this->m_info_header.width * this->m_info_header.bit_count / 8);
                    texture_file.read((char*) padding_count_per_row.data(), padding_count_per_row.size());
                }
            }
        }

        else {
            ERRME("Couldn't read texture file!");
            ERRME("File \"" + file_name + "\" does not exist!");
        }
    }

    bool TextureLoaderBMP::checkColorData() {
        BMPColorHeader expected_color_header;
        if(expected_color_header.red_mask != this->m_color_header.red_mask ||
            expected_color_header.green_mask != this->m_color_header.green_mask ||
            expected_color_header.blue_mask != this->m_color_header.blue_mask ||
            expected_color_header.alpha_mask != this->m_color_header.alpha_mask ||
            expected_color_header.color_space_type != this->m_color_header.color_space_type) {
                return false;
            }

        else return true;
    }

    void TextureLoaderBMP::getTextureDetails(uint32_t *texture_width, uint32_t *texture_height, VkDeviceSize *texture_size, std::vector<uint8_t> &texture_pixel_data) {
        *texture_width = this->m_info_header.width;
        *texture_height = this->m_info_header.height;
        *texture_size = this->m_info_header.width * this->m_info_header.height * 4;

        texture_pixel_data.resize(*texture_size);
        // checks if alpha channel is present and if not then create it
        if(this->m_info_header.bit_count == 32) {
            // rearrangement of pixel data from bottom - right to top - right format 
            for(int32_t hI = this->m_info_header.height - 1, i = 0; hI >= 0; hI--) 
                for(int32_t wI = 0; wI < this->m_info_header.width * 4; wI++, i++) texture_pixel_data[i] = this->pixel_data[hI][wI];
        }

        else if(this->m_info_header.bit_count == 24) {
            for(int32_t hI = this->m_info_header.height - 1, i = 0; hI >= 0; hI--) {
                for(int32_t wI = 0; wI < this->m_info_header.width * 3; wI += 3, i += 4) {
                    texture_pixel_data[i] = this->pixel_data[hI][wI];
                    texture_pixel_data[i + 1] = this->pixel_data[hI][wI + 1];
                    texture_pixel_data[i + 2] = this->pixel_data[hI][wI + 2];
                    texture_pixel_data[i + 3] = 255;
                }
            }
        }

        // FileManager fm;
        // fm.writeToFile("rgbbitmap.log", "#entry point", DENG_WRITEMODE_REWRITE);
        // fm.writeToFile("rgbbitmap.log", "width=" + std::to_string(this->m_info_header.width), DENG_WRITEMODE_FROM_END);
        // fm.writeToFile("rgbbitmap.log", "height=" + std::to_string(this->m_info_header.height), DENG_WRITEMODE_FROM_END);
        // for(size_t i = 0; i < texPixelData.size(); i++) {
        //     fm.writeToFile("rgbbitmap.log", "{" + std::to_string(texPixelData[i]) + "}", DENG_WRITEMODE_FROM_END);
        // }
    }


    //tga format
    TextureLoaderTGA::TextureLoaderTGA(const std::string &file_name) {
        std::ifstream texture_file(file_name.c_str(), std::fstream::binary);
        if(texture_file) {
            texture_file.read((char*) &this->m_type_header, sizeof(this->m_type_header));
            
            switch (this->m_type_header.image_type)
            {
            case 0:
                ERRME("No image data is available!");
                return;
                break;

            case 1:
                ERRME("Color-mapped tga is not supported!");
                return;
                break;

            case 3:
                ERRME("Grayscale tga is not supported!");
                return;
                break;

            case 9:
                ERRME("RLE color-mapped tga is not supported!");
                return;
                break;

            case 10:
                ERRME("RLE true-color tga is not supported!");
                return;
                break;

            case 11:
                ERRME("RLE grayscaled tga is not supported!");
                return;
                break;
            
            default:
                break;
            }

            texture_file.read((char*) &this->m_colormap_header, sizeof(this->m_colormap_header));
            texture_file.read((char*) &this->m_info_header, sizeof(this->m_info_header));
            this->m_pixeldata.resize(this->m_info_header.height);

            if(this->m_info_header.bit_count == 32 || this->m_info_header.bit_count == 24) {
                for(uint32_t hI = 0; hI < this->m_info_header.height; hI++) {
                    this->m_pixeldata[hI].resize(this->m_info_header.width * this->m_info_header.bit_count / 8);
                    texture_file.read((char*) this->m_pixeldata[hI].data(), this->m_info_header.width * this->m_info_header.bit_count / 8);
                }                
            }

            else {
                ERRME("Unknown or corrupt texture file!");
                return;
            }
        }

        else {
            ERRME("Couldn't read texture file!");
            ERRME("File \"" + file_name + "\" does not exist!");
        }
    }

    void TextureLoaderTGA::getTextureDetails(uint32_t *texWidth, uint32_t *texHeight, VkDeviceSize *texSize, std::vector<uint8_t> &texPixelData) {
        *texWidth = static_cast<uint32_t>(this->m_info_header.height);
        *texHeight = static_cast<uint32_t>(this->m_info_header.width);
        *texSize = this->m_info_header.width * this->m_info_header.height * 4;

        texPixelData.resize(*texSize);

        //no pixel rearrangement needed
        if(this->m_info_header.y_origin == this->m_info_header.height) {
            for(int32_t hI = 0, i = 0; hI < this->m_info_header.height; hI++) {
                for(int32_t wI = 0; wI < (this->m_info_header.width * this->m_info_header.bit_count / 8); wI++, i++) {
                    if(this->m_info_header.bit_count == 32) texPixelData[i] = this->m_pixeldata[hI][wI];
                    else if(this->m_info_header.bit_count == 24) {
                        texPixelData[i] = this->m_pixeldata[hI][wI];
                        texPixelData[i + 1] = this->m_pixeldata[hI][wI + 1];
                        texPixelData[i + 2] = this->m_pixeldata[hI][wI + 2];
                        texPixelData[i + 3] = 255;
                        wI += 2;
                        i += 3;
                    }
                }
            }
        }

        //pixels need to be rearranged from bottom - right to top - right order
        else if(this->m_info_header.y_origin == 0) {
            for(int32_t hI = this->m_info_header.height - 1, i = 0; hI >= 0; hI--) {
                for(int32_t wI = 0; wI < this->m_info_header.width * this->m_info_header.bit_count / 8; wI++, i++) {
                    if(this->m_info_header.bit_count == 32) texPixelData[i] = this->m_pixeldata[hI][wI];
                    else if(this->m_info_header.bit_count == 24) {
                        texPixelData[i] = this->m_pixeldata[hI][wI];
                        texPixelData[i + 1] = this->m_pixeldata[hI][wI + 1];
                        texPixelData[i + 2] = this->m_pixeldata[hI][wI + 2];
                        texPixelData[i + 3] = 255;
                        wI += 2;
                        i += 3;
                    }
                }
            }
        }
    }

    dengTextureFormat getTexFileFormat(const std::string &texFilePath) {
        if(texFilePath.find(".bmp") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_BMP;
        else if(texFilePath.find(".tga") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_TGA;
        else if(texFilePath.find(".png") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_PNG;
        else if(texFilePath.find(".jpg") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_JPG;
        else return DENG_TEXTURE_FORMAT_UNKNOWN;
    }
}