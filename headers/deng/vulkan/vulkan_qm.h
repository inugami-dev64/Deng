#ifndef __VULKAN_QM_H
#define __VULKAN_QM_H

#ifdef __VULKAN_QM_CPP
    #include <vector>
    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
#endif

namespace deng {
    namespace vulkan {
        /*
         * Contains information about queues and their family indices
         */
        class QueueManager {
        private:
            deng_ui32_t m_graphics_family_index;
            deng_ui32_t m_present_family_index;
        
        public:
            VkQueue graphics_queue;
            VkQueue present_queue;

        public:
            /*
             * Search for queue family that supports graphics queues
             * This method checks for all queue families available on given physical 
             * device and searches for one that supports graphics queues.
             */
            deng_bool_t findGraphicsFamily(const VkPhysicalDevice &gpu);

            /*
             * Search for queue family that supports present queues
             * This method checks for all queue families available on given physical 
             * device and searches for one that supports present queues
             */
            deng_bool_t findPresentSupportFamily (
                const VkPhysicalDevice &gpu, 
                const VkSurfaceKHR &surface
            );

            deng_ui32_t getGraphicsQFIndex();
            deng_ui32_t getPresentQFIndex();
        };
    }
}

#endif
