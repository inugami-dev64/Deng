#include "headers/renderer.h"

namespace deng {
    SwapChainDetails::SwapChainDetails(VkPhysicalDevice &gpu, VkSurfaceKHR &surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &this->capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);

        if(formatCount != 0) {
            this->formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, this->formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);

        if(presentModeCount != 0) {
            this->presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, this->presentModes.data());
        }
        else {
            ERR("No presentt modes available!");
        }
    }

    VkSurfaceCapabilitiesKHR SwapChainDetails::getCapabilities() {
        return this->capabilities;
    }

    std::vector<VkSurfaceFormatKHR> SwapChainDetails::getFormats() {
        return this->formats;
    }

    std::vector<VkPresentModeKHR> SwapChainDetails::getPresentModes() {
        return this->presentModes;
    }


}