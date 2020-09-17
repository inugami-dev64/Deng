
#define DENG_GRID_TRIANGLE_THICKNESS -0.0001
namespace deng {

    class GridManager {
    private:
        float m_max_vertex_buffer_distance;
        float *m_p_grid_height;
        float *m_p_grid_width;

        float *m_p_grid_color_r;
        float *m_p_grid_color_g;
        float *m_p_grid_color_b;

        // first element represents x axis and second element represents z axis
        vec2<int32_t> m_min_vertices;
        vec2<int32_t> m_max_vertices;

        SpecifiedObject *m_grid;

    private:
        vec2<float*> getOrdereredVertexDataElements(const uint32_t index, const dengCoordinateAxisType &axis);
        void generateAxisLineVertices(vec2<int32_t> &min_vertices, vec2<int32_t> &max_vertices, const dengCoordinateAxisType &axis, int32_t &index);

    public:
        GridManager(SpecifiedObject *grid, const float &max_distance, float *p_grid_height, float *p_grid_width, float *p_color_r, float *p_color_g, float *p_color_b);
        ~GridManager();
        void updateVertices(vec4<float> camera_position);
        void generateVertices(vec4<float> camera_position);
        void generateIndices();
    };
}