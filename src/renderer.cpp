#include "headers/renderer.h"

namespace Deng
{
    Renderer::Renderer(Window &win) {
        //Required extensions vector initialisation
        this->m_req_extensions_name.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        this->window = &win;
        this->initInstance();
        this->initWindowSurface();
        this->selectPhysicalDevice();
        this->initDeviceQueues();
        this->initSwapChainSettings();
        this->initSwapChain();
        this->initImageView();
    }

    Renderer::~Renderer() {
        this->deleteImageViews();
        this->deleteSwapChain();
        this->deleteDevice();
        this->deleteSurface();
        this->deleteInstance();
        delete this->m_device_swapChainDetails;
        delete this->ev;
    }

    void Renderer::deleteInstance() {
        vkDestroyInstance(this->m_instance, nullptr);
        this->m_instance = nullptr;
    }

    void Renderer::deleteDevice() {
        vkDestroyDevice(this->m_device, nullptr);
        this->m_gpu = nullptr;
    }

    void Renderer::deleteSurface() {
        vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
        this->m_surface = nullptr;
    }

    void Renderer::deleteSwapChain() {
        vkDestroySwapchainKHR(this->m_device, this->m_swapChain, nullptr);
        this->m_swapChain = nullptr;
    }

    void Renderer::deleteImageViews() {
        for(VkImageView &imageView : this->m_swapChain_imageviews) {
            vkDestroyImageView(this->m_device, imageView, nullptr);
        }
    }

    //Function that initialises instance 
    void Renderer::initInstance() {
        //initialise appinfo
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = this->window->getTitle();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Deng";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //initialise create info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        //get count of required extensions
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        LOG("GLFW extension count: " + std::to_string(glfwExtensionCount));
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        LOG(glfwExtensions);

        if(vkCreateInstance(&createInfo, nullptr, &this->m_instance) != VK_SUCCESS) {
            LOG("Failed to create an instance!");
        }

        else LOG("Successfully created an instance");
    }

    void Renderer::initWindowSurface() {
        LOG("Initialising window surface!");
        if(glfwCreateWindowSurface(this->m_instance, this->window->getWindow(), nullptr, &this->m_surface) != VK_SUCCESS) {
            LOG("Failed to create window surface!");
        }
        else {
            LOG("Window surface created successfully!");
        }
    }

    //Function that selects physical device to use 
    void Renderer::selectPhysicalDevice() {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, nullptr);

        if(deviceCount == 0) LOG("Failed to find graphics cards!");
        else {
            std::vector<VkPhysicalDevice> devices(deviceCount);
            std::multimap<uint32_t, VkPhysicalDevice> deviceCandidates;
            VkResult result = vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, devices.data());
            if(result != VK_SUCCESS) LOG("Failed to count physical GPUs!");

            for(uint32_t i = 0; i < deviceCount; i++) {
                uint32_t score = this->getDeviceScore(devices[i]);
                LOG("Score for device " + std::to_string(i) + ": " + std::to_string(score));
                SwapChainDetails swapChainDetails(devices[i], this->m_surface);

                if(!swapChainDetails.getFormats().empty() && !swapChainDetails.getPresentModes().empty()) {
                    deviceCandidates.insert(std::make_pair(score, devices[i]));
                }
            }

            if(!deviceCandidates.empty() && deviceCandidates.rbegin()->first > 0) {
                this->m_gpu = deviceCandidates.rbegin()->second;
                this->m_device_swapChainDetails = new SwapChainDetails(this->m_gpu, this->m_surface);
            }

