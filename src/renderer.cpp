#include "headers/renderer.h"

namespace Deng
{
    Renderer::Renderer(Window &win) {
        //Required extensions vector initialisation
        this->m_req_extensions_name.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        this->m_window = &win;
        this->initObjects(this->m_statue, "objects/obj1.obj", "textures/obj1.bmp", DENG_COORDINATE_MODE_REVERSE);
        this->initInstance();
        this->initWindowSurface();
        this->selectPhysicalDevice();
        this->initDeviceQueues();
        this->initSwapChainSettings();
        this->initSwapChain();
        this->initImageView();
        this->initRenderPass();
        // this->initDescriptorSetLayout();
        this->initGraphicsPipeline();
        this->initFrameBuffers();
        this->initCommandPool();
        // this->initTextureImage(this->m_statue);
        this->initBuffers(this->m_statue);
        this->initCommandBufferFromSwapChain();
        this->initSemaphores();
    }

    Renderer::~Renderer() {
        LOG("Destroying renderer!");
        // this->deleteTextureImage(this->m_statue);
        this->deleteFrameBuffers();
        LOG("Deleted frame buffers!");
        this->deletePipeline();
        LOG("Deleted pipeline!");
        this->deleteRenderPass();
        LOG("Deleted renderpass!");
        this->deleteImageViews();
        LOG("Deleted imageviews!");
        this->deleteSwapChain();
        LOG("Deleted swapchain!");
        this->deleteVertexBuffer();
        LOG("Deleted vertex buffer!");
        this->freeMemory();
        LOG("Freed buffer memory!");
        this->deleteSemaphores();
        LOG("Deleted semaphores!");
        this->deleteCommandPool();
        LOG("Deleted command pool!");
        this->deleteDevice();
        LOG("Deleted virtual device!");
        this->deleteSurface();
        LOG("Deleted surface!");
        this->deleteInstance();
        LOG("Deleted instance!");
        
        delete this->m_device_swapChainDetails;
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
        for(size_t i = 0; i < this->m_MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(this->m_device, this->m_imageAvailableSem_set[i], nullptr);
            vkDestroySemaphore(this->m_device, this->m_renderFinishedSem_set[i], nullptr);
        }
    }

    void Renderer::freeMemory() {
        vkFreeMemory(this->m_device, this->m_statue.buffers.vertex_bufferMem, nullptr);
    }

    void Renderer::deleteVertexBuffer() {
        vkDestroyBuffer(this->m_device, this->m_statue.buffers.vertex_buffer, nullptr);
    }

    void Renderer::deleteTextureImage(GameObject &obj) {
        vkDestroyImage(this->m_device, obj.images.textureImage, nullptr);
        vkFreeMemory(this->m_device, obj.images.textureImageMem, nullptr);
    }

    void Renderer::initObjects(GameObject &obj, const std::string &objFilePath, const std::string &texFilePath, const CoordinateMode &coordinateMode) {
        ObjLoader obj_loader(objFilePath, coordinateMode);
        obj_loader.getObjVerticesAndIndices(obj);

        TextureLoader tex_loader(texFilePath);
        ObjTextureData texture_data;
        tex_loader.getTextureDetails(&texture_data.width, &texture_data.height, &texture_data.texSize, &texture_data.texturePixelsData);
        obj.textureData = texture_data;
    }

    //Function that initialises instance 
    void Renderer::initInstance() {
        //initialise appinfo
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = this->m_window->getTitle();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Deng";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
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
            ERR("Failed to create an instance!");
        }

        else LOG("Successfully created an instance");
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

