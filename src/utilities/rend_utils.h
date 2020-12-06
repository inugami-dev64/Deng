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


    /* Struct used to store instance information for rendering text */ 
    struct TextInstance {
        std::vector<VkImage> glyph_image_data;
        std::vector<VkDeviceMemory> glyph_image_memory;
        FontFileData font_file_data;
        FT_Face font_face;
        std::vector<std::pair<char, FT_GlyphSlot>> glyph_slots;
        std::vector<uint16_t> glyph_indices; 
    };


    /* Class to manage everything related to font rendering */ 
    class FontInstanceCreator {
    private:
        FT_Library m_library;
        std::vector<FontFileData> m_font_file_data;

    private:
        void findFontFilesRecursively(std::string path, std::vector<FontFileData> &fonts);
        bool searchFont(std::string font_file, std::string &out_path);
        
        // Glyph reading related methods
        void loadNewGlyphs(const char *text, TextInstance &instance);
        void createFontTexture(TextInstance &instance);

    public:
        FontInstanceCreator();
        void createNewPixelBasedTextInstance(const char *text, uint16_t font_px_size, const char *font_file, TextInstance &instance);
        void createNewPointBasedTextInstance(const char *text, uint16_t font_pt_size, uint32_t window_width, uint32_t window_height, const char *font_file, TextInstance &instance);
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