#ifndef RENDERER_H
#define RENDERER_H

#include "objloader.h"

namespace deng {

    enum dengPipelines {
        DENG_PIPELINE_MAIN = 0,
        DENG_PIPELINE_GRID = 1
    };

    enum dengBufferType {
        DENG_BUFFER_TYPE_STAGING = 0,
        DENG_BUFFER_TYPE_VERTEX = 1,
        DENG_BUFFER_TYPE_INDICES = 2,
        DENG_BUFFER_TYPE_UNIFORM = 3,
        DENG_BUFFER_TYPE_GRID = 4
    };

    enum ImageType {
        DENG_IMAGE_TYPE_TEXTURE = 0,
        DENG_IMAGE_TYPE_DEPTH = 1
    };

    class Renderer
    {   
    private:
        const float m_nearPlane = 0.1f;
        const float m_farPlane = 10.0f;

        const char *m_p_validation_layer = "VK_LAYER_KHRONOS_validation";

        const int m_MAX_FRAMES_IN_FLIGHT = 2; 
        size_t m_current_frame = 0;
        vec2<PipelineData> m_pipeline_data;

        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debug_messenger;

        VkPhysicalDevice m_gpu;
        VkPhysicalDeviceProperties m_gpu_properties{};
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

        std::vector<VkSemaphore> m_image_available_semaphore_set;
        std::vector<VkSemaphore> m_render_finished_semaphore_set;
        std::vector<VkFence> m_flight_fences;

        std::vector<VkCommandBuffer> m_commandbuffers;

        // uniform buffers
        Buffers m_buffers;

        SwapChainDetails *m_p_device_swapchain_details;
        Camera *m_p_camera;
        Events *m_p_ev;
        Timer m_timer;
        Window *m_p_window;
        QueueFamilies m_queue_families;
        Queues m_queues;
        FileManager m_fm;
        DepthImageData m_depthimage_data;

        // config structs
        EditorEnvironmentConf m_environment_conf;
        EditorCameraConf m_camera_conf;

        //game objects (currently just hardcoded, soon to be added from editor in form of objects vector)
        GameObject m_sample_object;
        SpecialObject m_grid;

    private:
        void loadDataFromConf(const dengBool &load_camera_conf, const dengBool &load_environment_conf);
        void initInstance();
        void initDebugMessenger();
        bool checkValidationLayerSupport();
        uint32_t getDeviceScore(const VkPhysicalDevice &device);
        bool getExtensionSupport(const VkPhysicalDevice &gpu, const char *p_extension_name);
        void selectPhysicalDevice();
        void initLogicalDevice();
        void initWindowSurface();
        void initSwapChainSettings();
        void initSwapChain();
        VkImageViewCreateInfo getImageViewInfo(VkImage &image, const VkFormat &format, const VkImageAspectFlags &aspect_flags);
        void initImageView();
        VkShaderModule initShaderModule(const std::vector<char> &bin);
        void initRenderPass();
        void initGraphicsPipelines();
        void initFrameBuffers();
        void initDepthResources();
        void initTextureImage(GameObject &obj);
        void initTextureSampler(GameObject &obj);
        void initBuffers(GameObject &obj);
        void initDescriptorPool();
        void initDescriptorSets(GameObject &obj);
        void initTextureBuffer();
        uint32_t getMemType(const uint32_t &type_filter, const VkMemoryPropertyFlags &properties);
        void initCommandPool();
        void initCommandBufferFromSwapChain();
        void initSemaphores();
        void initObjects(GameObject &obj, const std::string &obj_source_filepath, const std::string &tex_source_filepath, const dengCoordinateMode &coordinate_mode);

        void deleteTextureImage(GameObject &obj);
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

        /* pipeline maker functions */
        void makeDescriptorSetLayout(const dengPipelineType &pipeline_type, VkDescriptorSetLayout *descriptor_set_layout);
        VkPipelineLayout makePipelineLayouts(VkDescriptorSetLayout &descriptor_set_layout);

        /* end of pipeline maker functions */

        void makeVertexBuffer();
        void makeFrame();
        void makeBuffer(const VkDeviceSize *p_size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, const dengBufferType &type, size_t *p_buffer_index);
        void makeImage(const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject *p_obj, const ImageType &image_mode);

        void populateBufferMem(const VkDeviceSize *p_size, const void *p_src_data, VkBuffer &buffer, VkDeviceMemory &buffer_memory);
        void copyBufferToBuffer(VkBuffer &src_buffer, VkBuffer &dst_buffer, const VkDeviceSize &size);

        void transitionImageLayout(VkImage &image, const VkFormat &format, const VkImageLayout &old_layout, const VkImageLayout &new_layout); 
        void copyBufferToImage(VkBuffer &src_buffer, VkImage &dst_image, const uint32_t &width, const uint32_t &height);

        void beginCommandBufferSingleCommand(VkCommandBuffer &commandbuffer);
        void endCommandBufferSingleCommand(VkCommandBuffer &commandbuffer);

        void updateUniformBufferData(const uint32_t &current_image, GameObject &obj);

        VkResult makeDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT  *p_messenger_createinfo);
        std::vector<const char*> getRequiredExtensions();


    public:
        void run();
        Renderer(Window &win);
        ~Renderer();
    };
}


#endif