        if(deviceCount == 0) ERR("Failed to find graphics cards!");
        else {
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
            ERR("Queue supporting GPU not found!");
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
                ERR("Failed to create logical device!");
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
        
        if(!foundSuitableFormat) {
            LOG("Didn't find suitable surface format! Using first format!");
            this->m_surface_format = this->m_device_swapChainDetails->getFormats()[0];
        }

        bool foundSuitablePresentMode = false;
        for(const VkPresentModeKHR &presentFormat : this->m_device_swapChainDetails->getPresentModes()) {
            if(presentFormat == VK_PRESENT_MODE_MAILBOX_KHR) {
                this->m_present_mode = presentFormat;
                foundSuitablePresentMode = true;
                LOG("Found suitable present mode for triple buffering!");
            }
        }

        if(!foundSuitablePresentMode) {
            LOG("Didn't find suitable present mode! Using Vsync instead!");
            this->m_present_mode = VK_PRESENT_MODE_FIFO_KHR;
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
        local_createInfo.oldSwapchain = VK_NULL_HANDLE;
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

        VkAttachmentReference local_colorAttachmentRef{};
        local_colorAttachmentRef.attachment = 0;
        local_colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription local_subpass_desc{};
        local_subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        local_subpass_desc.colorAttachmentCount = 1;
        local_subpass_desc.pColorAttachments = &local_colorAttachmentRef;

        VkSubpassDependency local_subpass_dependency{};
        local_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        local_subpass_dependency.dstSubpass = 0;
        local_subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        local_subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


        VkRenderPassCreateInfo local_renderPass_createInfo{};
        local_renderPass_createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        local_renderPass_createInfo.attachmentCount = 1;
        local_renderPass_createInfo.pAttachments = &local_colorAttachment;
        local_renderPass_createInfo.subpassCount = 1;
        local_renderPass_createInfo.pSubpasses = &local_subpass_desc;
        local_renderPass_createInfo.dependencyCount = 1;
        local_renderPass_createInfo.pDependencies = &local_subpass_dependency;


        if(vkCreateRenderPass(this->m_device, &local_renderPass_createInfo, nullptr, &this->m_renderPass) != VK_SUCCESS) {
            LOG("Failed to create render pass!");
        }
    }

    // void Renderer::initDescriptorSetLayout() {
    //     VkDescriptorSetLayoutBinding local_ubo_layout_binding{};
    //     local_ubo_layout_binding.binding = 0;
    //     local_ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //     local_ubo_layout_binding.descriptorCount = 1;
    // }

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

        VkPipelineShaderStageCreateInfo shaderStages[] = {local_vModule_createInfo, local_fModule_createinfo};

        auto local_input_binding_desc = VertexInputDesc::getBindingDesc(this->m_statue);
        auto local_input_attribute_desc = VertexInputDesc::getAttributeDesc();

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
        local_rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        local_rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo local_multisampling{};
        local_multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        local_multisampling.sampleShadingEnable = VK_FALSE;
        local_multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState local_colorBlendAttachment{};
        local_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        local_colorBlendAttachment.blendEnable =  VK_FALSE;

        VkPipelineColorBlendStateCreateInfo local_colorblend_state_createinfo{};
        local_colorblend_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        local_colorblend_state_createinfo.logicOpEnable = VK_FALSE;
        local_colorblend_state_createinfo.attachmentCount = 1;
        local_colorblend_state_createinfo.pAttachments = &local_colorBlendAttachment;
        
        VkPipelineLayoutCreateInfo local_pipelineLayout_createinfo{};
        local_pipelineLayout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        local_pipelineLayout_createinfo.setLayoutCount = 0;
        local_pipelineLayout_createinfo.pushConstantRangeCount = 0;

        if(vkCreatePipelineLayout(this->m_device, &local_pipelineLayout_createinfo, nullptr, &this->m_pipelineLayout) != VK_SUCCESS) {
            ERR("Failed to create pipeline layout!");
        } 

        else {
            LOG("Pipeline layout created successfully!");
            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = shaderStages;
            pipelineInfo.pVertexInputState = &local_vertexInput_createInfo;
            pipelineInfo.pInputAssemblyState = &local_inputAssembly_createInfo;
            pipelineInfo.pViewportState = &local_viewport_state_createinfo;
            pipelineInfo.pRasterizationState = &local_rasterizer;
            pipelineInfo.pMultisampleState = &local_multisampling;
            pipelineInfo.pColorBlendState = &local_colorblend_state_createinfo;
            pipelineInfo.layout = this->m_pipelineLayout;
            pipelineInfo.renderPass = this->m_renderPass;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;   

            LOG("Pipeline create info created!");
            if(vkCreateGraphicsPipelines(this->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->m_pipeline) != VK_SUCCESS) {
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
            VkImageView attachments[] = {this->m_swapChain_imageviews[i]};

            VkFramebufferCreateInfo local_framebuffer_createinfo{};
            local_framebuffer_createinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            local_framebuffer_createinfo.renderPass = this->m_renderPass;
            local_framebuffer_createinfo.attachmentCount = sizeof(attachments)/sizeof(attachments[0]);
            local_framebuffer_createinfo.pAttachments = attachments;
            local_framebuffer_createinfo.width = this->m_extent.width;
            local_framebuffer_createinfo.height = this->m_extent.height;
            local_framebuffer_createinfo.layers = 1;

            if(vkCreateFramebuffer(this->m_device, &local_framebuffer_createinfo, nullptr, &this->m_swapChain_frameBuffers[i]) != VK_SUCCESS) {
                ERR("Failed to create framebuffer!");
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

    void Renderer::initBuffers(GameObject &obj) {
        VkDeviceSize local_size = sizeof(obj.vertexData[0]) * obj.vertexData.size();

        this->makeBuffer(local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj, DENG_BUFFER_TYPE_STAGING);
        this->populateBufferMem(local_size, obj.vertexData.data(), obj.buffers.staging_buffer, obj.buffers.staging_bufferMem);

        this->makeBuffer(local_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, obj, DENG_BUFFER_TYPE_VERTEX);
        this->copyBuffer(obj.buffers.staging_buffer, obj.buffers.vertex_buffer, local_size);

        vkDestroyBuffer(this->m_device, obj.buffers.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, obj.buffers.staging_bufferMem, nullptr);

        local_size = sizeof(obj.vertexIndicesData.posIndices[0]) * obj.vertexIndicesData.posIndices.size();

        this->makeBuffer(local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj, DENG_BUFFER_TYPE_STAGING);
        this->populateBufferMem(local_size, obj.vertexIndicesData.posIndices.data(), obj.buffers.staging_buffer, obj.buffers.staging_bufferMem);
        
        this->makeBuffer(local_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, obj, DENG_BUFFER_TYPE_INDICES);
        this->copyBuffer(obj.buffers.staging_buffer, obj.buffers.index_buffer, local_size);

        vkDestroyBuffer(this->m_device, obj.buffers.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, obj.buffers.staging_bufferMem, nullptr);
    }

    void Renderer::initCommandBufferFromSwapChain() {
        this->m_commandBuffers.resize(this->m_swapChain_frameBuffers.size());

        VkCommandBufferAllocateInfo local_commandbuffer_allocinfo{};
        local_commandbuffer_allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocinfo.commandPool = this->m_commandPool;
        local_commandbuffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocinfo.commandBufferCount = (uint32_t) this->m_commandBuffers.size();

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

            VkRenderPassBeginInfo local_renderpass_begininfo{};
            local_renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            local_renderpass_begininfo.renderPass = this->m_renderPass;
            local_renderpass_begininfo.framebuffer = this->m_swapChain_frameBuffers[i];
            local_renderpass_begininfo.renderArea.offset = {0, 0};
            local_renderpass_begininfo.renderArea.extent = this->m_extent;

            VkClearValue local_clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
            local_renderpass_begininfo.clearValueCount = 1;
            local_renderpass_begininfo.pClearValues = &local_clearColor;

            vkCmdBeginRenderPass(this->m_commandBuffers[i], &local_renderpass_begininfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(this->m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipeline);

            VkBuffer local_vertex_buffers[] = {this->m_statue.buffers.vertex_buffer};
            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(this->m_commandBuffers[i], 0, 1, local_vertex_buffers, offsets);
            vkCmdBindIndexBuffer(this->m_commandBuffers[i], this->m_statue.buffers.index_buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(this->m_commandBuffers[i], static_cast<uint32_t>(this->m_statue.vertexIndicesData.posIndices.size()), 1, 0, 0, 0);
            vkCmdEndRenderPass(this->m_commandBuffers[i]);

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

        for(size_t i = 0; i < this->m_MAX_FRAMES_IN_FLIGHT; i++) {
            if(vkCreateSemaphore(this->m_device, &local_semaphore_info, nullptr, &this->m_imageAvailableSem_set[i]) != VK_SUCCESS ||
            vkCreateSemaphore(this->m_device, &local_semaphore_info, nullptr, &this->m_renderFinishedSem_set[i]) != VK_SUCCESS ||
            vkCreateFence(this->m_device, &local_fence_createInfo, nullptr, &this->m_flightFences[i])) {
                ERR("Failed to create semaphores and/or fences!");
            }
        }

        
        LOG("Successfully initialised semaphores and fences!");
    }

    /* maker functions */

    void Renderer::makeTextureImage(const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject &obj) {
        VkImageCreateInfo local_image_createInfo{};
        local_image_createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        local_image_createInfo.imageType = VK_IMAGE_TYPE_2D;
        local_image_createInfo.extent.width = static_cast<uint32_t>(obj.textureData.width);
        local_image_createInfo.extent.height = static_cast<uint32_t>(obj.textureData.height);
        local_image_createInfo.extent.depth = 1;
        local_image_createInfo.mipLevels = 1;
        local_image_createInfo.arrayLayers = static_cast<uint32_t>(std::floor(std::log2(std::max(obj.textureData.width, obj.textureData.height)))) + 1;

        local_image_createInfo.format = format;
        local_image_createInfo.tiling = tiling;
        local_image_createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_image_createInfo.usage = usage;
        local_image_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        local_image_createInfo.samples = VK_SAMPLE_COUNT_1_BIT;

        if(vkCreateImage(this->m_device, &local_image_createInfo, nullptr, &obj.images.textureImage) != VK_SUCCESS) {
            ERR("Failed to create a texture image!");
        }
        else {
            LOG("Successfully created texture image!");
        }

        VkMemoryRequirements local_mem_req;
        vkGetImageMemoryRequirements(this->m_device, obj.images.textureImage, &local_mem_req);

        VkMemoryAllocateInfo local_mem_allocInfo{};
        local_mem_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_mem_allocInfo.allocationSize = local_mem_req.size;
        local_mem_allocInfo.memoryTypeIndex = getMemType(local_mem_req.memoryTypeBits, properties);

        if(vkAllocateMemory(this->m_device, &local_mem_allocInfo, nullptr, &obj.images.textureImageMem) != VK_SUCCESS) {
            ERR("Failed to allocate memory for texture image!");
        }

        vkBindImageMemory(this->m_device, obj.images.textureImage, obj.images.textureImageMem, 0);
    }

    void Renderer::makeBuffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, GameObject &obj, const BufferMode &bufferType) {
        VkBuffer *local_buffer;
        VkDeviceMemory *local_bufferMem;

        switch (bufferType)
        {
        case DENG_BUFFER_TYPE_STAGING:
            local_buffer = &obj.buffers.staging_buffer;
            local_bufferMem = &obj.buffers.staging_bufferMem;
            break;
        
        case DENG_BUFFER_TYPE_VERTEX:
            local_buffer = &obj.buffers.vertex_buffer;
            local_bufferMem = &obj.buffers.vertex_bufferMem;
            break;

        case DENG_BUFFER_TYPE_INDICES:
            local_buffer = &obj.buffers.index_buffer;
            local_bufferMem = &obj.buffers.index_bufferMem;

        default:
            break;
        }
        
        VkBufferCreateInfo local_buffer_createInfo{};
        local_buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_buffer_createInfo.size = size;
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

    void Renderer::populateBufferMem(const VkDeviceSize &size, const void *srcData, VkBuffer &buffer, VkDeviceMemory &bufferMem) {
        LOG("Populating buffer memory!");
        void *data;
        vkMapMemory(this->m_device, bufferMem, 0, size, 0, &data);
        memcpy(data, srcData, static_cast<size_t>(size));
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

    void Renderer::copyBuffer(VkBuffer &srcBuf, VkBuffer &dstBuf, const VkDeviceSize &size) {
        VkCommandBuffer local_commandBuffer;
        this->beginCommandBufferSingleCommand(local_commandBuffer);

        VkBufferCopy local_copy_region{};
        local_copy_region.size = size;
        
        vkCmdCopyBuffer(local_commandBuffer, srcBuf, dstBuf, 1, &local_copy_region);
        this->endCommandBufferSingleCommand(local_commandBuffer);
    }

    void Renderer::run() {
        while(!glfwWindowShouldClose(this->m_window->getWindow())) {
            glfwPollEvents();
            this->makeFrame();
        }
        vkDeviceWaitIdle(this->m_device);
        glfwTerminate();
    }
} 