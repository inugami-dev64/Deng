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
        
        dengBool identifyLineComment(int *p_comment_start_index, const std::string &line, char comment);
    
    public:
        void getFileContents(const std::string &file_name, std::vector<char> *p_char_output_vector, std::vector<std::string> *p_string_output_vector);
        void writeToFile(const std::string &file_name, const std::string &line_contents, const dengWriteMode &write_mode);
        void getFolderContents(std::string folder_name, std::vector<std::string> *p_folder_contents, const dengFolderContentsReadMode &folder_contents_read_mode, const dengFolderContentsSortingMode &folder_contents_sort_mode);
        void filterByFileExtension(std::vector<std::string> *p_file_name_data, const std::string &file_extension);
        
        // return vector of strings of css property values
        dengBool getCSSProperyValue(const std::string &css_class, const std::string &attribute, const std::string &file_path, std::vector<std::string> *p_value_data);

        template<typename T>
        T getConfVal(const std::string &conf, const std::string &file_name, int *p_line_index, dengBool allow_whitespaces);

        size_t findFirstLineInstance(const std::string &keyword, std::vector<std::string> *p_file_contents);
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
    T FileManager::getConfVal(const std::string &conf, const std::string &file_name, int *p_line_index, dengBool allow_whitespaces) {
        size_t index, ch_index;
        std::vector<std::string> file_contents;
        this->getFileContents(file_name, nullptr, &file_contents);
        int *p_local_index;

        if(p_line_index == nullptr) {
            p_local_index = new int;
            *p_local_index = -1; 
        }

        else p_local_index = p_line_index;

        switch (*p_local_index)
        {
        case -1: {
            *p_local_index = this->findFirstLineInstance(conf, &file_contents);
            if(*p_local_index == (size_t) -1) ERRME("Couldn't find conf " + conf + " in file " + file_name);
            break;
        }
        
        default:
            break;
        }

        std::string value_str;
        if(allow_whitespaces) {
            if(static_cast<int>(file_contents[*p_local_index].find("#")) == -1 && static_cast<int>(file_contents[*p_local_index].find("=")) == -1)
                ERRME("Failed to get conf value in file " + file_name + " on line " + std::to_string(*p_local_index));

            else {
                index = static_cast<int8_t>(file_contents[*p_local_index].find("=")) + 1;
                while(file_contents[*p_local_index][ch_index] == ' ' || file_contents[*p_local_index][ch_index] == '"') ch_index++;

                for(; index < file_contents[*p_local_index].size() && file_contents[*p_local_index][ch_index] != ' ' && file_contents[*p_local_index][index] != '#' && file_contents[*p_local_index][index] != '"'; index++)
                    value_str += file_contents[*p_local_index][index];
            }
        }

        else {
            int comment_line;
            this->identifyLineComment(&comment_line, file_contents[*p_local_index], '#');
            
            if((int) (ch_index = file_contents[*p_local_index].find(conf)) == -1)  
                ERRME("Failed to get config " + conf + " in file " + file_name + " on line " + std::to_string(*p_local_index));

            ch_index += conf.size();
            if(file_contents[*p_local_index][ch_index] == '=' && file_contents[*p_local_index][ch_index++] != ' ') {
                for(; file_contents[*p_local_index][ch_index] != ' ' && file_contents[*p_local_index][ch_index] != '#' && ch_index < file_contents[*p_local_index].size(); ch_index++)
                    value_str+=file_contents[*p_local_index][ch_index];
            }

            else 
                ERRME("Failed to get conf value in file " + file_name + " on line " + std::to_string(*p_local_index));
        }

        if(p_line_index == nullptr) delete p_local_index;

        T i = {};
        if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_BOOL)) {
            if(value_str == "true") return true;
            else if(value_str == "false") return false; 
        }

        else if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_FLOAT))
            return static_cast<T>(std::stof(value_str));

        else if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_DOUBLE))
            return static_cast<T>(std::stod(value_str));

        else if(FindWithTypename::isNumeralType(i, DENG_NUMERAL_TYPE_NON_FLOATING))
            return static_cast<T>(std::stoi(value_str));
      
        return i;
    }
}

#endif