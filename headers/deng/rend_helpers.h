#ifndef REND_HELPERS_H
#define REND_HELPERS_H

namespace deng {

    /* Contains different buffer objects for multiple purposes */
    struct BufferData {
        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;

        VkBuffer main_buffer;
        VkDeviceMemory main_buffer_memory;
        
        std::vector<deng_ObjColorData> colors;

        std::vector<VkBuffer> mat_uniform_buffers;
        std::vector<VkDeviceMemory> mat_uniform_buffer_mem;
    };

    
    /* Contains texture image objects */
    struct TextureImageData {
        deng_Texture texture;
        VkImage image;
        VkImageView image_view;
        VkSampler sampler;
        VkDeviceMemory image_mem;
        std::vector<VkDescriptorSet> descriptor_sets;
    };

    
    /* Contains different getter functions for hardware specs */
    struct HardwareSpecs {
        static deng_bool_t getExtensionSupport (
            const VkPhysicalDevice &gpu, 
            char *ext_name
        );

        static deng_ui32_t getMemoryType (
            const VkPhysicalDevice &gpu, 
            deng_ui32_t type_filter, 
            VkMemoryPropertyFlags properties
        );

        static deng_ui32_t getDeviceScore (
            const VkPhysicalDevice &device, 
            std::vector<const char*> &exts
        );
    };

    
    /* 
     * Contains method for memory allocation 
     * This struct is inherited to ImageCreator and BufferCreator structs
     */
    struct MemoryAllocator {
        static void allocateMemory (
            const VkDevice &device, 
            const VkPhysicalDevice &gpu,  
            VkDeviceSize size, 
            VkDeviceMemory &memory,
            deng_ui32_t mem_type_bits, 
            VkMemoryPropertyFlags properties
        );
    };


    /* Contains methods for image VkImage creation */
    struct ImageCreator : MemoryAllocator {
        static VkImageViewCreateInfo getImageViewInfo (
            const VkImage &image, 
            VkFormat format, 
            VkImageAspectFlags aspect_flags,
            deng_ui32_t mip_levels
        );

        static VkMemoryRequirements makeImage (
            const VkDevice &device, 
            const VkPhysicalDevice &gpu, 
            VkImage &image, 
            deng_ui32_t width, 
            deng_ui32_t height, 
            deng_ui32_t mip_levels,
            VkFormat format, 
            VkImageTiling tiling, 
            VkImageUsageFlags usage,
            VkSampleCountFlagBits sample_c
        );

        static void transitionImageLayout (
            const VkDevice &device, 
            const VkImage &image, 
            const VkCommandPool &commandpool, 
            const VkQueue &g_queue, 
            VkFormat format, 
            VkImageLayout old_layout, 
            VkImageLayout new_layout,
            deng_ui32_t mip_levels
        ); 

        static void copyBufferToImage (
            const VkDevice &device, 
            const VkCommandPool &commandpool, 
            const VkQueue &g_queue, 
            const VkBuffer &src_buffer, 
            const VkImage &dst_image, 
            deng_ui32_t width, 
            deng_ui32_t height
        );
    };

    
    /* Contains different buffer handling functions */
    struct BufferCreator : MemoryAllocator {
        static VkMemoryRequirements makeBuffer (
            const VkDevice &device, 
            const VkPhysicalDevice &gpu, 
            VkDeviceSize size, 
            VkBufferUsageFlags usage, 
            VkBuffer &buffer
        );
        
        /* Copy src_data to buffer memory */
        static void cpyToBufferMem (
            const VkDevice &device, 
            VkDeviceSize size, 
            void *src_data, 
            const VkDeviceMemory &buf_mem, 
            VkDeviceSize offset
        );
    
        static void copyBufferToBuffer (
            const VkDevice &device, 
            const VkCommandPool &commandpool, 
            const VkQueue &g_queue, 
            const VkBuffer &src_buffer, 
            const VkBuffer &dst_buffer, 
            VkDeviceSize size, 
            VkDeviceSize offset
        );
    };


