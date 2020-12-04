#ifndef RENDERER_H
#define RENDERER_H

#if !DISABLE_DENGUI
    #define DENG_PIPELINE_COUNT 2
    #define DENGUI_PIPELINE_INDEX 1
#else
    #define DENG_PIPELINE_COUNT 2
    #define DENGUI_PIPELINE_INDEX 1
#endif

/* Generic default settings definitions */
#define DENG_DEFAULT_NEAR_PLANE 0.1f
#define DENG_DEFAULT_FAR_PLANE 25.0f
#define DENG_DEFAULT_MAX_FRAMES_IN_FLIGHT 2

namespace deng {
    class Renderer
    {   
    private:
        // Render usage specifications
        dengRendererUsageMode m_usage_mode;
        bool m_vsync;
        bool m_count_fps;
        dengMath::vec3<float> m_clear_color;

        // View distance settings
        const float m_near_plane = DENG_DEFAULT_NEAR_PLANE;
        const float m_far_plane = DENG_DEFAULT_FAR_PLANE;

        // Vulkan extension
        std::vector<const char*> m_required_extension_names;
        const char *m_p_validation_layer = "VK_LAYER_KHRONOS_validation";
        const char *m_p_fps_layer = "VK_LAYER_LUNARG_monitor";

        // Synchronisation (semophores and fences)
        size_t m_current_frame = 0;
        const int m_max_frames_in_flight = DENG_DEFAULT_MAX_FRAMES_IN_FLIGHT;
        std::vector<VkFence> m_flight_fences;
        std::vector<VkSemaphore> m_image_available_semaphore_set;
        std::vector<VkSemaphore> m_render_finished_semaphore_set;

        // Queues family structs
        QueueFamilies m_queue_families;
        Queues m_queues;
        
        // Pipelines, samplers and descriptor sets
        VkRenderPass m_renderpass;
        std::array<PipelineData, DENG_PIPELINE_COUNT> m_pipelines;
        VkPipelineLayout m_unmapped_pipeline_layout;
        VkPipelineLayout m_texture_mapped_pipeline_layout;
        VkDescriptorPool m_unmapped_descriptor_pool;
        VkDescriptorPool m_texture_mapped_descriptor_pool;
        VkDescriptorSetLayout m_unmapped_descriptor_set_layout;
        VkDescriptorSetLayout m_texture_mapped_descriptor_set_layout;
        std::vector<VkDescriptorSet> m_unmapped_descriptor_sets;
        VkSampler m_texture_sampler;

        /* Vulkan instance and surface related objects */
        VkInstance m_instance;
        VkPhysicalDevice m_gpu;
        VkDevice m_device;
        VkSurfaceKHR m_surface;
        VkSurfaceFormatKHR m_surface_format;
        VkDebugUtilsMessengerEXT m_debug_messenger;

        /* Swapchain related objects */
        VkSwapchainKHR m_swapchain;
        VkPresentModeKHR m_present_mode;
        VkExtent2D m_extent;
        std::vector<VkImage> m_swapchain_images;
        std::vector<VkImageView> m_swapchain_image_views;
        std::vector<VkFramebuffer> m_swapchain_framebuffers;
        SwapChainDetails *m_p_device_swapchain_details;

        /* Depth image datas */
        VkImage m_depth_image;
        VkDeviceMemory m_depth_image_memory;
        VkImageView m_depth_image_view;

        // Commandpools and commandbuffers
        VkCommandPool m_commandpool;
        std::vector<VkCommandBuffer> m_commandbuffers;
        
        // Assets
        std::vector<DENGasset> *m_p_game_objects;

        // Asset buffers and texture images
        BufferData m_buffer_data;
        std::vector<TextureImageData> m_texture_data;

        // Renderer utilities
        Camera *m_p_camera;
        dengMath::Events *m_p_ev;
        dengUtils::Timer m_timer;
        WindowWrap *m_p_window_wrap;
        
    private:
        void loadDataFromConf(const bool &load_camera_conf, const bool &load_environment_conf, const bool &load_dengUI_conf);
        void initInstance();
        void initDebugMessenger();
        bool checkValidationLayerSupport();
        void selectPhysicalDevice();
        void initLogicalDevice();
        void initWindowSurface();
        void initSwapChainSettings();
        void initSwapChain();
        VkImageViewCreateInfo getImageViewInfo(VkImage &image, const VkFormat &format, const VkImageAspectFlags &aspect_flags);
        void initImageView();
        void initRenderPass();
        void initDescriptorSetLayouts();
        void initPipelineLayouts();
        void initGraphicsPipelines();
        void initDengUI();
        void initFrameBuffers();
        void initDepthResources();
        void initTextureImage();
        void initTextureSampler();
        void initBuffers();
        void initDescriptorPool();
        void initMemoryOffsets();
        void initDescriptorSets(bool init_texture_mapped_descriptor_sets);
        void initTextureBuffer();
        void initCommandPool();
        void initCommandBuffers();
        void initSemaphores();
        
        void initGrid();

        void deleteTextureImage();
        void deleteCommandBuffers();
        void deleteSemaphores();
        void deleteCommandPool();
        void deleteFrameBuffers();
        void deletePipelines();
        void deleteRenderPass();
        void deleteImageViews();
        void deleteSwapChain();
        void deleteDescriptorSetLayout();
        void deleteBuffers();
        void deleteDepthImageData();
        void deleteDebugMessenger();
        void deleteInstance();
        void deleteSurface();
        void deleteDevice();

        void makeFrame();
        void updateUniformBufferData(const uint32_t &current_image);

        VkResult makeDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT  *p_messenger_createinfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data);

    public:
        void run();
        void submitAssets(std::vector<DENGasset> *p_game_objects);
        void submitTextures(std::vector<DENGtexture> *p_textures);
        VkDeviceSize getBufferMemoryOffset();
        void setHints(dengRendererHint hints);        
        Renderer(WindowWrap &win);
        ~Renderer();
    };
}


#endif