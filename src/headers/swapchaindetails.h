#ifndef SWAPCHAINDETAILS_H
#define SWAPCHAINDETAILS_H
#include "queuemanager.h"

namespace deng {
    class SwapChainDetails {
    private:
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

    public:
        SwapChainDetails(VkPhysicalDevice &gpu, VkSurfaceKHR &surface);
        VkSurfaceCapabilitiesKHR getCapabilities();
        std::vector<VkSurfaceFormatKHR> getFormats();
        std::vector<VkPresentModeKHR> getPresentModes();

    };
}

#endif