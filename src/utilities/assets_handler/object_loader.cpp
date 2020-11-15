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

                uint32_t coordinateType = DENG_COORD_AXIS_X;
                std::string x_str;
                std::string y_str;
                std::string z_str;
                
                for(size_t readIndex = firstCoordIndex; readIndex < (sizeof(local_buffer)/sizeof(local_buffer[0])); readIndex++) {\
                    switch (coordinateType)
                    {
                    case DENG_COORD_AXIS_X:
                        if(local_buffer[readIndex] != ' ') x_str += local_buffer[readIndex];
                        else coordinateType++;
                        break;

                    case DENG_COORD_AXIS_Y:
                        if(local_buffer[readIndex] != ' ') y_str += local_buffer[readIndex];
                        else if(readIndex == (sizeof(local_buffer)/sizeof(local_buffer[0])) - 1) coordinateType = DENG_COORD_AXIS_X;
                        else coordinateType++;
                        
                        break;
                    
                    case DENG_COORD_AXIS_Z:
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

    
    size_t ObjLoader::findTextureMappedVertexInstance(GameObject *p_obj, std::vector<TextureMappedVerticesData> &instance) {
        size_t index;
        for(index = 0; index < instance.size(); index++)
            if((*p_obj->p_texture_mapped_vertices_data)[index].position_vec == instance[index].position_vec &&
            (*p_obj->p_texture_mapped_vertices_data)[index].texture_vec == instance[index].texture_vec) return index;

        return (size_t) -1;
    }

    size_t ObjLoader::findUnmappedVertexInstance(GameObject *p_obj, std::vector<UnmappedVerticesData> &instance) {
        size_t index;
        for(index = 0; index < instance.size(); index++)
            if((*p_obj->p_unmapped_vertices_data)[index].position_vec == instance[index].position_vec && 
            (*p_obj->p_unmapped_vertices_data)[index].color_vec == instance[index].color_vec) return index;

        return (size_t) -1;
    }
    
    void ObjLoader::indexVertices(GameObject *p_obj) {
        size_t index;

        switch (p_obj->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED: {
            size_t instance_index;
            std::vector<TextureMappedVerticesData> texture_mapped_data;

            for(index = 0; index < p_obj->p_texture_mapped_vertices_data->size(); index++) {
                if((instance_index = findTextureMappedVertexInstance(p_obj, texture_mapped_data)) == -1) {
                    texture_mapped_data.push_back((*p_obj->p_texture_mapped_vertices_data)[index]);
                    p_obj->indices_data.push_back((uint32_t) index);
                }

                else p_obj->indices_data.push_back((uint32_t) instance_index);
            }

            p_obj->p_texture_mapped_vertices_data->clear();
            *p_obj->p_texture_mapped_vertices_data = texture_mapped_data;
            break;
        }

        case DENG_PIPELINE_TYPE_UNMAPPED: {
            size_t instance_index;
            std::vector<UnmappedVerticesData> unmapped_data;

            for(index = 0; index < p_obj->p_unmapped_vertices_data->size(); index++) {
                if((instance_index = findUnmappedVertexInstance(p_obj, unmapped_data)) == -1) {
                    unmapped_data.push_back((*p_obj->p_unmapped_vertices_data)[index]);
                    p_obj->indices_data.push_back((uint32_t) index);
                }

                else p_obj->indices_data.push_back((uint32_t) instance_index);
            }

            p_obj->p_unmapped_vertices_data->clear();
            *p_obj->p_unmapped_vertices_data = unmapped_data;
            break;
        }
        
        default:
            break;
        }
    }

    void ObjLoader::getObjVertices(GameObject *p_obj) {
        size_t index;

        if(this->vertex_coord_faces_vector.size() != this->vertex_texture_coord_faces_vector.size())
            ERR("Corrupted .obj file!");

        switch (p_obj->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
            p_obj->p_texture_mapped_vertices_data = new std::vector<TextureMappedVerticesData>;
            p_obj->p_texture_mapped_vertices_data->resize(this->vertex_coord_faces_vector.size());
        
            for(index = 0; index < p_obj->p_texture_mapped_vertices_data->size(); index++) {
                (*p_obj->p_texture_mapped_vertices_data)[index].position_vec = {this->vertex_coord_vector[this->vertex_coord_faces_vector[index]].first + p_obj->origin.first,
                this->vertex_coord_vector[this->vertex_coord_faces_vector[index]].second + p_obj->origin.second, 
                this->vertex_coord_vector[this->vertex_coord_faces_vector[index]].third + p_obj->origin.third};

                (*p_obj->p_texture_mapped_vertices_data)[index].texture_vec = {this->vertex_texture_coord_vector[this->vertex_texture_coord_faces_vector[index]].first, 
                this->vertex_texture_coord_vector[this->vertex_texture_coord_faces_vector[index]].second};
            }
            break;
        
        case DENG_PIPELINE_TYPE_UNMAPPED:
            p_obj->p_unmapped_vertices_data = new std::vector<UnmappedVerticesData>;
            p_obj->p_unmapped_vertices_data->resize(this->vertex_coord_faces_vector.size());
        
            for(index = 0; index < p_obj->p_unmapped_vertices_data->size(); index++) {
                (*p_obj->p_unmapped_vertices_data)[index].position_vec = {this->vertex_coord_vector[this->vertex_coord_faces_vector[index]].first,
                this->vertex_coord_vector[this->vertex_coord_faces_vector[index]].second,
                this->vertex_coord_vector[this->vertex_coord_faces_vector[index]].third};

                (*p_obj->p_unmapped_vertices_data)[index].color_vec = {p_obj->p_frag_color->first, p_obj->p_frag_color->second, p_obj->p_frag_color->third};
            }
            break;

        default:
            break;
        }

        this->indexVertices(p_obj);
    }


    ModelHandler::ModelHandler(const BufferDataFuncs &data_funcs, const VkDeviceSize &image_base_offset, const VkDeviceSize &buffer_base_offset) { 
        this->m_data_funcs = data_funcs; 
        this->m_image_base_offset = image_base_offset;
        this->m_buffer_base_offset = buffer_base_offset;
    }

    void ModelHandler::initTextureImage(const size_t &index) {
        
    }

    size_t ModelHandler::findPreviousTexturedObject(const size_t &current_index) {
        for(size_t index = current_index; index <= 0; index--)
            if(this->m_game_models[index].p_raw_texture_data != nullptr) return index;

        return (size_t) -1;
    }

    size_t ModelHandler::loadModel(const std::string &name, const std::string &desciption, const std::string &object_file, const dengMath::vec3<float> &origin, const FragInfo &frag_info) {
        RawTextureData *p_raw_texture_data;
        size_t current_model_index;

        switch (frag_info.pipeline_type)
        {
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED: {
            dengBool is_texture_failiure = DENG_FALSE;
            dengTextureFormat tex_format = getTexFileFormat(frag_info.texture_file);
            p_raw_texture_data = new RawTextureData;

            switch (tex_format)
            {
            case DENG_TEXTURE_FORMAT_BMP: {
                dengUtils::TextureLoaderBMP tex_loader(frag_info.texture_file);
                tex_loader.getTextureDetails(p_raw_texture_data);
                break;
            }

            case DENG_TEXTURE_FORMAT_TGA: {
                dengUtils::TextureLoaderTGA tex_loader(frag_info.texture_file);
                tex_loader.getTextureDetails(p_raw_texture_data);
                break;
            }

            case DENG_TEXTURE_FORMAT_JPG:
                ERRME(".jpg is not supported for texture format!");
                is_texture_failiure = DENG_TRUE;
                break;

            case DENG_TEXTURE_FORMAT_PNG:
                ERRME(".png is not supported for texture format!");
                break;

            case DENG_TEXTURE_FORMAT_UNKNOWN:
                ERRME("Unknown texture format!");
                break;

            default:
                break;
            }

            if(is_texture_failiure) {
                ERRME("Failed to load texture file: " + frag_info.texture_file);
                return (size_t) -1;
            }

            this->m_game_models.resize(this->m_game_models.size() + 1);
            current_model_index = this->m_game_models.size() - 1;

            this->m_game_models[current_model_index].p_raw_texture_data = new RawTextureData;
            *this->m_game_models[current_model_index].p_raw_texture_data = *p_raw_texture_data;
            
            size_t prev_obj_index;
            if(current_model_index != 0 && (prev_obj_index = this->findPreviousTexturedObject(current_model_index - 1)) != -1)
                this->m_game_models[current_model_index].p_raw_texture_data->memory_offset = this->m_game_models[current_model_index - 1].p_raw_texture_data->memory_offset +
                (this->m_game_models[current_model_index - 1].p_raw_texture_data->texture_pixels_data.size() * sizeof(this->m_game_models[current_model_index - 1].p_raw_texture_data->texture_pixels_data[0]));
            
            else this->m_game_models[current_model_index].p_raw_texture_data->memory_offset = this->m_image_base_offset;
            
            break;
        }

        case DENG_PIPELINE_TYPE_UNMAPPED: {
            this->m_game_models.resize(this->m_game_models.size() + 1);
            current_model_index = this->m_game_models.size() - 1;
            
            this->m_game_models[current_model_index].p_frag_color = new dengMath::vec3<float>;
            *this->m_game_models[current_model_index].p_frag_color = frag_info.object_color;
        }
        
        default:
            break;
        }

        this->m_game_models.resize(this->m_game_models.size() + 1);
        size_t current_model_index = this->m_game_models.size() - 1;

        this->m_game_models[current_model_index].object_name = name;
        this->m_game_models[current_model_index].object_description = desciption;
        this->m_game_models[current_model_index].origin = origin;

        ObjLoader object_loader(object_file, DENG_COORDINATE_MODE_DEFAULT);
        object_loader.getObjVertices(&this->m_game_models[current_model_index]);
        
        /* Setting the correct offsets */
        size_t previous_index;
        if(current_model_index != 0 && (previous_index = this->findPreviousTexturedObject(current_model_index)) != (size_t) -1) {
            if(this->m_game_models[current_model_index].pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED) {
                this->m_game_models[current_model_index].buffer_memory_offset = this->m_game_models[previous_index].buffer_memory_offset + 
                (sizeof((*this->m_game_models[previous_index].p_texture_mapped_vertices_data)[0]) * this->m_game_models[previous_index].p_texture_mapped_vertices_data->size());
            }

            else if(this->m_game_models[current_model_index].pipeline_type == DENG_PIPELINE_TYPE_UNMAPPED) {
                this->m_game_models[current_model_index].buffer_memory_offset = this->m_game_models[previous_index].buffer_memory_offset +
                (sizeof((*this->m_game_models[previous_index].p_unmapped_vertices_data)[0]) * this->m_game_models[previous_index].p_unmapped_vertices_data->size());
            }
        }
        
        else this->m_game_models[current_model_index].buffer_memory_offset = this->m_buffer_base_offset;

        return current_model_index;
    }

    void unloadModel() {

    }
}