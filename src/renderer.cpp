#include "headers/renderer.h"

namespace deng
{
    Renderer::Renderer(Window &win) {
        //Required extensions vector initialisation
        this->m_req_extensions_name.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        this->m_window = &win; 
        this->m_camera = new Camera({1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 65.0f, this->m_nearPlane, this->m_farPlane, this->m_window);
        this->m_ev = new Events(this->m_window, this->m_camera, &this->m_sample_object);

        this->initObjects(this->m_sample_object, "objects/obj1.obj", "textures/obj1.tga", DENG_COORDINATE_MODE_DEFAULT);
        this->initInstance();
        this->initDebugMessenger();
        this->initWindowSurface();
        this->selectPhysicalDevice();
        this->initLogicalDevice();
        this->initSwapChainSettings();
        this->initSwapChain();
        this->initImageView();
        this->initRenderPass();
        this->initDescriptorSetLayout();
        this->initGraphicsPipeline();
        this->initCommandPool();
        this->initDepthResources();
        this->initFrameBuffers();
        this->initTextureImage(this->m_sample_object); 
        this->initTextureSampler(this->m_sample_object);
        this->initBuffers(this->m_sample_object);
        this->initDescriptorPool();
        this->initDescriptorSets(this->m_sample_object);
        this->initCommandBufferFromSwapChain();
        this->initSemaphores();
    }

    Renderer::~Renderer() {
        this->deleteFrameBuffers();
        this->deleteCommandBuffers();
        this->deletePipeline();
        this->deleteRenderPass();
        this->deleteImageViews();
        this->deleteSwapChain();
        this->deleteTextureImage(this->m_sample_object);
        this->deleteDescriptorSetLayout();
        this->deleteVertexBuffer();
        this->freeMemory();
        this->deleteDepthImageData();
        this->deleteSemaphores();
        this->deleteCommandPool();
        this->deleteDevice();
        this->deleteSurface();
        this->deleteDebugMessenger();
        this->deleteInstance();
        
        delete this->m_ev;
        delete this->m_camera;
        delete this->m_device_swapChainDetails;
    }

    void Renderer::deleteCommandBuffers() {
        vkFreeCommandBuffers(this->m_device, this->m_commandPool, this->m_commandBuffers.size(), this->m_commandBuffers.data());
    }

    void Renderer::deleteDebugMessenger() {
        auto fun = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(this->m_instance, "vkDestroyDebugUtilsMessengerEXT");
        if(fun != nullptr) {
            fun(this->m_instance, this->m_debug_messenger, nullptr);
        }
    }

    void Renderer::deleteInstance() {
        vkDestroyInstance(this->m_instance, nullptr);
    }

    void Renderer::deleteDevice() {
        vkDestroyDevice(this->m_device, nullptr);
    }

    void Renderer::deleteSurface() {
        vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
    }

    void Renderer::deleteSwapChain() {
        vkDestroySwapchainKHR(this->m_device, this->m_swapChain, nullptr);

        for(size_t i = 0; i < this->m_swapChain_images.size(); i++) {
            vkDestroyBuffer(this->m_device, this->m_uniform_buffers[i], nullptr);
            vkFreeMemory(this->m_device, this->m_uniform_buffersMem[i], nullptr);
        }

        vkDestroyDescriptorPool(this->m_device, this->m_descriptorPool, nullptr);
    }

    void Renderer::deleteImageViews() {
        for(VkImageView &imageView : this->m_swapChain_imageviews) {
            vkDestroyImageView(this->m_device, imageView, nullptr);
        }
    }

    void Renderer::deleteCommandPool() {
        vkDestroyCommandPool(this->m_device, this->m_commandPool, nullptr);
    }

    void Renderer::deleteShaders(VkShaderModule &module) {
        vkDestroyShaderModule(this->m_device, module, nullptr);
    }

    void Renderer::deleteRenderPass() {
        vkDestroyRenderPass(this->m_device, this->m_renderPass, nullptr);
    }

    void Renderer::deletePipeline() {
        vkDestroyPipeline(this->m_device, this->m_pipeline, nullptr);
        vkDestroyPipelineLayout(this->m_device, this->m_pipelineLayout, nullptr);
    }

    void Renderer::deleteFrameBuffers() {
        for(VkFramebuffer &frameBuffer : this->m_swapChain_frameBuffers) {
            vkDestroyFramebuffer(this->m_device, frameBuffer, nullptr);
        }
    }

    void Renderer::deleteSemaphores() {
        for(int32_t i = 0; i < this->m_MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(this->m_device, this->m_imageAvailableSem_set[i], nullptr);
            vkDestroySemaphore(this->m_device, this->m_renderFinishedSem_set[i], nullptr);
            vkDestroyFence(this->m_device, this->m_flightFences[i], nullptr);
        }
    }

    void Renderer::freeMemory() {
        vkFreeMemory(this->m_device, this->buffers.vertex_bufferMem, nullptr);
        // vkFreeMemory(this->m_device, this->m_sample_object.buffers.index_bufferMem, nullptr);
    }

    void Renderer::deleteVertexBuffer() {
        vkDestroyBuffer(this->m_device, this->buffers.vertex_buffer, nullptr);
        // vkDestroyBuffer(this->m_device, this->m_sample_object.buffers.index_buffer, nullptr);
    }

    void Renderer::deleteTextureImage(GameObject &obj) {
        vkDestroySampler(this->m_device, obj.textureData.textureSampler, nullptr);
        vkDestroyImageView(this->m_device, obj.textureData.textureImageView, nullptr);
        vkDestroyImage(this->m_device, obj.textureData.textureImage, nullptr);
        vkFreeMemory(this->m_device, obj.textureData.textureImageMem, nullptr);
    }

    void Renderer::deleteDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(this->m_device, this->m_descriptorSet_Layout, nullptr);
    }

    void Renderer::deleteDepthImageData() {
        vkDestroyImageView(this->m_device, this->m_depthImage_data.depthImageView, nullptr);
        vkDestroyImage(this->m_device, this->m_depthImage_data.depthImage, nullptr);
        vkFreeMemory(this->m_device, this->m_depthImage_data.depthImageMem, nullptr);
    }

