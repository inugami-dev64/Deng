#ifndef RENDERER_H
#define RENDERER_H

#include <vulkan/vulkan_core.h>
namespace deng {

    #ifdef __DENG_API_CORE
        
        /* Generic default settings definitions */
        #define DENG_DEFAULT_NEAR_PLANE 0.1f
        #define DENG_DEFAULT_FAR_PLANE 25.0f
        #define DENG_MAX_FRAMES_IN_FLIGHT 3
        
        /* Pipeline indices */
        #define DENG_PIPELINE_COUNT 4
        #define UM3D_I  0
        #define TM3D_I  1
        #define UM2D_I  2
        #define TM2D_I  3

        /* Shader binary file names */
        #define UNMAPPED_VERT_SHADER_3D             "shaders/bin/3d_unmapped_vert.spv"
        #define UNMAPPED_FRAG_SHADER_3D             "shaders/bin/3d_unmapped_frag.spv"
        #define TEXTURE_MAPPED_VERT_SHADER_3D       "shaders/bin/3d_tex_mapped_vert.spv"
        #define TEXTURE_MAPPED_FRAG_SHADER_3D       "shaders/bin/3d_tex_mapped_frag.spv"
        #define UNMAPPED_VERT_SHADER_2D             "shaders/bin/2d_unmapped_vert.spv"
        #define UNMAPPED_FRAG_SHADER_2D             "shaders/bin/2d_unmapped_frag.spv"
        #define TEXTURE_MAPPED_VERT_SHADER_2D       "shaders/bin/2d_tex_mapped_vert.spv"
        #define TEXTURE_MAPPED_FRAG_SHADER_2D       "shaders/bin/2d_tex_mapped_frag.spv"
    #endif


    /* Struct for storing all Vulkan instance objects */
    struct VulkanInstanceInfo {
        QueueFamilyFinder m_qff;
        VkInstance m_instance;
        VkSurfaceKHR m_surface;
        VkDebugUtilsMessengerEXT m_debug_mes;
    };


    /* Struct for storing all Vulkan swapchain related objects */
    struct VulkanSwapChainInfo {
        VkRenderPass m_renderpass;
        VkSwapchainKHR m_swapchain;
        VkPresentModeKHR m_present_mode;
        VkExtent2D m_extent;
        VkFormat m_format;
        VkSurfaceFormatKHR m_surface_format;
        std::vector<VkImage> m_swapchain_images;
        std::vector<VkImageView> m_swapchain_image_views;
        SwapChainDetails *m_p_sc_details;
    };


    /* Struct for storing all Vulkan descriptor related objects */
    struct VulkanDescriptorInfo {
        std::array<PipelineData, DENG_PIPELINE_COUNT> m_pipelines;
        std::vector<VkDescriptorSet> m_unmapped_descriptor_sets;
        VkPipelineLayout m_unmapped_pl;
        VkPipelineLayout m_texture_mapped_pl;
        VkDescriptorPool m_unmapped_desc_pool;
        VkDescriptorPool m_texture_mapped_desc_pool;
        VkDescriptorSetLayout m_unmapped_desc_set_layout;
        VkDescriptorSetLayout m_texture_mapped_desc_set_layout;
    };


    /* Struct for storing all Vulkan resource allocation related objects */
    struct VulkanResourceInfo {
        std::vector<VkFramebuffer> m_framebuffers;
        VkImage m_depth_image;
        VkDeviceMemory m_depth_image_mem;
        VkImageView m_depth_image_view;

        VkSampleCountFlagBits m_sample_count;
        VkImage m_color_image;
        VkDeviceMemory m_color_image_mem;
        VkImageView m_color_image_view;

        BufferData m_buffer_data;
    };


    /* This class is for picking appropriate physical device,       *
     * creating logical device and creating new instance for Vulkan */
    class InstanceCreator : private VulkanInstanceInfo, private VulkanDeviceInfo {
    private:
        // Supported device properties flags
        bool m_tex_linear_filtering_support;

        // Required vulkan extensions
        std::vector<const char*> m_required_extension_names;
        VkSampleCountFlagBits m_max_sample_count;
        const char *m_p_validation_layer = "VK_LAYER_KHRONOS_validation";

    private:
        void mkInstance(bool &enable_validation_layers);
        bool checkValidationLayerSupport();
        void findSupportedProperties();
        void mkDebugMessenger();
        void selectPhysicalDevice();
        void mkLogicalDevice(bool &enable_validation_layers);
        void mkWindowSurface();

        // Debug messenger
        VkResult mkDebugMessenger (
            const VkDebugUtilsMessengerCreateInfoEXT *p_messenger_createinfo
        );

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback (
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
            VkDebugUtilsMessageTypeFlagsEXT message_type, 
            const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, 
            void *p_user_data
        );

    public:
        InstanceCreator (
            WindowWrap *p_window_wrap, 
            bool enable_validation_layers
        );

        static void destroyDebugUtils (
            VkInstance instance,
            VkDebugUtilsMessengerEXT messenger
        );

