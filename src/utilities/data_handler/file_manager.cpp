#include "../../core/deng_core.h"

namespace dengUtils {   

    void FileManager::getFileContents(const std::string &file_name, std::vector<char> *p_char_output_vector, std::vector<std::string> *p_string_output_vector) {
        this->p_file_in = new std::ifstream(file_name, std::ios::ate | std::ios::binary);
        size_t file_size = (size_t) this->p_file_in->tellg();
        this->p_file_in->seekg(0);

        if(!this->p_file_in->is_open()) {
            ERRME("Failed to load file '" + file_name + "'!");
        }

        if(p_char_output_vector != nullptr) {
            p_char_output_vector->resize(file_size);
            this->p_file_in->read(p_char_output_vector->data(), file_size);
            this->p_file_in->close();
        }

        if(p_string_output_vector != nullptr) {
            std::string buffer;
            while(std::getline(*this->p_file_in, buffer)) p_string_output_vector->push_back(buffer);
        }

        delete this->p_file_in;
    }

    void FileManager::writeToFile(const std::string &file_name, const std::string &line_contents, const dengWriteMode &write_mode) {

        switch (write_mode)
        {
        case DENG_WRITEMODE_REWRITE:
            this->p_file_out = new std::ofstream(file_name);
            break;

        case DENG_WRITEMODE_FROM_END:
            this->p_file_out = new std::ofstream(file_name, std::ios::app);
            break;
        
        default:
            break;
        }
        
        *this->p_file_out << line_contents << "\n";
        this->p_file_out->close();
    }

    void FileManager::filterByFileExtension(std::vector<std::string> *p_file_name_data, const std::string &file_extension) {
        for(size_t i = 0; i < p_file_name_data->size();) {
            if((*p_file_name_data)[i].find(file_extension) == (*p_file_name_data)[i].size() - file_extension.size())
                i++;
            
            else 
                p_file_name_data->erase(p_file_name_data->begin() + i);
        }
    }

    dengBool FileManager::identifyLineComment(int *p_comment_start_index, const std::string &line, char comment) {
        if((*p_comment_start_index = (int) line.find(comment)) != -1)
            return DENG_FALSE;

        else
            return DENG_TRUE;
    }

    size_t FileManager::findFirstLineInstance(const std::string &keyword, std::vector<std::string> *p_file_contents) {
        size_t line_index;
        dengBool is_found = DENG_FALSE;
        for(line_index = 0; line_index < p_file_contents->size(); line_index++) {
            if((*p_file_contents)[line_index].find(keyword) != (size_t) -1) {
                is_found = DENG_TRUE;   
                break;
            }
        }

        if(is_found) return line_index;
        else return (size_t) -1;
    }