    void Renderer::initObjects(GameObject &obj, const std::string &objFilePath, const std::string &texFilePath, const CoordinateMode &coordinateMode) {
        ObjLoader obj_loader(objFilePath, coordinateMode);
        obj_loader.getObjVerticesAndIndices(obj);

        TextureFormats local_tex_format = getTexFileFormat(texFilePath);
        ObjRawTextureData texture_data;

        // obj.vertexData = {{{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
        //                   {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        //                   {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}},
        //                   {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
        //                   {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        //                   {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}}};

        switch (local_tex_format)
        {
        case DENG_TEXTURE_FORMAT_BMP: {
            TextureLoaderBMP local_tex_loader(texFilePath);
            local_tex_loader.getTextureDetails(texture_data.width, texture_data.height, texture_data.texSize, texture_data.texturePixelsData);
            break;
        }

        case DENG_TEXTURE_FORMAT_TGA: {
            TextureLoaderTGA local_tex_loader(texFilePath);
            local_tex_loader.getTextureDetails(texture_data.width, texture_data.height, texture_data.texSize, texture_data.texturePixelsData);
            break;
        }

        case DENG_TEXTURE_FORMAT_PNG:
            ERR(".png textures are not yet supported");
            break;

        case DENG_TEXTURE_FORMAT_JPG:
            ERR(".jpg textures are not yet supported");
            break;

        case DENG_TEXTURE_FORMAT_UNKNOWN:
            ERR("Unknown texture file format!");
            break;
        
        default:
            break;
        }

        obj.rawTextureData = texture_data;

        obj.modelMatrix.setRotation(0, 0, 0);
        obj.modelMatrix.setScale(1, 1, 1);
        obj.modelMatrix.setTransformation(0, 0, 0);
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    //Function that initialises instance 
    void Renderer::initInstance() {
        //initialise appinfo
        VkApplicationInfo local_appInfo{};
        local_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        local_appInfo.pApplicationName = this->m_window->getTitle();
        local_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 3);
        local_appInfo.pEngineName = "Deng";
        local_appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        local_appInfo.apiVersion = VK_API_VERSION_1_0;

        //initialise create info
        VkInstanceCreateInfo local_instance_createInfo{}; 
        local_instance_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        local_instance_createInfo.pApplicationInfo = &local_appInfo;

        //get count of required extensions
        std::vector<const char*> local_extensions = this->getRequiredExtensions();
        local_instance_createInfo.enabledExtensionCount = local_extensions.size();
        local_instance_createInfo.ppEnabledExtensionNames = local_extensions.data();
        
        VkDebugUtilsMessengerCreateInfoEXT local_debug_createInfo{};
        if(enable_validation_layers && !this->checkValidationLayerSupport()) {
            ERR("No validation layers available!");
        }

        else if(enable_validation_layers && this->checkValidationLayerSupport()) {
            local_instance_createInfo.enabledLayerCount = 1;
            local_instance_createInfo.ppEnabledLayerNames = &this->m_validationLayer;

            local_debug_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            local_debug_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            local_debug_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            local_debug_createInfo.pfnUserCallback = debugCallback;

            local_instance_createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &local_debug_createInfo;
        }

        else if(!enable_validation_layers) {
            local_instance_createInfo.enabledLayerCount = 0;
            local_instance_createInfo.pNext = nullptr;
        }

        if(vkCreateInstance(&local_instance_createInfo, nullptr, &this->m_instance) != VK_SUCCESS) {
            ERR("Failed to create an instance!");
        }

        else LOG("Successfully created an instance");
    }

    std::vector<const char*> Renderer::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> local_extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if(enable_validation_layers) local_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        LOG("GLFW extension count: " + std::to_string(glfwExtensionCount));

