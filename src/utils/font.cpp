#define __FONT_CPP
#include <utils/font.h>


namespace dengUtils {

    const char *font_formats[] = {"otf", "ttf"};
    /* Search for certain font and return true if found */
    deng_bool_t StringRasterizer::__verifyFont (
        BitmapStr &str, 
        std::string &out_path
    ) {
        size_t l_index; 
        deng_i32_t r_index, ext_index, path_index;
        std::string list_font_name = "";
        deng_bool_t is_supported = false;

        // Check if specified font name matches any fonts from fonts vector
        for(l_index = 0; l_index < m_fonts.size(); l_index++) {
            // Find file extension
            for(ext_index = (deng_i32_t) m_fonts[l_index].size() - 1; ext_index >= 0; ext_index--) {
                if(m_fonts[l_index][ext_index] == '.') 
                    break;
            }
            
            if((size_t) ext_index >= strlen(str.font_file)) {
                // Extract font name from font file path
                for(path_index = ext_index - 1; path_index >= 0; path_index--) {
                    if(m_fonts[l_index][path_index] == '/')
                        break;
                }
                    
                path_index++;
                // Check if extracted string is equal to specified string
                if 
                (
                    ext_index - path_index >= (deng_i32_t) strlen(str.font_file) ?
                    !strncmp(m_fonts[l_index].c_str() + path_index, str.font_file, strlen(str.font_file)) :
                    !strncmp(m_fonts[l_index].c_str() + path_index, str.font_file, ext_index - path_index) &&
                    ext_index + 1 < (deng_i32_t) m_fonts[l_index].size()
                ) {
                    // Check for supported font file formats
                    for(r_index = 0; r_index < (deng_i32_t) C_ARR_SIZE(font_formats); r_index++) {
                        if(!strcmp(m_fonts[l_index].c_str() + ext_index + 1, font_formats[r_index])) {
                            is_supported = true;
                            break;     
                        }
                    }
                    if(is_supported) {
                        out_path = m_fonts[l_index];
                        return true;  
                    }
                }
            }
        }
        LOG("No font file found");
        return false;
    }


