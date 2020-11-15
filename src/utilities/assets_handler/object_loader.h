#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

namespace dengUtils {

    struct FragInfo {
        dengPipelineType pipeline_type;
        std::string texture_file;
        dengMath::vec3<float> object_color;
    };
    
    class ObjLoader {
    private:
        dengCoordinateMode reverse_coordinates;
        std::vector<dengMath::vec3<float>> vertex_coord_vector;
        std::vector<dengMath::vec3<float>> vertex_normal_coord_vector;
        std::vector<dengMath::vec2<float>> vertex_texture_coord_vector;

        std::vector<uint32_t> vertex_coord_faces_vector;
        std::vector<uint32_t> vertex_texture_coord_faces_vector;
        std::vector<uint32_t> vertex_normal_coord_faces_vector; 

    private:
        void getVerticesCoord(const std::vector<std::string> &object_contents);
        void getVertexFaces(const std::vector<std::string> &object_contents);
        size_t findTextureMappedVertexInstance(GameObject *p_obj, std::vector<TextureMappedVerticesData> &instance);
        size_t findUnmappedVertexInstance(GameObject *p_obj, std::vector<UnmappedVerticesData> &instance);
        void indexVertices(GameObject *p_obj);

    public:
        ObjLoader(const std::string &file_name, const dengCoordinateMode &reverse_coordinates);
        void getObjVertices(GameObject *p_obj);
    };


    class ModelHandler {
    private:
        std::vector<GameObject> m_game_models;
        BufferDataFuncs m_data_funcs;
        VkDeviceSize m_image_base_offset;
        VkDeviceSize m_buffer_base_offset;

    private:
        size_t findPreviousTexturedObject(const size_t &current_index);
        void initTextureImage(const size_t &index);

    public:
        ModelHandler(const BufferDataFuncs &data_funcs, const VkDeviceSize &image_base_offset, const VkDeviceSize &buffer_base_offset);
        void initModelMatrix(size_t index, dengMath::ModelMatrix *p_matrix);
        size_t loadModel(const std::string &name, const std::string &description, const std::string &object_file, const dengMath::vec3<float> &origin, const FragInfo &frag_info);
        void unloadModel(size_t model_index);
        dengMath::ModelMatrix model_matrix;
        void getModels(std::vector<GameObject> *p_game_objects);
    };
}

#endif