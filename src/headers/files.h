#ifndef FILES_H
#define FILES_H

#include "swapchaindetails.h"

namespace Deng {
    class FileManager {
        private:
            std::ifstream *file;

        public:
            std::vector<char> getFileContents(const std::string &fileName);
    };
}

#endif