    /* Find all available fonts */
    void StringRasterizer::__findFontFiles(std::string root_path) {
        char* file_ext;

        // Check if '/' needs to be added to the end of the path
        if (root_path != "" && root_path[root_path.size() - 1] != '/')
            root_path += '/';
        else if (root_path == "") return;

        #ifdef _WIN32
            char path[2048] = { 0 };
            sprintf(path, "%s*.*", root_path.c_str());
            WIN32_FIND_DATA file;
            HANDLE hdl = NULL;

            if ((hdl = FindFirstFile(path, &file)) == INVALID_HANDLE_VALUE)
                RUN_ERR("Invalid font path: " + root_path);
            
            // Skip ../ directory
            FindNextFile(hdl, &file);
            while (FindNextFile(hdl, &file)) {
                file_ext = cm_GetFileExtName(file.cFileName);
                sprintf(path, "%s%s", root_path.c_str(), file.cFileName);
                if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    __findFontFiles(path);
                else if (
                    (file_ext && !strcmp(file_ext, "ttf")) ||
                    (file_ext && !strcmp(file_ext, "otf"))
                ) m_fonts.push_back(path);
            }

            FindClose(hdl);
        #endif  
        #ifdef __linux__
            // Open directory   
            DIR *dir;
            dir = opendir(path.c_str());
            if(!dir) RUN_ERR("Invalid font path: " + path);
        
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
                        __findFontFiles(path + contents->d_name);
                    break;

                default:
                    break;
                }
            }
            closedir(dir);
        #endif
    }


    // Find unique glyphs and index them according to the text
    std::vector<char> StringRasterizer::__indexGlyphs(BitmapStr &str) {
        std::vector<char> unique_chars;
        str.rend_text = (BitmapChar*) calloc (
            strlen(str.text) + 1,
            sizeof(BitmapChar)
        );
        
        size_t l_index, r_index;
        deng_bool_t is_found = false;

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
        __findFontFiles(DEFAULT_FONT_PATH);
        __findFontFiles(custom_font_path);

        res = FT_Init_FreeType(&m_library_instance);
        if(res) FONT_ERR("Failed to initialise freetype library instance!");
    }

    StringRasterizer::~StringRasterizer() { FT_Done_FreeType(m_library_instance); }


    /* Find the total width of the textbox in vertices unit */
    deng_vec_t StringRasterizer::__findTextSizeVec(BitmapStr &str) {
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
    deng_px_t StringRasterizer::__findTextSizePx(BitmapStr &str) {
        deng_ui64_t l_index;
        deng_px_t total_size = 0;
        for(l_index = 0; l_index < strlen(str.text); l_index++) 
            total_size += (deng_px_t) str.unique_glyphs[str.rend_text[l_index].glyph_id].advance.first;

        return total_size;
    }


    /* Create new drawable string */
    void StringRasterizer::__mkGlyphs (
        BitmapStr &str, 
        deng_px_t px_size, 
        dengMath::vec2<deng_vec_t> pos, 
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

        if(res) return;

        // Index chars and get unique vector characters
        std::vector<char> unique_ch = __indexGlyphs(str);
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
        deng_px_t px_size,
        dengMath::vec3<unsigned char> color,
        dengMath::vec2<deng_vec_t> pos,
        deng_ui32_t hier_level,
        dengMath::vec2<deng_vec_t> origin
    ) {
        std::string path_str;
        str.font_file = font_name;
        // Check if font file exists
        if(!__verifyFont(str, path_str))
            FONT_ERR("Failed to find font file!");

        str.font_file = path_str.c_str();

        __mkGlyphs (
            str,
            px_size,
            pos,
            color
        );

        deng_px_t width = __findTextSizePx(str);
           
        __mkTextbox (
            str,
            width,
            hier_level,
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
        deng_ui32_t hier_level,
        dengMath::vec2<deng_vec_t> origin
    ) {
        std::string path_str;
        str.font_file = font_name;
        // Check if font file exists
        if(!__verifyFont(str, path_str))
            FONT_ERR("Failed to find font file!");
        
        str.font_file = path_str.c_str();
        deng_ui16_t px_size = (deng_vec_t) dengMath::Conversion::vector2DSizeToPixelSize (
            vec_size,
            m_p_win->getSize(),
            DENG_COORD_AXIS_Y
        );

        __mkGlyphs (
            str,
            px_size,
            pos,
            color
        );

        deng_px_t width = __findTextSizePx(str);

        __mkTextbox (
            str,
            width,
            hier_level,
            color,
            pos,
            origin
        );
    }


    /* Create textbox from glyphs */
    void StringRasterizer::__mkTextbox (
        BitmapStr &str,
        deng_px_t px_width,
        deng_ui32_t hier_level,
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

        origin_offset.first = 0;
        origin_offset.second = ln_bearing;
        
        dengMath::vec2<deng_i32_t> gl_rel_draw_coords;
        deng_i32_t gl_index;
        deng_bool_t draw_width, draw_height;
        
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
                    // Foreground pixel (user defined/transparent)
                    else {
                        str.tex_data[t_index] = color.third;
                        str.tex_data[t_index + 1] = color.second;
                        str.tex_data[t_index + 2] = color.first;
                        str.tex_data[t_index + 3] = 0x00;
                    }
                }

                // Foreground pixel (white/transparent)
                else {
                    str.tex_data[t_index] = color.third;
                    str.tex_data[t_index + 1] = color.second;
                    str.tex_data[t_index + 2] = color.first;
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
            hier_level,
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
        printf("Text: %s\n", ras_str.text);
        ras_str.font_file = font_name;

        std::string font_path;
        if(!__verifyFont(ras_str, font_path)) 
            return NULL;

        ras_str.font_file = font_path.c_str();
        deng_px_t px_height = dengMath::Conversion::vector2DSizeToPixelSize (
            vec_height,
            deng_window_size,
            DENG_COORD_AXIS_Y
        );

        // Find all unique glyphs
        __mkGlyphs (
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


    BitmapStr StringRasterizer::renderAbsLabel (
        deng_px_t px_padding, 
        dengMath::vec2<deng_vec_t> parent_elem_top_left, 
        dengMath::vec2<deng_vec_t> top_left_margin,
        dengMath::vec2<deng_vec_t> size, 
        dengMath::vec2<deng_vec_t> origin, 
        dengMath::vec3<unsigned char> color, 
        deng_ui32_t hier_level,
        const char *font_file, 
        const char *label
    ) {
        BitmapStr out_label;

        // Find the padding size in vector size
        dengMath::vec2<deng_vec_t> vec_padding;
        vec_padding.first = dengMath::Conversion::pixelSizeToVector2DSize (
            px_padding, 
            m_p_win->getSize(),
            DENG_COORD_AXIS_X 
        );

        vec_padding.second = dengMath::Conversion::pixelSizeToVector2DSize (
            px_padding, 
            m_p_win->getSize(), 
            DENG_COORD_AXIS_Y
        );

        LOG (
            "LABEL VEC_PADDING: " + 
            std::to_string(vec_padding.first) + 
            "; " +
            std::to_string(vec_padding.second)
        );

        deng_vec_t text_height = getMaxHeight (
            label,
            font_file,
            size.first - 2 * vec_padding.first
        );

        deng_vec_t label_height;
        deng_vec_t max_label_height = size.second - 2 * vec_padding.second;
        
        // Check if text heigth exceeds maximum label height value
        if(text_height >= max_label_height)
            label_height = max_label_height;
        else 
            label_height = text_height;

        out_label.text = label;
        printf("Label height: %f\n", label_height);
        newVecStr (
            out_label, 
            font_file, 
            label_height, 
            color,
            {
                parent_elem_top_left.first + (top_left_margin.first / 2 * size.first),
                parent_elem_top_left.second + (top_left_margin.second / 2 * size.second)
            },
            hier_level,
            origin
        );

        return out_label;
    }


    /* Render label for ui box shaped elements */
    BitmapStr StringRasterizer::renderRelLabel (
        dengMath::vec2<deng_vec_t> parent_cont_size,
        deng_px_t px_padding,
        dengMath::vec2<deng_vec_t> parent_elem_top_left,
        dengMath::vec2<deng_vec_t> top_left_margin,
        dengMath::vec2<deng_vec_t> size,
        dengMath::vec2<deng_vec_t> origin,
        dengMath::vec3<unsigned char> color,
        deng_ui32_t hier_level,
        const char *font_file,
        const char *label
    ) {
        // Find the absolute size of the child object
        dengMath::vec2<deng_vec_t> abs_size;
        abs_size.first = dengMath::Conversion::findAbsSize (
            parent_cont_size.first,
            size.first
        );

        abs_size.second = dengMath::Conversion::findAbsSize (
            parent_cont_size.second,
            size.second
        );
        
        BitmapStr out_str = renderAbsLabel (
            px_padding,
            parent_elem_top_left,
            top_left_margin,
            abs_size,
            origin,
            color,
            hier_level,
            font_file,
            label
        );

        return out_str;
    }


    /* Find maximum height allowed by specified width */
    deng_vec_t StringRasterizer::getMaxHeight (
        const char *str,
        const char *font_name,
        deng_vec_t max_width
    ) {
        // Rasterize string with height 1.0
        BitmapStr bm_str;
        bm_str.text = str;

        newVecStr (
            bm_str,
            font_name,
            1.0f,
            {0x00, 0x00},
            {0.0f, 0.0f},
            0,
            {0.0f, 0.0f}
        );

        deng_vec_t out_val = (deng_vec_t) bm_str.box_size.second / 
                             (deng_vec_t) bm_str.box_size.first * 
                             max_width;
        cleanBitmapString(bm_str);

        return out_val;
    }


    /* Clean rasterised font data */
    void StringRasterizer::cleanBitmapString(BitmapStr &str) {
        if(str.rend_text)
            free(str.rend_text);
        
        size_t i;
        for(i = 0; i < str.unique_glyphs.size(); i++)
            FT_Bitmap_Done (
                m_library_instance, 
                &str.unique_glyphs[i].bitmap
            );

        FT_Done_Face(str.font_face);
    }
}
