#include "../../core/deng_core.h"

namespace dengUtils {
    ObjLoader::ObjLoader(const std::string &file_name, const dengCoordinateMode &coordinate_mode) {
        FileManager local_fm;
        this->m_reverse_coordinates = coordinate_mode;
        std::vector<std::string> local_contents;
        local_fm.getFileContents(file_name, nullptr, &local_contents);
        
        this->getVerticesCoord(local_contents);
        this->getVertexFaces(local_contents);
    }

    void ObjLoader::getVerticesCoord(const std::vector<std::string> &file_contents) {
        uint32_t coord_vec_type;

        for(size_t i = 0; i < file_contents.size(); i++) {
            bool read_coords;
            size_t first_coord_index;

            if(file_contents[i].find("v ") == 0) read_coords = true, coord_vec_type = DENG_VERTEX_COORD, first_coord_index = 2;
            else if(file_contents[i].find("vn ") == 0) read_coords = true, coord_vec_type = DENG_VERTEX_NORMAL_COORD, first_coord_index = 3;
            else if(file_contents[i].find("vt ") == 0) read_coords = true, coord_vec_type = DENG_VERTEX_TEXTURE_COORD, first_coord_index = 3;
            else read_coords = false;

            if(read_coords) {

                char local_buffer[file_contents[i].size()];
                strncpy(local_buffer, file_contents[i].c_str(), file_contents[i].size());

                uint32_t coordinateType = DENG_COORD_AXIS_X;
                std::string x_str;
                std::string y_str;
                std::string z_str;
                
                for(size_t readIndex = first_coord_index; readIndex < (sizeof(local_buffer)/sizeof(local_buffer[0])); readIndex++) {\
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
                    switch (coord_vec_type)
                    {
                    case DENG_VERTEX_COORD:

                        if(this->m_reverse_coordinates) this->m_vertex_coord_vector.push_back({-(std::stof(x_str.c_str())), -(std::stof(y_str.c_str())), -(std::stof(z_str.c_str()))});    
                        else this->m_vertex_coord_vector.push_back({std::stof(x_str.c_str()), std::stof(y_str.c_str()), std::stof(z_str.c_str())});

                        break;

                    case DENG_VERTEX_TEXTURE_COORD:
                        if(this->m_reverse_coordinates) this->m_vertex_texture_coord_vector.push_back({-(std::stof(x_str.c_str())), -(std::stof(y_str.c_str()))});
                        else this->m_vertex_texture_coord_vector.push_back({std::stof(x_str.c_str()), std::stof(y_str.c_str())});

                        break;

                    case DENG_VERTEX_NORMAL_COORD:
                        if(this->m_reverse_coordinates) this->m_vertex_normal_coord_vector.push_back({-(std::stof(x_str.c_str())), -(std::stof(y_str.c_str())), -(std::stof(z_str.c_str()))});
                        else this->m_vertex_normal_coord_vector.push_back({std::stof(x_str.c_str()), std::stof(y_str.c_str()), std::stof(z_str.c_str())});
                        
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
        size_t line_index, ch_index;
        std::string val_str;

        for(line_index = 0; line_index < obj_contents.size(); line_index++) {
            
            if(obj_contents[line_index].find("f ") == 0) {
                char buffer[obj_contents[line_index].size()];
                strncpy(buffer, obj_contents[line_index].c_str(), obj_contents[line_index].size());
                dengCoordinateType local_vertex_type_count = DENG_VERTEX_COORD;

                for(ch_index = 2; ch_index < sizeof(buffer)/sizeof(buffer[0]); ch_index++) {
                    
                    if(buffer[ch_index] != '/' && buffer[ch_index] != ' ') val_str += buffer[ch_index];
                    else {
                        switch (local_vertex_type_count)
                        {
                        case DENG_VERTEX_COORD:
                            this->m_vertex_coord_faces_vector.push_back(std::stoi(val_str) - 1);
                            val_str.clear();
                            local_vertex_type_count = DENG_VERTEX_TEXTURE_COORD;
                            break;

                        case DENG_VERTEX_TEXTURE_COORD:
                            this->m_vertex_texture_coord_faces_vector.push_back(std::stoi(val_str) - 1);
                            val_str.clear();
                            local_vertex_type_count = DENG_VERTEX_NORMAL_COORD;
                            break;

                        case DENG_VERTEX_NORMAL_COORD:
                            this->m_vertex_normal_coord_faces_vector.push_back(std::stoi(val_str) - 1);
                            val_str.clear();
                            local_vertex_type_count = DENG_VERTEX_COORD;
                            break;
                    
                        default:
                            break;
                        }
                    }

                    if(ch_index == (sizeof(buffer)/sizeof(buffer[0]) - 1)) {
                        // val_str += buffer[ch_index];
                        this->m_vertex_normal_coord_faces_vector.push_back(std::stoi(val_str) - 1);
                        val_str.clear();
                        local_vertex_type_count = DENG_VERTEX_COORD;
                    }
                }
            }
        }
    }

    
    size_t ObjLoader::searchForMultipleTextureMappedVertexInstances(GameObject *p_obj, std::vector<TextureMappedVerticesData> &instance, size_t obj_vertices_index) {
        size_t instance_index; 
        for(instance_index = 0; instance_index < instance.size(); instance_index++)
            if(p_obj->texture_mapped_vertices_data[obj_vertices_index].position_vec == instance[instance_index].position_vec &&
            p_obj->texture_mapped_vertices_data[obj_vertices_index].texture_vec == instance[instance_index].texture_vec) return instance_index;
        
        
        return (size_t) -1;
    }

    size_t ObjLoader::findUnmappedVertexInstance(GameObject *p_obj, std::vector<UnmappedVerticesData> &instance) {
        size_t index;
        for(index = 0; index < instance.size(); index++)
            if(p_obj->unmapped_vertices_data[index].position_vec == instance[index].position_vec && 
            p_obj->unmapped_vertices_data[index].color_vec == instance[index].color_vec) return index;

        return (size_t) -1;
    }
    
    void ObjLoader::indexVertices(GameObject *p_obj) {
        size_t index;
        LOG("GameObject " + p_obj->object_name + " vertices size(before indexing) is: " + std::to_string(p_obj->texture_mapped_vertices_data.size()));
        LOG("GameObject " + p_obj->object_name + " indices size(before indexing) is: " + std::to_string(p_obj->indices_data.size()));
        
        p_obj->indices_data.resize(p_obj->texture_mapped_vertices_data.size());
        
        switch (p_obj->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED: {
            size_t instance_index;
            std::vector<TextureMappedVerticesData> texture_mapped_data;

            for(index = 0; index < p_obj->texture_mapped_vertices_data.size(); index++) {
                if(this->searchForMultipleTextureMappedVertexInstances(p_obj, texture_mapped_data, index) == (size_t) -1)
                    texture_mapped_data.push_back(p_obj->texture_mapped_vertices_data[index]);
            }
            
            for(index = 0; index < p_obj->texture_mapped_vertices_data.size(); index++) {
                instance_index = this->searchForMultipleTextureMappedVertexInstances(p_obj, texture_mapped_data, index);
                p_obj->indices_data[index] = instance_index;
            }

            LOG("GameObject " + p_obj->object_name + " vertices size(after indexing) is: " + std::to_string(texture_mapped_data.size()));
            LOG("GameObject " + p_obj->object_name + " indices size(after indexing) is: " + std::to_string(p_obj->indices_data.size()));

            p_obj->texture_mapped_vertices_data.clear();
            p_obj->texture_mapped_vertices_data = texture_mapped_data;

            break;
        }

        case DENG_PIPELINE_TYPE_UNMAPPED: {
            size_t instance_index;
            std::vector<UnmappedVerticesData> unmapped_data;

            for(index = 0; index < p_obj->unmapped_vertices_data.size(); index++) {
                if((instance_index = findUnmappedVertexInstance(p_obj, unmapped_data)) == -1) {
                    unmapped_data.push_back(p_obj->unmapped_vertices_data[index]);
                    p_obj->indices_data.push_back((uint32_t) index);
                }

                else p_obj->indices_data.push_back((uint32_t) instance_index);
            }

            p_obj->unmapped_vertices_data.clear();
            p_obj->unmapped_vertices_data = unmapped_data;
            break;
        }
        
        default:
            break;
        }
    }

    void ObjLoader::getObjVertices(GameObject *p_obj) {
        size_t index;

        if(this->m_vertex_coord_faces_vector.size() != this->m_vertex_texture_coord_faces_vector.size())
            ERR("Corrupted .obj file!");

        switch (p_obj->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
            LOG("Texture mapped!");
            p_obj->texture_mapped_vertices_data.resize(this->m_vertex_coord_faces_vector.size());
            p_obj->indices_data.resize(this->m_vertex_coord_faces_vector.size());

            for(index = 0; index < p_obj->texture_mapped_vertices_data.size(); index++) {
                p_obj->texture_mapped_vertices_data[index].position_vec = {this->m_vertex_coord_vector[this->m_vertex_coord_faces_vector[index]].first + p_obj->origin.first,
                this->m_vertex_coord_vector[this->m_vertex_coord_faces_vector[index]].second + p_obj->origin.second, 
                this->m_vertex_coord_vector[this->m_vertex_coord_faces_vector[index]].third + p_obj->origin.third};

                p_obj->texture_mapped_vertices_data[index].texture_vec = {this->m_vertex_texture_coord_vector[this->m_vertex_texture_coord_faces_vector[index]].first, 
                this->m_vertex_texture_coord_vector[this->m_vertex_texture_coord_faces_vector[index]].second};
            }

            // this->indexVertices(p_obj);
            break;
        
        case DENG_PIPELINE_TYPE_UNMAPPED:
            LOG("Unmapped");
            p_obj->unmapped_vertices_data.resize(this->m_vertex_coord_vector.size());
            LOG("vertices size for model " + p_obj->object_name + " is: " + std::to_string(p_obj->unmapped_vertices_data.size()));
            for(index = 0; index < p_obj->unmapped_vertices_data.size(); index++) {
                p_obj->unmapped_vertices_data[index].position_vec = {this->m_vertex_coord_vector[index].first + p_obj->origin.first, this->m_vertex_coord_vector[index].second + p_obj->origin.second,
                this->m_vertex_coord_vector[index].third + p_obj->origin.third};
                p_obj->unmapped_vertices_data[index].color_vec = {p_obj->p_frag_color->first, p_obj->p_frag_color->second, p_obj->p_frag_color->third, p_obj->p_frag_color->fourth};
            }

            p_obj->indices_data = this->m_vertex_coord_faces_vector;

            LOG("indices size for model " + p_obj->object_name + " is: " + std::to_string(p_obj->indices_data.size()));
            break;

        default:
            break;
        }

        /* TEMP CODE */
        // FileManager fm;
        // std::string contents;
        // fm.writeToFile("deng.log", "#entry point", DENG_WRITEMODE_REWRITE);
        
        // if(p_obj->pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED) {
        //     fm.writeToFile("deng.log", "textures", DENG_WRITEMODE_FROM_END);
        //     for(index = 0; index < p_obj->texture_mapped_vertices_data.size(); index++) {
        //         contents = "{" + std::to_string(p_obj->texture_mapped_vertices_data[index].position_vec.first) + ";" + std::to_string(p_obj->texture_mapped_vertices_data[index].position_vec.second) +
        //         ";" + std::to_string(p_obj->texture_mapped_vertices_data[index].position_vec.third) + "} {" + std::to_string(p_obj->texture_mapped_vertices_data[index].texture_vec.first) + std::to_string(p_obj->texture_mapped_vertices_data[index].texture_vec.second) + "}";
        //         fm.writeToFile("deng.log", contents, DENG_WRITEMODE_FROM_END);
        //     }
        // }

        // else if(p_obj->pipeline_type == DENG_PIPELINE_TYPE_UNMAPPED) {
        //     fm.writeToFile("deng.log", "unmapped", DENG_WRITEMODE_FROM_END);
        //     for(index = 0; index < p_obj->unmapped_vertices_data.size(); index++) {
        //         contents = "{" + std::to_string(p_obj->unmapped_vertices_data[index].position_vec.first) + ";" + std::to_string(p_obj->unmapped_vertices_data[index].position_vec.second) +
        //         ";" + std::to_string(p_obj->unmapped_vertices_data[index].position_vec.third) + "} {" + std::to_string(p_obj->unmapped_vertices_data[index].color_vec.first) + std::to_string(p_obj->unmapped_vertices_data[index].color_vec.second) + "}";
        //         fm.writeToFile("deng.log", contents, DENG_WRITEMODE_FROM_END);
        //     }
        // }
        
        // fm.writeToFile("deng.log", "indices", DENG_WRITEMODE_FROM_END);
        // for(index = 0; index < p_obj->indices_data.size(); index++)
        //     fm.writeToFile("deng.log", std::to_string(p_obj->indices_data[index]), DENG_WRITEMODE_FROM_END);

        /* END OF TEMP CODE */
    }


    ModelHandler::ModelHandler(const VkDeviceSize &current_offset) { this->m_current_offset = current_offset; }

    size_t ModelHandler::loadModel(const std::string &name, const std::string &desciption, const std::string &object_file, const dengMath::vec3<float> &origin, const FragInfo &frag_info) {
        RawTextureData *p_raw_texture_data;
        size_t current_index;

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
            current_index = this->m_game_models.size() - 1;
            
            /* vertices data is always located before indices data in buffer memory */ 
            this->m_game_models[current_index].pipeline_type = frag_info.pipeline_type;
            this->m_game_models[current_index].p_raw_texture_data = new RawTextureData;            
            *this->m_game_models[current_index].p_raw_texture_data = *p_raw_texture_data;

            // /* TEMP CODE */
            // FileManager fm;
            // fm.writeToFile("deng_tex.log", "#entry point", DENG_WRITEMODE_REWRITE);

            // for(size_t index = 0; index < this->m_game_models[current_index].p_raw_texture_data->texture_size; index++)
            //     fm.writeToFile("deng_tex.log", std::to_string(this->m_game_models[current_index].p_raw_texture_data->texture_pixels_data[index]), DENG_WRITEMODE_FROM_END);

            // /* END OF TEMP CODE */

            break;
        }

        case DENG_PIPELINE_TYPE_UNMAPPED: {
            this->m_game_models.resize(this->m_game_models.size() + 1);
            current_index = this->m_game_models.size() - 1;

            this->m_game_models[current_index].pipeline_type = frag_info.pipeline_type;
            this->m_game_models[current_index].p_frag_color = new dengMath::vec4<float>;
            *this->m_game_models[current_index].p_frag_color = frag_info.object_color;

        }
        
        default:
            break;
        }

        this->m_game_models[current_index].object_name = name;
        this->m_game_models[current_index].object_description = desciption;
        this->m_game_models[current_index].origin = origin;

        ObjLoader object_loader(object_file, DENG_COORDINATE_MODE_DEFAULT);
        object_loader.getObjVertices(&this->m_game_models[current_index]);

        if(this->m_game_models[current_index].pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED) {
            this->m_game_models[current_index].vertices_buffer_memory_offset = this->m_current_offset; 
            this->m_current_offset += this->m_game_models[current_index].texture_mapped_vertices_data.size() * sizeof(this->m_game_models[current_index].texture_mapped_vertices_data[0]);
        }

        else if(this->m_game_models[current_index].pipeline_type == DENG_PIPELINE_TYPE_UNMAPPED) {
            this->m_game_models[current_index].vertices_buffer_memory_offset = this->m_current_offset;
            this->m_current_offset += this->m_game_models[current_index].unmapped_vertices_data.size() * sizeof(this->m_game_models[current_index].unmapped_vertices_data[0]);
        }

        this->m_game_models[current_index].indices_buffer_memory_offset = this->m_current_offset;
        this->m_current_offset += this->m_game_models[current_index].indices_data.size() * sizeof(this->m_game_models[current_index].indices_data[0]);
        
        return current_index;
    }

    void ModelHandler::initModelMatrix(size_t model_index, dengMath::ModelMatrix *p_matrix) {
        size_t vertices_index;
        dengMath::mat4<float> mat;
        dengMath::vec4<float> vec;
        p_matrix->getModelMatrix(&mat);
        LOG(model_index);

        switch (this->m_game_models[model_index].pipeline_type)
        {
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
            for(vertices_index = 0; vertices_index < this->m_game_models[model_index].texture_mapped_vertices_data.size(); vertices_index++) {
                vec = mat * this->m_game_models[model_index].texture_mapped_vertices_data[vertices_index].position_vec; 
                this->m_game_models[model_index].texture_mapped_vertices_data[vertices_index].position_vec = {vec.first, vec.second, vec.third};
            }
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED: 
            for(vertices_index = 0; vertices_index < this->m_game_models[model_index].unmapped_vertices_data.size(); vertices_index++) {
                // LOG("seg test!");
                vec = mat * this->m_game_models[model_index].unmapped_vertices_data[vertices_index].position_vec;
                this->m_game_models[model_index].unmapped_vertices_data[vertices_index].position_vec = {vec.first, vec.second, vec.third};
            }
            break;
        
        default:
            break;
        }
    }

    void ModelHandler::getModels(std::vector<GameObject> *p_game_objects) { *p_game_objects = this->m_game_models; }
}