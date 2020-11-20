#ifndef RENDERER_H
#define RENDERER_H

#if !DISABLE_DENGUI
    #define DENG_PIPELINE_COUNT 2
    #define DENGUI_PIPELINE_INDEX 1
#else
    #define DENG_PIPELINE_COUNT 2
    #define DENGUI_PIPELINE_INDEX 1
#endif

namespace deng {

    class Renderer
    {   
    private:
        std::vector<const char*> m_required_extension_names;
        std::vector<dengMath::vec3<float>> m_dummy_vertices = {{0.0f, 0.0f, 0.0f}, 
                                                               {0.0f, 0.0f, 0.0f},
                                                               {0.0f, 0.0f, 0.0f}};

        const float m_near_plane = 0.1f;
        const float m_far_plane = 25.0f;

        const char *m_p_validation_layer = "VK_LAYER_KHRONOS_validation";

        const int m_max_frames_in_flight = 2; 
        size_t m_current_frame = 0;

        VkDeviceSize m_maximum_offset = 0;

        std::array<PipelineData, DENG_PIPELINE_COUNT> m_pipelines;

        dengMath::vec2<std::pair<dengPipelineType, VkDescriptorPool>> m_descriptor_pool_sets;
        dengMath::vec2<std::pair<dengPipelineType, VkPipelineLayout>> m_pipeline_layouts;
        dengMath::vec2<std::pair<dengPipelineType, VkDescriptorSetLayout>> m_descriptor_set_layouts;

        std::vector<VkDescriptorSet> m_unmapped_descriptor_sets;

        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debug_messenger;

        VkPhysicalDevice m_gpu;
        VkDevice m_device;
        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapchain;

        //swap chain settings
        VkSurfaceFormatKHR m_surface_format;
        VkPresentModeKHR m_present_mode;
        VkExtent2D m_extent;

        VkRenderPass m_renderpass;
        

        std::vector<VkFramebuffer> m_swapchain_framebuffers;
        VkCommandPool m_commandpool;

        std::vector<VkImage> m_swapchain_images;
        std::vector<VkImageView> m_swapchain_image_views;
        std::vector<const char*> m_required_extensions_name;
        std::vector<dengUtils::GameObject> *m_p_game_objects;

        std::vector<VkSemaphore> m_image_available_semaphore_set;
        std::vector<VkSemaphore> m_render_finished_semaphore_set;
        std::vector<VkFence> m_flight_fences;

        std::vector<VkCommandBuffer> m_commandbuffers;

        dengUtils::BufferData m_buffer_data;

        VkImage m_depth_image;
        VkDeviceMemory m_depth_image_memory;
        VkImageView m_depth_image_view;

        SwapChainDetails *m_p_device_swapchain_details;
        Camera *m_p_camera;
        dengMath::Events *m_p_ev;
        dengUtils::Timer m_timer;
        Window *m_p_window;
        QueueFamilies m_queue_families;
        Queues m_queues;
        dengUtils::FileManager m_fm;
        // dengUtils::GridManager *m_p_grid_manager;
        VkSampler m_sampler;

        // config structs
        dengUtils::DengUIConf m_dengui_conf;
        dengUtils::EditorEnvironmentConf m_environment_conf;
        dengUtils::EditorCameraConf m_camera_conf;

        //game objects (currently just hardcoded, soon to be added from editor in form of objects vector)
        // dengUI::Window *m_p_dengui_window;
        
        // std::vector<dengUI::UIVerticesData> m_dengui_window_vertices;
        // std::vector<uint16_t> m_dengui_window_indices;

    private:
        void loadDataFromConf(const dengBool &load_camera_conf, const dengBool &load_environment_conf, const dengBool &load_dengUI_conf);
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
        void initDescriptorSets(dengBool init_texture_mapped_descriptor_sets);
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

        /* frame update */
        void makeFrame();
        void updateUniformBufferData(const uint32_t &current_image);

        VkResult makeDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT  *p_messenger_createinfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data);

    public:
        void run();
        void initObjects(std::vector<dengUtils::GameObject> *p_game_objects);
        VkDeviceSize getBufferMemoryOffset();
        void setBufferMemoryOffset(VkDeviceSize size);
        Renderer(Window &win);
        ~Renderer();
    };
}


#endif