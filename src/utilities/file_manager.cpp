#include "../core/deng_core.h"

namespace deng {

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
}