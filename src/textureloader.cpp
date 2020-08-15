#include "headers/renderer.h"

namespace Deng {
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

            this->pixelData.resize(this->m_info_header.width * this->m_info_header.height);

            if(this->m_info_header.width % 4 == 0) {
                for(size_t i = 0; i < this->pixelData.size(); i++) {
                    vec4<unsigned char> rgb_per_pixel;
                    textureFile.read((char*) &rgb_per_pixel, 4);
                    pixelData[i] = rgb_per_pixel;

                }
                this->m_file_header.file_size += this->pixelData.size() * 4;
            }

            else {
                int32_t padding_count = this->m_info_header.width % 4 + 1;
                for(size_t hI = 1; hI <= this->m_info_header.height; hI++) {
                    for(size_t wI = 1; wI <= this->m_info_header.width - padding_count; wI++) {
                        vec4<unsigned char> rgb_per_pixel;
                        textureFile.read((char*) &rgb_per_pixel, 4);
                        pixelData[(hI - 1) * wI + wI] = rgb_per_pixel;
                    }
                    textureFile.seekg(textureFile.tellg() + padding_count);
                }
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

    void TextureLoader::getTextureDetails(uint32_t *texWidth, uint32_t *texHeight, VkDeviceSize *texSize, std::vector<vec4<unsigned char>> *texPixelData) {
        FileManager fm;

        fm.writeToFile("rgbbitmap.log", "#entry point", DENG_WRITEMODE_REWRITE);
        fm.writeToFile("rgbbitmap.log", ("Size: " + std::to_string(this->m_file_header.file_size / 1024 / 1024)), DENG_WRITEMODE_FROM_END);
        fm.writeToFile("rgbbitmap.log", ("Width: " + std::to_string(this->m_info_header.width)), DENG_WRITEMODE_FROM_END);
        fm.writeToFile("rgbbitmap.log", ("Height: " + std::to_string(this->m_info_header.height)), DENG_WRITEMODE_FROM_END);
        for(vec4<unsigned char> &rgbVal : this->pixelData) {
            fm.writeToFile("rgbbitmap.log", "{" + std::to_string(rgbVal.x) + ", " + std::to_string(rgbVal.y) + ", " + std::to_string(rgbVal.z) + ", " + std::to_string(rgbVal.a) + "}", DENG_WRITEMODE_FROM_END);
        }

        *texWidth = this->m_info_header.width;
        *texHeight = this->m_info_header.height;
        *texSize = this->m_info_header.width * m_info_header.height * 4;
        *texPixelData = this->pixelData;
    }
}