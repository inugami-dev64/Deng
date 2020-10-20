#include "../../core/deng_core.h"

namespace dengCSS {

    CSSDataHandler::CSSDataHandler(deng::Window *p_window, const std::string &css_file_path) {
        this->m_p_window = p_window;
        this->m_filemanager.getFolderContents(css_file_path, &this->m_css_files, DENG_FOLDER_CONTENTS_READ_MODE_RECURSIVE, DENG_FOLDER_CONTENTS_SORT_STYLE_DONT_CARE);
        this->m_filemanager.filterByFileExtension(&this->m_css_files, DENG_FILE_EXTENSION_CSS);
        dengMath::vec2<double> local_relative_block_size;

        for(std::string &str : this->m_css_files)
            LOG(str);

        this->readClassData(DENG_CSS_CLASS_BODY, DENG_CSS_CLASS_READ_MODE_MAIN_CLASS, this->m_p_window->getSize().vecCast<double>());
        local_relative_block_size = {this->m_body_data.width, this->m_body_data.height};
        this->readClassData(DENG_CSS_CLASS_HEAD, DENG_CSS_CLASS_READ_MODE_MAIN_CLASS, local_relative_block_size);
        this->readClassData(".minimise_triangle", DENG_CSS_CLASS_READ_MODE_CUSTOM_CLASS, local_relative_block_size);
    }

    void CSSDataHandler::getHeadData(CSSHeadData **pp_titlebar_data) {
        *pp_titlebar_data = &this->m_head_data;
    }

    void CSSDataHandler::getBodyData(CSSBodyData **pp_body_data) {
        *pp_body_data = &this->m_body_data;
    }

    void CSSDataHandler::getGenericObjectData(CSSGenericObjectData **pp_generic_object_data) {
        *pp_generic_object_data = this->m_p_generic_object_data;
    }

    double CSSDataHandler::handleGeneralSize(const std::string &str_size, const dengCoordinateAxisType &deng_coord_axis_type, const dengMath::vec2<double> &block_size) {
        if(str_size == "") return 0.0;

        size_t local_unit_index;
        std::string local_str_number;
        if(static_cast<int>(local_unit_index = str_size.find("px")) > 0) {
            for(size_t i = 0; i < local_unit_index; i++)
                local_str_number += str_size[i];

            return std::stod(local_str_number);
        }

        else if(static_cast<int>(local_unit_index = str_size.find("%")) > 0) {
            for(size_t i = 0; i < local_unit_index; i++) 
                local_str_number += str_size[i];

            switch (deng_coord_axis_type)
            {
            case DENG_COORD_AXIS_X:
                return round(std::stod(local_str_number) / 100.0 * block_size.first);
                break;
            
            case DENG_COORD_AXIS_Y:
                return round(std::stod(local_str_number) / 100.0 * block_size.second);
                break;

            case DENG_COORD_AXIS_UNDEFINED: case DENG_COORD_AXIS_Z: {
                // calculate the mean size between window width and height
                double local_mean_window_size = (block_size.first + block_size.second) / 2;

                return round(std::stod(local_str_number) / 100.0 * local_mean_window_size);
                break;
            }


            default:
                break;
            }

            return 0.0;
        }
    }

    dengMath::vec3<float> CSSDataHandler::handleGeneralColor(std::string &str_color) {
        if(str_color == "") return {0.0f, 0.0f, 0.0f};

        if(str_color[0] != '#') {
            ColorDefinitions local_color_def;
            return local_color_def.deng_css_colors[str_color];
        }

        else if(str_color.size() == 7) {
            dengMath::Conversion local_conversion;
            std::string local_r, local_g, local_b;
            local_r += str_color[1], local_r += str_color[2];
            local_g += str_color[3], local_g += str_color[4];
            local_b += str_color[5], local_b += str_color[6];

            return {static_cast<float>(local_conversion.hexToDec(local_r)) / 255.0f, static_cast<float>(local_conversion.hexToDec(local_g)) / 255.0f, static_cast<float>(local_conversion.hexToDec(local_b)) / 255.0f};
        }   

        return {0.0f, 0.0f, 0.0f};
    }