    /* Record command buffers */
    struct CommandBufferRecorder {
        /* single commandbuffer command recorder function */
        static void beginCommandBufferSingleCommand (
            VkDevice device, 
            VkCommandPool commandpool, 
            VkCommandBuffer *p_commandbuffer
        );
        static void endCommandBufferSingleCommand (
            VkDevice device, 
            VkQueue graphics_queue, 
            VkCommandPool commandpool, 
            VkCommandBuffer *p_commandBuffer
        );
    };


    /* Contains queue families finder and getter functions */
    class QueueFamilyFinder {
    private:
        deng_ui32_t m_graphics_family_index;
        deng_ui32_t m_present_family_index;
    
    public:
        VkQueue graphics_queue;
        VkQueue present_queue;

    public:
        deng_bool_t findGraphicsFamily(VkPhysicalDevice gpu);
        deng_bool_t findPresentSupportFamily (
            VkPhysicalDevice gpu, 
            VkSurfaceKHR surface
        );

        deng_ui32_t getGraphicsQFIndex();
        deng_ui32_t getPresentQFIndex();
    };
    

    /* Contains information about Vulkan pipeline */
    struct PipelineData {
        VkPipeline pipeline;
        deng_PipelineType pipeline_type;
        VkPipelineLayout *p_pipeline_layout;
    };


    /* Struct for all pipeline createinfo specifier objects */
    struct PipelineCreateinfoSpecifiers {
        std::array<VkPipelineShaderStageCreateInfo, 2> m_shader_stage_createinfos{};
        std::array<VkShaderModule, 2> m_shader_modules{};
        VkVertexInputBindingDescription m_input_binding_desc{};
        std::vector<VkVertexInputAttributeDescription> m_input_attr_descs{};

        VkViewport m_viewport{};
        VkRect2D m_scissor{};

        VkPipelineVertexInputStateCreateInfo m_vert_input_create_info{};
        VkPipelineInputAssemblyStateCreateInfo m_input_asm_createinfo{};
        VkPipelineViewportStateCreateInfo m_viewport_state_createinfo{};
        VkPipelineRasterizationStateCreateInfo m_rasterization_createinfo{};
        VkPipelineMultisampleStateCreateInfo m_multisample_createinfo{};
        VkPipelineColorBlendAttachmentState m_colorblend_attachment{};
        VkPipelineDepthStencilStateCreateInfo m_depth_stencil{};
        VkPipelineColorBlendStateCreateInfo m_colorblend_state_createinfo{};
    };


    /* Struct for storing all vulkan device related information */
    struct VulkanDeviceInfo {
        VkDevice m_device;
        WindowWrap *m_p_win;
        VkPhysicalDevice m_gpu;
    };


    /* Handle pipeline creation */
    class PipelineCreator : private VulkanDeviceInfo, private PipelineCreateinfoSpecifiers {
    private:
        PipelineData *m_p_pipeline_data;
        VkRenderPass m_renderpass;
        VkExtent2D m_extent;

    private:
        VkShaderModule __getShaderModule(std::vector<char> &shader_bins);
        VkVertexInputBindingDescription __getBindingDesc();
        std::vector<VkVertexInputAttributeDescription> __getAttributeDescs();
        
    public:
        PipelineCreator (
            PipelineData *p_pipeline_data, 
            VkDevice device, 
            VkExtent2D extent, 
            VkRenderPass renderpass
        );
        ~PipelineCreator();
        VkGraphicsPipelineCreateInfo getGraphicsPipelineInfo (
            std::string vert_shader, 
            std::string frag_shader, 
            char *shader_module_name,
            VkPolygonMode polygon_mode, 
            VkCullModeFlagBits cull_mode, 
            VkFrontFace front_face, 
            VkPrimitiveTopology primitive_topology, 
            deng_bool_t add_depth_stencil, 
            deng_bool_t add_color_blend, 
            VkSampleCountFlagBits sample_c,
            deng_ui32_t subpass_index
        );
    };


    /* Initialise details about swapchain */ 
    class SwapChainDetails {
    private:
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_present_modes;

    public:
        SwapChainDetails (
            VkPhysicalDevice &gpu, 
            VkSurfaceKHR &surface
        );
        VkSurfaceCapabilitiesKHR getCapabilities();
        std::vector<VkSurfaceFormatKHR> getFormats();
        std::vector<VkPresentModeKHR> getPresentModes();
    };
}

#endif
