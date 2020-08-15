#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "objectshandler.h"

namespace Deng {
    
    enum coordTypes {
        DENG_VERTEX_COORD = 0,
        DENG_VERTEX_NORMAL_COORD = 1,
        DENG_VERTEX_TEXTURE_COORD = 2
    };

    class ObjLoader {
        private:
            bool reverseCoordinates;
            std::vector<vec3<float>> vertexCoordVec;
            std::vector<vec3<float>> vertexNormCoordVec;
            std::vector<vec2<float>> vertexTexCoordVec;

            std::vector<vec3<int>> vertexCoordFacesVec;
            std::vector<vec3<int>> vertexTexCoordFacesVec;
            std::vector<vec3<int>> vertexNormCoordFacesVec; 

        private:
            void getVerticesCoord(const std::vector<std::string> &objContents);
            void getVertexFaces(const std::vector<std::string> &objContents);

        public:
            ObjLoader(const std::string &fileName, const bool &reverseCoordinates);
            void getObjMatrices(GameObject &obj);
    };
}

#endif