    CSSGeneralBorderInfo CSSDataHandler::handleBorderData(std::string &property_values) {
        CSSGeneralBorderInfo local_border_info = {0.0f, {0.0f, 0.0f, 0.0f}};
        int index, ch_index;
        std::array<std::string, 3> local_border_str_values = {""}; 
        
        if(property_values == "") {
            ERRMEDB("No general border property values given!");
            return local_border_info; 
        }

        LOG("property values: " + property_values);
        
        for(index = 0, ch_index = 0; ch_index < property_values.size(); ch_index++) {
            if(property_values[ch_index] != ' ' && property_values[ch_index] != ';')
                local_border_str_values[index] += property_values[ch_index];

            else if(index < local_border_str_values.size()) index++;
        }

        if(local_border_str_values[1] == "solid") {
            local_border_info.border_width = this->handleGeneralSize(local_border_str_values[0], DENG_COORD_AXIS_X, this->m_p_window->getSize().vecCast<double>());
            local_border_info.border_color = this->handleGeneralColor(local_border_str_values[2]);
        }
    }

    dengCSSFontWeight CSSDataHandler::handleFontWeight(const std::string &str_weight) {
        if(str_weight == "normal") return DENG_CSS_FONT_WEIGHT_NORMAL;
        else if(str_weight == "bold") return DENG_CSS_FONT_WEIGHT_BOLD;
        else return DENG_CSS_FONT_WEIGHT_NORMAL;
    }

    dengBool CSSDataHandler::populateDataHeader(const std::string &css_file_name, const std::string &css_class_name, CSSDataHeader *p_data_header) {
        PropertiesDefinitions local_properties_def;
        dengBool local_is_class_found_in_file = DENG_FALSE;
        std::vector<std::string> local_css_data;
        LOG(css_class_name + ": ");

        for(size_t i = 0; i < local_properties_def.properties_names.size(); i++) {
            switch (this->m_filemanager.getCSSProperyValue(css_class_name, local_properties_def.properties_names[i], css_file_name, &local_css_data))
            {
            case DENG_TRUE:
                p_data_header->properties_data[i] = local_css_data[0];
                LOG(p_data_header->properties_data[i]);
                local_is_class_found_in_file = DENG_TRUE;
                local_css_data.clear();
                break;

            case DENG_FALSE:
                p_data_header->properties_data[i] = "";
                break; 
            default:
                break;
            }
        }

        return local_is_class_found_in_file;
    }