    void FileManager::sortAlphabetically(std::vector<std::string> *p_sortable_data, const dengBool &is_reverse_alphabet) {
        if(p_sortable_data->size() != 0) {
            dengMath::vec2<std::string> local_copy_buffer;
            dengBool local_is_list_sorted = DENG_FALSE;
            
            switch (is_reverse_alphabet)
            {
            case DENG_FALSE:
                while(!local_is_list_sorted) {
                    local_is_list_sorted = DENG_TRUE;
                    for(size_t content_index = 0; content_index < p_sortable_data->size() - 1; content_index++) {
                        for(size_t str_index = 0; str_index < p_sortable_data->at(content_index).size() && str_index < p_sortable_data->at(content_index + 1).size(); str_index++) {
                            if(static_cast<int>((*p_sortable_data)[content_index][str_index]) > static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) {
                                local_is_list_sorted = DENG_FALSE;
                                local_copy_buffer.first = (*p_sortable_data)[content_index + 1];
                                local_copy_buffer.second = (*p_sortable_data)[content_index];

                                (*p_sortable_data)[content_index] = local_copy_buffer.first;
                                (*p_sortable_data)[content_index + 1] = local_copy_buffer.second;
                                break;
                            }

                            else if(static_cast<int>((*p_sortable_data)[content_index][str_index]) < static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) break;
                        }

                        if(local_is_list_sorted && p_sortable_data->at(content_index).size() > p_sortable_data->at(content_index + 1).size()) {
                            local_copy_buffer.first = p_sortable_data->at(content_index + 1);
                            local_copy_buffer.second = p_sortable_data->at(content_index);

                            (*p_sortable_data)[content_index] = local_copy_buffer.first;
                            (*p_sortable_data)[content_index + 1] = local_copy_buffer.second;
                        }
                    }
                } 
                break;

            case DENG_TRUE:
                while(!local_is_list_sorted) {
                    local_is_list_sorted = DENG_TRUE;
                    for(size_t content_index = 0; content_index < p_sortable_data->size() - 1; content_index++) {
                        for(size_t str_index = 0; str_index < p_sortable_data->at(content_index).size() && str_index < p_sortable_data->at(content_index + 1).size(); str_index++) {
                            if(static_cast<int>((*p_sortable_data)[content_index][str_index]) < static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) {
                                local_is_list_sorted = DENG_FALSE;
                                local_copy_buffer.first = (*p_sortable_data)[content_index + 1];
                                local_copy_buffer.second = (*p_sortable_data)[content_index];

                                (*p_sortable_data)[content_index] = local_copy_buffer.first;
                                (*p_sortable_data)[content_index + 1] = local_copy_buffer.second;
                                break;
                            }

                            else if(static_cast<int>((*p_sortable_data)[content_index][str_index]) > static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) break;
                        }

                        if(local_is_list_sorted && p_sortable_data->at(content_index).size() < p_sortable_data->at(content_index + 1).size()) {
                            local_copy_buffer.first = p_sortable_data->at(content_index + 1);
                            local_copy_buffer.second = p_sortable_data->at(content_index);

                            (*p_sortable_data)[content_index] = local_copy_buffer.first;
                            (*p_sortable_data)[content_index + 1] = local_copy_buffer.second;
                        }
                    }
                } 
                break;
            
            default:
                break;
            }
        }
    }

    void FileManager::searchFilesRecursively(std::string folder_path, std::vector<std::string> *p_folder_contents, DIR *p_dir) {
        if(folder_path[folder_path.size() - 1] != '/') folder_path += "/";
        p_dir = opendir(folder_path.c_str());

        if(p_dir != nullptr) {
            struct dirent *local_entry;
            while((local_entry = readdir(p_dir)) != NULL) {
                switch(local_entry->d_type) 
                {
                case DT_REG:
                    p_folder_contents->push_back(folder_path + std::string(local_entry->d_name));
                    break;

                case DT_DIR:
                    if(std::string(local_entry->d_name) != "." && std::string(local_entry->d_name) != "..") this->searchFilesRecursively(folder_path + std::string(local_entry->d_name) + "/", p_folder_contents, p_dir);
                    break;
                }
            }
        }
    }

