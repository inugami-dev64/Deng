#ifndef __VULKAN_SD_H
#define __VULKAN_SD_H

#ifdef __VULKAN_SD_CPP
    #include <vector>
    #include <stdexcept>
    #include <vulkan/vulkan.h>

    #include <common/err_def.h>
    #include <common/base_types.h>
#endif

namespace deng {
    namespace vulkan {
        /*
         * Container to keep some specific information about swapchains
         */
        class SwapChainDetails {
        private:
            VkSurfaceCapabilitiesKHR m_capabilities;
            std::vector<VkSurfaceFormatKHR> m_formats;
            std::vector<VkPresentModeKHR> m_present_modes;

        public:
            SwapChainDetails (
                VkPhysicalDevice &gpu, 
                VkSurfaceKHR &surface
            );

            VkSurfaceCapabilitiesKHR getCapabilities();
            std::vector<VkSurfaceFormatKHR> getFormats();
            std::vector<VkPresentModeKHR> getPresentModes();
        };
    }
}

#endif
