#ifndef RENDERER_H
#define RENDERER_H

#include "files.h"
#define DEBUG 0
#define ERR(x) throw std::runtime_error(x) 

#if DEBUG
    #define LOG(x) std::cout << x << "\n"
#else
    #define LOG(x)
#endif

namespace Deng {
    class Renderer
    {   
        private:
            const int m_MAX_FRAMES_IN_FLIGHT = 2; 
            size_t m_currentFrame = 0;

            VkInstance m_instance = nullptr;
            VkPhysicalDevice m_gpu = nullptr;
            VkPhysicalDeviceProperties m_gpu_properties{};
            VkPhysicalDeviceFeatures m_gpu_features{};
            VkDevice m_device = nullptr;
            VkSurfaceKHR m_surface;
            VkSwapchainKHR m_swapChain;

            //swap chain settings
            VkSurfaceFormatKHR m_surface_format;
            VkPresentModeKHR m_present_mode;
            VkExtent2D m_extent;

            VkRenderPass m_renderPass;
            VkPipelineLayout m_pipelineLayout;
            VkPipeline m_pipeline;

            std::vector<VkFramebuffer> m_swapChain_frameBuffers;
            VkCommandPool m_commandPool;
            std::vector<VkCommandBuffer> m_commandBuffers;

            std::vector<VkImage> m_swapChain_images;
            std::vector<VkImageView> m_swapChain_imageviews;
            std::vector<const char*> m_req_extensions_name;

            std::vector<VkSemaphore> m_imageAvailableSem_set;
            std::vector<VkSemaphore> m_renderFinishedSem_set;
            std::vector<VkFence> m_flightFences;

            SwapChainDetails *m_device_swapChainDetails;
            Events *ev;
            Window *window;
            QueueFamilies m_queueFamilies;
            Queues m_queues;
            FileManager fm;

        private:
            void initInstance();
            uint32_t getDeviceScore(const VkPhysicalDevice &device);
            bool getExtensionSupport(const VkPhysicalDevice &gpu, const char *extName);
            void selectPhysicalDevice();
            void initWindowSurface();
            void initDeviceQueues();
            void initSwapChainSettings();
            void initSwapChain();
            VkImageViewCreateInfo getImageViewInfo(uint32_t imageIndex);
            void initImageView();
            VkShaderModule initShaderModule(const std::vector<char> &bin);
            void initRenderPass();
            void initGraphicsPipeline();
            void initFrameBuffers();
            void initCommandPool();
            void initCommandBuffer();
            void initSemaphores();

            void deleteSemaphores();
            void deleteCommandPool();
            void deleteFrameBuffers();
            void deletePipeline();
            void deleteRenderPass();
            void deleteShaders(VkShaderModule &module);
            void deleteImageViews();
            void deleteSwapChain();
            void deleteInstance();
            void deleteSurface();
            void deleteDevice();

            void makeFrame();

        public:
            void run();
            Renderer(Window &win);
            ~Renderer();
    };
}


#endif