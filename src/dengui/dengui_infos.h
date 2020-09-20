#ifndef DENGUI_INFOS_H
#define DENGUI_INFOS_H

namespace dengUI {
    struct WindowInfo {
        deng::vec2<float> size;
        deng::vec2<float> origin;
        deng::vec2<float> position;
        deng::vec4<float> color;

        VkDevice *p_device;
        VkPhysicalDevice *p_gpu;
        deng::Window *p_window;
        
        std::string vert_shader_path;
        std::string frag_shader_path;

        const char *window_name;
        deng::FileManager *p_file_manager;
    };

    struct WindowBorderInfo {
        float thickness;
        float titlebar_height;
        deng::vec4<float> border_color;
        deng::vec4<float> minimizing_triangle_color;
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
        BufferMemoryPopulateFunc p_buffer_memory_populate_func;
        BufferCopyFunc p_buffer_copy_func;
    };

    struct RectangleInfo {
        deng::vec4<deng::vec2<float>> *p_vertices;
        deng::vec4<float> *p_color;
        std::vector<deng::UIVerticesData> *p_vertices_data;
        std::vector<uint16_t> *p_indices_data;
        
        dengBool enable_fill;
        dengUINonFilledBorderOffsetMode border_offset_mode;
        float border_thickness;

        dengUIWindowLayerSequenceId sequence_id;
    };

    struct TriangleInfo {
        deng::vec3<deng::vec2<float>> *p_vertices;
        deng::vec4<float> *p_color;

        std::vector<deng::UIVerticesData> *p_vertices_data;
        std::vector<uint16_t> *p_indices_data;

        dengUIWindowLayerSequenceId sequence_id;
    };
    
};

#endif