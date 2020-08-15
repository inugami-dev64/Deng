#include "headers/renderer.h"

namespace Deng {

    void FileManager::getFileContents(const std::string &fileName, std::vector<char> *charOutputVector, std::vector<std::string> *stringOutputVector) {
        this->fileIn = new std::ifstream(fileName, std::ios::ate | std::ios::binary);
        size_t fileSize = (size_t) this->fileIn->tellg();
        this->fileIn->seekg(0);

        if(!this->fileIn->is_open()) {
            ERRME("Failed to load file '" + fileName + "'!");
        }

        if(charOutputVector != nullptr) {
            charOutputVector->resize(fileSize);
            this->fileIn->read(charOutputVector->data(), fileSize);
            this->fileIn->close();
        }

        if(stringOutputVector != nullptr) {
            std::string buffer;
            while(std::getline(*this->fileIn, buffer)) stringOutputVector->push_back(buffer);
        }

        delete this->fileIn;
    }

    void FileManager::writeToFile(const std::string &fileName, const std::string &lineContents, const uint32_t &writeMode) {

        switch (writeMode)
        {
        case DENG_WRITEMODE_REWRITE:
            this->fileOut = new std::ofstream(fileName);
            break;

        case DENG_WRITEMODE_FROM_END:
            this->fileOut = new std::ofstream(fileName, std::ios::app);
            break;
        
        default:
            break;
        }
        
        *this->fileOut << lineContents << "\n";
        this->fileOut->close();
    }
}