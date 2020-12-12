#ifndef REND_UTILS_H
#define REND_UTILS_H

#ifdef __linux__
#define DEFAULT_FONT_PATH "/usr/share/fonts"
#endif

#ifdef _WIN32
#define DEFAULT_FONT_PAHT "C:/Windows/Fonts"
#endif

namespace dengUtils {
    /* Struct for keeping font file data */
    struct FontFileData {
        std::string font_name;
        std::string font_path;
    };


    struct GlyphInfo {
        char glyph;
        FT_GlyphSlot glyph_slot;
        std::vector<uint8_t> pixel_data;
    };


    /* Struct used to store instance information for rendering text */ 
    struct TextInstance {
        const char *text;
        std::array<VERT_MAPPED_2D, 4> text_box_vertices;
        std::array<uint32_t, 6>  text_box_indices;
        dengMath::vec2<uint16_t> texture_bounds;
        std::vector<uint8_t> uni_text_box_texture_data;

        VkImage text_box_image_data;
        VkDeviceMemory text_box_image_memory;
        FontFileData font_file_data;
        FT_Face font_face;
        std::vector<GlyphInfo> glyph_infos;
        std::vector<size_t> glyph_indices; 
    };


    /* Class to manage everything related to font rendering */ 
    class FontInstanceCreator {
    private:
        deng::WindowWrap *m_p_window_wrapper;
        FT_Library m_library;
        std::vector<FontFileData> m_font_file_data;

    private:
        void findFontFilesRecursively(std::string path, std::vector<FontFileData> &fonts);
        bool searchFont(std::string font_file, std::string &out_path);
        
        // Glyph reading related methods
        void loadNewGlyphs(TextInstance &instance);
        void createFontTexture(TextInstance &instance);
        void createTextBox(TextInstance &instance, dengMath::vec2<float> text_box_position);

    public:
        FontInstanceCreator(deng::WindowWrap &window_wrap);
        void createPixelBasedFontInstance(const char *text, uint16_t font_px_size, const char *font_file, dengMath::vec2<float> text_coords, TextInstance &instance);
        void createPtBasedFontInstance(const char *text, int32_t font_pt_size, uint32_t window_width, uint32_t window_height, dengMath::vec2<float> text_coords, 
        const char *font_file, TextInstance &instance);
    };


    /* Count the fps and display it on screen */
    class FrameCounter {
    private:
        FT_Library library;
        FT_Face m_source_code_pro_semi_bold;
        uint32_t m_frame_count = 0;
        
    public:
        FrameCounter();
        void countFrame() { this->m_frame_count++; }
    };
}

#endif