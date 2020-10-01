#ifndef DENGUI_PIXEL_COLLISION_H
#define DENGUI_PIXEL_COLLISION_H

namespace dengUI {

    struct PixelGroup {
        const char *object_description;
        std::vector<dengMath::vec2<double>> pixels_data;
        std::vector<dengMath::vec2<dengMath::vec2<double>>> horisontal_pixel_boundaries;
    };

    class PixelCollision {
    private:
        deng::Window *m_p_window;
        std::vector<dengUtils::UIVerticesData> *m_p_vertices_data;
        std::vector<uint16_t> *m_p_indices_data;
        std::vector<PixelGroup> m_objects_pixel_data;

    private:
        void getRowPixelBoundaries(const size_t &vertical_index, dengMath::vec2<dengMath::vec2<double>> &boundaries, const char *p_description);
        void createPixelDataBoundaries();

    public:
        PixelCollision(deng::Window *p_window, std::vector<dengUtils::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data);
        void setupPixelRangesFromVertices(dengMath::vec2<size_t> *vertices_bounds, dengMath::vec2<size_t> *indices_bounds, const char *description);
        dengBool isCollided(const char *object_description, const dengMath::vec2<size_t> &colliding_pixel);
    };
}

#endif