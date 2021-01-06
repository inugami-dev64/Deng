#ifndef REND_UTILS_H
#define REND_UTILS_H
#include <ft2build.h>
#include <freetype/ftbitmap.h>
#include FT_FREETYPE_H

#ifdef __linux__
#define DEFAULT_FONT_PATH "/usr/share/fonts"
#endif

#ifdef _WIN32
#define DEFAULT_FONT_PATH "C:/Windows/Fonts"
#endif

namespace dengUtils {
    struct bitmapChar {
        char ascii_ch;
        uint16_t glyph_id; // ID of glyph texture
    };

    struct bitmapGlyphData {
        dengMath::vec2<int32_t> bearings;
        dengMath::vec2<int32_t> advance;
        FT_Bitmap bitmap;
    };
    
    struct bitmapStr {
        const char *text;
        const char *font_file;
        
        std::vector<bitmapGlyphData> unique_glyphs;
        std::vector<uint8_t> tex_data;
        std::array<VERT_MAPPED_2D, 4> vert_pos;
        std::array<uint32_t, 6> vert_indices;
        dengMath::vec2<int32_t> box_size;

        bitmapChar *rend_text;
        FT_Face font_face;

        void operator=(const char* input) { text = input; }
    };


    // Class for handling font rendering
    class FontManager {
    private:
        FT_Library m_library_instance;
        deng::WindowWrap *m_p_window_wrap;
        std::vector<std::string> m_fonts;

    private:
        bool verifyFont(bitmapStr &str, std::string &out_path);
        std::vector<char> indexGlyphs(bitmapStr &str);
        void findFontFiles(std::string custom_path);
        void mkTextbox(bitmapStr &str, dengMath::vec3<unsigned char> color);
        // Generic text box creationg function
        dengError mkNewStr (
            bitmapStr &str, 
            uint16_t px_size,
            dengMath::vec2<float> pos, 
            dengMath::vec3<unsigned char> color
        );

    public:
        // If custom font path is not needed then just pass nullptr
        FontManager(std::string custom_font_path, deng::WindowWrap *p_window_wrap);
        ~FontManager();
        
        /* Create new text box instance from pixel size */
        dengError newPxStr ( 
            bitmapStr &str,
            const char *font_name,
            uint16_t px_size,
            dengMath::vec2<float> pos,
            dengMath::vec3<unsigned char> color
        );
        
        /* Create new text box instance from vector size */
        dengError newVecStr (
            bitmapStr &str,
            const char *font_name,
            float vec_size,
            dengMath::vec2<float> pos,
            dengMath::vec3<unsigned char> color  
        );
    };
}

#endif