#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "textureloader.h"

namespace deng {
    
    enum CoordinateMode {
        DENG_COORDINATE_MODE_DEFAULT = 0,
        DENG_COORDINATE_MODE_REVERSE = 1
    };

    enum coordTypes {
        DENG_VERTEX_COORD = 0,
        DENG_VERTEX_TEXTURE_COORD = 1,
        DENG_VERTEX_NORMAL_COORD = 2
    };

    class ObjLoader {
    private:
        CoordinateMode reverseCoordinates;
        std::vector<vec3<float>> vertexCoordVec;
        std::vector<vec3<float>> vertexNormCoordVec;
        std::vector<vec2<float>> vertexTexCoordVec;

        std::vector<uint32_t> vertexCoordFacesVec;
        std::vector<uint32_t> vertexTexCoordFacesVec;
        std::vector<uint32_t> vertexNormCoordFacesVec; 

    private:
        void getVerticesCoord(const std::vector<std::string> &objContents);
        void getVertexFaces(const std::vector<std::string> &objContents);

    public:
        ObjLoader(const std::string &fileName, const CoordinateMode &reverseCoordinates);
        void getObjVerticesAndIndices(GameObject &obj);
    };
}

#endif