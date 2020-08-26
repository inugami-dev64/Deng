#include "headers/renderer.h"

namespace deng {
    TextureLoader::TextureLoader(const std::string &fileName) {
        std::ifstream textureFile{fileName.c_str(), std::fstream::binary};
        if(textureFile) {
            textureFile.read((char*) &this->m_file_header, (int) sizeof(this->m_file_header));
            if(this->m_file_header.file_type != 0x4D42) {
                ERRME("Unrecognised texture file format!"); 
                return;
            }
            
            textureFile.read((char*) &this->m_info_header, (int) sizeof(this->m_info_header));

            if(this->m_info_header.bit_count == 32) {
                if(this->m_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                    textureFile.read((char*) &this->m_color_header, sizeof(this->m_color_header));
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

            textureFile.seekg(this->m_file_header.offset_data, textureFile.beg);

            if(this->m_info_header.bit_count == 32) {
                this->m_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                this->m_file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            }
            else {
                ERRME("Detected image bit count of " + std::to_string(this->m_info_header.bit_count) + "bits !");
                ERRME("Only 32bit bitmap images are allowed!");
                return;
            }

            this->m_file_header.file_size = m_file_header.offset_data;
            if(this->m_info_header.height < 0) {
                ERRME("Only texture files with bottom left origin are supported!");
                return;
            }

            this->pixelData.resize(this->m_info_header.width * this->m_info_header.height * 4);

            //without padding
            if(this->m_info_header.width % 4 == 0) {
                textureFile.read((char*) this->pixelData.data(), this->m_info_header.width * this->m_info_header.height * 4);
                this->m_file_header.file_size += static_cast<uint32_t>(this->pixelData.size());
            }

            //with padding
            else {
                // int32_t padding_count = this->m_info_header.width * 4;
                // for(size_t hI = 1; hI <= this->m_info_header.height; hI++) {
                //     for(size_t wI = 1; wI <= this->m_info_header.width - padding_count; wI++) {
                //         textureFile.read((char*) this->pixelData.data(), 4);
                //     }
                //     textureFile.seekg(textureFile.tellg() + padding_count);
                // }

                ERRME("No padding supported!");
                ERR("Please use image with following dimentional requirements fulfilled: w % 4 = 0 && h % 4 = 0!");
            }
        }

        else {
            ERRME("Couldn't read texture file!");
            ERRME("File \"" + std::string(fileName) + "\" does not exist!");
        }
    }

    bool TextureLoader::checkColorData() {
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

    void TextureLoader::getTextureDetails(uint32_t *texWidth, uint32_t *texHeight, VkDeviceSize *texSize, std::vector<uint8_t> &texPixelData) {
        *texWidth = this->m_info_header.width;
        *texHeight = this->m_info_header.height;
        *texSize = this->m_info_header.width * this->m_info_header.height * 4;
        texPixelData = this->pixelData;
    }
}