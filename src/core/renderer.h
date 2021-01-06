#ifndef RENDERER_H
#define RENDERER_H

namespace deng {

    #ifdef __DENG_API_CORE
        #define DENG_PIPELINE_COUNT 4
        
        /* Generic default settings definitions */
        #define DENG_DEFAULT_NEAR_PLANE 0.1f
        #define DENG_DEFAULT_FAR_PLANE 25.0f
        #define DENG_MAX_FRAMES_IN_FLIGHT 3
    #endif
    /* Picking appropriate physical device, creating logical    *
     * device and creating new instance for Vulkan              */
    class InstanceCreator {
    private:
        // Required vulkan extensions
        std::vector<const char*> m_required_extension_names;
        const char *m_p_validation_layer = "VK_LAYER_KHRONOS_validation";

        // Queues family objects
        QueueFamilies m_queue_families;
        Queues m_queues;

        WindowWrap *m_p_window_wrap;
        VkInstance m_instance;
        VkPhysicalDevice m_gpu;
        VkDevice m_device;
        VkSurfaceKHR m_surface;
        VkDebugUtilsMessengerEXT m_debug_messenger;

    private:
        void mkInstance(bool &enable_validation_layers);
        bool checkValidationLayerSupport();
        void mkDebugMessenger();
        void selectPhysicalDevice();
        void mkLogicalDevice(bool &enable_validation_layers);
        void mkWindowSurface();

        // Debug messenger
        VkResult mkDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT  *p_messenger_createinfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, 
        const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data);

    public:
        InstanceCreator(WindowWrap *p_window_wrap, bool enable_validation_layers);

