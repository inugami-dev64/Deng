#ifndef FILES_H
#define FILES_H

#include "swapchaindetails.h"

namespace deng {

    enum writeModes {
        DENG_WRITEMODE_REWRITE = 0,
        DENG_WRITEMODE_FROM_END = 1
    };

    class FileManager {
    private:
        std::ifstream *fileIn;
        std::ofstream *fileOut;

    public:
        void getFileContents(const std::string &file_name, std::vector<char> *charOutputVector, std::vector<std::string> *stringOutputVector);
        void writeToFile(const std::string &fileName, const std::string &lineContents, const uint32_t &writeMode);

        template<typename T>
        T getConfVal(const std::string &conf, const std::string &fileName) {
            std::vector<std::string> fileContents;
            this->getFileContents(fileName, nullptr, &fileContents);
            int32_t lineIndex = 0;

            for(std::string &line : fileContents) {
                if(line.find(conf) == 0) {
                    std::string valStr;
                    int8_t index;

                    if(static_cast<int>(line.find("#")) == -1 && static_cast<int>(line.find("=")) == -1) {
                        ERR(DENG_CONF_PARSING_ERROR_MSG(fileName) + DENG_GENERAL_PARSING_ERROR_LINE_MSG(std::to_string(lineIndex)));
                    }

                    else {
                        index = static_cast<int8_t>(line.find("=")) + 1;
                        while(line[index] == ' ' || line[index] == '"') index++;

                        for(; index < line.size() && line[index] != ' ' && line[index] != '#' && line[index] != '"'; index++) {
                            valStr += line[index];
                        }
                    }

                    T i;
                    LOG("ValStr: \"" + valStr + "\"");
                    if(typeid(i).name() == typeid(bool).name()) {
                        if(valStr == "true") return true;
                        else if(valStr == "false") return false; 
                    }

                    else if(typeid(i).name() == typeid(float).name()) {
                        return std::stof(valStr);
                    }

                    else if(typeid(i).name() == typeid(int).name()) {
                        return std::stoi(valStr);
                    }                       
                }
                lineIndex++;
            }
            T i;
            return i;
        }
    };

    struct EditorEnvironmentConf {
        float environment_color_r;
        float environment_color_g;
        float environment_color_b;

        DengBool show_grid;
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
}

#endif