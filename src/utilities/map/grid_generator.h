#ifndef GRID_GENERATOR_H
#define GRID_GENERATOR_H

#define DENG_GRID_TRIANGLE_THICKNESS -0.0001
namespace dengUtils {

    class GridManager {
    private:
        float m_max_vertex_buffer_distance;
        float *m_p_grid_lenght;
        float *m_p_grid_width;

        float *m_p_grid_color_r;
        float *m_p_grid_color_g;
        float *m_p_grid_color_b;

        // first element represents x axis and second element represents z axis
        dengMath::vec2<int32_t> m_min_vertices;
        dengMath::vec2<int32_t> m_max_vertices;

        SpecifiedObject *m_grid;

    private:
        dengMath::vec2<float*> getOrdereredVertexDataElements(const uint32_t index, const deng_CoordinateAxisType &axis);
        void generateAxisLineVertices(dengMath::vec2<int32_t> &min_vertices, dengMath::vec2<int32_t> &max_vertices, const deng_CoordinateAxisType &axis, int32_t &index);

    public:
        GridManager(SpecifiedObject *grid, const float &max_distance, float *p_grid_height, float *p_grid_width, float *p_color_r, float *p_color_g, float *p_color_b);
        ~GridManager();
        void updateVertices(dengMath::vec4<float> camera_position);
        void generateVertices(dengMath::vec4<float> camera_position);
        void generateIndices();
    };
}

#endif