    public:
        VkInstance getIns();
        VkDevice getDev();
        VkPhysicalDevice getGpu();
        VkSurfaceKHR getSu();
        VkQueue getGQ();
        VkQueue getPQ();
        uint32_t getGQi();
        uint32_t getPQi();
        VkDebugUtilsMessengerEXT getDMEXT();
    };


    /* Creating swapchains and swapchain image views */
    class SwapChainCreator {
    private:
        VkDevice m_device;
        WindowWrap *m_p_window_wrap;
        VkRenderPass m_renderpass;
        VkSwapchainKHR m_swapchain;
        VkPresentModeKHR m_present_mode;
        VkExtent2D m_extent;
        VkSurfaceFormatKHR m_surface_format;
        std::vector<VkImage> m_swapchain_images;
        std::vector<VkImageView> m_swapchain_image_views;
        SwapChainDetails *m_p_device_swapchain_details;

    private:
        void mkSwapChainSettings();
        void mkSwapChain(VkSurfaceKHR &surface, uint32_t g_queue_i, uint32_t p_queue_i);
        void mkRenderPass();
        void mkSCImageViews();
        void SCCleanup();
    
    public:
        SwapChainCreator(WindowWrap *p_window_wrap, VkDevice device, VkPhysicalDevice gpu, VkSurfaceKHR surface, uint32_t g_queue_i, uint32_t p_queue_i);
        ~SwapChainCreator();
        void remkSwapChain(VkSurfaceKHR surface, VkSurfaceFormatKHR s_format, uint32_t g_queue_i, uint32_t p_queue_i);

    public:
        VkRenderPass getRp();
        VkExtent2D getExt();
        VkSwapchainKHR getSC();
        std::vector<VkImage> getSCImg();
        std::vector<VkImageView> getSCImgViews();
    };  


    /* Create descriptor set layouts, pipelines, descriptor sets */
    class DescriptorCreator {
    private:
        std::vector<DENGAsset> *m_p_assets;
        std::vector<TextureImageData> *m_p_textures;

        uint32_t m_texture_descriptor_size;

        std::array<PipelineData, DENG_PIPELINE_COUNT> m_pipelines;
        VkPipelineLayout m_unmapped_pipeline_layout;
        VkPipelineLayout m_texture_mapped_pipeline_layout;
        VkDescriptorPool m_unmapped_descriptor_pool;
        VkDescriptorPool m_texture_mapped_descriptor_pool;
        VkDescriptorSetLayout m_unmapped_descriptor_set_layout;
        VkDescriptorSetLayout m_texture_mapped_descriptor_set_layout;
        std::vector<VkDescriptorSet> m_unmapped_descriptor_sets;

    private:
        void mkDescriptorSetLayouts(VkDevice &device);
        void mkPipelineLayouts(VkDevice &device);
        void mkGraphicsPipelines(VkDevice &device, VkExtent2D &extent, VkRenderPass &renderpass);
        void mkDescriptorPools(VkDevice &device, size_t sc_img_size);

    public:
        DescriptorCreator(VkDevice device, VkExtent2D extent, VkRenderPass renderpass, std::vector<DENGAsset> *p_assets, std::vector<TextureImageData> *p_textures, size_t sc_img_size);
        void mkUnmappedDS(VkDevice device, size_t sc_img_size, BufferData buffer_data);
        void mkTexMappedDS(VkDevice device, size_t sc_img_size, VkSampler tex_sampler, BufferData buffer_data);

    public:
        std::array<PipelineData, DENG_PIPELINE_COUNT> getPipelines();
        std::vector<VkDescriptorSet> getUnmappedDS();
        VkDescriptorSetLayout getUnmappedDSL();
        VkDescriptorSetLayout getTexMappedDSL();
        VkDescriptorPool getUnmappedDP();
        VkDescriptorPool getTexMappedDP();
    };


    /* Allocate buffer and image resources */ 
    class ResourceAllocator {
    private:
        std::vector<DENGAsset> *m_p_assets;
        std::vector<TextureImageData> *m_p_textures;
        std::vector<VkFramebuffer> m_framebuffers;

        /* Depth image datas */
        VkImage m_depth_image;
        VkDeviceMemory m_depth_image_memory;
        VkImageView m_depth_image_view;
        
        VkSampler m_texture_sampler;
        BufferData m_buffer_data;

    private:
        void mkUniformBuffers(VkDevice &device, VkPhysicalDevice &gpu, size_t sc_img_size);
        void mkTextureSampler(VkDevice &device);
        void mkFrameBuffers(VkDevice &device, VkRenderPass &renderpass, VkExtent2D &extent, std::vector<VkImageView> &sc_img_views);
        void mkDepthResources(VkDevice &device, VkPhysicalDevice &gpu, VkExtent2D &extent);
        
    public:
        ResourceAllocator(VkDevice device, VkPhysicalDevice gpu, VkExtent2D extent, VkRenderPass renderpass, std::vector<VkImageView> sc_img_views);
        
        void setAssetsData(std::vector<DENGAsset> *p_assets, std::vector<TextureImageData> *p_textures);
        void mkTextureImages(VkDevice device, VkPhysicalDevice gpu, VkCommandPool command_pool, VkQueue g_queue, size_t sc_img_size);
        void mkBuffers(VkDevice device, VkPhysicalDevice gpu, VkCommandPool command_pool, VkQueue g_queue);
        void updateUniformBufferData(VkDevice device, const uint32_t current_image, Camera *p_camera, dengCameraUniformFlagBits flag_bits);
        
    public:
        BufferData getBD();
        VkSampler getSamp();
        std::vector<VkFramebuffer> getFB();
        VkImage getDepImg();
        VkDeviceMemory getDepImgMem();
        VkImageView getDepImgView();
    };


    /* Make drawcalls and set up proper synchronisation */
    class DrawCaller {
    private:
        std::vector<DENGAsset> *m_p_assets;
        std::vector<TextureImageData> *m_p_textures;

        std::array<PipelineData, DENG_PIPELINE_COUNT> m_pl_data;
        std::vector<VkFramebuffer> m_framebuffers;
        std::vector<VkDescriptorSet> m_unmapped_ds;
        BufferData m_buffer_data;

        // Commandpools and commandbuffers
        VkCommandPool m_commandpool;
        std::vector<VkCommandBuffer> m_commandbuffers;

    private:
        void mkCommandPool(VkDevice &device, uint32_t &g_queue_i);
        void mkSynchronisation(VkDevice &device);
        TextureImageData findTextureImageDataByID(char *id);

    public:
        // Needed for synchronising frames
        size_t current_frame = 0;
        std::vector<VkFence> flight_fences;
        std::vector<VkSemaphore> image_available_semaphore_set;
        std::vector<VkSemaphore> render_finished_semaphore_set;

    public:
        DrawCaller (
            VkDevice device, 
            uint32_t g_queue_i
        );
        void setAssetsData (
            std::vector<DENGAsset> *p_assets, 
            std::vector<TextureImageData> *p_textures
        );
        void setMiscData (
            std::array<PipelineData, DENG_PIPELINE_COUNT> pl_data, 
            std::vector<VkFramebuffer> fb, 
            std::vector<VkDescriptorSet> unmapped_ds, 
            BufferData bd
        );
        void recordDrawCommands (
            VkDevice device, 
            VkQueue g_queue, 
            VkRenderPass renderpass, 
            VkExtent2D extent,
            dengMath::vec4<float> background
        );
    
    public:
        VkCommandPool getComPool();
        std::vector<VkCommandBuffer> *getComBufs();
    };


    class Renderer {   
    private:
        InstanceCreator *m_p_ic;
        SwapChainCreator *m_p_scc;
        DescriptorCreator *m_p_desc_c;
        ResourceAllocator *m_p_ra;
        DrawCaller *m_p_dc;
        dengUtils::FontManager *m_p_fm;
        
        // Render usage specifications
        dengRendererUsageMode m_usage_mode;
        bool m_enable_vsync;
        bool m_enable_validation_layers;
        bool m_count_fps;
        dengMath::vec3<float> m_clear_color;

        // View distance settings
        const float m_near_plane = DENG_DEFAULT_NEAR_PLANE;
        const float m_far_plane = DENG_DEFAULT_FAR_PLANE;

        // Assets, buffers and texture images
        std::vector<DENGAsset> m_assets;
        std::vector<TextureImageData> m_textures;        

        // Renderer utilities
        Camera *m_p_camera;
        dengMath::Events *m_p_ev;
        dengUtils::Timer m_timer;
        WindowWrap *m_p_ww;

        // GUI windows 
        dengui::Window *m_p_map_gui;
        dengui::Events *m_p_gui_ev;

    private:
        void cleanup();
        void makeFrame();
        void initMapEditor();

    public:
        void submitAssets(DENGAsset *p_game_objects, size_t size);
        void submitTexture(std::string tex_file, std::string id);
        void submitRendStr(dengUtils::bitmapStr *rend_strs, size_t size);
        void setHints(dengRendererHintBits hints);
        void initRenderer(WindowWrap *p_ww, dengRendererUsageMode usage, dengMath::vec4<float> background);
        void run();        
    };
}


#endif