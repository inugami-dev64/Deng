#ifndef DENGUI_INFOS_H
#define DENGUI_INFOS_H

namespace dengUI {
    

    struct WindowObject {
        dengMath::vec2<size_t> vertices_bounds;
        dengMath::vec2<size_t> indices_bounds;
        dengUIWindowLayerSequenceId sequence_id;
        const char *description;
        dengBool is_clickable;

        uint16_t handle_id;
        dengBool is_drawn;
        dengBool is_clicked;
        void *p_click_handler_object;
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
        dengMath::vec2<float> size;
        dengMath::vec2<float> origin;
        dengMath::vec2<float> position;
        dengMath::vec4<float> color;

        VkDevice *p_device;
        VkPhysicalDevice *p_gpu;
        deng::Window *p_window;
        
        std::string vert_shader_path;
        std::string frag_shader_path;

        const char *window_name;
        dengUtils::FileManager *p_file_manager;
    };

    struct ClickInfo {
        WindowInfo *p_windowinfo;

        std::vector<dengUtils::UIVerticesData> *p_vertices_data;
        std::vector<uint16_t> *p_indices_data;
        std::vector<WindowObject> *p_window_objects;
        BufferInfo *p_bufferinfo;

        uint16_t handle_id;
        dengBool *p_is_clicked;
        dengMath::vec2<size_t> vertices_bounds;
        dengMath::vec2<size_t> indices_bounds;
        const char *description;
    };

    struct WindowBorderInfo {
        float thickness;
        float titlebar_height;
        dengMath::vec4<float> border_color;
        dengMath::vec4<float> minimizing_triangle_color;
    };

    struct RectangleInfo {
        dengMath::vec4<dengMath::vec2<float>> *p_vertices;
        dengMath::vec4<float> *p_color;
        std::vector<dengUtils::UIVerticesData> *p_vertices_data;
        std::vector<uint16_t> *p_indices_data;
        
        dengBool enable_fill;
        dengUINonFilledBorderOffsetMode border_offset_mode;
        float border_thickness;

        dengUIWindowLayerSequenceId sequence_id;
    };

    struct TriangleInfo {
        dengMath::vec3<dengMath::vec2<float>> *p_vertices;
        dengMath::vec4<float> *p_color;

        std::vector<dengUtils::UIVerticesData> *p_vertices_data;
        std::vector<uint16_t> *p_indices_data;

        dengUIWindowLayerSequenceId sequence_id;
    };
    
};

#endif