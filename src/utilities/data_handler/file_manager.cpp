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

    void FileManager::writeToFile(const std::string &file_name, const std::string &line_contents, const uint32_t &write_mode) {

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
        LOG("Extension is: " + file_extension);
        for(size_t i = 0; i < p_file_name_data->size();) {
            if((*p_file_name_data)[i].find(file_extension) == (*p_file_name_data)[i].size() - file_extension.size())
                i++;
            
            else 
                p_file_name_data->erase(p_file_name_data->begin() + i);
        }
    }

    void FileManager::sortAlphabetically(std::vector<std::string> *p_sortable_data, const dengBool &is_reverse_alphabet) {
        if(p_sortable_data->size() != 0) {
            dengMath::vec2<std::string> local_copy_buffer;
            dengBool local_is_pair_sorted = DENG_FALSE;
            dengBool local_is_list_sorted = DENG_FALSE;
            
            switch (is_reverse_alphabet)
            {
            case DENG_FALSE:
                while(!local_is_list_sorted) {
                    for(size_t content_index = 0; content_index < p_sortable_data->size() - 1; content_index++) {
                        
                        for(size_t str_index = 0; str_index < p_sortable_data->at(content_index).size() && str_index < p_sortable_data->at(content_index + 1).size(); str_index++) {
                            local_is_list_sorted = DENG_TRUE;
                            if(static_cast<int>((*p_sortable_data)[content_index][str_index]) > static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) {
                                local_is_list_sorted = DENG_FALSE;
                                local_copy_buffer.first = (*p_sortable_data)[content_index + 1];
                                local_copy_buffer.second = (*p_sortable_data)[content_index];

                                (*p_sortable_data)[content_index] = local_copy_buffer.first;
                                (*p_sortable_data)[content_index + 1] = local_copy_buffer.second;
                                local_is_pair_sorted = DENG_TRUE;
                                break;
                            }

                            else if(static_cast<int>((*p_sortable_data)[content_index][str_index]) < static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) break;
                        }

                        if(!local_is_pair_sorted && p_sortable_data->at(content_index).size() > p_sortable_data->at(content_index + 1).size()) {
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
                    for(size_t content_index = 0; content_index < p_sortable_data->size() - 1; content_index++) {
                        
                        for(size_t str_index = 0; str_index < p_sortable_data->at(content_index).size() && str_index < p_sortable_data->at(content_index + 1).size(); str_index++) {
                            local_is_list_sorted = DENG_TRUE;
                            if(static_cast<int>((*p_sortable_data)[content_index][str_index]) < static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) {
                                local_is_list_sorted = DENG_FALSE;
                                ERRME("first element is bigger than second one!");
                                local_copy_buffer.first = (*p_sortable_data)[content_index + 1];
                                local_copy_buffer.second = (*p_sortable_data)[content_index];

                                (*p_sortable_data)[content_index] = local_copy_buffer.first;
                                (*p_sortable_data)[content_index + 1] = local_copy_buffer.second;
                                local_is_pair_sorted = DENG_TRUE;
                                break;
                            }

                            else if(static_cast<int>((*p_sortable_data)[content_index][str_index]) > static_cast<int>((*p_sortable_data)[content_index + 1][str_index])) break;
                        }

                        if(!local_is_pair_sorted && p_sortable_data->at(content_index).size() < p_sortable_data->at(content_index + 1).size()) {
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

    void FileManager::getFolderContents(const std::string &folder_name, std::vector<std::string> *p_folder_contents, const dengFolderContentsReadMode &folder_contents_read_mode, const dengFolderContentsSortingMode &folder_contents_sort_mode) {
        std::vector<std::string> local_subfolder_names;
        std::vector<std::string> local_file_names;
        
        DIR *local_dir;
        local_dir = opendir(folder_name.c_str());
        
        if(local_dir != nullptr) {
            struct dirent *local_entry;
            while((local_entry = readdir(local_dir)) != NULL) {                
                switch (folder_contents_read_mode)
                {
                case DENG_FOLDER_CONTENTS_READ_MODE_FILES_ONLY:
                    if(local_entry->d_type == DT_REG) p_folder_contents->push_back(std::string(local_entry->d_name));
                    break;

                case DENG_FOLDER_CONTENTS_READ_MODE_FOLDERS_ONLY:
                    if(local_entry->d_type == DT_DIR && std::string(local_entry->d_name) != std::string(".") && std::string(local_entry->d_name) != std::string("..")) p_folder_contents->push_back(std::string(local_entry->d_name) + "/");
                    break;

                case DENG_FOLDER_CONTENTS_READ_MODE_FILES_AND_FOLDERS:
                    if(folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FOLDERS_FIRST || folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_ALPHABETICAL_FILES_FIRST ||
                    folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FILES_FIRST || folder_contents_sort_mode == DENG_FOLDER_CONTENTS_SORT_STYLE_REVERSE_ALPHABETICAL_FOLDERS_FIRST) {
                        if(local_entry->d_type == DT_REG) local_file_names.push_back(std::string(local_entry->d_name));
                        else if(local_entry->d_type == DT_DIR && std::string(local_entry->d_name) != std::string(".") && std::string(local_entry->d_name) != std::string("..")) local_subfolder_names.push_back(std::string(local_entry->d_name) + "/");
                    } 

                    else {
                        if(local_entry->d_type == DT_REG) p_folder_contents->push_back(std::string(local_entry->d_name));
                        else if(local_entry->d_type == DT_DIR && std::string(local_entry->d_name) != std::string(".") && std::string(local_entry->d_name) != std::string("..")) p_folder_contents->push_back(std::string(local_entry->d_name) + "/");
                    }
                    break;

                default:
                    break;
                }
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
        else {
            ERRME("Directory '" + folder_name + "' not found!");
        }
    }
}