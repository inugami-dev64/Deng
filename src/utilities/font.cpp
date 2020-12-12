#include "../core/api_core.h"

namespace dengUtils {

    FontInstanceCreator::FontInstanceCreator(deng::WindowWrap &window_wrap) {
        // Initialize freetype library
        m_p_window_wrapper = &window_wrap;
        if(FT_Init_FreeType(&m_library))
            ERR("Failed to initialise freetype library!");
        
        // Find all available font files 
        findFontFilesRecursively(DEFAULT_FONT_PATH, m_font_file_data);
    }


    /* Search for certain font */
    bool FontInstanceCreator::searchFont(std::string font_file, std::string &out_path) {
        size_t index;
        for(index = 0; index < m_font_file_data.size(); index++) {
            if(font_file == m_font_file_data[index].font_name) {
                out_path = m_font_file_data[index].font_path;
                return true;
            }
        }

        return false;
    }


    /* Create a new pixel based text insance */
    void FontInstanceCreator::createPixelBasedFontInstance(const char *text, uint16_t font_px_size, const char *font_file, dengMath::vec2<float> text_coords, TextInstance &instance) {
        // Search the font file given in the method argument 
        std::string file_path;
        if(!searchFont(font_file, file_path))
            ERR("Failed to create font face for font: " + std::string(font_file));

        LOG("Found font path: " + file_path);

        FT_Error res;
        res = FT_New_Face(m_library, file_path.c_str(), 0, &instance.font_face);
        if(res) ERR("Failed to create font face for font: " + std::string(font_file));        
        res = FT_Set_Pixel_Sizes(instance.font_face, 10, font_px_size);
        if(res) ERR("Failed to set char size for font: " + std::string(font_file));

        instance.text = text;
        loadNewGlyphs(instance);
        createFontTexture(instance);
        createTextBox(instance, text_coords);
    }


    void FontInstanceCreator::createPtBasedFontInstance(const char *text, int32_t font_pt_size, uint32_t window_width, uint32_t window_height, dengMath::vec2<float> text_coords, 
    const char *font_file, TextInstance &instance) {
        
        std::string file_path;
        instance.font_file_data.font_path = font_file;

        if(!searchFont(font_file, file_path))
            ERR("Failed to find font name: " + std::string(font_file));
        
        FT_Error res;
        res = FT_New_Face(m_library, file_path.c_str(), 0, &instance.font_face);
        if(res) ERR("Failed to create font face for font: " + std::string(font_file));
        
        res = FT_Set_Char_Size(instance.font_face, 0, font_pt_size, (FT_UInt) m_p_window_wrapper->getSize().first, (FT_UInt) m_p_window_wrapper->getSize().second);
        if(res) ERR("Failed to set char size for font: " + std::string(font_file));

        instance.text = text;
        loadNewGlyphs(instance);
        createFontTexture(instance);
        createTextBox(instance, text_coords);
    }


    /* Load new glyphs */
    void FontInstanceCreator::loadNewGlyphs(TextInstance &instance) {
        size_t l_index, r_index, unique_ch_count;
        uint16_t ch_index;
        FT_Error res;
        bool is_repeated_found = false;
        instance.glyph_indices.resize(strlen(instance.text));

        // Get all glyph indices and load them
        for(l_index = 0, unique_ch_count = 0; l_index < strlen(instance.text); l_index++) {
            ch_index = FT_Get_Char_Index(instance.font_face, instance.text[l_index]);
            // Find if char index is repeating with already converted ones
            for(r_index = 0; r_index < instance.glyph_indices.size(); r_index++) {
                if(ch_index == instance.glyph_indices[r_index]) {
                    is_repeated_found = true;
                    break;
                }
            }

            if(!is_repeated_found) {
                unique_ch_count++;
                instance.glyph_indices[l_index] = unique_ch_count - 1;
                instance.glyph_indices.push_back(ch_index);
                instance.glyph_infos.resize(instance.glyph_indices.size());
                
                res = FT_Load_Glyph(instance.font_face, instance.glyph_indices[instance.glyph_indices.size() - 1], FT_LOAD_DEFAULT);
                if(res) ERR("Failed to load glyphs for font: " + std::string(instance.font_file_data.font_name));

                instance.glyph_infos[instance.glyph_infos.size() - 1].glyph = instance.text[l_index];
                instance.glyph_infos[instance.glyph_infos.size() - 1].glyph_slot =  instance.font_face->glyph;

                res = FT_Render_Glyph(instance.glyph_infos[instance.glyph_infos.size() - 1].glyph_slot, FT_RENDER_MODE_NORMAL);
                if(res) ERR("Failed to render glyphs for font: " + std::string(instance.font_file_data.font_name));
            }

            else {
                instance.glyph_indices[l_index] = r_index;
                is_repeated_found = false;
            }
        }
    }
    

