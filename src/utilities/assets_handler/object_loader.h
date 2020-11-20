#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

namespace dengUtils {

    struct FragInfo {
        dengPipelineType pipeline_type;
        std::string texture_file;
        dengMath::vec4<float> object_color;
    };
    
    class ObjLoader {
    private:
        dengCoordinateMode m_reverse_coordinates;
        std::vector<dengMath::vec3<float>> m_vertex_coord_vector;
        std::vector<dengMath::vec3<float>> m_vertex_normal_coord_vector;
        std::vector<dengMath::vec2<float>> m_vertex_texture_coord_vector;

        std::vector<uint32_t> m_vertex_coord_faces_vector;
        std::vector<uint32_t> m_vertex_texture_coord_faces_vector;
        std::vector<uint32_t> m_vertex_normal_coord_faces_vector; 

    private:
        void getVerticesCoord(const std::vector<std::string> &object_contents);
        void getVertexFaces(const std::vector<std::string> &object_contents);
        size_t findTextureMappedVertexInstance(GameObject *p_obj, std::vector<TextureMappedVerticesData> &instance);
        size_t findUnmappedVertexInstance(GameObject *p_obj, std::vector<UnmappedVerticesData> &instance);
        size_t searchForMultipleTextureMappedVertexInstances(GameObject *p_obj, std::vector<TextureMappedVerticesData> &instance, size_t index);
        void indexVertices(GameObject *p_obj);

    public:
        ObjLoader(const std::string &file_name, const dengCoordinateMode &reverse_coordinates);
        void getObjVertices(GameObject *p_obj);
    };


    class ModelHandler {
    private:
        std::vector<GameObject> m_game_models;
        VkDeviceSize m_current_offset;

    public:
        ModelHandler(const VkDeviceSize &current_offset);
        void initModelMatrix(size_t index, dengMath::ModelMatrix *p_matrix);
        size_t loadModel(const std::string &name, const std::string &description, const std::string &object_file, const dengMath::vec3<float> &origin, const FragInfo &frag_info);
        void unloadModel(size_t model_index);
        dengMath::ModelMatrix model_matrix;
        void getModels(std::vector<GameObject> *p_game_objects);
    };
}

#endif