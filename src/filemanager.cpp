#include "headers/renderer.h"

namespace Deng {

    std::vector<char> FileManager::getFileContents(const std::string &fileName) {
        this->file = new std::ifstream(fileName, std::ios::ate | std::ios::binary);
        std::vector<char> contents;

        if(!this->file->is_open()) {
            LOG("Failed to load file");
        }
        else {
            size_t fileSize = (size_t) this->file->tellg();
            contents.resize(fileSize);
            this->file->seekg(0);
            this->file->read(contents.data(), fileSize);
            this->file->close();

            delete this->file;
        }

        return contents;
    }
}