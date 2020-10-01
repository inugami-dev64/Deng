#ifndef CSS_DATA_MANAGER_H
#define CSS_DATA_MANAGER_H

namespace dengCSS {
    struct CSSGeneralBorderInfo {
        uint16_t border_width;
        dengMath::vec3<float> border_color;
    };

    struct CSSSpecifiedBorderInfo {
        uint16_t border_left_width;
        dengMath::vec3<float> border_left_color;

        uint16_t border_top_width;
        dengMath::vec3<float> border_top_color;

        uint16_t border_right_width;
        dengMath::vec3<float> border_right_color;

        uint16_t border_bottom_width;
        dengMath::vec3<float> border_bottom_color;
    };

    struct CSSTitleData {
        uint16_t height;
        CSSGeneralBorderInfo *p_general_border_info = nullptr;
        CSSSpecifiedBorderInfo *p_specified_border_info = nullptr;
    };

    struct CSSBodyData {
        uint16_t height;
        uint16_t width;
        CSSGeneralBorderInfo *p_general_border_info = nullptr;
        CSSSpecifiedBorderInfo *p_specified_border_info = nullptr;
    };

    struct CSSMinimiseTriangleData {
        uint16_t margin_left;
        uint16_t margin_top;
    };

    class CSSDataHandler {
    private:
        dengUtils::FileManager m_filemanager;
        CSSTitleData m_title_data;
        CSSBodyData m_body_data;
        CSSMinimiseTriangleData m_minimise_triangle_data;

    private:
        dengBool verifyTitleData(std::vector<std::vector<std::string>> *p_css_files_data);
        void getTitleData(std::vector<std::vector<std::string>> *p_css_files_data);
        dengBool verifyBodyData(std::vector<std::vector<std::string>> *p_css_files_data);
        void getBodyData(std::vector<std::vector<std::string>> *p_css_files_data);
        dengBool verifyMinimiseTriangleData(std::vector<std::vector<std::string>> *p_css_files_data);
        void getMinimiseTriangleData(std::vector<std::vector<std::string>> *p_css_files_data);

    public: 
        CSSDataHandler();
    };
};

#endif