#include "../core/api_core.h"

namespace dengUtils {

    /* Search for certain font and return true if found */
    bool FontManager::verifyFont(bitmapStr &str, std::string &out_path) {
        size_t l_index, r_index, path_index;
        std::string list_font_name = "";

        printf("FONT FILE: %s\n", str.font_file);
        for(l_index = 0; l_index < m_fonts.size(); l_index++) {
            printf("seg test\n");
            // Extract font name from the font file path
            // Find last instance of slash
            for(r_index = 0; r_index < m_fonts[l_index].size(); r_index++)
                if(m_fonts[l_index][r_index] == 0x2F) path_index = r_index; 

            // Populate list_font_name string with found font file
            path_index++;
            for(r_index = path_index; r_index < m_fonts[l_index].size(); r_index++)
                list_font_name += m_fonts[l_index][r_index];

            if(str.font_file == list_font_name) {
                out_path = m_fonts[l_index];
                LOG("Found font file: " + out_path);
                return true;
            }

            list_font_name = "";
        }

        LOG("No font file found");
        return false;
    }


    /* Find all available fonts */
    // Untested
    void FontManager::findFontFiles(std::string path) {
        // Check if '/' needs to be added to the end of the path
        if(path != "" && path[path.size() - 1] != '/') 
            path += '/';
        else if(path == "") return;

        // Open directory
        DIR *dir;
        dir = opendir(path.c_str());
        if(!dir) WARNME("Invalid font path: " + path);
        char *file_ext;
        
        // Read directory contents 
        struct dirent *contents;
        std::string tmp;

        while((contents = readdir(dir))) {
            switch (contents->d_type)
            {
            case DT_REG:
                file_ext = cm_GetFileExtName(contents->d_name);
                if(file_ext && !strcmp(file_ext, "ttf"))
                    m_fonts.push_back(path + contents->d_name);
                
                if(file_ext) free(file_ext);
                break;

            case DT_DIR:
                if(std::string(contents->d_name) != "." && std::string(contents->d_name) != "..")
                    findFontFiles(path + contents->d_name);
                break;

            default:
                break;
            }
        }
        closedir(dir);
    }


    // Find unique glyphs and index them according to the text
    std::vector<char> FontManager::indexGlyphs(bitmapStr &str) {
        std::vector<char> unique_chars;
        str.rend_text = (bitmapChar*) malloc(sizeof(bitmapChar) * strlen(str.text));
        
        size_t l_index, r_index;
        bool is_found = false;

        // Find if chars repeat in text, sort and index them
        for(l_index = 0; l_index < strlen(str.text); l_index++) {
            // Find if char exists in unique chars
            for(r_index = 0; r_index < unique_chars.size(); r_index++) {
                if(str.text[l_index] == unique_chars[r_index]) {
                    is_found = true;
                    break;
                }
            }

            if(!is_found) {
                unique_chars.push_back(str.text[l_index]);
                str.rend_text[l_index].glyph_id = (uint16_t) unique_chars.size() - 1;
            }
            else {
                is_found = false;
                str.rend_text[l_index].glyph_id = (uint16_t) r_index;
            }
            
            str.rend_text[l_index].ascii_ch = str.text[l_index];
        }

        return unique_chars;
    }


    FontManager::FontManager(std::string custom_font_path, deng::WindowWrap *p_window_wrap) {
        FT_Error res;
        m_p_window_wrap = p_window_wrap;
        findFontFiles(DEFAULT_FONT_PATH);
        findFontFiles(custom_font_path);

        res = FT_Init_FreeType(&m_library_instance);
        if(res) ERR("Failed to initialise freetype library instance!");
    }

    FontManager::~FontManager() { FT_Done_FreeType(m_library_instance); }