            else LOG("Failed to find suitable GPU!");
            LOG(this->m_gpu);
        }
    }

    uint32_t Renderer::getDeviceScore(const VkPhysicalDevice &device) {
        uint32_t score = 0;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &this->m_gpu_properties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if(this->m_gpu_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
        score += this->m_gpu_properties.limits.maxImageDimension2D;
        score += this->m_gpu_properties.limits.maxImageDimension3D;
        score += this->m_gpu_properties.limits.maxMemoryAllocationCount;
        score += this->m_gpu_properties.limits.maxVertexOutputComponents;

        if(!deviceFeatures.geometryShader) return 0;

        for(const char* extName : this->m_req_extensions_name) {
            if(!this->getExtensionSupport(device, extName)) return 0;
        }

        return score;
    }

    bool Renderer::getExtensionSupport(const VkPhysicalDevice &gpu, const char *extName) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> allExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, allExtensions.data());
        
        for(VkExtensionProperties &extension : allExtensions) {
            const char *currentExtName = extension.extensionName;
            if(std::string(currentExtName) == std::string(extName)) return true;
        }

        return false;
    }

    void Renderer::initDeviceQueues() {
        if(!this->m_queueFamilies.findGraphicsFamily(this->m_gpu) || !this->m_queueFamilies.findPresentSupportFamily(this->m_gpu, this->m_surface)) {
            LOG("Queue supporting GPU not found!");
        }

        else {
            uint32_t queueFamilies_indexes[] = {this->m_queueFamilies.getGraphicsFamily(), this->m_queueFamilies.getPresentFamily()};
            std::vector<VkDeviceQueueCreateInfo> queueInfos;

            float queue_priority = 1.0f;
            for(uint32_t i = 0; i < 2; i++) {
                VkDeviceQueueCreateInfo dev_queue_create_info{};
                dev_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                dev_queue_create_info.queueFamilyIndex = queueFamilies_indexes[i];
                dev_queue_create_info.queueCount = 1;
                dev_queue_create_info.pQueuePriorities = &queue_priority;
                queueInfos.push_back(dev_queue_create_info);                
            } 

            VkDeviceCreateInfo dev_create_info{};
            dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            dev_create_info.queueCreateInfoCount = queueInfos.size();
            dev_create_info.pQueueCreateInfos = queueInfos.data();
            dev_create_info.pEnabledFeatures = &this->m_gpu_features;
            dev_create_info.enabledExtensionCount = this->m_req_extensions_name.size();
            dev_create_info.ppEnabledExtensionNames = this->m_req_extensions_name.data();


            if(vkCreateDevice(this->m_gpu, &dev_create_info, nullptr, &this->m_device) != VK_SUCCESS) {
                LOG("Failed to create logical device!");
            }
            else {
                LOG("Logical device created successfully!");
                vkGetDeviceQueue(this->m_device, this->m_queueFamilies.getGraphicsFamily(), 0, &this->m_queues.graphicsQueue);
                LOG("Device graphics queue recieved successfully!");
                vkGetDeviceQueue(this->m_device, this->m_queueFamilies.getPresentFamily(), 0, &this->m_queues.presentQueue);
                LOG("Device present queue recieved successfully!");
            }
        }
    }

    void Renderer::initSwapChainSettings() {
        bool foundSuitableFormat = false;
        for(const VkSurfaceFormatKHR &surfaceFormat : this->m_device_swapChainDetails->getFormats()) {
            if(surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                this->m_surface_format = surfaceFormat;
                foundSuitableFormat = true;
                LOG("Found suitable surface format!");
            }
        }
        
        if(!foundSuitableFormat) LOG("Didn't find suitable surface format! Using first format!"), this->m_surface_format = this->m_device_swapChainDetails->getFormats()[0];

        bool foundSuitablePresentMode = false;
        for(const VkPresentModeKHR &presentFormat : this->m_device_swapChainDetails->getPresentModes()) {
            if(presentFormat == VK_PRESENT_MODE_MAILBOX_KHR) {
                this->m_present_mode = presentFormat;
                foundSuitablePresentMode = true;
                LOG("Found suitable present mode for triple buffering!");
            }
        }

        if(!foundSuitablePresentMode) LOG("Didn't find suitable present mode! Using Vsync instead!"), this->m_present_mode = VK_PRESENT_MODE_FIFO_KHR;

        if(this->m_device_swapChainDetails->getCapabilities().currentExtent.width != UINT32_MAX && this->m_device_swapChainDetails->getCapabilities().currentExtent.height != UINT32_MAX) {
            this->m_extent = this->m_device_swapChainDetails->getCapabilities().currentExtent;
        }
        else {
            this->m_extent.width = this->window->getSize().x;
            this->m_extent.height = this->window->getSize().y;
        }

        LOG("Successfully initialised swap chain settings!");
    }

    void Renderer::initSwapChain() {
        uint32_t minImageCount = this->m_device_swapChainDetails->getCapabilities().minImageCount + 1;
        if(this->m_device_swapChainDetails->getCapabilities().maxImageCount > 0 && minImageCount > this->m_device_swapChainDetails->getCapabilities().maxImageCount) {
            minImageCount = this->m_device_swapChainDetails->getCapabilities().maxImageCount;
        }

        VkSwapchainCreateInfoKHR local_createInfo{};
        local_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        local_createInfo.surface = this->m_surface;
        local_createInfo.minImageCount = minImageCount;
        local_createInfo.imageFormat = this->m_surface_format.format;
        local_createInfo.imageColorSpace = this->m_surface_format.colorSpace;
        local_createInfo.imageExtent = this->m_extent;
        local_createInfo.imageArrayLayers = 1;
        local_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        if(this->m_queueFamilies.getGraphicsFamily() != this->m_queueFamilies.getPresentFamily()) {
            uint32_t queueFamilyIndices[] = {this->m_queueFamilies.getGraphicsFamily(), this->m_queueFamilies.getPresentFamily()};
            local_createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            local_createInfo.queueFamilyIndexCount = 2;
            local_createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }

        else {
            local_createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        local_createInfo.preTransform = this->m_device_swapChainDetails->getCapabilities().currentTransform;
        local_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        local_createInfo.presentMode = this->m_present_mode;
        local_createInfo.clipped = VK_TRUE;
        local_createInfo.oldSwapchain = VK_NULL_HANDLE;
        LOG("Filled out createInfo structure!");

        if(vkCreateSwapchainKHR(this->m_device, &local_createInfo, nullptr, &this->m_swapChain) != VK_SUCCESS) {
            LOG("Failed to create swap chain!");
        }
        else {
            LOG("Swap chain created successfully!");
            uint32_t imageCount;
            vkGetSwapchainImagesKHR(this->m_device, this->m_swapChain, &imageCount, nullptr);
            this->m_swapChain_images.resize(imageCount);
            vkGetSwapchainImagesKHR(this->m_device, this->m_swapChain, &imageCount, this->m_swapChain_images.data());
            
        }
    }

    VkImageViewCreateInfo Renderer::getImageViewInfo(uint32_t imageIndex) {
            VkImageViewCreateInfo local_createInfo{};
            local_createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            local_createInfo.image = this->m_swapChain_images[imageIndex];
            local_createInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
            local_createInfo.format = this->m_surface_format.format;

            local_createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            local_createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            local_createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            local_createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            local_createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            local_createInfo.subresourceRange.baseMipLevel = 0;
            local_createInfo.subresourceRange.levelCount = 1;
            local_createInfo.subresourceRange.baseArrayLayer = 0;
            local_createInfo.subresourceRange.layerCount = 1;
            return local_createInfo;
    }

    void Renderer::initImageView() {
        this->m_swapChain_imageviews.resize(this->m_swapChain_images.size());
        for(uint32_t i = 0; i < this->m_swapChain_imageviews.size(); i++) {
            VkImageViewCreateInfo local_createInfo = this->getImageViewInfo(i);
            if(vkCreateImageView(this->m_device, &local_createInfo, nullptr, &this->m_swapChain_imageviews[i]) != VK_SUCCESS) {
                LOG("Failed to create image views!");
            }
        }
    }


    void Renderer::run() {
        while(!glfwWindowShouldClose(this->window->getWindow())) {
            glfwPollEvents();
        }
        glfwTerminate();
    }


    
} 