    void CSSDataHandler::populateMarginData(CSSDataHeader *p_data_header, CSSMargindata *p_margin_data, const dengMath::vec2<double> &block_size) {
        if(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN] != "") {
            double local_margin_x = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN], DENG_COORD_AXIS_X, block_size);
            double local_margin_y = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN], DENG_COORD_AXIS_Y, block_size);
            p_margin_data->margin_top = local_margin_y;
            p_margin_data->margin_right = local_margin_x;
            p_margin_data->margin_bottom = local_margin_y;
            p_margin_data->margin_left = local_margin_x;
        }
        else {
            p_margin_data->margin_top = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN_TOP], DENG_COORD_AXIS_Y, block_size);
            p_margin_data->margin_right = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN_RIGHT], DENG_COORD_AXIS_X, block_size);
            p_margin_data->margin_bottom = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN_BOTTOM], DENG_COORD_AXIS_Y, block_size);
            p_margin_data->margin_left = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_MARGIN_LEFT], DENG_COORD_AXIS_X, block_size);
        }
    }

    void CSSDataHandler::populateBorderValueData(CSSDataHeader *p_data_header, CSSGeneralBorderInfo **pp_general_border_info, CSSSpecifiedBorderInfo **pp_specified_border_info, const dengMath::vec2<double> &block_size) {
        if(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER] != "") {
            *pp_general_border_info = new CSSGeneralBorderInfo;
            **pp_general_border_info = this->handleBorderData(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER]);
            LOG("border color is: {" + std::to_string((*pp_general_border_info)->border_color.first) + ";" + std::to_string((*pp_general_border_info)->border_color.second) + ";" + std::to_string((*pp_general_border_info)->border_color.third) + "}");
            LOG("border width is: " + std::to_string((*pp_general_border_info)->border_width));
        }
        
        else if(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_COLOR] != "" && p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_WIDTH] != "") {
            *pp_general_border_info = new CSSGeneralBorderInfo;
            (*pp_general_border_info)->border_width = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_WIDTH], DENG_COORD_AXIS_X, block_size);
            (*pp_general_border_info)->border_color = this->handleGeneralColor(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_COLOR]);
        }

        else if(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_TOP_COLOR] != "" || p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_TOP_WIDTH] != "" || p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_RIGHT_COLOR] != "" || p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_RIGHT_WIDTH] != "" ||
        p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_BOTTOM_COLOR] != "" || p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_BOTTOM_WIDTH] != "" || p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_LEFT_COLOR] != "" || p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_LEFT_WIDTH] != "") {
            *pp_specified_border_info = new CSSSpecifiedBorderInfo;
            (*pp_specified_border_info)->border_top_color = this->handleGeneralColor(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_TOP_COLOR]);
            (*pp_specified_border_info)->border_top_width = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_TOP_WIDTH], DENG_COORD_AXIS_Y, block_size);
            (*pp_specified_border_info)->border_right_color = this->handleGeneralColor(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_RIGHT_COLOR]);
            (*pp_specified_border_info)->border_right_width = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_RIGHT_WIDTH], DENG_COORD_AXIS_X, block_size);
            (*pp_specified_border_info)->border_bottom_color = this->handleGeneralColor(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_BOTTOM_COLOR]);
            (*pp_specified_border_info)->border_bottom_width = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_BOTTOM_WIDTH], DENG_COORD_AXIS_Y, block_size);
            (*pp_specified_border_info)->border_left_color = this->handleGeneralColor(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_LEFT_COLOR]);
            (*pp_specified_border_info)->border_left_width = this->handleGeneralSize(p_data_header->properties_data[DENG_CSS_PROPERTY_BORDER_LEFT_WIDTH], DENG_COORD_AXIS_X, block_size);
        }
    }

    void CSSDataHandler::readClassData(const std::string &class_name, const dengCSSClassReadMode &class_read_mode, const dengMath::vec2<double> &block_size) {
        CSSDataHeader local_css_data_header;

        for(size_t i = 0; i < this->m_css_files.size(); i++)
            if(this->populateDataHeader(this->m_css_files[i], class_name, &local_css_data_header)) break;

        switch (class_read_mode)
        {
        case DENG_CSS_CLASS_READ_MODE_MAIN_CLASS:            
            if(class_name == DENG_CSS_CLASS_HEAD) {
                this->populateBorderValueData(&local_css_data_header, &this->m_head_data.p_general_border_info, &this->m_head_data.p_specified_border_info, this->m_p_window->getSize().vecCast<double>());
                this->m_head_data.background_color = this->handleGeneralColor(local_css_data_header.properties_data[DENG_CSS_PROPERTY_BACKGROUND_COLOR]);
                this->m_head_data.height = this->handleGeneralSize(local_css_data_header.properties_data[DENG_CSS_PROPERTY_HEIGHT], DENG_COORD_AXIS_Y, this->m_p_window->getSize().vecCast<double>());
            }

            else if(class_name == DENG_CSS_CLASS_BODY) {
                this->populateBorderValueData(&local_css_data_header, &this->m_body_data.p_general_border_info, &this->m_body_data.p_specified_border_info, this->m_p_window->getSize().vecCast<double>());
                this->m_body_data.background_color = this->handleGeneralColor(local_css_data_header.properties_data[DENG_CSS_PROPERTY_BACKGROUND_COLOR]);
                this->m_body_data.width = this->handleGeneralSize(local_css_data_header.properties_data[DENG_CSS_PROPERTY_WIDTH], DENG_COORD_AXIS_X, this->m_p_window->getSize().vecCast<double>());
                this->m_body_data.height = this->handleGeneralSize(local_css_data_header.properties_data[DENG_CSS_PROPERTY_HEIGHT], DENG_COORD_AXIS_Y, this->m_p_window->getSize().vecCast<double>());
            }
            break;
        
        case DENG_CSS_CLASS_READ_MODE_CUSTOM_CLASS:

            delete this->m_p_generic_object_data;
            this->m_p_generic_object_data = new CSSGenericObjectData;

            this->populateBorderValueData(&local_css_data_header, &this->m_p_generic_object_data->p_general_border_info, &this->m_p_generic_object_data->p_specified_border_info, block_size);
            this->m_p_generic_object_data->background_color = this->handleGeneralColor(local_css_data_header.properties_data[DENG_CSS_PROPERTY_BACKGROUND_COLOR]);
            this->m_p_generic_object_data->font_size = this->handleGeneralSize(local_css_data_header.properties_data[DENG_CSS_PROPERTY_FONT_SIZE], DENG_COORD_AXIS_UNDEFINED, block_size);
            this->m_p_generic_object_data->font_weight = this->handleFontWeight(local_css_data_header.properties_data[DENG_CSS_PROPERTY_FONT_WEIGHT]);
            this->m_p_generic_object_data->height = this->handleGeneralSize(local_css_data_header.properties_data[DENG_CSS_PROPERTY_HEIGHT], DENG_COORD_AXIS_Y, block_size);
            this->m_p_generic_object_data->width = this->handleGeneralSize(local_css_data_header.properties_data[DENG_CSS_PROPERTY_WIDTH], DENG_COORD_AXIS_X, block_size);
            this->populateMarginData(&local_css_data_header, &this->m_p_generic_object_data->margin_data, block_size);
            
            break;
        default:
            break;
        }
    }
}