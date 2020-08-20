#include "headers/renderer.h"

namespace deng {
    ObjLoader::ObjLoader(const std::string &fileName, const CoordinateMode &coordinateMode) {
        FileManager fm;
        this->reverseCoordinates = coordinateMode;
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

                std::string iStr;

                for(size_t lineIndex = 2; lineIndex < sizeof(buffer)/sizeof(buffer[0]); lineIndex++) {
                    
                    if(buffer[lineIndex] != '/' && buffer[lineIndex] != ' ') iStr += buffer[lineIndex];
                    else {
                        switch (vertexTypeCount)
                        {
                        case DENG_VERTEX_COORD:
                            this->vertexCoordFacesVec.push_back(std::stoi(iStr) - 1);
                            iStr.clear();
                            vertexTypeCount++;
                            break;

                        case DENG_VERTEX_NORMAL_COORD:
                            this->vertexNormCoordFacesVec.push_back(std::stoi(iStr) - 1);
                            iStr.clear();
                            vertexTypeCount++;
                            break;

                        case DENG_VERTEX_TEXTURE_COORD:
                            this->vertexTexCoordFacesVec.push_back(std::stoi(iStr) - 1);
                            iStr.clear();
                            vertexTypeCount = DENG_VERTEX_COORD;
                            break;
                    
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    void ObjLoader::getObjVerticesAndIndices(GameObject &obj) {
        obj.vertexData.resize(this->vertexCoordVec.size());
        obj.vertexIndicesData.texIndices.resize(this->vertexTexCoordFacesVec.size());
        obj.vertexIndicesData.posIndices.resize(this->vertexCoordFacesVec.size());
        
        for(size_t i = 0; i < this->vertexCoordVec.size(); i++) {
            obj.vertexData[i].posVec = this->vertexCoordVec[i];
            obj.vertexData[i].colorVec = {1.0f, 1.0f, 1.0f, 1.0f};
        }

        for(size_t i = 0; i < this->vertexCoordFacesVec.size(); i++) {
            obj.vertexIndicesData.posIndices[i] = this->vertexCoordFacesVec[i];
        }

        for(size_t i = 0; i < this->vertexTexCoordFacesVec.size(); i++) {
            obj.vertexIndicesData.texIndices[i] = this->vertexTexCoordFacesVec[i];
        }
    }
}