    /* Create new drawable string */
    dengError FontManager::mkNewStr (
        bitmapStr &str, 
        uint16_t px_size, 
        dengMath::vec2<float> pos, 
        dengMath::vec3<unsigned char> color
    ) {
        size_t index;

        // Set up new freetype font face        
        FT_Error res;
        res = FT_New_Face(m_library_instance, str.font_file, 0, &str.font_face);
        if(res) {
            WARNME("Failed to create new font face!");
            return DENG_ERROR_TYPE_GENERAL_THIRD_PARTY_LIB_CALLBACK_ERROR;
        }

        // Set sizes for glyphs
        res = FT_Set_Pixel_Sizes(str.font_face, 0, px_size);
        if(res) return DENG_ERROR_TYPE_GENERAL_THIRD_PARTY_LIB_CALLBACK_ERROR;

        // Index chars and get unique vector characters
        std::vector<char> unique_ch = indexGlyphs(str);
        str.unique_glyphs.resize(unique_ch.size());
        
        for(index = 0; index < unique_ch.size(); index++) {
            res = FT_Load_Char(str.font_face, unique_ch[index], FT_LOAD_RENDER);    
            if(res) return DENG_ERROR_TYPE_GENERAL_THIRD_PARTY_LIB_CALLBACK_ERROR;

            str.unique_glyphs[index].advance.first = (int32_t) str.font_face->glyph->advance.x / 64;
            str.unique_glyphs[index].advance.second = (int32_t) str.font_face->glyph->advance.y / 64;

            str.unique_glyphs[index].bearings.first = (int32_t) str.font_face->glyph->bitmap_left;
            str.unique_glyphs[index].bearings.second = (int32_t) str.font_face->glyph->bitmap_top;

            res = FT_Bitmap_Copy(m_library_instance, &str.font_face->glyph->bitmap, &str.unique_glyphs[index].bitmap);
            if(res) return DENG_ERROR_TYPE_GENERAL_THIRD_PARTY_LIB_CALLBACK_ERROR;
        }
        
        mkTextbox(str, color);

        str.vert_pos[0] = {pos.first, pos.second};
        str.vert_pos[0].tex_data = {0.0f, 0.0f};
        
        str.vert_pos[1].vert_data.vert_x = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) str.box_size.first, 
            m_p_window_wrap->getSize(), 
            DENG_COORD_AXIS_X
        ) + pos.first;
        str.vert_pos[1].vert_data.vert_y = pos.second;
        str.vert_pos[1].tex_data = {1.0f, 0.0f};
        
        str.vert_pos[2].vert_data.vert_x = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) str.box_size.first, 
            m_p_window_wrap->getSize(), 
            DENG_COORD_AXIS_X
        ) + pos.first;
        str.vert_pos[2].vert_data.vert_y = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) str.box_size.second, 
            m_p_window_wrap->getSize(), 
            DENG_COORD_AXIS_Y
        ) + pos.second;
        str.vert_pos[2].tex_data = {1.0f, 1.0f};
        
        str.vert_pos[3].vert_data.vert_x = pos.first;
        str.vert_pos[3].vert_data.vert_y = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) str.box_size.second + (double) pos.second, 
            m_p_window_wrap->getSize(), 
            DENG_COORD_AXIS_Y
        ) + pos.second;
        str.vert_pos[3].tex_data = {0.0f, 1.0f};

        str.vert_indices = {0, 1, 2, 2, 3, 0};

        return DENG_NO_ERRORS;
    }


    /* Callback function for creating new text box instance based on pixel size */
    dengError FontManager::newPxStr (
        bitmapStr &str,
        const char *font_name,
        uint16_t px_size,
        dengMath::vec2<float> pos,
        dengMath::vec3<unsigned char> color
    ) {
        std::string path_str;
        str.font_file = font_name;
        // Check if font file exists
        if(!verifyFont(str, path_str))
            ERR("Failed to find font file!");

        str.font_file = path_str.c_str();

        dengError res;
        res = mkNewStr (
            str,
            px_size,
            pos,
            color
        );

        return res;
    }


    /* Callback function for creating new text box instance based on vector size */
    dengError FontManager::newVecStr (
        bitmapStr &str,
        const char *font_name,
        float vec_size,
        dengMath::vec2<float> pos,
        dengMath::vec3<unsigned char> color
    ) {
        std::string path_str;
        str.font_file = font_name;
        // Check if font file exists
        if(!verifyFont(str, path_str))
            ERR("Failed to find font file!");
        
        str.font_file = path_str.c_str();
        float px_size = (float) dengMath::Conversion::vector2DSizeToPixelSize (
            (double) vec_size,
            m_p_window_wrap->getSize(),
            DENG_COORD_AXIS_Y
        );

        dengError res;
        res = mkNewStr (
            str,
            (uint16_t) px_size,
            pos,
            color
        );

        return res;
    }


    /* Create textbox from glyphs */
    void FontManager::mkTextbox(bitmapStr &str, dengMath::vec3<unsigned char> color) {
        int32_t ln_bearing = 0; 
        int32_t l_bearing = 0;
        dengMath::vec2<int32_t> origin_offset;
        int32_t l_index, r_index, t_index, g_index;

        // Find the height of textbox and verify colormode
        for(l_index = 0, str.box_size.second = 0; l_index < (int32_t) str.unique_glyphs.size(); l_index++) {
            if(l_bearing < str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second)
                l_bearing = str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second;

            // Check for the maximum negative bearing
            if(ln_bearing < ((int32_t) str.unique_glyphs[str.rend_text[l_index].glyph_id].bitmap.rows) - str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second)
                ln_bearing = ((int32_t) (str.unique_glyphs[str.rend_text[l_index].glyph_id].bitmap.rows) - str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second);
        }   
        
        str.box_size.second = ln_bearing + l_bearing;
        
        // Find the width of textbox
        for(l_index = 0, str.box_size.first = 0; l_index < (int32_t) strlen(str.text); l_index++)
            str.box_size.first += (int32_t) str.unique_glyphs[str.rend_text[l_index].glyph_id].advance.first;

        str.tex_data.resize(str.box_size.first * str.box_size.second * 4);
        origin_offset.first = 0;
        origin_offset.second = ln_bearing;
        
        dengMath::vec2<int32_t> gl_rel_draw_coords;
        int32_t gl_index;
        bool draw_width, draw_height;
        
        // y axis iteration
        for(l_index = 0, t_index = 0, g_index = 0; l_index < str.box_size.second; l_index++) {
            g_index = 0;
            origin_offset.first = 0;
            // x axis iteration
            for(r_index = 0; r_index < str.box_size.first; r_index++, t_index += 4) {
                // Get the relative coordinates of the nearest glyphs
                gl_rel_draw_coords.first = r_index - origin_offset.first - str.unique_glyphs[str.rend_text[g_index].glyph_id].bearings.first; 
                gl_rel_draw_coords.second = l_index - (str.box_size.second - (str.unique_glyphs[str.rend_text[g_index].glyph_id].bearings.second + 
                origin_offset.second));

                draw_width = gl_rel_draw_coords.first >= 0 && gl_rel_draw_coords.first < (int32_t) str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.width;
                draw_height = gl_rel_draw_coords.second >= 0 && gl_rel_draw_coords.second < (int32_t) str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.rows;
                if(draw_width && draw_height) {
                    gl_index = gl_rel_draw_coords.first + 
                    (gl_rel_draw_coords.second * str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.width);

                    // Actual glyph pixel (user defined)
                    if(str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.buffer[gl_index]) {
                        // Pixel data is inBGR 
                        str.tex_data[t_index] = color.third;
                        str.tex_data[t_index + 1] = color.second;
                        str.tex_data[t_index + 2] = color.first;
                        str.tex_data[t_index + 3] = 0xff; 
                    }
                    // Foreground pixel (white/transparent)
                    else {
                        str.tex_data[t_index] = 0xff;
                        str.tex_data[t_index + 1] = 0xff;
                        str.tex_data[t_index + 2] = 0xff;
                        str.tex_data[t_index + 3] = 0x00;
                    }
                }

                // Foreground pixel (white/transparent)
                else {
                    str.tex_data[t_index] = 0xff;
                    str.tex_data[t_index + 1] = 0xff;
                    str.tex_data[t_index + 2] = 0xff;
                    str.tex_data[t_index + 3] = 0x00;
                }


                // Check if glyph index should be incremented
                if(gl_rel_draw_coords.first >= (int32_t) str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.width
                && g_index != (int32_t) strlen(str.text) - 1){
                    origin_offset.first += str.unique_glyphs[str.rend_text[g_index].glyph_id].advance.first;
                    g_index++;
                }
            }
        }
    }
}