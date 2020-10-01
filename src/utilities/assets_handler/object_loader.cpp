#include "../../core/deng_core.h"

namespace dengUtils {
    ObjLoader::ObjLoader(const std::string &file_name, const dengCoordinateMode &coordinate_mode) {
        FileManager local_fm;
        this->reverse_coordinates = coordinate_mode;
        std::vector<std::string> local_contents;
        local_fm.getFileContents(file_name, nullptr, &local_contents);
        
        this->getVerticesCoord(local_contents);
        this->getVertexFaces(local_contents);
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

                char local_buffer[fileContents[i].size()];
                strncpy(local_buffer, fileContents[i].c_str(), fileContents[i].size());

                uint32_t coordinateType = DENG_X;
                std::string x_str;
                std::string y_str;
                std::string z_str;
                
                for(size_t readIndex = firstCoordIndex; readIndex < (sizeof(local_buffer)/sizeof(local_buffer[0])); readIndex++) {\
                    switch (coordinateType)
                    {
                    case DENG_X:
                        if(local_buffer[readIndex] != ' ') x_str += local_buffer[readIndex];
                        else coordinateType++;
                        break;

                    case DENG_Y:
                        if(local_buffer[readIndex] != ' ') y_str += local_buffer[readIndex];
                        else if(readIndex == (sizeof(local_buffer)/sizeof(local_buffer[0])) - 1) coordinateType = DENG_X;
                        else coordinateType++;
                        
                        break;
                    
                    case DENG_Z:
                        z_str += local_buffer[readIndex]; 
                        break;

                    default:
                        break;
                    }
                } 

                try {
                    switch (coordVecType)
                    {
                    case DENG_VERTEX_COORD:

                        if(this->reverse_coordinates) this->vertex_coord_vector.push_back({-(std::stof(x_str.c_str())), -(std::stof(y_str.c_str())), -(std::stof(z_str.c_str()))});    
                        else this->vertex_coord_vector.push_back({std::stof(x_str.c_str()), std::stof(y_str.c_str()), std::stof(z_str.c_str())});

                        break;

                    case DENG_VERTEX_TEXTURE_COORD:
                        if(this->reverse_coordinates) this->vertex_texture_coord_vector.push_back({-(std::stof(x_str.c_str())), -(std::stof(y_str.c_str()))});
                        else this->vertex_texture_coord_vector.push_back({std::stof(x_str.c_str()), std::stof(y_str.c_str())});

                        break;

                    case DENG_VERTEX_NORMAL_COORD:
                        if(this->reverse_coordinates) this->vertex_normal_coord_vector.push_back({-(std::stof(x_str.c_str())), -(std::stof(y_str.c_str())), -(std::stof(z_str.c_str()))});
                        else this->vertex_normal_coord_vector.push_back({std::stof(x_str.c_str()), std::stof(y_str.c_str()), std::stof(z_str.c_str())});
                        
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

    void ObjLoader::getVertexFaces(const std::vector<std::string> &obj_contents) {
        for(size_t i = 0; i < obj_contents.size(); i++) {
            
            if(obj_contents[i].find("f ") == 0) {
                char buffer[obj_contents[i].size()];
                strncpy(buffer, obj_contents[i].c_str(), obj_contents[i].size());
                dengCoordinateType local_vertex_type_count = DENG_VERTEX_COORD;

                std::string iStr;

                for(size_t lineIndex = 2; lineIndex < sizeof(buffer)/sizeof(buffer[0]); lineIndex++) {
                    
                    if(buffer[lineIndex] != '/' && buffer[lineIndex] != ' ') iStr += buffer[lineIndex];
                    else {
                        switch (local_vertex_type_count)
                        {
                        case DENG_VERTEX_COORD:
                            this->vertex_coord_faces_vector.push_back(std::stoi(iStr) - 1);
                            iStr.clear();
                            local_vertex_type_count = DENG_VERTEX_TEXTURE_COORD;
                            break;

                        case DENG_VERTEX_TEXTURE_COORD:
                            this->vertex_texture_coord_faces_vector.push_back(std::stoi(iStr) - 1);
                            iStr.clear();
                            local_vertex_type_count = DENG_VERTEX_NORMAL_COORD;
                            break;

                        case DENG_VERTEX_NORMAL_COORD:
                            this->vertex_normal_coord_faces_vector.push_back(std::stoi(iStr) - 1);
                            iStr.clear();
                            local_vertex_type_count = DENG_VERTEX_COORD;
                            break;
                    
                        default:
                            break;
                        }
                    }

                    if(lineIndex == (sizeof(buffer)/sizeof(buffer[0]) - 1)) {
                        // iStr += buffer[lineIndex];
                        this->vertex_normal_coord_faces_vector.push_back(std::stoi(iStr) - 1);
                        iStr.clear();
                        local_vertex_type_count = DENG_VERTEX_COORD;
                    }
                }
            }
        }
    }

    void ObjLoader::getObjVerticesAndIndices(GameObject &obj) {
        obj.vertex_data.resize(this->vertex_coord_faces_vector.size());

        if(this->vertex_coord_faces_vector.size() != this->vertex_texture_coord_faces_vector.size()) {
            ERR("Corrupted .obj file!");
        }

        // obj.vertex_indices_data.texture_indices.resize(this->vertex_texture_coord_faces_vector.size());
        // obj.vertex_indices_data.position_indices.resize(this->vertex_coord_faces_vector.size());
        
        for(size_t i = 0; i < this->vertex_coord_faces_vector.size(); i++) {
            obj.vertex_data[i].position_vec = {this->vertex_coord_vector[this->vertex_coord_faces_vector[i]].first + obj.origin.first, this->vertex_coord_vector[this->vertex_coord_faces_vector[i]].second + obj.origin.second, this->vertex_coord_vector[this->vertex_coord_faces_vector[i]].third + obj.origin.third};
            obj.vertex_data[i].texture_vec = {this->vertex_texture_coord_vector[this->vertex_texture_coord_faces_vector[i]].first, 1.0f - this->vertex_texture_coord_vector[this->vertex_texture_coord_faces_vector[i]].second};
        }
    }
}