    /* Create texture buffer for every character */
    void FontInstanceCreator::createFontTexture(TextInstance &instance) {
        size_t index, w_index, h_index;
        for(index = 0; index < instance.glyph_infos.size(); index++) {
            // Check if bitmap pixel mode is grayscale (only supported option)
            if(!instance.glyph_infos[index].glyph_slot)
                ERR("NULLPTR");

            if(instance.glyph_infos[index].glyph_slot->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY || 
            instance.glyph_infos[index].glyph_slot->bitmap.num_grays != 256) 
                ERR("Unsupported font pixel mode for font: " + instance.font_file_data.font_name + "!");

            LOG("Seg test!");
            // Get the buffer pointer and resize glyph pixel data to 32bit bit mode format 
            uint8_t *src_data_buffer = instance.glyph_infos[index].glyph_slot->bitmap.buffer; 
            instance.glyph_infos[index].pixel_data.resize(instance.glyph_infos[index].glyph_slot->bitmap.width * instance.glyph_infos[index].glyph_slot->bitmap.rows * 4);
            size_t pixel_data_index = 0;
            
            LOG("Bitmap pitch is: " + std::to_string(instance.glyph_infos[index].glyph_slot->bitmap.pitch));
            for(h_index = 0; h_index < instance.glyph_infos[index].glyph_slot->bitmap.rows; h_index++) {
                for(w_index = 0; w_index < instance.glyph_infos[index].glyph_slot->bitmap.width; w_index++) {
                    // Make white opaque background for now 
                    if(*src_data_buffer == 0) {
                        instance.glyph_infos[index].pixel_data[pixel_data_index++] = 0xff;
                        instance.glyph_infos[index].pixel_data[pixel_data_index++] = 0xff;
                        instance.glyph_infos[index].pixel_data[pixel_data_index++] = 0xff;
                    }

                    else {
                        instance.glyph_infos[index].pixel_data[pixel_data_index++] = 0x00;
                        instance.glyph_infos[index].pixel_data[pixel_data_index++] = 0x00;
                        instance.glyph_infos[index].pixel_data[pixel_data_index++] = 0x00;
                    }

                    instance.glyph_infos[index].pixel_data[pixel_data_index++] = (*src_data_buffer);
                }
                src_data_buffer += (uint64_t) instance.glyph_infos[index].glyph_slot->bitmap.pitch;
            }
        }
    }


    /* Calculate text box vertices and create buffers */
    void FontInstanceCreator::createTextBox(TextInstance &instance, dengMath::vec2<float> text_box_positions) {
        size_t g_index, x1_index, x2_index, y1_index;
        dengMath::vec2<double> box_dimentions_px = {0.0f, (double) instance.glyph_infos[0].glyph_slot->bitmap.rows}; // In pixels
        dengMath::vec2<float> box_dimentions_vec = {0.0f, 0.0f};

        // Find the glyph with the largest height while summarizing the width of glyphs
        for(x1_index = 0; x1_index < strlen(instance.text); x1_index++) {
            if(instance.glyph_infos[instance.glyph_indices[x1_index]].glyph_slot->bitmap.rows > (double) box_dimentions_px.second)
                ERR("Incorrect font glyph heights");

            box_dimentions_px.first += (double) instance.glyph_infos[x1_index].glyph_slot->bitmap.width;
        }

        instance.uni_text_box_texture_data.resize(box_dimentions_px.first * box_dimentions_px.second * 4);

        box_dimentions_vec.first = dengMath::Conversion::pixelSizeToVector2DSize(box_dimentions_px.first, 
        m_p_window_wrapper->getSize(), DENG_COORD_AXIS_X);

        box_dimentions_vec.second = dengMath::Conversion::pixelSizeToVector2DSize(box_dimentions_px.second, 
        m_p_window_wrapper->getSize(), DENG_COORD_AXIS_Y);

        // Assign vertices data values
        instance.text_box_vertices[0].vert_data = {text_box_positions.first, text_box_positions.second};
        instance.text_box_vertices[0].tex_data = {0.0f, 0.0f};
        instance.text_box_vertices[1].vert_data = {text_box_positions.first + box_dimentions_vec.first, 
        text_box_positions.second};
        instance.text_box_vertices[1].tex_data = {1.0f, 0.0f};
        instance.text_box_vertices[2].vert_data = {text_box_positions.first + box_dimentions_vec.first,
        text_box_positions.second + box_dimentions_vec.second};
        instance.text_box_vertices[2].tex_data = {1.0f, 1.0f};
        instance.text_box_vertices[3].vert_data = {text_box_positions.first, 
        text_box_positions.second + box_dimentions_vec.second};
        instance.text_box_vertices[3].tex_data = {0.0f, 1.0f};
        
        // Copy values from individual glyph texture arrays into one big array
        for(y1_index = 0, x1_index = 0; y1_index < (size_t) box_dimentions_px.second; y1_index++) {
            for(g_index = 0, x1_index = 0; g_index < strlen(instance.text); g_index++) {
                for(x2_index = 0; x2_index < instance.glyph_infos[instance.glyph_indices[g_index]].glyph_slot->bitmap.width; 
                x2_index++, x1_index++) {
                    instance.uni_text_box_texture_data[x1_index] = 
                    instance.glyph_infos[g_index].pixel_data[(y1_index * instance.glyph_infos[g_index].glyph_slot->bitmap.width) + x2_index];
                }

                instance.glyph_infos[g_index].pixel_data.clear();
            }
        }

        instance.texture_bounds.first = (uint16_t) box_dimentions_vec.first;
        instance.texture_bounds.second = (uint16_t) box_dimentions_vec.second;
        instance.text_box_indices = {0, 1, 2, 2, 3, 0};
    }


    /* Find all available fonts */
    void FontInstanceCreator::findFontFilesRecursively(std::string path, std::vector<FontFileData> &fonts) {
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
                    findFontFilesRecursively(path + contents->d_name, fonts);
                break;

            default:
                break;
            }
        }

        closedir(dir);
    }
}