#ifndef REND_HELPERS_H
#define REND_HELPERS_H

namespace deng {

    /* Contains different buffer objects for multiple purposes */
    struct BufferData {
        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;

        VkBuffer *p_main_buffer                 = NULL;
        VkDeviceMemory *p_main_buffer_memory    = NULL;
        
        std::vector<deng_ObjColorData> colors;

        std::vector<VkBuffer> mat_uniform_buffers;
        std::vector<VkDeviceMemory> mat_uniform_buffer_mem;
    };

    
    /* Contains texture image objects */
    struct TextureImageData {
        deng_Texture texture;
        VkImage image;
        VkImageView image_view;
        VkDeviceMemory image_mem;
        std::vector<VkDescriptorSet> descriptor_sets;
    };

    
    /* Contains different getter functions for hardware specs */
    struct HardwareSpecs {
        static bool getExtensionSupport (
            const VkPhysicalDevice &gpu, 
            const char *p_extension_name
        );

        static deng_ui32_t getMemoryType (
            VkPhysicalDevice *p_gpu, 
            const deng_ui32_t &type_filter, 
            const VkMemoryPropertyFlags &properties
        );

        static deng_ui32_t getDeviceScore (
            VkPhysicalDevice *p_device, 
            std::vector<const char*> &extenstions
        );
    };
    
    /* Contains different buffer handling functions */
    struct BufferCreator {
        static VkImageViewCreateInfo getImageViewInfo (
            VkImage &image, 
            const VkFormat &format, 
            const VkImageAspectFlags &aspect_flags
        );

        static void allocateMemory (
            VkDevice device, 
            VkPhysicalDevice gpu,  
            VkDeviceSize size, 
            VkDeviceMemory *p_memory,
            deng_ui32_t mem_type_bits, 
            VkMemoryPropertyFlags properties
        );

        /* VkImage related functions */
        static VkMemoryRequirements makeImage (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkImage &image, 
            deng_ui32_t width, 
            deng_ui32_t height, 
            VkFormat format, 
            VkImageTiling tiling, 
            VkImageUsageFlags usage
        );
        static void transitionImageLayout (
            VkDevice device, 
            VkImage &image, 
            VkCommandPool commandpool, 
            VkQueue g_queue, 
            VkFormat format, 
            VkImageLayout old_layout, 
            VkImageLayout new_layout
        ); 
        static void copyBufferToImage (
            VkDevice device, 
            VkCommandPool commandpool, 
            VkQueue g_queue, 
            VkBuffer src_buffer, 
            VkImage dst_image, 
            deng_ui32_t width, 
            deng_ui32_t height
        );
        
        /* VkBuffer related functions */
        static VkMemoryRequirements makeBuffer (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkDeviceSize size, 
            VkBufferUsageFlags usage, 
            VkBuffer *p_buffer
        );
        static void populateBufferMem (
            VkDevice device, 
            VkDeviceSize size, 
            void *src_data, 
            VkDeviceMemory buffer_memory, 
            VkDeviceSize offset
        );
        static void copyBufferToBuffer (
            VkDevice device, 
            VkCommandPool commandpool, 
            VkQueue g_queue, 
            VkBuffer src_buffer, 
            VkBuffer dst_buffer, 
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
        bool findGraphicsFamily(VkPhysicalDevice gpu);
        bool findPresentSupportFamily (
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
        VkShaderModule getShaderModule(std::vector<char> &shader_bins);
        VkVertexInputBindingDescription getBindingDesc();
        std::vector<VkVertexInputAttributeDescription> getAttributeDescs();
        
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
            bool add_depth_stencil, 
            bool add_color_blend, 
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