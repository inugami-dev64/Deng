#include "headers/renderer.h"

namespace Deng {
    ObjLoader::ObjLoader(const std::string &fileName, const bool &reverseCoordinates) {
        FileManager fm;
        this->reverseCoordinates = reverseCoordinates;
        std::vector<std::string> contents;
        fm.getFileContents(fileName, nullptr, &contents);
        
        this->getVertexFaces(contents);
        this->getVerticesCoord(contents);
    }

    void ObjLoader::getVerticesCoord(const std::vector<std::string> &fileContents) {
        uint32_t coordVecType;

        for(size_t i = 0; i < fileContents.size(); i++) {
            bool readCoords;
            size_t firstCoordIndex;

            if(fileContents[i].find("v ") == 0) readCoords = true, coordVecType = DENG_VERTEX_COORD, firstCoordIndex = 2;
            else if(fileContents[i].find("vn ") == 0) readCoords = true, coordVecType = DENG_VERTEX_NORMAL_COORD, firstCoordIndex = 3;
            else if(fileContents[i].find("vt ") == 0) readCoords = true, coordVecType = DENG_VERTEX_TEXTURE_COORD, firstCoordIndex = 3;
            else readCoords = false;

            if(readCoords) {

                char buffer[fileContents[i].size()];
                strncpy(buffer, fileContents[i].c_str(), fileContents[i].size());

                uint32_t coordinateType = DENG_X;
                std::string x_str;
                std::string y_str;
                std::string z_str;
                
                for(size_t readIndex = firstCoordIndex; readIndex < (sizeof(buffer)/sizeof(buffer[0])); readIndex++) {
                    switch (coordinateType)
                    {
                    case DENG_X:
                        if(buffer[readIndex] != ' ') x_str += buffer[readIndex];
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
                    switch (coordVecType)
                    {
                    case DENG_VERTEX_COORD:

                        if(this->reverseCoordinates) this->vertexCoordVec.push_back({-(::atof(x_str.c_str())), -(::atof(y_str.c_str())), -(::atof(z_str.c_str()))});    
                        else this->vertexCoordVec.push_back({::atof(x_str.c_str()), ::atof(y_str.c_str()), ::atof(z_str.c_str())});

                        break;

                    case DENG_VERTEX_NORMAL_COORD:
                        if(this->reverseCoordinates) this->vertexNormCoordVec.push_back({-(::atof(x_str.c_str())), -(::atof(y_str.c_str())), -(::atof(z_str.c_str()))});
                        else this->vertexNormCoordVec.push_back({::atof(x_str.c_str()), ::atof(y_str.c_str()), ::atof(z_str.c_str())});
                        
                        break;

                    case DENG_VERTEX_TEXTURE_COORD:
                        if(this->reverseCoordinates) this->vertexTexCoordVec.push_back({-(::atof(x_str.c_str())), -(::atof(y_str.c_str()))});
                        else this->vertexTexCoordVec.push_back({::atof(x_str.c_str()), ::atof(y_str.c_str())});

                        break;

                    default:
                        break;
                    }
                }

                catch(std::exception &e) {
                    ERRME("Couldn't read vertices properly because file is corrupted!");
                }
            }
        }
    }

    void ObjLoader::getVertexFaces(const std::vector<std::string> &objContents) {
        for(size_t i = 0; i < objContents.size(); i++) {
            
            if(objContents[i].find("f ") == 0) {
                char buffer[objContents[i].size()];
                strncpy(buffer, objContents[i].c_str(), objContents[i].size());
                uint32_t vertexTypeCount = DENG_VERTEX_COORD;
                uint32_t vertexCount = 0;

                std::string iStr;
                vec3<int> vertexTriangleVector;
                vec3<int> vertexNormalTriangleVector;
                vec3<int> vertexTextureTriangleVector;

                for(size_t lineIndex = 2; lineIndex < sizeof(buffer)/sizeof(buffer[0]); lineIndex++) {
                    
                    if(buffer[lineIndex] != '/' && buffer[lineIndex] != ' ') iStr += buffer[lineIndex];
                    else {
                        switch (vertexTypeCount)
                        {
                        case DENG_VERTEX_COORD:
                            if(vertexCount == DENG_X) vertexTriangleVector.x = (std::stoi(iStr) - 1);
                            else if(vertexCount == DENG_Y) vertexTriangleVector.y = (std::stoi(iStr) - 1);
                            else if(vertexCount == DENG_Z) vertexTriangleVector.z = (std::stoi(iStr) - 1);
                            iStr.clear();
                            vertexTypeCount++;
                            break;

                        case DENG_VERTEX_NORMAL_COORD:
                            if(vertexCount == DENG_X) vertexNormalTriangleVector.x = (std::stoi(iStr) - 1);
                            else if(vertexCount == DENG_Y) vertexNormalTriangleVector.y = (std::stoi(iStr) - 1);
                            else if(vertexCount == DENG_Z) vertexNormalTriangleVector.z = (std::stoi(iStr) - 1);
                            iStr.clear();
                            vertexTypeCount++;
                            break;

                        case DENG_VERTEX_TEXTURE_COORD:
                            if(vertexCount == DENG_X) vertexTextureTriangleVector.x = (std::stoi(iStr) - 1);
                            else if(vertexCount == DENG_Y) vertexTextureTriangleVector.y = (std::stoi(iStr) - 1);
                            else if(vertexCount == DENG_Z) vertexTextureTriangleVector.z = (std::stoi(iStr) - 1);
                            iStr.clear();
                            vertexTypeCount = DENG_VERTEX_COORD;
                            vertexCount++;
                            break;
                    
                        default:
                            break;
                        }
                    }
                }

                this->vertexCoordFacesVec.push_back(vertexTriangleVector);
                this->vertexNormCoordFacesVec.push_back(vertexNormalTriangleVector); 
                this->vertexTexCoordFacesVec.push_back(vertexTextureTriangleVector);
            }
        }
    }

    void ObjLoader::getObjMatrices(GameObject &obj) {
        obj.vertexData.resize(this->vertexCoordFacesVec.size() * 3);

        
        for(size_t i = 0; i < vertexCoordFacesVec.size(); i++) {
            for(size_t ii = 0; ii < 3; ii++) {
                vec3<float> posTemp;
                vec2<float> texTemp;
                switch (ii)
                {
                case DENG_X:
                    posTemp = this->vertexCoordVec[this->vertexCoordFacesVec[i].x];
                    texTemp = this->vertexTexCoordVec[this->vertexTexCoordFacesVec[i].x];
                    break;
                
                case DENG_Y:
                    posTemp = this->vertexCoordVec[this->vertexCoordFacesVec[i].y];
                    texTemp = this->vertexTexCoordVec[this->vertexTexCoordFacesVec[i].y];
                    break;

                case DENG_Z:
                    posTemp = this->vertexCoordVec[this->vertexCoordFacesVec[i].z];
                    texTemp = this->vertexTexCoordVec[this->vertexTexCoordFacesVec[i].z];
                    break;

                default:
                    break;
                }

                ObjVertexData vertexData;
                vertexData.posVec = posTemp;
                vertexData.textureVec = texTemp;

                obj.vertexData.push_back(vertexData);
            }
        }
    }
}