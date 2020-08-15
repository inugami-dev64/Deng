#ifndef FILES_H
#define FILES_H

#include "swapchaindetails.h"

namespace Deng {

    enum coordinateTypes {
        DENG_X = 0,
        DENG_Y = 1,
        DENG_Z = 2
    };

    enum writeModes {
        DENG_WRITEMODE_REWRITE = 0,
        DENG_WRITEMODE_FROM_END = 1
    };

    class FileManager {
        private:
            std::ifstream *fileIn;
            std::ofstream *fileOut;

        public:
            void getFileContents(const std::string &fileName, std::vector<char> *charOutputVector, std::vector<std::string> *stringOutputVector);
            void writeToFile(const std::string &fileName, const std::string &lineContents, const uint32_t &writeMode);
    };
}

#endif