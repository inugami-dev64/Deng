#include "headers/renderer.h"

namespace deng {
    TextureLoaderBMP::TextureLoaderBMP(const std::string &fileName) {
        std::ifstream textureFile{fileName.c_str(), std::fstream::binary};
        if(textureFile) {
            textureFile.read((char*) &this->m_file_header, (int) sizeof(this->m_file_header));
            if(this->m_file_header.file_type != 0x4D42) {
                ERRME("Unrecognised texture file format!"); 
                return;
            }
            
            textureFile.read((char*) &this->m_info_header, sizeof(this->m_info_header));

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

            this->pixelData.resize(this->m_info_header.width * this->m_info_header.height * this->m_info_header.bit_count / 8);

            // without padding
            if(this->m_info_header.width % 4 == 0) {
                textureFile.read((char*) this->pixelData.data(), this->m_info_header.width * this->m_info_header.height * (this->m_info_header.bit_count / 8));
            }

            // with padding
            else {
                std::vector<uint8_t> padding_count_per_row(4 - (this->m_info_header.width * this->m_info_header.bit_count / 8) % 4);
                for(int32_t hI = 0; hI < this->m_info_header.height; hI++) {
                    textureFile.read((char*) (this->pixelData.data() + (this->m_info_header.width * this->m_info_header.bit_count / 8) * hI), (this->m_info_header.width * this->m_info_header.bit_count / 8));
                    textureFile.read((char*) padding_count_per_row.data(), padding_count_per_row.size());
                }

                this->m_file_header.file_size += this->pixelData.size() * this->m_info_header.height * padding_count_per_row.size();
            }
        }

        else {
            ERRME("Couldn't read texture file!");
            ERRME("File \"" + std::string(fileName) + "\" does not exist!");
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

    void TextureLoaderBMP::getTextureDetails(uint32_t *texWidth, uint32_t *texHeight, VkDeviceSize *texSize, std::vector<uint8_t> &texPixelData) {
        *texWidth = this->m_info_header.width;
        *texHeight = this->m_info_header.height;
        *texSize = this->m_info_header.width * this->m_info_header.height * 4;

        // checks if alpha channel is present and if not then create it
        if(this->m_info_header.bit_count == 32) texPixelData = this->pixelData;
        else {
            for(uint32_t i = 1; i <= this->m_info_header.width * this->m_info_header.height; i++) {
                texPixelData.push_back(this->pixelData[i * 3 - 3]);
                texPixelData.push_back(this->pixelData[i * 3 - 2]);
                texPixelData.push_back(this->pixelData[i * 3 - 1]);
                texPixelData.push_back(255);
            }
        }

        // FileManager fm;
        // fm.writeToFile("rgbbitmap.log", "#entry point", DENG_WRITEMODE_REWRITE);
        // fm.writeToFile("rgbbitmap.log", "width=" + std::to_string(this->m_info_header.width), DENG_WRITEMODE_FROM_END);
        // fm.writeToFile("rgbbitmap.log", "height=" + std::to_string(this->m_info_header.height), DENG_WRITEMODE_FROM_END);
        // for(size_t i = 0; i < this->pixelData.size(); i++) {
        //     fm.writeToFile("rgbbitmap.log", "{" + std::to_string(texPixelData[i]) + "}", DENG_WRITEMODE_FROM_END);
        // }
    }

    TextureFormats getTexFileFormat(const std::string &texFilePath) {
        if(texFilePath.find(".bmp") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_BMP;
        else if(texFilePath.find(".tga") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_TGA;
        else if(texFilePath.find(".png") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_PNG;
        else if(texFilePath.find(".jpg") == texFilePath.size() - 4) return DENG_TEXTURE_FORMAT_JPG;
        else return DENG_TEXTURE_FORMAT_UNKNOWN;
    }
}