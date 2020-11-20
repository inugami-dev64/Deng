#ifndef OBJECT_DATA_STRUCTS_H
#define OBJECT_DATA_STRUCTS_H

namespace dengUtils {
    
    struct TextureMappedVerticesData {
        dengMath::vec3<float> position_vec;
        dengMath::vec2<float> texture_vec;
    };

    struct UnmappedVerticesData {
        dengMath::vec3<float> position_vec;
        dengMath::vec4<float> color_vec;
    };

    struct BufferData {
        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;

        VkBuffer main_buffer;
        VkDeviceMemory main_buffer_memory;

        std::vector<VkBuffer> uniform_buffers;
        std::vector<VkDeviceMemory> uniform_buffers_memory;
    };

    struct RawTextureData {
        std::vector<uint8_t> texture_pixels_data;
        uint32_t width;
        uint32_t height;
        VkDeviceSize texture_size;
        VkImage image;
        VkDeviceMemory image_memory;
        std::vector<VkDescriptorSet> descriptor_sets;
        VkImageView image_view;
    };

    class GameObject {
    public:
        std::string object_name;
        std::string object_description;
        dengMath::vec3<float> origin;
        std::vector<TextureMappedVerticesData> texture_mapped_vertices_data;
        std::vector<UnmappedVerticesData> unmapped_vertices_data;
        std::vector<uint32_t> indices_data;

        VkDeviceSize vertices_buffer_memory_offset{0};
        VkDeviceSize indices_buffer_memory_offset{0};
        dengPipelineType pipeline_type;
        RawTextureData *p_raw_texture_data = nullptr;
        dengMath::vec4<float> *p_frag_color = nullptr;
        dengMath::ModelMatrix model_matrix;   

    public: 
        ~GameObject() {
            switch (this->pipeline_type)
            {
            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
                delete this->p_raw_texture_data;
                // delete this->p_texture_mapped_vertices_data;
                break;
            
            case DENG_PIPELINE_TYPE_UNMAPPED:
                delete this->p_frag_color;
                // delete this->p_unmapped_vertices_data;
                break;
            
            default:
                break;
            }
        }

        void clean();
    };
}

#endif