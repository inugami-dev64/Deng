#include "headers/renderer.h"

namespace deng
{
    bool QueueFamilies::findGraphicsFamily(VkPhysicalDevice &gpu) {
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, queue_family_properties.data());

        for(uint32_t i = 0; i < family_count; i++) {
            if(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                this->graphicsFamilyIndex = i;
                LOG("Graphics queue family index: " + std::to_string(this->getGraphicsFamily()));
                return true;
            }
        }

        return false;
    }

    uint32_t QueueFamilies::getGraphicsFamily() {
        return this->graphicsFamilyIndex;
    }

    bool QueueFamilies::findPresentSupportFamily(VkPhysicalDevice &gpu, VkSurfaceKHR &surface) {
        uint32_t family_count = 0;
        VkBool32 support = false;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, family_properties.data());

        for(uint32_t i = 0; i < family_count; i++) {
            vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &support);
            if(support) {
                if(this->graphicsFamilyIndex != i) {
                    this->presentFamilyIndex = i; 
                    LOG("Presentation queue family index: " + std::to_string(this->presentFamilyIndex));
                    return true;
                }
            }
        }
        return false;
    }

    uint32_t QueueFamilies::getPresentFamily() {
        return this->presentFamilyIndex;
    }
} 