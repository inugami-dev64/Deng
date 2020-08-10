#ifndef RENDERER_H
#define RENDERER_H

#include "files.h"
#define DEBUG 1

#if DEBUG
    #define LOG(x) std::cout << x << "\n"
#else
    #define LOG(x)
#endif

namespace Deng {
    class Renderer
    {   
        private:
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

            std::vector<VkImage> m_swapChain_images;
            std::vector<VkImageView> m_swapChain_imageviews;
            std::vector<const char*> m_req_extensions_name;
            SwapChainDetails *m_device_swapChainDetails;
            Events *ev;
            Window *window;
            QueueFamilies m_queueFamilies;
            Queues m_queues;

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
            void initGraphicsPipeline();

            void deleteImageViews();
            void deleteSwapChain();
            void deleteInstance();
            void deleteSurface();
            void deleteDevice();
        public:
            void run();
            Renderer(Window &win);
            ~Renderer();
    };
}


#endif