#ifndef FILES_H
#define FILES_H

#include "swapchaindetails.h"

enum coordinateTypes {
    DENG_X = 0,
    DENG_Y = 1,
    DENG_Z = 2
};

namespace Deng {

    class FileManager {
        private:
            std::ifstream *file;

            class ObjHandler
            {
                public:
                    std::vector<vec3> getVertices(const std::vector<std::string> &objContents);
            };

        public:
            void getFileContents(const std::string &fileName, std::vector<char> *charOutputVector, std::vector<std::string> *stringOutputVector);
            ObjHandler objHandle;
    };
}

#endif