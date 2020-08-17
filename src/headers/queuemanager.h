#ifndef QUEUEFAMILIES_H
#define QUEUEFAMILIES_H
#include "events.h"

namespace deng {
    class QueueFamilies {
    private:
        uint32_t graphicsFamilyIndex;
        uint32_t presentFamilyIndex;
    
    public:
        bool findGraphicsFamily(VkPhysicalDevice &gpu);
        uint32_t getGraphicsFamily();

        bool findPresentSupportFamily(VkPhysicalDevice &gpu, VkSurfaceKHR &surface);
        uint32_t getPresentFamily();
    };

    class Queues {
        public:
            VkQueue graphicsQueue;
            VkQueue presentQueue;
    };
}

#endif