#ifndef DENGUI_INFOS_H
#define DENGUI_INFOS_H

namespace dengUI {

    struct TriangleAndRectangleVerticesRelativityInfo {
        dengCoordinateAxisType rectangle_axis_type;
        float rectangle_vertex_distance;
        size_t rectangle_vertex_index;
    };

    struct BaseWindowVertices {
        std::vector<dengUtils::UIVerticesData> window_main_vertices;
        std::vector<uint16_t> window_main_indices;
        
        std::vector<dengUtils::UIVerticesData> window_head_vertices;
        std::vector<uint16_t> window_head_indices;

        std::vector<dengUtils::UIVerticesData> window_minimise_triangle_vertices;
        std::vector<uint16_t> window_minimise_triangle_indices;
    };

    struct WindowAssets {
        dengUIWindowAssetType window_asset_type;
        dengBool is_clickable;
    };

    struct BorderInfo {
        dengCSS::CSSGeneralBorderInfo *p_css_general_border_info;
        dengCSS::CSSSpecifiedBorderInfo *p_css_specified_border_info;
    };

    struct RectangleInfo {
        std::vector<dengUtils::UIVerticesData> *p_vertices;
        std::vector<uint16_t> *p_indices;
        dengCSS::CSSMarginData margin_data;

        dengMath::vec2<float> rectangle_size;
        dengMath::vec3<float> rectangle_color;
    };

    struct TriangleInfo {
        std::vector<dengUtils::UIVerticesData> *p_vertices;
        std::vector<uint16_t> *p_indices;
        dengMath::vec3<float> triangle_color;
        dengCSS::CSSMarginData margin_data;

        dengMath::vec2<float> surround_rectangle_size;
        dengMath::vec3<TriangleAndRectangleVerticesRelativityInfo> vertices_relativity_infos;
    };

    struct BufferInfo {
        VkBuffer *p_vertices_buffer;
        VkDeviceMemory *p_vertices_buffer_memory;

        VkBuffer *p_indices_buffer;
        VkDeviceMemory *p_indices_buffer_memory;

        uint32_t *p_indices_size;

        VkBuffer *p_staging_buffer;
        VkDeviceMemory *p_staging_buffer_memory;

        VkCommandPool *p_commandpool;
        VkQueue *p_graphics_queue;

        BufferCreateFunc p_buffer_create_func;
        BufferCopyFunc p_buffer_copy_func;
        BufferMemoryPopulateFunc p_buffer_memory_populate_func;
    };

    struct WindowInfo {
        dengMath::vec2<float> origin;
        dengMath::vec2<float> position;

        VkDevice *p_device;
        VkPhysicalDevice *p_gpu;
        deng::Window *p_window;
        
        std::string vert_shader_path;
        std::string frag_shader_path;

        const char *window_title;
        dengUtils::FileManager *p_file_manager;
    };

    struct ClickInfo {
        WindowInfo *p_windowinfo;

        std::vector<dengUtils::UIVerticesData> *p_vertices_data;
        std::vector<uint16_t> *p_indices_data;
        BufferInfo *p_bufferinfo;

        uint16_t handle_id;
        dengBool *p_is_clicked;
        dengMath::vec2<size_t> vertices_bounds;
        dengMath::vec2<size_t> indices_bounds;
        const char *description;
    };
}

#endif