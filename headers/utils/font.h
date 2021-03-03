#ifndef FONT_H
#define FONT_H

#ifdef __FONT_CPP
    #include <vector>
    #include <array>
    #include <stdlib.h>    
    #include <dirent.h>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/common.h>
    #include <common/err_def.h>
    #include <das/assets.h>
    
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <utils/shapes.h>
#endif

#include <ft2build.h>
#include <freetype/ftbitmap.h>

#ifdef __linux__
    #define DEFAULT_FONT_PATH "/usr/share/fonts"
#endif

#ifdef _WIN32
    #define DEFAULT_FONT_PATH "C:/Windows/Fonts"
#endif

namespace dengUtils {
    struct BitmapChar {
        char ascii_ch;
        deng_ui16_t glyph_id; // ID of glyph texture
    };

    struct BitmapGlyphData {
        dengMath::vec2<deng_i32_t> bearings;
        dengMath::vec2<deng_i32_t> advance;
        FT_Bitmap bitmap;
    };
    
    struct BitmapStr {
        const char *text;
        const char *font_file;

        std::vector<BitmapGlyphData> unique_glyphs;
        std::vector<deng_ui8_t> tex_data;
        std::array<VERT_MAPPED_2D, 4> vert_pos;
        std::array<deng_ui32_t, 6> vert_indices;
        dengMath::vec2<deng_i32_t> box_size;

        BitmapChar *rend_text = NULL;
        FT_Face font_face;

        void operator=(const char* input) { text = input; }
    };


    // Class for handling font rendering
    class StringRasterizer {
    private:
        FT_Library m_library_instance;
        deng::WindowWrap *m_p_win;
        std::vector<std::string> m_fonts;

    protected:
        deng_bool_t __verifyFont (
            BitmapStr &str, 
            std::string &out_path
        );

        deng_px_t __findTextSizePx(BitmapStr &str);
        deng_vec_t __findTextSizeVec(BitmapStr &str);
        std::vector<char> __indexGlyphs(BitmapStr &str);
        void __findFontFiles(std::string custom_path);
        
        // Generic text box creationg function
        void __mkGlyphs (
            BitmapStr &str, 
            deng_ui16_t px_size,
            dengMath::vec2<deng_vec_t> pos, 
            dengMath::vec3<unsigned char> color
        );

        void __mkTextbox (
            BitmapStr &str, 
            deng_px_t text_width,
            deng_ui32_t hier_level,
            dengMath::vec3<unsigned char> color,
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> origin
        );

    public:
        // If custom font path is not needed then just pass nullptr
        StringRasterizer (
            std::string custom_font_path, 
            deng::WindowWrap *p_window_wrap
        );
        ~StringRasterizer();
        
        /* Create new text box instance from pixel size */
        void newPxStr ( 
            BitmapStr &str,
            const char *font_name,
            deng_ui16_t px_size,
            dengMath::vec3<unsigned char> color,
            dengMath::vec2<deng_vec_t> pos,
            deng_ui32_t hier_level,
            dengMath::vec2<deng_vec_t> origin
        );
        
        /* Create new text box instance from vector size */
        void newVecStr (
            BitmapStr &str,
            const char *font_name,
            deng_vec_t vec_size,
            dengMath::vec3<unsigned char> color,
            dengMath::vec2<deng_vec_t> pos,
            deng_ui32_t hier_level,
            dengMath::vec2<deng_vec_t> origin
        );

        /* Check for the width of rasterised textbox and reduce string length if needed */
        char *strRasterWidthTrim (
            const char *str,
            const char *font_name,
            deng_vec_t vec_height,
            deng_vec_t max_vec_width,
            dengMath::vec2<deng_ui32_t> deng_window_size,
            deng_vec_t *p_out_width
        );

        /* Get the maximum height of text from specified maximum width */
        deng_vec_t getMaxHeight (
            const char *str,
            const char *font_name,
            deng_vec_t max_width
        );


        /* Rasterise absolutely positioned label for ui elements */
        BitmapStr renderAbsLabel (
            deng_px_t px_padding,
            dengMath::vec2<deng_vec_t> parent_elem_top_left,
            dengMath::vec2<deng_vec_t> top_left_margin,
            dengMath::vec2<deng_vec_t> size,
            dengMath::vec2<deng_vec_t> origin,
            dengMath::vec3<unsigned char> color,
            deng_ui32_t hier_level,
            const char *font_file,
            const char *label
        );


        /* Rasterise relatively positioned label for ui elements */
        BitmapStr renderRelLabel (
            dengMath::vec2<deng_vec_t> parent_cont_size,
            deng_px_t px_padding,
            dengMath::vec2<deng_vec_t> parent_elem_top_left,
            dengMath::vec2<deng_vec_t> top_left_pos,
            dengMath::vec2<deng_vec_t> size,
            dengMath::vec2<deng_vec_t> origin,
            dengMath::vec3<unsigned char> color,
            deng_ui32_t hier_level,
            const char *font_file,
            const char *label
        );


        /* Clean rasterised string */
        void cleanBitmapString(BitmapStr &str);
    };
}

#endif