    void FileManager::getFolderContents(std::string folder_name, std::vector<std::string> *p_folder_contents, const dengFolderContentsReadMode &folder_contents_read_mode, const dengFolderContentsSortingMode &folder_contents_sort_mode) {
        if(folder_name[folder_name.size() - 1] != '/') folder_name += "/";
        std::string local_prev_subfolder_path = folder_name;
        std::vector<std::string> local_subfolder_names;
        std::vector<std::string> local_file_names;
        
        DIR *p_dir;

        p_dir = opendir(folder_name.c_str());
        
        if(p_dir != nullptr) {
            struct dirent *local_p_entry;
            dengBool local_end_loop = DENG_FALSE;
            while((local_p_entry = readdir(p_dir)) != NULL) {        
                switch (folder_contents_read_mode)
                {
                case DENG_FOLDER_CONTENTS_READ_MODE_FILES_ONLY:
                    if(local_p_entry->d_type == DT_REG)
                        p_folder_contents->push_back(folder_name + std::string(local_p_entry->d_name));
                    break;

                case DENG_FOLDER_CONTENTS_READ_MODE_FOLDERS_ONLY:
                    if(local_p_entry->d_type == DT_DIR && std::string(local_p_entry->d_name) != std::string(".") && std::string(local_p_entry->d_name) != std::string("..")) 
                        p_folder_contents->push_back(folder_name + std::string(local_p_entry->d_name) + "/");

                    break;

                case DENG_FOLDER_CONTENTS_READ_MODE_FILES_AND_FOLDERS: {
                    bool local_is_content_separated = folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FOLDERS_FIRST || folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FILES_FIRST ||
                                                      folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FILES_FIRST || folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FOLDERS_FIRST;
                        
                    switch (local_p_entry->d_type)
                    {
                    case DT_REG:
                        if(local_is_content_separated)
                            local_file_names.push_back(folder_name + std::string(local_p_entry->d_name));

                        else if(!local_is_content_separated)
                            p_folder_contents->push_back(std::string(folder_name + local_p_entry->d_name));

                        break;

                    case DT_DIR:
                        if(local_is_content_separated && std::string(local_p_entry->d_name) != "." && std::string(local_p_entry->d_name) != "..")
                            local_subfolder_names.push_back(std::string(folder_name + local_p_entry->d_name) + "/");

                        else if(!local_is_content_separated && std::string(local_p_entry->d_name) != "." && std::string(local_p_entry->d_name) != "..")
                            p_folder_contents->push_back(std::string(folder_name + local_p_entry->d_name) + "/");
                        
                        break;

                    default:
                        break;
                    }
                    break;
                }

                case DENG_FOLDER_CONTENTS_READ_MODE_RECURSIVE: {
                    this->searchFilesRecursively(folder_name, p_folder_contents, p_dir);
                    local_end_loop = DENG_TRUE;
                    break;
                }

                default:
                    break;

                }
                if(local_end_loop) break;
            }

            // sort all the files in specified order
            switch (folder_contents_sort_mode)
            {
            case DENG_FOLDER_CONTENTS_SORT_STYLE_ALL_ALPHABETICAL:
                this->sortAlphabetically(p_folder_contents, DENG_FALSE);
                break;

            case DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FOLDERS_FIRST:
                this->sortAlphabetically(&local_subfolder_names, DENG_FALSE);
                this->sortAlphabetically(&local_file_names, DENG_FALSE);
                p_folder_contents->reserve(local_subfolder_names.size() + local_file_names.size());
                p_folder_contents->insert(p_folder_contents->end(), local_subfolder_names.begin(), local_subfolder_names.end());
                p_folder_contents->insert(p_folder_contents->end(), local_file_names.begin(), local_file_names.end());
                break;

            case DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FILES_FIRST:
                this->sortAlphabetically(&local_file_names, DENG_FALSE);
                this->sortAlphabetically(&local_subfolder_names, DENG_FALSE);
                p_folder_contents->reserve(local_file_names.size() + local_subfolder_names.size());
                p_folder_contents->insert(p_folder_contents->end(), local_file_names.begin(), local_file_names.end());
                p_folder_contents->insert(p_folder_contents->end(), local_subfolder_names.begin(), local_subfolder_names.end());
                break;

            case DENG_FOLDER_CONTENTS_SORT_STYLE_ALL_REVERSE_ALPHABETICAL:
                this->sortAlphabetically(p_folder_contents, DENG_TRUE);
                break;

            case DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FOLDERS_FIRST:
                this->sortAlphabetically(&local_subfolder_names, DENG_TRUE);
                this->sortAlphabetically(&local_file_names, DENG_TRUE);
                p_folder_contents->reserve(local_subfolder_names.size() + local_file_names.size());
                p_folder_contents->insert(p_folder_contents->end(), local_subfolder_names.begin(), local_subfolder_names.end());
                p_folder_contents->insert(p_folder_contents->end(), local_file_names.begin(), local_file_names.end());
                break;

            case DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FILES_FIRST:
                this->sortAlphabetically(&local_file_names, DENG_TRUE);
                this->sortAlphabetically(&local_subfolder_names, DENG_TRUE);
                p_folder_contents->reserve(local_file_names.size() + local_subfolder_names.size());
                p_folder_contents->insert(p_folder_contents->end(), local_file_names.begin(), local_file_names.end());
                p_folder_contents->insert(p_folder_contents->end(), local_subfolder_names.begin(), local_subfolder_names.end());
                break;

            default:
                break;
            }
        }

        else ERRME("Directory '" + folder_name + " not found!");
    }

