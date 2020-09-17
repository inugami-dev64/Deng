#ifndef OBJECTSHANDLER_H
#define OBJECTSHANDLER_H

namespace deng {
    
    struct ObjVertexData {
        vec3<float> position_vec;
        vec2<float> texture_vec;
    };

    struct SpecifiedVertexData {
        vec3<float> position_vec;
        vec3<float> color_vec;
    };

    struct Buffers {
        VkBuffer vertex_buffer{};
        VkDeviceMemory vertex_buffer_memory{};

        VkBuffer grid_buffer{};
        VkDeviceMemory grid_buffer_memory{};

        VkBuffer staging_buffer{};
        VkDeviceMemory staging_buffer_memory{};

        VkBuffer window_buffer{};
        VkDeviceMemory window_buffer_memory{};

        VkBuffer window_index_buffer{};
        VkDeviceMemory window_index_buffer_memory{};

        std::vector<VkBuffer> uniform_buffers{};
        std::vector<VkDeviceMemory> uniform_buffers_memory{};
    };

    struct DepthImageData {
        VkImage depthimage;
        VkDeviceMemory depthimage_memory;
        VkImageView depthimage_view;
    };

    struct ObjTextureData {
        uint32_t width;
        uint32_t height;

        VkImage texture_image;
        VkDeviceMemory texture_image_memory;
        VkImageView texture_image_view;
        VkSampler texture_sampler;
    };

    struct ObjRawTextureData {
        std::vector<uint8_t> texture_pixels_data;

        uint32_t *p_width;
        uint32_t *p_height;
        VkDeviceSize *p_texture_size;
        ObjRawTextureData();

    public:
        void cpyDims(ObjTextureData &texture_Data);
        void clear();
    };

    struct GameObject {
        vec3<float> origin;
        std::vector<ObjVertexData> vertex_data;
        ObjRawTextureData raw_texture_data;
        ObjTextureData texture_data;
        ModelMatrix model_matrix;        
    };

    struct SpecifiedObject {
        vec3<float> origin;
        std::vector<SpecifiedVertexData> vertex_data;
        std::vector<uint32_t> *p_index_data;
        ModelMatrix model_matrix;
    };
}

#endif