    public:
        bool getLFSupport();
        VkInstance getIns();
        VkDevice getDev();
        VkSampleCountFlagBits getMaxSampleCount();
        VkPhysicalDevice getGpu();
        VkSurfaceKHR getSu();
        QueueFamilyFinder getQFF();
        VkDebugUtilsMessengerEXT getDMEXT();
    };


    /* Creating swapchains and swapchain image views */
    class SwapChainCreator : private VulkanSwapChainInfo, private VulkanDeviceInfo {
    private:
        QueueFamilyFinder m_qff;
        VkSampleCountFlagBits m_msaa_sample_c;
    
    private:
        void mkSwapChainSettings();
        void mkSwapChain (
            VkSurfaceKHR &surface, 
            deng_ui32_t g_queue_i, 
            deng_ui32_t p_queue_i
        );
        void mkRenderPass();
        void mkSCImageViews();
    
    public:
        SwapChainCreator (
            VkDevice device, 
            WindowWrap *p_win,
            VkPhysicalDevice gpu, 
            VkSurfaceKHR surface, 
            QueueFamilyFinder qff,
            VkSampleCountFlagBits sample_c
        );
        ~SwapChainCreator();
        void SCCleanup();

        /* System that checks for window resizing should be made */
        void remkSwapChain (
            VkDevice device,
            WindowWrap *p_win,
            VkSurfaceKHR surface, 
            VkSurfaceFormatKHR s_format
        );

    public:
        VkRenderPass getRp();
        VkExtent2D getExt();
        VkSwapchainKHR getSC();
        VkFormat getSF();
        std::vector<VkImage> getSCImg();
        std::vector<VkImageView> getSCImgViews();
    };  


    /* Create descriptor set layouts, pipelines, descriptor sets */
    class DescriptorCreator : private VulkanDescriptorInfo {
    private:
        std::vector<deng_Asset> *m_p_assets = NULL;
        std::vector<TextureImageData> *m_p_textures = NULL;
        deng_ui32_t m_texture_descriptor_size;

    private:
        void mkDescriptorSetLayouts(VkDevice &device);
        void mkPipelineLayouts(VkDevice &device);
        void mkGraphicsPipelines (
            VkDevice &device, 
            VkExtent2D &extent, 
            VkRenderPass &renderpass,
            VkSampleCountFlagBits sample_c
        );
        void mkDescriptorPools (
            VkDevice &device, 
            size_t sc_img_c
        );

    public:
        DescriptorCreator (
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderpass, 
            std::vector<deng_Asset> *p_assets, 
            std::vector<TextureImageData> *p_textures, 
            size_t sc_img_size,
            VkSampleCountFlagBits sample_c
        );
        
        void mkUnmappedDS (
            VkDevice device, 
            size_t sc_img_size, 
            BufferData buffer_data
        );
        
        void mkTexMappedDS (
            VkDevice device, 
            size_t sc_img_size, 
            BufferData buffer_data
        );

        void updateDescriptorPools (
            VkDevice device,
            size_t sc_img_c
        );

    public:
        std::array<PipelineData, DENG_PIPELINE_COUNT> getPipelines();
        VkPipelineLayout getUnmappedPL();
        VkPipelineLayout getTexMappedPL();
        std::vector<VkDescriptorSet> *getUnmappedDS();
        VkDescriptorSetLayout getUnmappedDSL();
        VkDescriptorSetLayout getTexMappedDSL();
        VkDescriptorPool getUnmappedDP();
        VkDescriptorPool getTexMappedDP();
    };


    /* Allocate buffer and image resources */ 
    class ResourceAllocator : private VulkanResourceInfo {
    private:
        std::vector<deng_Asset> *m_p_assets;
        std::vector<TextureImageData> *m_p_textures;

    private:
        void mkUniformBuffers (
            VkDevice &device, 
            VkPhysicalDevice &gpu, 
            size_t sc_img_size
        );
        void mkTextureSampler (
            VkDevice &device,
            VkSampler &sampler,
            deng_ui32_t mip_levels
        );
        void mkFrameBuffers (
            VkDevice &device, 
            VkRenderPass &renderpass, 
            VkExtent2D &extent, 
            std::vector<VkImageView> &sc_img_views
        );
        void mkColorResources (
            VkDevice &device,
            VkPhysicalDevice &gpu,
            VkExtent2D &extent,
            VkFormat sc_color_format
        );
        void mkDepthResources (
            VkDevice &device, 
            VkPhysicalDevice &gpu, 
            VkExtent2D &extent
        );
        void mkMipMaps (
            VkDevice &device,
            VkCommandPool &cmd_pool,
            VkImage image,
            VkQueue g_queue,
            deng_i32_t width,
            deng_i32_t height,
            deng_ui32_t mip_levels
        );
        
    public:
        ResourceAllocator (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkExtent2D extent, 
            VkSampleCountFlagBits sample_c,
            VkRenderPass renderpass, 
            std::vector<VkImageView> sc_img_views,
            VkFormat sc_color_format
        );
        
