#include "headers/renderer.h"

namespace Deng {

    void FileManager::getFileContents(const std::string &fileName, std::vector<char> *charOutputVector, std::vector<std::string> *stringOutputVector) {
        this->file = new std::ifstream(fileName, std::ios::ate | std::ios::binary);
        size_t fileSize = (size_t) this->file->tellg();
        this->file->seekg(0);

        if(!this->file->is_open()) {
            ERRME("Failed to load file '" + fileName + "'!");
        }

        if(charOutputVector != nullptr) {
            charOutputVector->resize(fileSize);
            this->file->read(charOutputVector->data(), fileSize);
            this->file->close();
        }

        if(stringOutputVector != nullptr) {
            stringOutputVector->resize(fileSize);
            std::string buffer;
            while(std::getline(*this->file, buffer)) stringOutputVector->push_back(buffer);
        }

        delete this->file;
    }

    std::vector<vec3> FileManager::ObjHandler::getVertices(const std::vector<std::string> &fileContents) {
        std::vector<vec3> vertices; 

        for(size_t i = 0; i < fileContents.size(); i++) {
            if(fileContents[i].find("v") == 0) {
                char buffer[fileContents[i].size()];
                strncpy(buffer, fileContents[i].c_str(), fileContents[i].size());

                uint32_t coordinateType = DENG_X;
                std::string x_str;
                std::string y_str;
                std::string z_str;

                for(size_t readIndex = 2; readIndex < (sizeof(buffer)/sizeof(buffer[0])); readIndex++) {
                    switch (coordinateType)
                    {
                        case DENG_X:
                            if(buffer[readIndex] != ' ') {
                                x_str += buffer[readIndex];
                            }
                            else coordinateType++;
                            break;

                        case DENG_Y:
                            if(buffer[readIndex] != ' ') y_str += buffer[readIndex];
                            else coordinateType++;
                            break;
                        
                        case DENG_Z:
                            z_str += buffer[readIndex]; 
                            break;

                        default:
                            break;
                    }
                } 

                try {
                    vertices.push_back({::atof(x_str.c_str()), ::atof(y_str.c_str()), ::atof(z_str.c_str())});
                }

                catch(std::exception &e) {
                    ERRME("Couldn't read vertices properly because file is corrupted!");
                }
            }
        }

        return vertices;
    }
}