#include "../core/api_core.h"
#include <string>

namespace dengUtils {

    /* Search for certain font and return true if found */
    bool StringRasterizer::verifyFont(BitmapStr &str, std::string &out_path) {
        size_t l_index, r_index, path_index;
        std::string list_font_name = "";

        for(l_index = 0; l_index < m_fonts.size(); l_index++) {
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
    void StringRasterizer::findFontFiles(std::string path) {
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
                if
                (
                    (file_ext && !strcmp(file_ext, "ttf")) || 
                    (file_ext && !strcmp(file_ext, "otf"))
                ) m_fonts.push_back(path + contents->d_name);
                
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
    std::vector<char> StringRasterizer::indexGlyphs(BitmapStr &str) {
        std::vector<char> unique_chars;
        str.rend_text = (BitmapChar*) malloc(sizeof(BitmapChar) * strlen(str.text));
        
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
                str.rend_text[l_index].glyph_id = (deng_ui16_t) unique_chars.size() - 1;
            }
            else {
                is_found = false;
                str.rend_text[l_index].glyph_id = (deng_ui16_t) r_index;
            }
            
            str.rend_text[l_index].ascii_ch = str.text[l_index];
        }

        return unique_chars;
    }


    StringRasterizer::StringRasterizer (
        std::string custom_font_path, 
        deng::WindowWrap *p_window_wrap
    ) {
        FT_Error res;
        m_p_win = p_window_wrap;
        findFontFiles(DEFAULT_FONT_PATH);
        findFontFiles(custom_font_path);

        res = FT_Init_FreeType(&m_library_instance);
        if(res) FONT_ERR("Failed to initialise freetype library instance!");
    }

    StringRasterizer::~StringRasterizer() { FT_Done_FreeType(m_library_instance); }


    /* Find the total width of the textbox in vertices unit */
    deng_vec_t StringRasterizer::findTextSizeVec(BitmapStr &str) {
        deng_ui64_t l_index;
        deng_px_t total_size = 0;
        for(l_index = 0; l_index < strlen(str.text); l_index++)
            total_size += str.unique_glyphs[str.rend_text[l_index].glyph_id].advance.first;

        return dengMath::Conversion::pixelSizeToVector2DSize (
            total_size,
            m_p_win->getSize(),
            DENG_COORD_AXIS_X
        );
    }


    /* Find the total width of the textbox in pixel units */
    deng_px_t StringRasterizer::findTextSizePx(BitmapStr &str) {
        deng_ui64_t l_index;
        deng_px_t total_size = 0;
        for(l_index = 0; l_index < strlen(str.text); l_index++) 
            total_size += str.unique_glyphs[str.rend_text[l_index].glyph_id].advance.first;

        return total_size;
    }


    /* Create new drawable string */
    void StringRasterizer::mkGlyphs (
        BitmapStr &str, 
        deng_ui16_t px_size, 
        dengMath::vec2<float> pos, 
        dengMath::vec3<unsigned char> color
    ) {
        size_t index;

        // Set up new freetype font face        
        FT_Error res;
        res = FT_New_Face (
            m_library_instance, 
            str.font_file, 
            0, 
            &str.font_face
        );

        if(res) {
            WARNME("Failed to create new font face!");
            return;
        }

        // Set sizes for glyphs
        res = FT_Set_Pixel_Sizes (
            str.font_face, 
            0, 
            px_size
        );

        LOG("FNT_PX_SIZE: " + std::to_string(px_size));
        
        if(res) return;

        // Index chars and get unique vector characters
        std::vector<char> unique_ch = indexGlyphs(str);
        str.unique_glyphs.resize(unique_ch.size());
        
        for(index = 0; index < unique_ch.size(); index++) {
            res = FT_Load_Char(str.font_face, unique_ch[index], FT_LOAD_RENDER);    
            if(res) return;

            str.unique_glyphs[index].advance.first = (deng_i32_t) str.font_face->glyph->advance.x / 64;
            str.unique_glyphs[index].advance.second = (deng_i32_t) str.font_face->glyph->advance.y / 64;

            str.unique_glyphs[index].bearings.first = (deng_i32_t) str.font_face->glyph->bitmap_left;
            str.unique_glyphs[index].bearings.second = (deng_i32_t) str.font_face->glyph->bitmap_top;

            res = FT_Bitmap_Copy (
                m_library_instance, 
                &str.font_face->glyph->bitmap, 
                &str.unique_glyphs[index].bitmap
            );

            LOG (
                "GLYPH_SIZE: " +
                std::to_string(str.unique_glyphs[index].bitmap.width)
            );

            FT_Bitmap_Done (
                m_library_instance, 
                &str.font_face->glyph->bitmap
            );

            if(res) return;
        }
    }


    /* Callback function for creating new text box instance based on pixel size */
    void StringRasterizer::newPxStr (
        BitmapStr &str,
        const char *font_name,
        deng_ui16_t px_size,
        dengMath::vec3<unsigned char> color,
        dengMath::vec2<deng_vec_t> pos,
        dengMath::vec2<deng_vec_t> origin
    ) {
        std::string path_str;
        str.font_file = font_name;
        // Check if font file exists
        if(!verifyFont(str, path_str))
            FONT_ERR("Failed to find font file!");

        str.font_file = path_str.c_str();

        mkGlyphs (
            str,
            px_size,
            pos,
            color
        );

        deng_px_t width = findTextSizePx(str);

        mkTextbox (
            str,
            width,
            color,
            pos,
            origin
        );
    }


    /* Callback function for creating new text box instance based on vector size */
    void StringRasterizer::newVecStr (
        BitmapStr &str,
        const char *font_name,
        deng_vec_t vec_size,
        dengMath::vec3<unsigned char> color,
        dengMath::vec2<deng_vec_t> pos,
        dengMath::vec2<deng_vec_t> origin
    ) {
        std::string path_str;
        str.font_file = font_name;
        // Check if font file exists
        if(!verifyFont(str, path_str))
            FONT_ERR("Failed to find font file!");
        
        str.font_file = path_str.c_str();
        deng_vec_t px_size = (float) dengMath::Conversion::vector2DSizeToPixelSize (
            (deng_px_t) vec_size,
            m_p_win->getSize(),
            DENG_COORD_AXIS_Y
        );

        mkGlyphs (
            str,
            (deng_ui16_t) px_size,
            pos,
            color
        );

        deng_px_t width = findTextSizePx(str);

        mkTextbox (
            str,
            width,
            color,
            pos,
            origin
        );
    }


    /* Create textbox from glyphs */
    void StringRasterizer::mkTextbox (
        BitmapStr &str,
        deng_px_t px_width,
        dengMath::vec3<unsigned char> color,
        dengMath::vec2<deng_vec_t> pos, 
        dengMath::vec2<deng_vec_t> origin
    ) {
        deng_i32_t ln_bearing = 0; 
        deng_i32_t l_bearing = 0;
        dengMath::vec2<deng_i32_t> origin_offset;
        deng_i32_t l_index, r_index, t_index, g_index;

        // Find the height of textbox and verify colormode
        for(l_index = 0, str.box_size.second = 0; l_index < (deng_i32_t) str.unique_glyphs.size(); l_index++) {
            if(l_bearing < str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second)
                l_bearing = str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second;

            // Check for the maximum negative bearing
            if(ln_bearing < ((deng_i32_t) str.unique_glyphs[str.rend_text[l_index].glyph_id].bitmap.rows) - str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second)
                ln_bearing = ((deng_i32_t) (str.unique_glyphs[str.rend_text[l_index].glyph_id].bitmap.rows) - str.unique_glyphs[str.rend_text[l_index].glyph_id].bearings.second);
        }   
        
        str.box_size.first = (deng_i32_t) px_width;
        str.box_size.second = ln_bearing + l_bearing;

        dengMath::vec2<deng_vec_t> vec_size;
        vec_size.first = dengMath::Conversion::pixelSizeToVector2DSize (
            str.box_size.first,
            m_p_win->getSize(),
            DENG_COORD_AXIS_X
        );

        vec_size.second = dengMath::Conversion::pixelSizeToVector2DSize (
            str.box_size.second,
            m_p_win->getSize(),
            DENG_COORD_AXIS_Y
        );
        
        str.tex_data.resize (
            str.box_size.first * 
            str.box_size.second * 
            4
        );

        LOG (
            "TEXT_SIZE_PX: " +
            std::to_string(str.box_size.first) +
            ", " +
            std::to_string(str.box_size.second)
        );

        origin_offset.first = 0;
        origin_offset.second = ln_bearing;
        
        dengMath::vec2<deng_i32_t> gl_rel_draw_coords;
        deng_i32_t gl_index;
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

                draw_width = gl_rel_draw_coords.first >= 0 && gl_rel_draw_coords.first < (deng_i32_t) str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.width;
                draw_height = gl_rel_draw_coords.second >= 0 && gl_rel_draw_coords.second < (deng_i32_t) str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.rows;
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
                if
                (
                    gl_rel_draw_coords.first >= (deng_i32_t) str.unique_glyphs[str.rend_text[g_index].glyph_id].bitmap.width && 
                    g_index != (deng_i32_t) strlen(str.text) - 1
                ) {
                    origin_offset.first += str.unique_glyphs[str.rend_text[g_index].glyph_id].advance.first;
                    g_index++;
                }
            }
        }

