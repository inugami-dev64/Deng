#include "../../core/deng_core.h"

namespace dengUI {
    Font::Font(const char *font_family, const uint32_t &font_size) {
        this->m_font_family_name = font_family;
        this->m_font_path_name = this->getFontPathName(font_family);
        std::string font_conf_path = "fonts/" + std::string(this->m_font_path_name) + "/" + std::string(this->m_font_path_name) + ".fnt";
        std::string font_map_image_path = "fonts/" + std::string(this->m_font_path_name) + "/" + std::string(this->m_font_path_name) + ".tga";

        dengUtils::TextureLoaderTGA texture_loader(font_map_image_path.c_str());
        texture_loader.getTextureDetails(&this->m_raw_char_map_data);

        dengUtils::FileManager file_manager;
        std::vector<std::string> file_contents;
        file_manager.getFileContents("fonts/" + std::string(this->m_font_path_name) + "/" + std::string(this->m_font_path_name) + ".fnt", nullptr, &file_contents);
        int line_index = -1;
        double tmp_val;
        size_t ch_info_index;

        this->m_char_map_data.resize(file_manager.getConfVal<size_t>("count", font_conf_path, &line_index, DENG_FALSE));
        line_index++;
        
        LOG("Texture image width: " + std::to_string(this->m_raw_char_map_data.width));
        LOG("Texture image height: " + std::to_string(this->m_raw_char_map_data.height));

        for(ch_info_index = 0; ch_info_index < this->m_char_map_data.size(); ch_info_index++, line_index++) {
            this->m_char_map_data[ch_info_index].id = file_manager.getConfVal<uint8_t>("id", font_conf_path, &line_index, DENG_FALSE);
            
            tmp_val = file_manager.getConfVal<double>("x", font_conf_path, &line_index, DENG_FALSE);
            this->m_char_map_data[ch_info_index].ch_vector_positions.first.first = tmp_val / (double) this->m_raw_char_map_data.width;
            this->m_char_map_data[ch_info_index].ch_vector_positions.third.first = tmp_val / (double) this->m_raw_char_map_data.width;
            tmp_val += file_manager.getConfVal<double>("width", font_conf_path, &line_index, DENG_FALSE);
            this->m_char_map_data[ch_info_index].ch_vector_positions.second.first += tmp_val / (double) this->m_raw_char_map_data.width;
            this->m_char_map_data[ch_info_index].ch_vector_positions.fourth.first += tmp_val / (double) this->m_raw_char_map_data.width;

            tmp_val = file_manager.getConfVal<double>("y", font_conf_path, &line_index, DENG_FALSE);
            this->m_char_map_data[ch_info_index].ch_vector_positions.first.second = tmp_val / (double) this->m_raw_char_map_data.height;
            this->m_char_map_data[ch_info_index].ch_vector_positions.second.second = tmp_val / (double) this->m_raw_char_map_data.height;
            tmp_val += file_manager.getConfVal<double>("height", font_conf_path, &line_index, DENG_FALSE);
            this->m_char_map_data[ch_info_index].ch_vector_positions.third.second += tmp_val / (double) this->m_raw_char_map_data.height;
            this->m_char_map_data[ch_info_index].ch_vector_positions.fourth.second += tmp_val / (double) this->m_raw_char_map_data.height;
        }
    }

    void Font::getCharInfo(CharData **pp_char_data, char ch) {
        dengBool is_found = DENG_FALSE;
        size_t index;
        for(index = 0; index < this->m_char_map_data.size(); index++) {
            if((uint8_t) ch == this->m_char_map_data[index].id) {
                is_found = DENG_TRUE;
                break;
            }
        }

        if(!is_found) return;
        else *pp_char_data = &this->m_char_map_data[index];
    }

    const char *Font::getFontPathName(const char *font_family) {
        size_t index;
        char *tmp_font_path = (char*) malloc(strlen(font_family) * sizeof(char)); 

        for(index = 0; index < strlen(font_family); index++) {
            if(font_family[index] > 65 && font_family[index] < 91) tmp_font_path[index] = font_family[index] + 32;
            else if(font_family[index] == ' ') tmp_font_path[index] = '_';
            else tmp_font_path[index] = font_family[index];
        }
    
        return (const char*) tmp_font_path; 
    }

    uint32_t Font::getFontSize() { return this->m_font_size; }

    Font::~Font() {
        delete this->m_font_path_name;
    }
}