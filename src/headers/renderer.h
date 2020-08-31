#ifndef RENDERER_H
#define RENDERER_H

#include "objloader.h"

/*  BEFORE EVERY GIT COMMIT SET DEBUG VALUE TO 0!!!     */

#define DEBUG 0
#define ERR(x) throw std::runtime_error(x)
#define ERRME(x) std::cout << "\033[1;31m" << x << "\033[0m\n" 

#if DEBUG
    #define LOG(x) std::cout << x << "\n"
    const bool enable_validation_layers = true;
#else
    #define LOG(x)
    const bool enable_validation_layers = false;
#endif

namespace deng {

    enum BufferType {
        DENG_BUFFER_TYPE_STAGING = 0,
        DENG_BUFFER_TYPE_VERTEX = 1,
        DENG_BUFFER_TYPE_INDICES = 2,
        DENG_BUFFER_TYPE_UNIFORM = 3
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

        const char* m_validationLayer = "VK_LAYER_KHRONOS_validation";

        const int m_MAX_FRAMES_IN_FLIGHT = 2; 
        size_t m_currentFrame = 0;

        VkInstance m_instance;

        VkDebugUtilsMessengerEXT m_debug_messenger;

        VkPhysicalDevice m_gpu;
        VkPhysicalDeviceProperties m_gpu_properties{};
        VkDevice m_device;
        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapChain;

        //swap chain settings
        VkSurfaceFormatKHR m_surface_format;
        VkPresentModeKHR m_present_mode;
        VkExtent2D m_extent;

        VkRenderPass m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
        VkDescriptorSetLayout m_descriptorSet_Layout;


        std::vector<VkFramebuffer> m_swapChain_frameBuffers;
        VkCommandPool m_commandPool;
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;

        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<VkImage> m_swapChain_images;
        std::vector<VkImageView> m_swapChain_imageviews;
        std::vector<const char*> m_req_extensions_name;

        std::vector<VkSemaphore> m_imageAvailableSem_set;
        std::vector<VkSemaphore> m_renderFinishedSem_set;
        std::vector<VkFence> m_flightFences;

        // uniform buffers
        std::vector<VkBuffer> m_uniform_buffers;
        std::vector<VkDeviceMemory> m_uniform_buffersMem;

        SwapChainDetails *m_device_swapChainDetails;
        Camera *m_camera;
        Events *m_ev;
        Timer m_timer;
        Window *m_window;
        QueueFamilies m_queueFamilies;
        Queues m_queues;
        FileManager fm;
        Buffers buffers;
        DepthImageData m_depthImage_data;

        //game objects (currently just hardcoded, soon to be added from editor in form of objects vector)
        GameObject m_sample_object;

    private:
        void initInstance();
        void initDebugMessenger();
        bool checkValidationLayerSupport();
        uint32_t getDeviceScore(const VkPhysicalDevice &device);
        bool getExtensionSupport(const VkPhysicalDevice &gpu, const char *extName);
        void selectPhysicalDevice();
        void initLogicalDevice();
        void initWindowSurface();
        void initSwapChainSettings();
        void initSwapChain();
        VkImageViewCreateInfo getImageViewInfo(VkImage &image, const VkFormat &format, const VkImageAspectFlags &aspectFlags);
        void initImageView();
        VkShaderModule initShaderModule(const std::vector<char> &bin);
        void initRenderPass();
        void initDescriptorSetLayout();
        void initGraphicsPipeline();
        void initFrameBuffers();
        void initCommandPool();
        void initDepthResources();
        void initTextureImage(GameObject &obj);
        void initTextureSampler(GameObject &obj);
        void initBuffers(GameObject &obj);
        void initDescriptorPool();
        void initDescriptorSets(GameObject &obj);
        void initTextureBuffer();
        uint32_t getMemType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void initCommandBufferFromSwapChain();
        void initSemaphores();
        void initObjects(GameObject &obj, const std::string &objSourceFilePath, const std::string &texSourceFilePath, const CoordinateMode &coordinateMode);

        void deleteTextureImage(GameObject &obj);
        void deleteCommandBuffers();
        void deleteSemaphores();
        void deleteCommandPool();
        void deleteFrameBuffers();
        void deletePipeline();
        void deleteRenderPass();
        void deleteShaders(VkShaderModule &module);
        void deleteImageViews();
        void deleteSwapChain();
        void deleteDescriptorSetLayout();
        void deleteVertexBuffer();
        void freeMemory();
        void deleteDepthImageData();
        void deleteDebugMessenger();
        void deleteInstance();
        void deleteSurface();
        void deleteDevice();

        void makeVertexBuffer();
        void makeFrame();
        void makeBuffer(const VkDeviceSize *size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject &obj, const BufferType &type, size_t *bufferIndex);
        void makeImage(const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject *obj, const ImageType &imageMode);

        void populateBufferMem(const VkDeviceSize *size, const void *srcData, VkBuffer &buffer, VkDeviceMemory &bufferMem);
        void copyBufferToBuffer(VkBuffer &srcBuf, VkBuffer &dstBuf, const VkDeviceSize &size);

        void transitionImageLayout(VkImage &image, const VkFormat &format, const VkImageLayout &oldLayout, const VkImageLayout &newLayout); 
        void copyBufferToImage(VkBuffer &srcBuf, VkImage &dstImg, const uint32_t &width, const uint32_t &height);

        void beginCommandBufferSingleCommand(VkCommandBuffer &commandBuffer);
        void endCommandBufferSingleCommand(VkCommandBuffer &commandBuffer);

        void updateUniformBufferData(const uint32_t &currentImg, GameObject &obj);

        VkResult createDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo);
        std::vector<const char*> getRequiredExtensions();


    public:
        void run();
        Renderer(Window &win);
        ~Renderer();
    };
}


#endif