        std::vector<VERT_MAPPED_2D> tmp_vec;
        RectangleGenerator::generateMappedAbsRec (
            pos,
            vec_size,
            origin,
            tmp_vec
        );

        str.vert_pos[0] = tmp_vec[0];
        str.vert_pos[1] = tmp_vec[1];
        str.vert_pos[2] = tmp_vec[2];
        str.vert_pos[3] = tmp_vec[3];

        str.vert_indices = {0, 1, 2, 2, 3, 0};
    }


    /* Trim the width of potential rasterised string */
    char *StringRasterizer::strRasterWidthTrim (
        const char *str,
        const char *font_name,
        deng_vec_t vec_height,
        deng_vec_t max_vec_width,
        dengMath::vec2<deng_ui32_t> deng_window_size,
        deng_vec_t *p_out_width 
    ) {
        BitmapStr ras_str;
        ras_str.text = str;
        ras_str.font_file = font_name;

        std::string font_path;
        if(!verifyFont(ras_str, font_path)) 
            return NULL;

        ras_str.font_file = font_path.c_str();
        deng_px_t px_height = dengMath::Conversion::vector2DSizeToPixelSize (
            vec_height,
            deng_window_size,
            DENG_COORD_AXIS_Y
        );

        // Find all unique glyphs
        mkGlyphs (
            ras_str,
            px_height,
            {0.0f, 0.0f},
            {0x00, 0x00, 0x00}
        );

        deng_px_t max_px_width = dengMath::Conversion::vector2DSizeToPixelSize (
            max_vec_width,
            deng_window_size,
            DENG_COORD_AXIS_X
        );

        LOG("MAX_HEIGHT: " + std::to_string(max_px_width));

        // Count the total size of the string
        deng_i32_t l_index;        
        deng_bool_t is_max = false;
        deng_px_t cur_px_width = 0.0;
        
        for(l_index = 0; l_index < (deng_i32_t) strlen(ras_str.text); l_index++) {
            // Check if the current size is bigger than maximum size
            if(cur_px_width >= max_px_width) { 
                is_max = true;
                break;
            } 
            cur_px_width += 
            ras_str.unique_glyphs[ras_str.rend_text[l_index].glyph_id].advance.first;
        } 

        *p_out_width = dengMath::Conversion::pixelSizeToVector2DSize (
            cur_px_width - ras_str.unique_glyphs[ras_str.rend_text[l_index].glyph_id].advance.first,
            deng_window_size,
            DENG_COORD_AXIS_X
        );

        char *out_str;
        if(is_max) {
            out_str = (char*) calloc (
                l_index + 1,
                sizeof(char)
            );

            out_str[l_index - 1] = 0x2E;
            out_str[l_index - 2] = 0x2E;
            out_str[l_index - 3] = 0x2E;

            strncpy (
                out_str,
                ras_str.text,
                l_index - 3
            );
        }

        else {
            out_str = (char*) calloc (
                strlen(ras_str.text) + 1,
                sizeof(char)
            );

            strcpy (
                out_str,
                ras_str.text
            );
        }

        // Cleanup glyphs
        for(l_index = 0; l_index < (deng_i32_t) ras_str.unique_glyphs.size(); l_index++) {
            FT_Bitmap_Done (
                m_library_instance, 
                &ras_str.unique_glyphs[l_index].bitmap
            );
        }

        return out_str;
    }   
}
