#ifndef FILES_H
#define FILES_H

#define DENG_FILE_EXTENSION_CSS ".css"
#define DENG_FILE_EXTENSION_PY ".py"
#define DENG_FILE_EXTENSION_CS ".cs"

namespace dengUtils {
    class FileManager {
    private:
        std::ifstream *p_file_in;
        std::ofstream *p_file_out;

    private:
        void sortAlphabetically(std::vector<std::string> *p_sortable_data, const dengBool &is_reverse_alphabet);
        void searchFilesRecursively(std::string folder_path, std::vector<std::string> *p_folder_contents, DIR *p_dir);
        
        // function that gets the bounds between certain strings in a file 
        void getBounds(std::vector<dengMath::vec2<dengMath::vec2<size_t>>> *p_bounds_data, const dengMath::vec2<std::string> &bound_strings, std::vector<std::string> *p_file_contents);
        dengBool isBetweenComments(std::vector<dengMath::vec2<dengMath::vec2<size_t>>> *comment_bounds, const dengMath::vec2<dengMath::vec2<size_t>> &attribute_boundaries);

    public:
        void getFileContents(const std::string &file_name, std::vector<char> *p_char_output_vector, std::vector<std::string> *p_string_output_vector);
        void writeToFile(const std::string &file_name, const std::string &line_contents, const uint32_t &write_mode);
        void getFolderContents(std::string folder_name, std::vector<std::string> *p_folder_contents, const dengFolderContentsReadMode &folder_contents_read_mode, const dengFolderContentsSortingMode &folder_contents_sort_mode);
        void filterByFileExtension(std::vector<std::string> *p_file_name_data, const std::string &file_extension);
        
        // return vector of strings of css property values
        dengBool getCSSProperyValue(const std::string &css_class, const std::string &attribute, const std::string &file_path, std::vector<std::string> *p_value_data);

        template<typename T>
        T getConfVal(const std::string &conf, const std::string &file_name);
    };

    struct EditorEnvironmentConf {
        float environment_color_r;
        float environment_color_g;
        float environment_color_b;

        dengBool show_grid;
        float grid_height;
        float grid_width;
        
        float grid_line_color_r;
        float grid_line_color_g;
        float grid_line_color_b;
    };

    struct EditorCameraConf {
        float movement_x;
        float movement_y;
        float movement_z;

        float mouse_movement_x;
        float mouse_movement_y;

        float fov;
    };

    struct DengUIConf {
        float dengui_window_color_r;
        float dengui_window_color_g;
        float dengui_window_color_b;
        float dengui_window_color_a;

        float dengui_border_thickness;
        float dengui_titlebar_height;
        float dengui_border_color_r;
        float dengui_border_color_g;
        float dengui_border_color_b;
        float dengui_border_color_a;

        float dengui_minimizing_triangle_color_r;
        float dengui_minimizing_triangle_color_g;
        float dengui_minimizing_triangle_color_b;
        float dengui_minimizing_triangle_color_a;
    };

    template<typename T>
    T FileManager::getConfVal(const std::string &conf, const std::string &file_name) {
        std::vector<std::string> file_contents;
        this->getFileContents(file_name, nullptr, &file_contents);
        int32_t line_index = 0;

        for(std::string &line : file_contents) {
            if(line.find(conf) == 0) {
                std::string value_str;
                int8_t index;

                if(static_cast<int>(line.find("#")) == -1 && static_cast<int>(line.find("=")) == -1) {
                    ERR(DENG_CONF_PARSING_ERROR_MSG(file_name) + DENG_GENERAL_PARSING_ERROR_LINE_MSG(std::to_string(line_index)));
                }

                else {
                    index = static_cast<int8_t>(line.find("=")) + 1;
                    while(line[index] == ' ' || line[index] == '"') index++;

                    for(; index < line.size() && line[index] != ' ' && line[index] != '#' && line[index] != '"'; index++) {
                        value_str += line[index];
                    }
                }

                T i;
                if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_BOOL)) {
                    if(value_str == "true") return true;
                    else if(value_str == "false") return false; 
                }

                else if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_FLOAT)) {
                    return static_cast<T>(std::stof(value_str));
                }

                else if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_DOUBLE)) {
                    return static_cast<T>(std::stod(value_str));
                }

                else if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_NON_FLOATING)) {
                    return static_cast<T>(std::stoi(value_str));
                }                       
            }
            line_index++;
        }
        T i;
        return i;
    }
}

#endif