#include "objectshandler.h"

namespace deng {

    class GridManager {
    private:
        float m_max_vertex_buffer_distance;
        float m_grid_height;
        float m_grid_width;

        float m_grid_color_r;
        float m_grid_color_g;
        float m_grid_color_b;

        SpecifiedObject m_grid;

    public:
        GridManager(const float &max_distance, const float &grid_height, const float &grid_width, const float &color_r, const float &color_g, const float &color_b);
        ~GridManager();
        void updateVertices(vec4<float> camera_position);
        void generateVertices(vec4<float> camera_position);
        SpecifiedObject *getGridObject();
    };
}