        void setAssetsData (
            std::vector<deng_Asset> *p_assets, 
            std::vector<TextureImageData> *p_textures
        );

        void mkTextureImages (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool command_pool,
            bool is_lf_supported, 
            VkQueue g_queue, 
            size_t sc_img_size
        );

        void mkBuffers (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool command_pool, 
            VkQueue g_queue
        );
        
        void reMkBuffers (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool command_pool,
            VkQueue g_queue,
            std::vector<VkCommandBuffer> *p_com_bufs
        );

        void updateUniformBufferData (
            VkDevice device, 
            const deng_ui32_t current_image, 
            Camera *p_camera, 
            deng_CameraUniformFlagBits flag_bits
        );

        void updateMainBuffer (
            VkDevice device,            
            VkBuffer *p_main_buf,
            VkDeviceMemory *p_main_buf_mem
        );
        
    public:
        BufferData getBD();
        std::vector<VkFramebuffer> getFB();
        VkImage getDepImg();
        VkDeviceMemory getDepImgMem();
        VkImageView getDepImgView();
        VkImage getColorImg();
        VkDeviceMemory getColorImgMem();
        VkImageView getColorImgView();
    };


    /* Make drawcalls and set up proper synchronisation */
    class DrawCaller {
    private:
        std::vector<deng_Asset> *m_p_assets = NULL;
        std::vector<TextureImageData> *m_p_textures = NULL;
        std::vector<VkFramebuffer> m_framebuffers;
        std::array<PipelineData, DENG_PIPELINE_COUNT> m_pl_data;
        std::vector<VkDescriptorSet> *m_p_unmapped_ds = NULL;
        QueueFamilyFinder m_qff;

        // Commandpools and commandbuffers
        VkCommandPool *m_p_commandpool = NULL;
        std::vector<VkCommandBuffer> *m_p_commandbuffers = NULL;

    private:
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
            QueueFamilyFinder qff
        );
        
        void setAssetsData (
            std::vector<deng_Asset> *p_assets, 
            std::vector<TextureImageData> *p_textures
        );
        
        void setMiscData (
            std::array<PipelineData, DENG_PIPELINE_COUNT> pl_data, 
            std::vector<VkFramebuffer> fb, 
            std::vector<VkDescriptorSet> *p_unmapped_ds
        );

        void mkCommandPool(VkDevice device);

        void recordDrawCommands (
            VkDevice device, 
            VkQueue g_queue, 
            VkRenderPass renderpass, 
            VkExtent2D extent,
            dengMath::vec4<float> background,
            BufferData buffer_data
        );

        void updateCommandBuffers (
            VkDevice device,
            std::vector<VkCommandBuffer> *p_cmd_bufs,
            VkCommandPool *p_cmd_pool
        );
    
    public:
        VkCommandPool *getComPool();
        std::vector<VkCommandBuffer> *getComBufs();
    };


    class Renderer {   
    private:
        InstanceCreator *m_p_ic = NULL;
        SwapChainCreator *m_p_scc = NULL;
        DescriptorCreator *m_p_desc_c;
        ResourceAllocator *m_p_ra = NULL;
        DrawCaller *m_p_dc = NULL;
        dengUtils::StringRasterizer *m_p_sr = NULL;
        std::mutex m_frame_mut;
        
        // Render usage specifications
        deng_RendererUsageMode m_usage_mode;
        bool m_enable_vsync;
        bool m_enable_validation_layers;
        bool m_count_fps;
        VkSampleCountFlagBits m_msaa_sample_count = VK_SAMPLE_COUNT_1_BIT;
        dengMath::vec3<float> m_clear_color;

        // View distance settings
        const float m_near_plane = DENG_DEFAULT_NEAR_PLANE;
        const float m_far_plane = DENG_DEFAULT_FAR_PLANE;

        // Assets, buffers and texture images
        std::vector<deng_Asset> m_assets;
        std::vector<TextureImageData> m_textures;
        std::mutex m_asset_mut;        

        // Renderer utilities
        Camera *m_p_camera;
        dengMath::Events *m_p_ev;
        dengUtils::Timer m_timer;
        WindowWrap *m_p_ww = NULL;

        // GUI objects
        bool m_cmd_update = false;
        std::mutex m_update_mut;
        dengui::MapEditor *m_p_map_editor;

    private:
        void cleanup();
        void makeFrame();

    public:
        void submitAssets (
            deng_Asset *assets, 
            deng_i32_t size
        );

        void submitTextures (
            deng_Texture *textures,
            deng_i32_t tex_c
        );
        
        void submitTextureFile (
            std::string tex_file, 
            std::string id
        );
        
        void submitRendStr (
            dengUtils::BitmapStr *rend_strs, 
            size_t size
        );

        void setHints (
            deng_RendererHintBits hints,
            WindowWrap *p_win
        );
        void initRenderer (
            WindowWrap *p_ww, 
            deng_RendererUsageMode usage, 
            dengMath::vec4<float> background
        );
        void run();        
    };
}


#endif
