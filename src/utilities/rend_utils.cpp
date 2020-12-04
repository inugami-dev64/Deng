#include "../core/deng_core.h"
#include "rend_utils.h"

namespace dengUtils {

    FontManager::FontManager() {
        // Initialize freetype library
        if(FT_Init_FreeType(&this->m_library))
            ERR("Failed to initialise freetype library!");
        
        // Find all available font files 
        this->findFontFilesRecursively(DEFAULT_FONT_PATH, this->m_font_file_data);
    }


    /* Search for certain font */
    bool FontManager::searchFont(std::string font_file, std::string &out_path) {
        size_t index;
        for(index = 0; index < this->m_font_file_data.size(); index++) {
            if(font_file == this->m_font_file_data[index].font_name) {
                out_path = this->m_font_file_data[index].font_path;
                return true;
            }
        }

        WARNME("Font file: " + out_path + " does not exist in font path " + DEFAULT_FONT_PATH + "!");
        return false;
    }


    /* Create a new pixel based text insance */
    void FontManager::createNewPixelBasedTextInstance(const char *text, uint16_t font_px_size, const char *font_file, TextInstance &instance) {
        // Search the font file given in the method argument 
        FT_New_Face(this->m_library, font_file, 0, &instance.font_face);

        std::string file_path;
        if(this->searchFont(font_file, file_path));
        
        FT_Set_Pixel_Sizes(instance.font_face, 0, font_px_size);
        this->loadNewGlyphs(text, instance);
    }


    /* Load new glyphs */
    void loadNewGlyphs(const char *text, TextInstance &instance) {
        size_t l_index, r_index;
        uint16_t ch_index;
        bool is_repeated_found = false;

        // Get all glyph indices and load them
        for(l_index = 0; l_index < strlen(text); l_index++) {
            ch_index = FT_Get_Char_Index(instance.font_face, text[l_index]);
            // Find if char index is repeating with already converted ones
            for(r_index = 0; r_index < instance.glyph_indices.size(); r_index++) {
                if(ch_index == instance.glyph_indices[r_index]) {
                    is_repeated_found = true;
                    break;
                }
            }

            if(!is_repeated_found) {
                instance.glyph_indices.push_back(ch_index);
                instance.glyph_slots.resize(instance.glyph_indices.size());
                
                FT_Load_Glyph(instance.font_face, instance.glyph_indices[instance.glyph_indices.size() - 1], FT_LOAD_DEFAULT);
                instance.glyph_slots[instance.glyph_slots.size() - 1].first = text[l_index];
                instance.glyph_slots[instance.glyph_slots.size() - 1].second =  instance.font_face->glyph;

                FT_Render_Glyph(instance.glyph_slots[instance.glyph_slots.size() - 1].second, FT_RENDER_MODE_NORMAL);
            }
        }
    }

    void FontManager::createFontTexture(TextInstance &instance) {
        size_t index;
        for(index = 0; index < instance.glyph_slots.size(); index++) {
            if(instance.glyph_slots[index].second->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY || 
            instance.glyph_slots[index].second->bitmap.num_grays != 256) 
                ERR("Unsupported font pixel mode for font: " + instance.font_file_data.font_name + "!");

            std::vector<uint8_t> pixel_data;
            
        }
    }


    /* Find all available fonts */
    void FontManager::findFontFilesRecursively(std::string path, std::vector<FontFileData> &fonts) {
        // Check if '/' needs to be added to the end of the path
        if(path[path.size() - 1] != '/') path += '/';

        // Open directory
        DIR *dir;
        dir = opendir(path.c_str());
        char *file_ext;
        
        // Read directory contents 
        struct dirent *contents;

        while((contents = readdir(dir))) {
            switch (contents->d_type)
            {
            case DT_REG:
                file_ext = cm_GetFileExtName(contents->d_name);
                if(file_ext && !strcmp(file_ext, "ttf")) { 
                    fonts.resize(fonts.size() + 1);
                    fonts[fonts.size() - 1].font_name = (char*) std::string(contents->d_name).c_str();
                    fonts[fonts.size() - 1].font_path = (char*) std::string(path + contents->d_name).c_str();
                }
                
                if(file_ext) free(file_ext);
                break;

            case DT_DIR:
                if(std::string(contents->d_name) != "." && std::string(contents->d_name) != "..")
                    this->findFontFilesRecursively(path + contents->d_name, fonts);
                break;

            default:
                break;
            }
        }

        closedir(dir);
    }
}