        return local_extensions;
    }

    VkResult Renderer::createDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo) {
        auto fun = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(this->m_instance, "vkCreateDebugUtilsMessengerEXT");
        if(fun == nullptr) {
            ERRME("Couldn't find vkCreateDebugUtilsMessengerEXT locations!");
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
        else {
            fun(this->m_instance, pCreateInfo, nullptr, &this->m_debug_messenger);
            return VK_SUCCESS;
        }
    }

    void Renderer::initDebugMessenger() {
        if(!enable_validation_layers) return;
        else {
            VkDebugUtilsMessengerCreateInfoEXT local_messenger_createInfo{};
            local_messenger_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            local_messenger_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            local_messenger_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            local_messenger_createInfo.pfnUserCallback = debugCallback;

            if(this->createDebugMessenger(&local_messenger_createInfo) != VK_SUCCESS) {
                ERR("Couldn't create debug messenger!");
            }
            else {
                LOG("Debug messenger created successfully!");
            }

        }
    }

    bool Renderer::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> local_available_layers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, local_available_layers.data());
        bool isLayer = false;

        for(const VkLayerProperties &properties : local_available_layers) {
            if(strcmp(this->m_validationLayer, properties.layerName) == 0) {
                isLayer = true;
                break;
            }
        }

        return isLayer;
    }

    void Renderer::initWindowSurface() {
        LOG("Initialising window surface!");
        if(glfwCreateWindowSurface(this->m_instance, this->m_window->getWindow(), nullptr, &this->m_surface) != VK_SUCCESS) {
            ERR("Failed to create window surface!");
        }
        else {
            LOG("Window surface created successfully!");
        }
    }

    //Function that selects physical device to use 
    void Renderer::selectPhysicalDevice() {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, nullptr);

        if(deviceCount == 0) {
            ERR("Failed to find graphics cards!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        std::multimap<uint32_t, VkPhysicalDevice> deviceCandidates;
        VkResult result = vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, devices.data());
        if(result != VK_SUCCESS) ERR("Failed to count physical GPUs!");

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

        else ERR("Failed to find suitable GPU!");
        LOG(this->m_gpu);
        
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
        if(!deviceFeatures.samplerAnisotropy) return 0;

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

    void Renderer::initLogicalDevice() {
        if(!this->m_queueFamilies.findGraphicsFamily(this->m_gpu) || !this->m_queueFamilies.findPresentSupportFamily(this->m_gpu, this->m_surface)) {
            ERR("Queue supporting GPU not found!");
        }

        std::vector<VkDeviceQueueCreateInfo> queueInfos;
        uint32_t queueFamilies_indexes[] = {this->m_queueFamilies.getGraphicsFamily(), this->m_queueFamilies.getPresentFamily()};

        // queueInfos.resize(2);
        float queue_priority = 1.0f;

        for(uint32_t i = 0; i < 2; i++) {
            VkDeviceQueueCreateInfo dev_queue_create_info{};
            dev_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            dev_queue_create_info.queueFamilyIndex = queueFamilies_indexes[i];
            dev_queue_create_info.queueCount = 1;
            dev_queue_create_info.pQueuePriorities = &queue_priority;
            queueInfos.push_back(dev_queue_create_info);                
        } 

        VkPhysicalDeviceFeatures local_devFeatures{};
        local_devFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo local_logicalDev_create_info{};
        local_logicalDev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        local_logicalDev_create_info.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
        local_logicalDev_create_info.pQueueCreateInfos = queueInfos.data();
        local_logicalDev_create_info.pEnabledFeatures = &local_devFeatures;
        local_logicalDev_create_info.enabledExtensionCount = this->m_req_extensions_name.size();
        local_logicalDev_create_info.ppEnabledExtensionNames = this->m_req_extensions_name.data();

        if(enable_validation_layers) {
            local_logicalDev_create_info.enabledLayerCount = 1;
            local_logicalDev_create_info.ppEnabledLayerNames = &this->m_validationLayer;
        }

        else {
            local_logicalDev_create_info.enabledLayerCount = 0;
        }

        if(vkCreateDevice(this->m_gpu, &local_logicalDev_create_info, nullptr, &this->m_device) != VK_SUCCESS) {
            ERR("Failed to create logical device!");
        }

        vkGetDeviceQueue(this->m_device, this->m_queueFamilies.getGraphicsFamily(), 0, &this->m_queues.graphicsQueue);
        LOG("Device graphics queue recieved successfully!");
        vkGetDeviceQueue(this->m_device, this->m_queueFamilies.getPresentFamily(), 0, &this->m_queues.presentQueue);
        LOG("Device present queue recieved successfully!");
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
        
        if(!foundSuitableFormat) {
            ERRME("Didn't find suitable surface format! Using first format!");
            this->m_surface_format = this->m_device_swapChainDetails->getFormats()[0];
        }

        bool foundSuitablePresentMode = false;
        for(const VkPresentModeKHR &presentFormat : this->m_device_swapChainDetails->getPresentModes()) {

            // LOGGING
            switch (presentFormat)
            {
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                LOG("VK_PRESENT_MODE_IMMEDIATE_KHR is available!");
                break;

            case VK_PRESENT_MODE_MAILBOX_KHR:
                LOG("VK_PRESENT_MODE_MAILBOX_KHR is available!");
                break;

            case VK_PRESENT_MODE_FIFO_KHR:
                LOG("VK_PRESENT_MODE_FIFO_KHR is available!");
                break;

            case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
                LOG("VK_PRESENT_MODE_FIFO_KHR is available!");
                break;

            case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
                LOG("VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR is available!");
                break;

            case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
                LOG("VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR is available!");
                break;

            default:
                break;
            }

            if(presentFormat == VK_PRESENT_MODE_MAILBOX_KHR) {
                this->m_present_mode = presentFormat;
                foundSuitablePresentMode = true;
                LOG("Found suitable present mode for triple buffering!");
            }
        }

        if(!foundSuitablePresentMode) {
            ERRME("Didn't find suitable present mode! Using immediate instead!");
            this->m_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        if(this->m_device_swapChainDetails->getCapabilities().currentExtent.width != UINT32_MAX && this->m_device_swapChainDetails->getCapabilities().currentExtent.height != UINT32_MAX) {
            this->m_extent = this->m_device_swapChainDetails->getCapabilities().currentExtent;
        }
        else {
            this->m_extent.width = this->m_window->getSize().x;
            this->m_extent.height = this->m_window->getSize().y;
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
        LOG("Filled out createInfo structure!");

        if(vkCreateSwapchainKHR(this->m_device, &local_createInfo, nullptr, &this->m_swapChain) != VK_SUCCESS) {
            ERR("Failed to create swap chain!");
        }
        else {
            LOG("Swap chain created successfully!");
            uint32_t imageCount;
            vkGetSwapchainImagesKHR(this->m_device, this->m_swapChain, &imageCount, nullptr);
            this->m_swapChain_images.resize(imageCount);
            vkGetSwapchainImagesKHR(this->m_device, this->m_swapChain, &imageCount, this->m_swapChain_images.data());
            
        }
    }

    VkImageViewCreateInfo Renderer::getImageViewInfo(VkImage &image, const VkFormat &format, const VkImageAspectFlags &aspectFlags) {
            VkImageViewCreateInfo local_createInfo{};
            local_createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            local_createInfo.image = image;
            local_createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            local_createInfo.format = format;

            local_createInfo.subresourceRange.aspectMask = aspectFlags;
            local_createInfo.subresourceRange.baseMipLevel = 0;
            local_createInfo.subresourceRange.levelCount = 1;
            local_createInfo.subresourceRange.baseArrayLayer = 0;
            local_createInfo.subresourceRange.layerCount = 1;
            return local_createInfo;
    }

    void Renderer::initImageView() {
        this->m_swapChain_imageviews.resize(this->m_swapChain_images.size());
        for(uint32_t i = 0; i < this->m_swapChain_imageviews.size(); i++) {
            VkImageViewCreateInfo local_createInfo = this->getImageViewInfo(this->m_swapChain_images[i], this->m_surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT);
            if(vkCreateImageView(this->m_device, &local_createInfo, nullptr, &this->m_swapChain_imageviews[i]) != VK_SUCCESS) {
                ERR("Failed to create image views!");
            }
        }
    }

    VkShaderModule Renderer::initShaderModule(const std::vector<char> &bin) {
        VkShaderModuleCreateInfo local_createinfo{};
        local_createinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        local_createinfo.codeSize = bin.size();
        local_createinfo.pCode = reinterpret_cast<const uint32_t*>(bin.data());
        VkShaderModule shaderModule;
        if(vkCreateShaderModule(this->m_device, &local_createinfo, nullptr, &shaderModule) != VK_SUCCESS) {
            ERR("Failed to create shader module!");
        }

        return shaderModule;
    }

    void Renderer::initRenderPass() {
        VkAttachmentDescription local_colorAttachment{};
        local_colorAttachment.format = this->m_surface_format.format;
        local_colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        local_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        local_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        local_colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        local_colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        local_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentDescription local_depthAttachment{};
        local_depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        local_depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        local_depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        local_depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        local_depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        local_depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        local_depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference local_colorAttachment_ref{};
        local_colorAttachment_ref.attachment = 0;
        local_colorAttachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference local_depthAttachment_ref{};
        local_depthAttachment_ref.attachment = 1;
        local_depthAttachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription local_subpass_desc{};
        local_subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        local_subpass_desc.colorAttachmentCount = 1;
        local_subpass_desc.pColorAttachments = &local_colorAttachment_ref;
        local_subpass_desc.pDepthStencilAttachment = &local_depthAttachment_ref;

        VkSubpassDependency local_subpass_dependency{};
        local_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        local_subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        local_subpass_dependency.srcAccessMask = 0;
        local_subpass_dependency.dstSubpass = 0;
        local_subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        local_subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> local_attachments = {local_colorAttachment, local_depthAttachment};
        VkRenderPassCreateInfo local_renderPass_createInfo{};
        local_renderPass_createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        local_renderPass_createInfo.attachmentCount = local_attachments.size();
        local_renderPass_createInfo.pAttachments = local_attachments.data();
        local_renderPass_createInfo.subpassCount = 1;
        local_renderPass_createInfo.pSubpasses = &local_subpass_desc;
        local_renderPass_createInfo.dependencyCount = 1;
        local_renderPass_createInfo.pDependencies = &local_subpass_dependency;


        if(vkCreateRenderPass(this->m_device, &local_renderPass_createInfo, nullptr, &this->m_renderPass) != VK_SUCCESS) {
            LOG("Failed to create render pass!");
        }
    }

    void Renderer::initDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding local_ubo_layout_binding{};
        local_ubo_layout_binding.binding = 0;
        local_ubo_layout_binding.descriptorCount = 1;
        local_ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_ubo_layout_binding.pImmutableSamplers = nullptr;
        local_ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding local_sampler_layout_binding{};
        local_sampler_layout_binding.binding = 1;
        local_sampler_layout_binding.descriptorCount = 1;
        local_sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        local_sampler_layout_binding.pImmutableSamplers = nullptr;
        local_sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> local_bindings = {local_ubo_layout_binding, local_sampler_layout_binding};

        VkDescriptorSetLayoutCreateInfo local_layout_createInfo{};
        local_layout_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_descriptorSet_Layout) != VK_SUCCESS) {
            ERR("Failed to create descriptor set layout!");
        }

    }

    void Renderer::initGraphicsPipeline() {
        std::vector<char> vertShaderBinVec;
        std::vector<char> fragShaderBinVec;
        this->fm.getFileContents("shaders/vert.spv", &vertShaderBinVec, nullptr);
        this->fm.getFileContents("shaders/frag.spv", &fragShaderBinVec, nullptr);

        VkShaderModule vertexModule = this->initShaderModule(vertShaderBinVec);
        VkShaderModule fragModule = this->initShaderModule(fragShaderBinVec);

        VkPipelineShaderStageCreateInfo local_vModule_createInfo{};
        local_vModule_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        local_vModule_createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        local_vModule_createInfo.module = vertexModule;
        local_vModule_createInfo.pName = "main";

        VkPipelineShaderStageCreateInfo local_fModule_createinfo{};
        local_fModule_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        local_fModule_createinfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        local_fModule_createinfo.module = fragModule;
        local_fModule_createinfo.pName = "main";

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {local_vModule_createInfo, local_fModule_createinfo};

        auto local_input_binding_desc = ObjVertexData::getBindingDesc();
        auto local_input_attribute_desc = ObjVertexData::getAttributeDesc();

        VkPipelineVertexInputStateCreateInfo local_vertexInput_createInfo{};
        local_vertexInput_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        local_vertexInput_createInfo.vertexBindingDescriptionCount = 1;
        local_vertexInput_createInfo.vertexAttributeDescriptionCount = local_input_attribute_desc.size();
        local_vertexInput_createInfo.pVertexBindingDescriptions = &local_input_binding_desc;
        local_vertexInput_createInfo.pVertexAttributeDescriptions = local_input_attribute_desc.data();


        VkPipelineInputAssemblyStateCreateInfo local_inputAssembly_createInfo{};
        local_inputAssembly_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        local_inputAssembly_createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        local_inputAssembly_createInfo.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) this->m_extent.width;
        viewport.height = (float) this->m_extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = this->m_extent;

        VkPipelineViewportStateCreateInfo local_viewport_state_createinfo{};
        local_viewport_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        local_viewport_state_createinfo.viewportCount = 1;
        local_viewport_state_createinfo.pViewports = &viewport;
        local_viewport_state_createinfo.scissorCount = 1;
        local_viewport_state_createinfo.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo local_rasterizer{};
        local_rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        local_rasterizer.depthClampEnable = VK_FALSE;
        local_rasterizer.rasterizerDiscardEnable = VK_FALSE;
        local_rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        local_rasterizer.lineWidth = 1.0f;
        local_rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        local_rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        local_rasterizer.depthBiasEnable = VK_TRUE;

        VkPipelineMultisampleStateCreateInfo local_multisampling{};
        local_multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        local_multisampling.sampleShadingEnable = VK_FALSE;
        local_multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState local_colorBlendAttachment{};
        local_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        local_colorBlendAttachment.blendEnable =  VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo local_depthStencil{};
        local_depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        local_depthStencil.depthTestEnable = VK_TRUE;
        local_depthStencil.depthWriteEnable = VK_TRUE;
        local_depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        local_depthStencil.depthBoundsTestEnable = VK_FALSE;
        local_depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo local_colorblend_state_createinfo{};
        local_colorblend_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        local_colorblend_state_createinfo.logicOpEnable = VK_FALSE;
        local_colorblend_state_createinfo.attachmentCount = 1;
        local_colorblend_state_createinfo.pAttachments = &local_colorBlendAttachment;
        
        VkPipelineLayoutCreateInfo local_pipelineLayout_createinfo{};
        local_pipelineLayout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        local_pipelineLayout_createinfo.setLayoutCount = 1;
        local_pipelineLayout_createinfo.pSetLayouts = &this->m_descriptorSet_Layout;
        local_pipelineLayout_createinfo.pushConstantRangeCount = 0;

        // VkPipelineDynamicStateCreateInfo local_pipeline_dynamicState_createinfo{};
        // local_pipeline_dynamicState_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

        if(vkCreatePipelineLayout(this->m_device, &local_pipelineLayout_createinfo, nullptr, &this->m_pipelineLayout) != VK_SUCCESS) {
            ERR("Failed to create pipeline layout!");
        } 

        else {
            LOG("Pipeline layout created successfully!");
            VkGraphicsPipelineCreateInfo local_pipelineInfo{};
            local_pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            local_pipelineInfo.stageCount = shaderStages.size();
            local_pipelineInfo.pStages = shaderStages.data();
            local_pipelineInfo.pVertexInputState = &local_vertexInput_createInfo;
            local_pipelineInfo.pInputAssemblyState = &local_inputAssembly_createInfo;
            local_pipelineInfo.pViewportState = &local_viewport_state_createinfo;
            local_pipelineInfo.pRasterizationState = &local_rasterizer;
            local_pipelineInfo.pMultisampleState = &local_multisampling;
            local_pipelineInfo.pColorBlendState = &local_colorblend_state_createinfo;
            local_pipelineInfo.pDepthStencilState = &local_depthStencil;
            local_pipelineInfo.layout = this->m_pipelineLayout;
            local_pipelineInfo.renderPass = this->m_renderPass;
            local_pipelineInfo.subpass = 0;
            local_pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;   

            LOG("Pipeline create info created!");
            if(vkCreateGraphicsPipelines(this->m_device, VK_NULL_HANDLE, 1, &local_pipelineInfo, nullptr, &this->m_pipeline) != VK_SUCCESS) {
                ERR("Failed to create graphics pipeline!");
            }

            else {
                LOG("Graphics pipeline created successfully!");
            }
        }

        this->deleteShaders(vertexModule);
        this->deleteShaders(fragModule);
        LOG("Exiting initGraphicsPipeline function!");
    }

    void Renderer::initFrameBuffers() {
        this->m_swapChain_frameBuffers.resize(this->m_swapChain_imageviews.size());

        for(size_t i = 0; i < this->m_swapChain_imageviews.size(); i++) {
            std::array<VkImageView, 2> attachments = {this->m_swapChain_imageviews[i], this->m_depthImage_data.depthImageView};

            VkFramebufferCreateInfo local_framebuffer_createinfo{};
            local_framebuffer_createinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            local_framebuffer_createinfo.renderPass = this->m_renderPass;
            local_framebuffer_createinfo.attachmentCount = attachments.size();
            local_framebuffer_createinfo.pAttachments = attachments.data();
            local_framebuffer_createinfo.width = this->m_extent.width;
            local_framebuffer_createinfo.height = this->m_extent.height;
            local_framebuffer_createinfo.layers = 1;

            if(vkCreateFramebuffer(this->m_device, &local_framebuffer_createinfo, nullptr, &this->m_swapChain_frameBuffers[i]) != VK_SUCCESS) {
                ERR("Failed to create framebuffer!");
            }
            else {
                LOG("Successfully created framebuffer!");
            }
        }
    }

    void Renderer::initCommandPool() {
        VkCommandPoolCreateInfo local_commandPool_createinfo{};
        local_commandPool_createinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        local_commandPool_createinfo.queueFamilyIndex = this->m_queueFamilies.getGraphicsFamily();

        if(vkCreateCommandPool(this->m_device, &local_commandPool_createinfo, nullptr, &this->m_commandPool) != VK_SUCCESS) {
            ERR("Failed to create command pool!");
        }
        else {
            LOG("Successfully created commandpool!");
        }
    }

    uint32_t Renderer::getMemType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties local_memProperties;
        vkGetPhysicalDeviceMemoryProperties(this->m_gpu, &local_memProperties);

        for(uint32_t i = 0; i < local_memProperties.memoryTypeCount; i++) {
            if(typeFilter & (1 << i) && (local_memProperties.memoryTypes[i].propertyFlags & properties)) {
                LOG("Vertex buffer mem type: " + std::to_string(i));
                return i;
            }
        }

        ERR("Failed to find suitable memory type");
    }

    void Renderer::initDepthResources() {
        this->makeImage(VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, nullptr, DENG_IMAGE_TYPE_DEPTH);
        
        VkImageViewCreateInfo local_imgView_createinfo = this->getImageViewInfo(this->m_depthImage_data.depthImage, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);

        if(vkCreateImageView(this->m_device, &local_imgView_createinfo, nullptr, &this->m_depthImage_data.depthImageView) != VK_SUCCESS) {
            ERR("Failed to create depth image view!");
        }

    }

    void Renderer::initTextureImage(GameObject &obj) {
        this->makeBuffer(obj.rawTextureData.texSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj, DENG_BUFFER_TYPE_STAGING, nullptr);
        LOG("Successfully created texture staging buffer!");
        this->populateBufferMem(obj.rawTextureData.texSize, obj.rawTextureData.texturePixelsData.data(), buffers.staging_buffer, buffers.staging_bufferMem);

        obj.rawTextureData.cpyDims(obj.textureData);
        obj.rawTextureData.clear();

        this->makeImage(VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &obj, DENG_IMAGE_TYPE_TEXTURE);

        this->transitionImageLayout(obj.textureData.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        this->copyBufferToImage(buffers.staging_buffer, obj.textureData.textureImage, obj.textureData.width, obj.textureData.height);
        this->transitionImageLayout(obj.textureData.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(this->m_device, buffers.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, buffers.staging_bufferMem, nullptr);

        VkImageViewCreateInfo local_viewInfo = this->getImageViewInfo(obj.textureData.textureImage, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

        if(vkCreateImageView(this->m_device, &local_viewInfo, nullptr, &obj.textureData.textureImageView) != VK_SUCCESS) {
            ERR("Failed to create texture image view!");
        }
    }

    void Renderer::initTextureSampler(GameObject &obj) {
        VkSamplerCreateInfo local_samplerInfo{};
        local_samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        local_samplerInfo.magFilter = VK_FILTER_LINEAR;
        local_samplerInfo.minFilter = VK_FILTER_LINEAR;
        local_samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        local_samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        local_samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        local_samplerInfo.anisotropyEnable = VK_TRUE;
        local_samplerInfo.maxAnisotropy = 16.0f;
        local_samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        local_samplerInfo.unnormalizedCoordinates = VK_FALSE;
        local_samplerInfo.compareEnable = VK_FALSE;
        local_samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        local_samplerInfo.mipLodBias = 0.0f;
        local_samplerInfo.minLod = 0.0f;
        local_samplerInfo.maxLod = 0.0f;

        if(vkCreateSampler(this->m_device, &local_samplerInfo, nullptr, &obj.textureData.textureSampler) != VK_SUCCESS) {
            ERR("Failed to create texture sampler!");
        }

    }

    void Renderer::initBuffers(GameObject &obj) {
        VkDeviceSize local_size = sizeof(obj.vertexData[0]) * obj.vertexData.size();

        this->makeBuffer(&local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj, DENG_BUFFER_TYPE_STAGING, nullptr);
        this->populateBufferMem(&local_size, obj.vertexData.data(), buffers.staging_buffer, buffers.staging_bufferMem);

        this->makeBuffer(&local_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, obj, DENG_BUFFER_TYPE_VERTEX, nullptr);
        this->copyBufferToBuffer(buffers.staging_buffer, buffers.vertex_buffer, local_size);

        vkDestroyBuffer(this->m_device, buffers.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, buffers.staging_bufferMem, nullptr);

        // local_size = sizeof(obj.vertexIndicesData.posIndices[0]) * obj.vertexIndicesData.posIndices.size();

        // this->makeBuffer(&local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj, DENG_BUFFER_TYPE_STAGING, nullptr);
        // this->populateBufferMem(&local_size, obj.vertexIndicesData.posIndices.data(), obj.buffers.staging_buffer, obj.buffers.staging_bufferMem);
        
        // this->makeBuffer(&local_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, obj, DENG_BUFFER_TYPE_INDICES, nullptr);
        // this->copyBufferToBuffer(obj.buffers.staging_buffer, obj.buffers.index_buffer, local_size);

        // vkDestroyBuffer(this->m_device, obj.buffers.staging_buffer, nullptr);
        // vkFreeMemory(this->m_device, obj.buffers.staging_bufferMem, nullptr);

        local_size = sizeof(UniformBufferData);

        this->m_uniform_buffers.resize(this->m_swapChain_images.size());
        this->m_uniform_buffersMem.resize(this->m_swapChain_images.size());

        for(size_t i = 0; i < this->m_swapChain_images.size(); i++) {
            this->makeBuffer(&local_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj, DENG_BUFFER_TYPE_UNIFORM, &i);
        }

    }

    void Renderer::initDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> local_desc_poolSizes{};
        local_desc_poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_desc_poolSizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapChain_images.size());
        local_desc_poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        local_desc_poolSizes[1].descriptorCount = static_cast<uint32_t>(this->m_swapChain_images.size());

        VkDescriptorPoolCreateInfo local_desc_pool_createInfo{};
        local_desc_pool_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        local_desc_pool_createInfo.poolSizeCount = local_desc_poolSizes.size();
        local_desc_pool_createInfo.pPoolSizes = local_desc_poolSizes.data();
        local_desc_pool_createInfo.maxSets = static_cast<uint32_t>(this->m_swapChain_images.size());

        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_descriptorPool) != VK_SUCCESS) {
            ERR("Failed to create descriptor pool!");
        }

        else {
            LOG("Descriptor pool created successfully!");
        }
    }

    void Renderer::initDescriptorSets(GameObject &obj) {
        std::vector<VkDescriptorSetLayout> local_descset_layouts(this->m_swapChain_images.size(), this->m_descriptorSet_Layout);
        VkDescriptorSetAllocateInfo local_allocInfo{};
        local_allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        local_allocInfo.descriptorPool = this->m_descriptorPool;
        local_allocInfo.descriptorSetCount = static_cast<uint32_t>(this->m_swapChain_images.size());
        local_allocInfo.pSetLayouts = local_descset_layouts.data();

        this->m_descriptorSets.resize(this->m_swapChain_images.size());
        if(vkAllocateDescriptorSets(this->m_device, &local_allocInfo, this->m_descriptorSets.data()) != VK_SUCCESS) {
            ERR("Failed to allocate descriptor sets!");
        }

        else {
            LOG("Successfully allocated descriptor sets!");
        }

        for(size_t i = 0; i < this->m_swapChain_images.size(); i++) {
            VkDescriptorBufferInfo local_bufferInfo{};
            local_bufferInfo.buffer = this->m_uniform_buffers[i];
            local_bufferInfo.offset = 0;
            local_bufferInfo.range = sizeof(UniformBufferData);

            VkDescriptorImageInfo local_desc_imageInfo{};
            local_desc_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            local_desc_imageInfo.imageView = obj.textureData.textureImageView;
            local_desc_imageInfo.sampler = obj.textureData.textureSampler;

            std::array<VkWriteDescriptorSet, 2> local_desc_writes{};
            local_desc_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            local_desc_writes[0].dstSet = this->m_descriptorSets[i];
            local_desc_writes[0].dstBinding = 0;
            local_desc_writes[0].dstArrayElement = 0;
            local_desc_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            local_desc_writes[0].descriptorCount = 1;
            local_desc_writes[0].pBufferInfo = &local_bufferInfo;

            local_desc_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            local_desc_writes[1].dstSet = this->m_descriptorSets[i];
            local_desc_writes[1].dstBinding = 1;
            local_desc_writes[1].dstArrayElement = 0;
            local_desc_writes[1].descriptorCount = 1;
            local_desc_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            local_desc_writes[1].pImageInfo = &local_desc_imageInfo;

            vkUpdateDescriptorSets(this->m_device, local_desc_writes.size(), local_desc_writes.data(), 0, nullptr);
        }
    }

    void Renderer::initCommandBufferFromSwapChain() {
        this->m_commandBuffers.resize(this->m_swapChain_frameBuffers.size());

        VkCommandBufferAllocateInfo local_commandbuffer_allocinfo{};
        local_commandbuffer_allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocinfo.commandPool = this->m_commandPool;
        local_commandbuffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocinfo.commandBufferCount = this->m_commandBuffers.size();

        if(vkAllocateCommandBuffers(this->m_device, &local_commandbuffer_allocinfo, this->m_commandBuffers.data())) {
            ERR("Failed to allocate command buffers!");
        }
        else {
            LOG("Successfully allocated command buffers!");
        }

        for(size_t i = 0; i < this->m_commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo local_commandbuffer_begininfo{};
            local_commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(this->m_commandBuffers[i], &local_commandbuffer_begininfo) != VK_SUCCESS) {
                ERR("Failed to begin recording command buffers!");
            }

            else {
                LOG("Successfully begun to record command buffers!");
            }

            VkRenderPassBeginInfo local_renderpass_begininfo{};
            local_renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            local_renderpass_begininfo.renderPass = this->m_renderPass;
            local_renderpass_begininfo.framebuffer = this->m_swapChain_frameBuffers[i];
            local_renderpass_begininfo.renderArea.offset = {0, 0};
            local_renderpass_begininfo.renderArea.extent = this->m_extent;

            std::array<VkClearValue, 2> local_clearValues;
            local_clearValues[0].color = {0.0f, 0.0f, 0.0f, 0.0f};
            local_clearValues[1].depthStencil = {1.0f, 0};

            local_renderpass_begininfo.clearValueCount = local_clearValues.size();
            local_renderpass_begininfo.pClearValues = local_clearValues.data();

            vkCmdBeginRenderPass(this->m_commandBuffers[i], &local_renderpass_begininfo, VK_SUBPASS_CONTENTS_INLINE);
                vkCmdBindPipeline(this->m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipeline);

                VkBuffer local_vertex_buffers[] = {this->buffers.vertex_buffer};
                VkDeviceSize offsets[] = {0};

                vkCmdBindVertexBuffers(this->m_commandBuffers[i], 0, 1, local_vertex_buffers, offsets);
                // vkCmdBindIndexBuffer(this->m_commandBuffers[i], this->m_sample_object.buffers.index_buffer, 0, VK_INDEX_TYPE_UINT32);
                vkCmdBindDescriptorSets(this->m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipelineLayout, 0, 1, &this->m_descriptorSets[i], 0, nullptr);

                vkCmdDraw(this->m_commandBuffers[i], static_cast<uint32_t>(this->m_sample_object.vertexData.size()), 1, 0, 0);

            vkCmdEndRenderPass(this->m_commandBuffers[i]);
            LOG("Ended renderPass!");

            if(vkEndCommandBuffer(this->m_commandBuffers[i]) != VK_SUCCESS) {
                ERR("Failed to end recording command buffer!");
            }
        }
    }

    void Renderer::initSemaphores() {
        this->m_imageAvailableSem_set.resize(this->m_MAX_FRAMES_IN_FLIGHT);
        this->m_renderFinishedSem_set.resize(this->m_MAX_FRAMES_IN_FLIGHT);
        this->m_flightFences.resize(this->m_MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo local_semaphore_info{};
        local_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo local_fence_createInfo{};
        local_fence_createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        local_fence_createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(int32_t i = 0; i < this->m_MAX_FRAMES_IN_FLIGHT; i++) {
            if(vkCreateSemaphore(this->m_device, &local_semaphore_info, nullptr, &this->m_imageAvailableSem_set[i]) != VK_SUCCESS ||
            vkCreateSemaphore(this->m_device, &local_semaphore_info, nullptr, &this->m_renderFinishedSem_set[i]) != VK_SUCCESS ||
            vkCreateFence(this->m_device, &local_fence_createInfo, nullptr, &this->m_flightFences[i])) {
                ERR("Failed to create semaphores and/or fences!");
            }
        }

        
        LOG("Successfully initialised semaphores and fences!");
    }

    /* maker functions */

    void Renderer::makeBuffer(const VkDeviceSize *size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject &obj, const BufferType &bufferType, size_t *bufferIndex) {
        VkBuffer *local_buffer;
        VkDeviceMemory *local_bufferMem;

        switch (bufferType)
        {
        case DENG_BUFFER_TYPE_STAGING:
            local_buffer = &buffers.staging_buffer;
            local_bufferMem = &buffers.staging_bufferMem;
            break;
        
        case DENG_BUFFER_TYPE_VERTEX:
            local_buffer = &buffers.vertex_buffer;
            local_bufferMem = &buffers.vertex_bufferMem;
            break;

        case DENG_BUFFER_TYPE_INDICES:
            local_buffer = &buffers.index_buffer;
            local_bufferMem = &buffers.index_bufferMem;
            break;

        case DENG_BUFFER_TYPE_UNIFORM:
            local_buffer = &this->m_uniform_buffers[*bufferIndex];
            local_bufferMem = &this->m_uniform_buffersMem[*bufferIndex];
            break;

        default:
            break;
        }
        
        VkBufferCreateInfo local_buffer_createInfo{};
        local_buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_buffer_createInfo.size = *size;
        local_buffer_createInfo.usage = usage;
        local_buffer_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(this->m_device, &local_buffer_createInfo, nullptr, local_buffer) != VK_SUCCESS) {
            ERR("Failed to create a buffer!");
        }

        VkMemoryRequirements local_mem_req;
        vkGetBufferMemoryRequirements(this->m_device, *local_buffer, &local_mem_req);

        VkMemoryAllocateInfo local_mem_allocInfo{};
        local_mem_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_mem_allocInfo.allocationSize = local_mem_req.size;
        local_mem_allocInfo.memoryTypeIndex = this->getMemType(local_mem_req.memoryTypeBits, properties);

        if(vkAllocateMemory(this->m_device, &local_mem_allocInfo, nullptr, local_bufferMem) != VK_SUCCESS) {
            ERR("Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(this->m_device, *local_buffer, *local_bufferMem, 0);
    }

    void Renderer::makeImage(const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject *obj, const ImageType &imgType) {
        uint32_t local_width, local_height;
        VkImage *local_image;
        VkDeviceMemory *local_imgMem;

        switch (imgType)
        {
        case DENG_IMAGE_TYPE_TEXTURE:
            local_width = obj->textureData.width;
            local_height = obj->textureData.height;
            local_image = &obj->textureData.textureImage;
            local_imgMem = &obj->textureData.textureImageMem;
            break;

        case DENG_IMAGE_TYPE_DEPTH: 
            local_width = this->m_extent.width;
            local_height = this->m_extent.height;
            local_image = &this->m_depthImage_data.depthImage;
            local_imgMem = &this->m_depthImage_data.depthImageMem;
            break;
        
        default:
            break;
        }
        
        VkImageCreateInfo local_image_createInfo{};
        local_image_createInfo.sType  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        local_image_createInfo.imageType = VK_IMAGE_TYPE_2D;
        local_image_createInfo.extent.width = local_width;
        local_image_createInfo.extent.height = local_height;
        local_image_createInfo.extent.depth = 1;
        local_image_createInfo.mipLevels = 1;
        local_image_createInfo.arrayLayers = 1;
        local_image_createInfo.format = format;
        local_image_createInfo.tiling = tiling;
        local_image_createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_image_createInfo.usage = usage;
        local_image_createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        local_image_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateImage(this->m_device, &local_image_createInfo, nullptr, local_image) != VK_SUCCESS) {
            ERR("Failed to create image!");
        }

        VkMemoryRequirements local_memReq;
        vkGetImageMemoryRequirements(this->m_device, *local_image, &local_memReq);

        VkMemoryAllocateInfo local_allocInfo{};
        local_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_allocInfo.allocationSize = local_memReq.size;
        local_allocInfo.memoryTypeIndex = this->getMemType(local_memReq.memoryTypeBits, properties);

        if(vkAllocateMemory(this->m_device, &local_allocInfo, nullptr, local_imgMem) != VK_SUCCESS) {
            ERR("Failed to allocate image memory!");
        }

        vkBindImageMemory(this->m_device, *local_image, *local_imgMem, 0);
    }

    void Renderer::updateUniformBufferData(const uint32_t &currentImg, GameObject &obj) {
        UniformBufferData ubo;
        obj.modelMatrix.getModelMatrix(&ubo.model);
        
        this->m_camera->view_matrix.getViewMatrix(&ubo.view);
        this->m_camera->proj_matrix->getProjectionMatrix(&ubo.projection);

        void *data;
        vkMapMemory(this->m_device, this->m_uniform_buffersMem[currentImg], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(this->m_device, this->m_uniform_buffersMem[currentImg]);

    }

    void Renderer::makeFrame() {
        vkWaitForFences(this->m_device, 1, &this->m_flightFences[this->m_currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imgIndex;
        VkResult result = vkAcquireNextImageKHR(this->m_device, this->m_swapChain, UINT64_MAX, this->m_imageAvailableSem_set[this->m_currentFrame], VK_NULL_HANDLE, &imgIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            LOG("Acquiring new image from swap chain timed out!");
            return;
        }
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            ERR("Error acquiring swap chain image!");
        }

        this->updateUniformBufferData(imgIndex, this->m_sample_object);

        VkSubmitInfo local_submitInfo{};
        local_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        
        VkSemaphore local_waitSemaphores[] = {this->m_imageAvailableSem_set[m_currentFrame]};
        VkPipelineStageFlags local_waitStages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        local_submitInfo.waitSemaphoreCount = 1;
        local_submitInfo.pWaitSemaphores = local_waitSemaphores;
        local_submitInfo.pWaitDstStageMask = local_waitStages;

        local_submitInfo.commandBufferCount = 1;
        local_submitInfo.pCommandBuffers = &this->m_commandBuffers[imgIndex];

        VkSemaphore local_signalSemaphores[] = {this->m_renderFinishedSem_set[this->m_currentFrame]};
        local_submitInfo.signalSemaphoreCount = 1;
        local_submitInfo.pSignalSemaphores = local_signalSemaphores;

        vkResetFences(this->m_device, 1, &this->m_flightFences[this->m_currentFrame]);

        if(vkQueueSubmit(this->m_queues.graphicsQueue, 1, &local_submitInfo, this->m_flightFences[this->m_currentFrame]) != VK_SUCCESS) {
            ERR("Failed to submit draw command buffer!"); 
        }

        VkPresentInfoKHR local_presentInfo{};
        local_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        local_presentInfo.waitSemaphoreCount = 1;
        local_presentInfo.pWaitSemaphores = local_signalSemaphores;
        local_presentInfo.pImageIndices = &imgIndex;

        VkSwapchainKHR swapChains[] = {this->m_swapChain};
        local_presentInfo.swapchainCount = 1;
        local_presentInfo.pSwapchains = swapChains;

        vkQueuePresentKHR(this->m_queues.presentQueue, &local_presentInfo);

        this->m_currentFrame = (m_currentFrame + 1) % this->m_MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::populateBufferMem(const VkDeviceSize *size, const void *srcData, VkBuffer &buffer, VkDeviceMemory &bufferMem) {
        LOG("Populating buffer memory!");
        void *data;
        vkMapMemory(this->m_device, bufferMem, 0, *size, 0, &data);
            memcpy(data, srcData, static_cast<size_t>(*size));
        vkUnmapMemory(this->m_device, bufferMem);
    }

    void Renderer::beginCommandBufferSingleCommand(VkCommandBuffer &commandBuffer) {
        VkCommandBufferAllocateInfo local_commandBuffer_allocInfo{};
        local_commandBuffer_allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandBuffer_allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandBuffer_allocInfo.commandPool = this->m_commandPool;
        local_commandBuffer_allocInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(this->m_device, &local_commandBuffer_allocInfo, &commandBuffer);
        
        VkCommandBufferBeginInfo local_commandBuffer_beginInfo{};
        local_commandBuffer_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        local_commandBuffer_beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if(vkBeginCommandBuffer(commandBuffer, &local_commandBuffer_beginInfo) != VK_SUCCESS) {
            ERR("Failed to begin command recording buffer!");
        }
    }

    void Renderer::endCommandBufferSingleCommand(VkCommandBuffer &commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo local_submitInfo{};
        local_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        local_submitInfo.commandBufferCount = 1;
        local_submitInfo.pCommandBuffers = &commandBuffer;
        
        vkQueueSubmit(this->m_queues.graphicsQueue, 1, &local_submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(this->m_queues.graphicsQueue);

        vkFreeCommandBuffers(this->m_device, this->m_commandPool, 1, &commandBuffer);
    }

    void Renderer::copyBufferToBuffer(VkBuffer &srcBuf, VkBuffer &dstBuf, const VkDeviceSize &size) {
        VkCommandBuffer local_commandBuffer;
        this->beginCommandBufferSingleCommand(local_commandBuffer);

        VkBufferCopy local_copy_region{};
        local_copy_region.size = size;
        
        vkCmdCopyBuffer(local_commandBuffer, srcBuf, dstBuf, 1, &local_copy_region);
        this->endCommandBufferSingleCommand(local_commandBuffer);
    }

    void Renderer::transitionImageLayout(VkImage &image, const VkFormat &format, const VkImageLayout &oldLayout, const VkImageLayout &newLayout) {
        VkCommandBuffer local_commandBuffer;
        this->beginCommandBufferSingleCommand(local_commandBuffer);

        VkImageMemoryBarrier local_memBarrier{};
        local_memBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        local_memBarrier.oldLayout = oldLayout;
        local_memBarrier.newLayout = newLayout;
        local_memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        local_memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        local_memBarrier.image = image;
        local_memBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        local_memBarrier.subresourceRange.baseMipLevel = 0;
        local_memBarrier.subresourceRange.levelCount = 1;
        local_memBarrier.subresourceRange.baseArrayLayer = 0;
        local_memBarrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags local_srcStage;
        VkPipelineStageFlags local_dstStage;
        
        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            local_memBarrier.srcAccessMask = 0;
            local_memBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            local_srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            local_dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            local_memBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            local_memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            local_srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            local_dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            ERR("Invalid layout transitions!");
        }

        vkCmdPipelineBarrier(local_commandBuffer, local_srcStage, local_dstStage, 0, 0, nullptr, 0, nullptr, 1, &local_memBarrier);

        this->endCommandBufferSingleCommand(local_commandBuffer);
    }

    void Renderer::copyBufferToImage(VkBuffer &srcBuf, VkImage &dstImg, const uint32_t &width, const uint32_t &height) {
        VkCommandBuffer local_commandBuffer;
        this->beginCommandBufferSingleCommand(local_commandBuffer);

        VkBufferImageCopy local_copy_region{};
        local_copy_region.bufferOffset = 0;
        local_copy_region.bufferRowLength = 0;
        local_copy_region.bufferImageHeight = 0;

        local_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        local_copy_region.imageSubresource.mipLevel = 0;
        local_copy_region.imageSubresource.baseArrayLayer = 0;
        local_copy_region.imageSubresource.layerCount = 1;

        local_copy_region.imageOffset = {0, 0, 0};
        local_copy_region.imageExtent = {width, height, 1};

        vkCmdCopyBufferToImage(local_commandBuffer, srcBuf, dstImg, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &local_copy_region);

        this->endCommandBufferSingleCommand(local_commandBuffer);
    }

    void Renderer::run() {
        this->m_window->setInputMode(DENG_INPUT_MOVEMENT);
        while(!glfwWindowShouldClose(this->m_window->getWindow())) {
            glfwPollEvents();
            this->m_ev->update();
            this->makeFrame();
        }
        vkDeviceWaitIdle(this->m_device);
        glfwTerminate();
    }
} 