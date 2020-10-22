#ifndef CSS_DATA_MANAGER_H
#define CSS_DATA_MANAGER_H

namespace dengCSS {
    struct CSSDataHeader {
        std::array<std::string, DENG_CSS_SUPPORTED_PROPERTIES_COUNT> properties_data;
    };

    struct CSSGeneralBorderInfo {
        double border_width;
        dengMath::vec3<float> border_color;
    };

    struct CSSSpecifiedBorderInfo {
        double border_left_width;
        dengMath::vec3<float> border_left_color;

        double border_top_width;
        dengMath::vec3<float> border_top_color;

        double border_right_width;
        dengMath::vec3<float> border_right_color;

        double border_bottom_width;
        dengMath::vec3<float> border_bottom_color;
    };

    struct CSSMarginData {
        double margin_top, margin_right, margin_bottom, margin_left;
    };

    struct CSSHeadData {
        double height;
        dengMath::vec3<float> background_color;
        CSSGeneralBorderInfo *p_general_border_info = NULL;
        CSSSpecifiedBorderInfo *p_specified_border_info = NULL;

        ~CSSHeadData() {
            if(this->p_general_border_info != NULL) delete this->p_general_border_info;
            if(this->p_specified_border_info != NULL) delete this->p_specified_border_info;
        }
    };

    struct CSSBodyData {
        double height;
        double width;
        dengMath::vec3<float> background_color;
        CSSGeneralBorderInfo *p_general_border_info = NULL;
        CSSSpecifiedBorderInfo *p_specified_border_info = NULL;

        ~CSSBodyData() {
            if(this->p_general_border_info != NULL) delete this->p_general_border_info;
            if(this->p_specified_border_info != NULL) delete this->p_specified_border_info;
        }
    };

    struct CSSGenericObjectData {
        dengMath::vec3<float> background_color = {0.0f, 0.0f, 0.0f};
        CSSGeneralBorderInfo *p_general_border_info = NULL;
        CSSSpecifiedBorderInfo *p_specified_border_info = NULL;
        double font_size;
        dengCSSFontWeight font_weight;
        
        double height, width;
        CSSMarginData margin_data;

        ~CSSGenericObjectData() {
            if(this->p_general_border_info != nullptr) delete this->p_general_border_info;
            if(this->p_specified_border_info != nullptr) delete this->p_specified_border_info;
        }
    };

    class CSSDataHandler {
    private:
        dengUtils::FileManager m_filemanager;
        deng::Window *m_p_window;
        CSSHeadData m_head_data;
        CSSBodyData m_body_data;
        CSSGenericObjectData *m_p_generic_object_data = nullptr;
        std::vector<std::string> m_css_files;

    private:
        CSSGeneralBorderInfo handleBorderData(std::string &property_values);
        dengMath::vec3<float> handleGeneralColor(std::string &str_color);
        double handleGeneralSize(const std::string &size, const dengCoordinateAxisType &deng_css_size_type, const dengMath::vec2<double> &block_size);
        dengCSSFontWeight handleFontWeight(const std::string &str_weight);

        dengBool populateDataHeader(const std::string &css_file_name, const std::string &css_class_name, CSSDataHeader *p_data_header);
        void populateBorderValueData(CSSDataHeader *p_data_header, CSSGeneralBorderInfo **pp_general_border_info, CSSSpecifiedBorderInfo **pp_specified_border_info, const dengMath::vec2<double> &block_size);
        void populateMarginData(CSSDataHeader *p_data_header, CSSMarginData *p_margin_data, const dengMath::vec2<double> &block_size);

    public: 
        void readClassData(const std::string &class_name, const dengCSSClassReadMode &class_read_mode, const dengMath::vec2<double> &block_size);
        void getHeadData(CSSHeadData **pp_titlebar_data);
        void getBodyData(CSSBodyData **pp_body_data);
        void getGenericObjectData(CSSGenericObjectData **pp_generic_object_data);

        CSSDataHandler(deng::Window *p_window, const std::string &css_file_path);
        ~CSSDataHandler() {
            if(this->m_p_generic_object_data != nullptr) delete this->m_p_generic_object_data;
        }
    };
};

#endif