    void FileManager::getBounds(std::vector<dengMath::vec2<dengMath::vec2<size_t>>> *p_bounds_data, const dengMath::vec2<std::string> &bound_strings, std::vector<std::string> *p_file_contents) {
        size_t local_char_index = 0;
        size_t local_line_index = 0;
        dengBool local_is_bounded;
        dengMath::vec2<dengMath::vec2<size_t>> local_bounds_buffer;

        while(local_line_index < p_file_contents->size()) {
            local_bounds_buffer = {{0, 0}, {0, 0}};
            local_is_bounded = DENG_FALSE;
            for(; local_line_index < p_file_contents->size(); local_line_index++) {
                if(static_cast<int>(local_char_index = (*p_file_contents)[local_line_index].find(bound_strings.first)) != -1) {
                    local_bounds_buffer.first = {local_line_index, local_char_index};
                    local_is_bounded = DENG_TRUE;
                    break;
                }
            }

            for(; local_line_index < p_file_contents->size(); local_line_index++) {
                if(static_cast<int>(local_char_index = (*p_file_contents)[local_line_index].find(bound_strings.second)) != -1) {
                    local_bounds_buffer.second = {local_line_index, local_char_index};
                    break;
                }
            }
            if(local_is_bounded) p_bounds_data->push_back(local_bounds_buffer);
            if(local_bounds_buffer.first.first == local_bounds_buffer.second.first) local_line_index++;
        }
    }

    dengBool FileManager::isBetweenComments(std::vector<dengMath::vec2<dengMath::vec2<size_t>>> *p_comment_bounds, const dengMath::vec2<dengMath::vec2<size_t>> &attribute_boundaries) {
        dengBool local_return_value = DENG_FALSE;

        for(size_t i = 0; i < p_comment_bounds->size(); i++) {
            if((*p_comment_bounds)[i].first.first <= attribute_boundaries.first.first && (*p_comment_bounds)[i].second.first >= attribute_boundaries.second.first &&
            (*p_comment_bounds)[i].first.second <= attribute_boundaries.first.second && (*p_comment_bounds)[i].second.second >= attribute_boundaries.second.second) {
                local_return_value = DENG_TRUE;
            } 
        }

        return local_return_value;
    }

    dengBool FileManager::getCSSProperyValue(const std::string &css_class, const std::string &attribute, const std::string &file_path, std::vector<std::string> *p_values_data) {
        std::vector<std::string> local_file_contents;
        size_t local_char_index = 0;
        this->getFileContents(file_path, nullptr, &local_file_contents);
        
        std::string local_buffer_string;
        size_t local_bounds_index = 0;
        std::vector<dengMath::vec2<dengMath::vec2<size_t>>> local_class_bounds;
        
        this->getBounds(&local_class_bounds, {css_class, std::string("}")}, &local_file_contents);
        
        if(local_class_bounds.size() > 1) local_bounds_index = local_class_bounds.size() - 1;
        else if(local_class_bounds.size() == 0) {
            return DENG_FALSE;
        }

        std::vector<dengMath::vec2<dengMath::vec2<size_t>>> local_comment_bounds; 
        
        for(size_t i = local_class_bounds[local_bounds_index].first.first; i <= local_class_bounds[local_bounds_index].second.first; i++) {
            if(static_cast<int>(local_file_contents[i].find(attribute)) != -1 && (local_file_contents[i].find(attribute) == 0 || local_file_contents[i][local_file_contents[i].find(attribute) - 1] == ' ') && (local_file_contents[i][local_file_contents[i].find(attribute) + attribute.size()] == ' ' || local_file_contents[i][local_file_contents[i].find(attribute) + attribute.size()] == ':') && static_cast<int>(local_char_index = local_file_contents[i].find(":")) != -1 && !this->isBetweenComments(&local_comment_bounds, local_class_bounds[local_bounds_index])) {
                local_char_index++;
                while(local_char_index < local_file_contents[i].size() && local_file_contents[i][local_char_index] == ' ') local_char_index++;
                for(; local_char_index < local_file_contents[i].size(); local_char_index++) {
                    if(local_file_contents[i][local_char_index] != ';')
                        local_buffer_string += local_file_contents[i][local_char_index];

                    else {
                        p_values_data->push_back(local_buffer_string);
                        local_buffer_string.clear();
                        break;
                    }
                }
            }
        }

        if(p_values_data->size() != 0) return DENG_TRUE;
        return DENG_FALSE; 
    }
}