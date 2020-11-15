#ifndef OBJECT_DATA_STRUCTS_H
#define OBJECT_DATA_STRUCTS_H

namespace dengUtils {

    struct BufferDataFuncs {
        BufferCreateFunc p_buffer_create_func;
        BufferMemoryPopulateFunc p_buffer_mem_populate_func;
        BufferCopyFunc p_buffer_copy_func;

        ImageCreateFunc p_image_create_func;
        ImageLayoutTransitionFunc p_image_layout_transition_func;
        CopyBufferToImageFunc p_copy_buffer_to_image_func;
    };

    struct TextureMappedVerticesData {
        dengMath::vec3<float> position_vec;
        dengMath::vec2<float> texture_vec;
    };

    struct UnmappedVerticesData {
        dengMath::vec3<float> position_vec;
        dengMath::vec4<float> color_vec;
    };

    struct BufferData {
        VkBuffer staging_buffer{};
        VkDeviceMemory staging_buffer_memory{};

        VkBuffer main_buffer{};
        VkDeviceMemory main_buffer_memory{};
        
        std::vector<VkBuffer> uniform_buffers{};
        std::vector<VkDeviceMemory> uniform_buffers_memory{};
    };

    struct RawTextureData {
        VkImage image;
        std::vector<uint8_t> texture_pixels_data;
        uint32_t width;
        uint32_t height;
        VkDeviceSize texture_size;
        VkDeviceSize memory_offset{0};
    };

    struct GameObject {
        std::string object_name;
        std::string object_description;
        dengMath::vec3<float> origin;
        std::vector<TextureMappedVerticesData> *p_texture_mapped_vertices_data;
        std::vector<UnmappedVerticesData> *p_unmapped_vertices_data;
        std::vector<uint32_t> indices_data;

        VkDeviceSize buffer_memory_offset{0};
        dengPipelineType pipeline_type;
        RawTextureData *p_raw_texture_data = nullptr;
        dengMath::vec3<float> *p_frag_color = nullptr;
        deng::PipelineData *p_pipeline;
        dengMath::ModelMatrix model_matrix;   

        ~GameObject() {
            switch (this->pipeline_type)
            {
            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
                delete this->p_raw_texture_data;
                delete this->p_texture_mapped_vertices_data;
                break;
            
            case DENG_PIPELINE_TYPE_UNMAPPED:
                delete this->p_frag_color;
                delete this->p_unmapped_vertices_data;
                break;
            
            default:
                break;
            }
        }
    };
}

#endif