#ifndef OBJLOADER_H
#define OBJLOADER_H

namespace deng {
    
    class ObjLoader {
    private:
        dengCoordinateMode reverse_coordinates;
        std::vector<vec3<float>> vertex_coord_vector;
        std::vector<vec3<float>> vertex_normal_coord_vector;
        std::vector<vec2<float>> vertex_texture_coord_vector;

        std::vector<uint32_t> vertex_coord_faces_vector;
        std::vector<uint32_t> vertex_texture_coord_faces_vector;
        std::vector<uint32_t> vertex_normal_coord_faces_vector; 

    private:
        void getVerticesCoord(const std::vector<std::string> &object_contents);
        void getVertexFaces(const std::vector<std::string> &object_contents);

    public:
        ObjLoader(const std::string &file_name, const dengCoordinateMode &reverse_coordinates);
        void getObjVerticesAndIndices(GameObject &obj);
    };
}

#endif