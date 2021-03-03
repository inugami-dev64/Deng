#define __VULKAN_QM_CPP
#include <deng/vulkan/vulkan_qm.h>

namespace deng {
    namespace vulkan {
        /******************************/
        /******** QueueManager ********/
        /******************************/

        /*
         * Search for queue family that supports graphics queues
         * This method checks for all queue families available on given physical 
         * device and searches for one that supports graphics queues.
         */
        deng_bool_t QueueManager::findGraphicsFamily(const VkPhysicalDevice &gpu) {
            deng_ui32_t index;
            // Get the total count of queue families
            deng_ui32_t family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties (
                gpu, 
                &family_count, 
                nullptr
            );
            
            // Get properties of all queue families
            std::vector<VkQueueFamilyProperties> queue_family_properties(family_count);
            vkGetPhysicalDeviceQueueFamilyProperties (
                gpu, 
                &family_count, 
                queue_family_properties.data()
            );

            // Check which family is graphics family
            for(index = 0; index < family_count; index++) {
                if(queue_family_properties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    m_graphics_family_index = index;
                    return true;
                }
            }

            return false;
        }


        /*
         * Search for queue family that supports present queues
         * This method checks for all queue families available on given physical 
         * device and searches for one that supports present queues
         */
        deng_bool_t QueueManager::findPresentSupportFamily (
            const VkPhysicalDevice &gpu, 
            const VkSurfaceKHR &surface
        ) {
            deng_ui32_t index;
            // Get the total count of queue families
            deng_ui32_t family_count = 0;
            VkBool32 support = false;
            vkGetPhysicalDeviceQueueFamilyProperties (
                gpu, 
                &family_count, 
                nullptr
            );
            
            // Get properties of all queue families
            std::vector<VkQueueFamilyProperties> family_properties(family_count);
            vkGetPhysicalDeviceQueueFamilyProperties (
                gpu, 
                &family_count, 
                family_properties.data()
            );

            for(index = 0; index < family_count; index++) {
                vkGetPhysicalDeviceSurfaceSupportKHR (
                    gpu, 
                    index, 
                    surface, 
                    &support
                );

                if(support && m_graphics_family_index != index) {
                    m_present_family_index = index; 
                    return true;
                }
            }
            return false;
        }


        /* Graphics queuefamily getter method */
        deng_ui32_t QueueManager::getGraphicsQFIndex() { return m_graphics_family_index; }
        /* Present queue family getter method */
        deng_ui32_t QueueManager::getPresentQFIndex() { return m_present_family_index; }
    }
}
