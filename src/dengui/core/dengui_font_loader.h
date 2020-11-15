#ifndef DENGUI_FONT_LOADER_H
#define DENGUI_FONT_LOADER_H
#define DENG_FONT_WIDTH_MULTIPLIER 0.6

namespace dengUI {
    struct CharData {
        unsigned char id;
        dengMath::vec4<dengMath::vec2<float>> ch_vector_positions;
    };

    class Font {
    private:
        const char *m_font_family_name;
        const char *m_font_path_name;
        uint32_t m_font_size;
        std::vector<CharData> m_char_map_data;
        dengUtils::RawTextureData m_raw_char_map_data;

    private:
        const char *getFontPathName(const char *font_family);

    public:
        Font(const char *font_family, const uint32_t &font_size);
        void getCharInfo(CharData **pp_char_data, char ch);
        uint32_t getFontSize();
        ~Font();
    };
}

#endif