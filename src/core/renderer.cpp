#include "deng_core.h"

namespace deng
{
    Renderer::Renderer(WindowWrap &win) {
        //Required extensions vector initialisation
        this->m_required_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        this->m_p_window_wrap = &win;

        // this->m_p_grid_manager = new dengUtils::GridManager(&this->m_grid, this->m_far_plane + 5, &this->m_environment_conf.grid_height, &this->m_environment_conf.grid_width, &this->m_environment_conf.grid_line_color_r, &this->m_environment_conf.grid_line_color_g, &this->m_environment_conf.grid_line_color_b);
        this->m_p_camera = new Camera({1.2f, 1.2f, 1.2f}, {0.2f, 0.2f}, 65, this->m_near_plane, this->m_far_plane, this->m_p_window_wrap);
        this->m_p_ev = new dengMath::Events(this->m_p_window_wrap, this->m_p_camera);

        // Start initialising the renderer
        this->initInstance();
        this->initDebugMessenger();
        this->initWindowSurface();
        this->selectPhysicalDevice();
        this->initLogicalDevice();
        this->initSwapChainSettings();
        this->initSwapChain();
        this->initImageView();
        this->initRenderPass();
        this->initDescriptorSetLayouts();
        this->initPipelineLayouts();
        this->initCommandPool();
    }


    // API callback function to submit assets into renderer 
    void Renderer::submitAssets(std::vector<DENGasset> *p_game_objects) {
        size_t index, textures_size;
        this->m_p_game_objects = p_game_objects;
        
    }
    

    /* Set renderer hints */
    void Renderer::setHints(dengRendererHint hints) {
        // Check for usage hint
        if((hints & DENG_RENDERER_HINT_ASSET_EDITOR) == DENG_RENDERER_HINT_ASSET_EDITOR)
            this->m_usage_mode = DENG_RENDERER_USAGE_ASSET_EDITOR;
        
        else if((hints & DENG_RENDERER_HINT_MAP_EDITOR) == DENG_RENDERER_HINT_MAP_EDITOR)
            this->m_usage_mode = DENG_RENDERER_USAGE_MAP_EDITOR;
        
        else if((hints & DENG_RENDERER_HINT_GAME_MODE) == DENG_RENDERER_HINT_GAME_MODE)
            this->m_usage_mode = DENG_RENDERER_USAGE_GAME_MODE;

        else this->m_usage_mode = DENG_RENDERER_USAGE_GAME_MODE;

        
        // Miscelanious hints
        if((hints & DENG_RENDERER_HINT_ENABLE_VSYNC) == DENG_RENDERER_HINT_ENABLE_VSYNC)
            this->m_vsync = true;
        else this->m_vsync = false;

        if((hints & DENG_RENDERER_HINT_SHOW_FPS_COUNTER) == DENG_RENDERER_HINT_SHOW_FPS_COUNTER)
            this->m_count_fps = true;
    }

    Renderer::~Renderer() {
        // delete this->m_p_grid_manager;
        delete this->m_p_ev;
        delete this->m_p_camera;
        delete this->m_p_device_swapchain_details;

        this->deleteFrameBuffers();
        this->deleteCommandBuffers();
        this->deletePipelines();
        this->deleteRenderPass();
        this->deleteImageViews();
        this->deleteSwapChain();
        this->deleteTextureImage();
        this->deleteDescriptorSetLayout();
        this->deleteBuffers();
        this->deleteDepthImageData();
        this->deleteSemaphores();
        this->deleteCommandPool();
        this->deleteDevice();
        this->deleteSurface();
        this->deleteDebugMessenger();
        this->deleteInstance();
    }

    // void Renderer::initGrid() {
    //     LOG("initGrid camera view matrix position is: {" + std::to_string(this->m_p_camera->view_matrix.getPosition().first) + ";" + std::to_string(this->m_p_camera->view_matrix.getPosition().second) + ";" + std::to_string(this->m_p_camera->view_matrix.getPosition().third) + "}");
    //     this->m_p_grid_manager->generateVertices(this->m_p_camera->view_matrix.getPosition());
    // }



    void Renderer::initInstance() {
        // Set up Vulkan application info
        VkApplicationInfo local_appinfo{};
        local_appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        local_appinfo.pApplicationName = this->m_p_window_wrap->getTitle();
        local_appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        local_appinfo.pEngineName = "DENG";
        local_appinfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        local_appinfo.apiVersion = VK_API_VERSION_1_0;

        // Set up instance create info
        VkInstanceCreateInfo local_instance_createInfo{}; 
        local_instance_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        local_instance_createInfo.pApplicationInfo = &local_appinfo;

        // Get all required extensions
        uint32_t local_extension_count;
        const char **local_extensions = get_required_surface_extensions(this->m_p_window_wrap->getWindow(), &local_extension_count, static_cast<int>(enable_validation_layers));
        local_instance_createInfo.enabledExtensionCount = local_extension_count;
        local_instance_createInfo.ppEnabledExtensionNames = local_extensions;
        LOG("Required extensions count is: " + std::to_string(local_instance_createInfo.enabledExtensionCount));
        
        VkDebugUtilsMessengerCreateInfoEXT local_debug_createInfo{};
        // Check for validatation layer support
        if(enable_validation_layers && !this->checkValidationLayerSupport())
            ERR("No validation layers available!");

        else if(enable_validation_layers && this->checkValidationLayerSupport()) {
            // Set up instance info to support validation layers
            local_instance_createInfo.enabledLayerCount = 1;
            local_instance_createInfo.ppEnabledLayerNames = &this->m_p_validation_layer;
            local_instance_createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &local_debug_createInfo;

            // Set up debug messenger createinfo
            local_debug_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            local_debug_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            local_debug_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            local_debug_createInfo.pfnUserCallback = debugCallback;
        }

        else if(!enable_validation_layers) {
            // Set up instance info to not support validation layers
            local_instance_createInfo.enabledLayerCount = 0;
            local_instance_createInfo.pNext = nullptr;
        }
        
        // Create new Vulkan instance
        if(vkCreateInstance(&local_instance_createInfo, nullptr, &this->m_instance) != VK_SUCCESS)
            ERR("Failed to create an instance!");

        else LOG("Successfully created an instance");
    }

    void Renderer::initDebugMessenger() {
        if(!enable_validation_layers) return;
        else {
            VkDebugUtilsMessengerCreateInfoEXT local_messenger_createinfo{};
            local_messenger_createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            local_messenger_createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            local_messenger_createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            local_messenger_createinfo.pfnUserCallback = debugCallback;

            if(this->makeDebugMessenger(&local_messenger_createinfo) != VK_SUCCESS) {
                ERR("Couldn't create debug messenger!");
            }
            else {
                LOG("Debug messenger created successfully!");
            }

        }
    }

    void Renderer::initWindowSurface() {
        LOG("Initialising window surface!");
        if(init_surface(this->m_p_window_wrap->getWindow(), &this->m_instance, &this->m_surface) != VK_SUCCESS) {
            ERR("Failed to create window surface!");
        }
        else {
            LOG("Window surface created successfully!");
        }
    }

    void Renderer::selectPhysicalDevice() {
        uint32_t local_device_count;
        vkEnumeratePhysicalDevices(this->m_instance, &local_device_count, nullptr);

        if(local_device_count == 0) {
            ERR("Failed to find graphics cards!");
        }

        std::vector<VkPhysicalDevice> local_devices(local_device_count);
        std::multimap<uint32_t, VkPhysicalDevice> local_device_candidates;
        VkResult result = vkEnumeratePhysicalDevices(this->m_instance, &local_device_count, local_devices.data());
        if(result != VK_SUCCESS) ERR("Failed to count physical GPUs!");

        for(uint32_t i = 0; i < local_device_count; i++) {
            uint32_t score = HardwareSpecs::getDeviceScore(&local_devices[i], this->m_required_extension_names);
            LOG("Score for device " + std::to_string(i) + ": " + std::to_string(score));
            SwapChainDetails swapChainDetails(local_devices[i], this->m_surface);

            if(!swapChainDetails.getFormats().empty() && !swapChainDetails.getPresentModes().empty()) {
                local_device_candidates.insert(std::make_pair(score, local_devices[i]));
            }
        }

        if(!local_device_candidates.empty() && local_device_candidates.rbegin()->first > 0) {
            this->m_gpu = local_device_candidates.rbegin()->second;
            this->m_p_device_swapchain_details = new SwapChainDetails(this->m_gpu, this->m_surface);
        }

        else ERR("Failed to find suitable GPU!");
        LOG(this->m_gpu);   
    }

    void Renderer::initLogicalDevice() {
        if(!this->m_queue_families.findGraphicsFamily(this->m_gpu) || !this->m_queue_families.findPresentSupportFamily(this->m_gpu, this->m_surface)) {
            ERR("Queue supporting GPU not found!");
        }

        std::vector<VkDeviceQueueCreateInfo> local_queue_createinfos;
        uint32_t queueFamilies_indexes[] = {this->m_queue_families.getGraphicsFamily(), this->m_queue_families.getPresentFamily()};

        // queueInfos.resize(2);
        float queue_priority = 1.0f;

        for(uint32_t i = 0; i < 2; i++) {
            VkDeviceQueueCreateInfo dev_queue_createinfo{};
            dev_queue_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            dev_queue_createinfo.queueFamilyIndex = queueFamilies_indexes[i];
            dev_queue_createinfo.queueCount = 1;
            dev_queue_createinfo.pQueuePriorities = &queue_priority;
            local_queue_createinfos.push_back(dev_queue_createinfo);                
        } 

        VkPhysicalDeviceFeatures local_device_features{};
        local_device_features.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo local_logical_device_createinfo{};
        local_logical_device_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        local_logical_device_createinfo.queueCreateInfoCount = static_cast<uint32_t>(local_queue_createinfos.size());
        local_logical_device_createinfo.pQueueCreateInfos = local_queue_createinfos.data();
        local_logical_device_createinfo.pEnabledFeatures = &local_device_features;
        local_logical_device_createinfo.enabledExtensionCount = this->m_required_extension_names.size();
        local_logical_device_createinfo.ppEnabledExtensionNames = this->m_required_extension_names.data();

        if(enable_validation_layers) {
            local_logical_device_createinfo.enabledLayerCount = 1;
            local_logical_device_createinfo.ppEnabledLayerNames = &this->m_p_validation_layer;
        }

        else {
            local_logical_device_createinfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(this->m_gpu, &local_logical_device_createinfo, nullptr, &this->m_device) != VK_SUCCESS) {
            ERR("Failed to create logical device!");
        }

        vkGetDeviceQueue(this->m_device, this->m_queue_families.getGraphicsFamily(), 0, &this->m_queues.graphics_queue);
        LOG("Device graphics queue recieved successfully!");
        vkGetDeviceQueue(this->m_device, this->m_queue_families.getPresentFamily(), 0, &this->m_queues.present_queue);
        LOG("Device present queue recieved successfully!");
    }
    
    
    /* Initialise swapchain settings in order to create swapchain */
    void Renderer::initSwapChainSettings() {
        bool found_suitable_format = false;
        for(const VkSurfaceFormatKHR &surface_format : this->m_p_device_swapchain_details->getFormats()) {
            if(surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                this->m_surface_format = surface_format;
                found_suitable_format = true;
                LOG("Found suitable surface format!");
            }
        }
        
        if(!found_suitable_format) {
            WARNME("Didn't find suitable surface format! Using first format!");
            this->m_surface_format = this->m_p_device_swapchain_details->getFormats()[0];
        }

        bool foundSuitablePresentMode = false;
        for(const VkPresentModeKHR &presentFormat : this->m_p_device_swapchain_details->getPresentModes()) {

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
            WARNME("Didn't find suitable present mode! Using vSync instead!");
            this->m_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        if(this->m_p_device_swapchain_details->getCapabilities().currentExtent.width != UINT32_MAX && this->m_p_device_swapchain_details->getCapabilities().currentExtent.height != UINT32_MAX) {
            this->m_extent = this->m_p_device_swapchain_details->getCapabilities().currentExtent;
        }
        else {
            this->m_extent.width = this->m_p_window_wrap->getSize().first;
            this->m_extent.height = this->m_p_window_wrap->getSize().second;
        }

        LOG("Successfully initialised swap chain settings!");
    }

    void Renderer::initSwapChain() {
        uint32_t local_min_image_count = this->m_p_device_swapchain_details->getCapabilities().minImageCount + 1;
        if(this->m_p_device_swapchain_details->getCapabilities().maxImageCount > 0 && local_min_image_count > this->m_p_device_swapchain_details->getCapabilities().maxImageCount) {
            local_min_image_count = this->m_p_device_swapchain_details->getCapabilities().maxImageCount;
        }

        VkSwapchainCreateInfoKHR local_swapchain_createInfo{};
        local_swapchain_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        local_swapchain_createInfo.surface = this->m_surface;
        local_swapchain_createInfo.minImageCount = local_min_image_count;
        local_swapchain_createInfo.imageFormat = this->m_surface_format.format;
        local_swapchain_createInfo.imageColorSpace = this->m_surface_format.colorSpace;
        local_swapchain_createInfo.imageExtent = this->m_extent;
        local_swapchain_createInfo.imageArrayLayers = 1;
        local_swapchain_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        if(this->m_queue_families.getGraphicsFamily() != this->m_queue_families.getPresentFamily()) {
            std::array<uint32_t, 2> local_queue_family_indices = {this->m_queue_families.getGraphicsFamily(), this->m_queue_families.getPresentFamily()};
            local_swapchain_createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            local_swapchain_createInfo.queueFamilyIndexCount = 2;
            local_swapchain_createInfo.pQueueFamilyIndices = local_queue_family_indices.data();
        }

        else {
            local_swapchain_createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        local_swapchain_createInfo.preTransform = this->m_p_device_swapchain_details->getCapabilities().currentTransform;
        local_swapchain_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        local_swapchain_createInfo.presentMode = this->m_present_mode;
        local_swapchain_createInfo.clipped = VK_TRUE;

        if(vkCreateSwapchainKHR(this->m_device, &local_swapchain_createInfo, nullptr, &this->m_swapchain) != VK_SUCCESS) {
            ERR("Failed to create swap chain!");
        }
        else {
            LOG("Swapchain created successfully!");
            uint32_t local_image_count;
            vkGetSwapchainImagesKHR(this->m_device, this->m_swapchain, &local_image_count, nullptr);
            LOG("Currently available " + std::to_string(local_image_count) + " swapchain images!");
            this->m_swapchain_images.resize(local_image_count);
            vkGetSwapchainImagesKHR(this->m_device, this->m_swapchain, &local_image_count, this->m_swapchain_images.data());
            
        }
    }

    void Renderer::initImageView() {
        this->m_swapchain_image_views.resize(this->m_swapchain_images.size());
        for(uint32_t i = 0; i < this->m_swapchain_image_views.size(); i++) {
            VkImageViewCreateInfo local_createInfo = this->getImageViewInfo(this->m_swapchain_images[i], this->m_surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT);
            if(vkCreateImageView(this->m_device, &local_createInfo, nullptr, &this->m_swapchain_image_views[i]) != VK_SUCCESS)
                ERR("Failed to create image views!");
        }
    }

    void Renderer::initRenderPass() {
        VkAttachmentDescription local_color_attachment{};
        local_color_attachment.format = this->m_surface_format.format;
        local_color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        local_color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        local_color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        local_color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentDescription local_depth_attachment{};
        local_depth_attachment.format = VK_FORMAT_D32_SFLOAT;
        local_depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        local_depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        local_depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        local_depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        local_depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        local_depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference local_color_attachment_reference{};
        local_color_attachment_reference.attachment = 0;
        local_color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference local_depth_attachment_reference{};
        local_depth_attachment_reference.attachment = 1;
        local_depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription local_subpass_desc{};
        local_subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        local_subpass_desc.colorAttachmentCount = 1;
        local_subpass_desc.pColorAttachments = &local_color_attachment_reference;
        local_subpass_desc.pDepthStencilAttachment = &local_depth_attachment_reference;

        VkSubpassDependency local_subpass_dependency{};
        local_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        local_subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        local_subpass_dependency.srcAccessMask = 0;

        local_subpass_dependency.dstSubpass = 0;
        local_subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        local_subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> local_attachments = {local_color_attachment, local_depth_attachment};
        VkRenderPassCreateInfo local_renderpass_createinfo{};
        local_renderpass_createinfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        local_renderpass_createinfo.attachmentCount = static_cast<uint32_t>(local_attachments.size());
        local_renderpass_createinfo.pAttachments = local_attachments.data();
        local_renderpass_createinfo.subpassCount = 1;
        local_renderpass_createinfo.pSubpasses = &local_subpass_desc;
        local_renderpass_createinfo.dependencyCount = 1;
        local_renderpass_createinfo.pDependencies = &local_subpass_dependency;


        if(vkCreateRenderPass(this->m_device, &local_renderpass_createinfo, nullptr, &this->m_renderpass) != VK_SUCCESS)
            LOG("Failed to create render pass!");
    }


    /* Create descriptor set layouts */
    void Renderer::initDescriptorSetLayouts() {
        // Set up descriptor set layout bindings for uniform buffers
        VkDescriptorSetLayoutBinding local_ubo_layout_binding{};
        local_ubo_layout_binding.binding = 0;
        local_ubo_layout_binding.descriptorCount = 1;
        local_ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_ubo_layout_binding.pImmutableSamplers = nullptr;
        local_ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        
        // Set up descriptor set layout bindings for image sampler
        VkDescriptorSetLayoutBinding local_sampler_layout_binding{};
        local_sampler_layout_binding.binding = 1;
        local_sampler_layout_binding.descriptorCount = 1;
        local_sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        local_sampler_layout_binding.pImmutableSamplers = nullptr;
        local_sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; 

        // Declare vector for bindings and descriptor set layout info
        std::vector<VkDescriptorSetLayoutBinding> local_bindings;
        VkDescriptorSetLayoutCreateInfo local_layout_createInfo{};

        // Use only uniform buffers for unmapped assets
        local_bindings.resize(1);
        local_bindings[0] = local_ubo_layout_binding;

        // Set up descriptor set layout info for unmapped assets
        local_layout_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        // Create descriptor set layout for unmapped vertices
        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_unmapped_descriptor_set_layout) != VK_SUCCESS)
            ERR("Failed to create descriptor set layout!");

        else LOG("Successfully created pipeline layout for specific pipelines!");
        
        // Use both bindings for texture mapped vertices
        local_bindings.resize(2);
        local_bindings[0] = local_ubo_layout_binding;
        local_bindings[1] = local_sampler_layout_binding;
        
        // Set up descriptor set layout info for texture mapped assets
        local_layout_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        // Create descriptor set layout for texture 
        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_texture_mapped_descriptor_set_layout) != VK_SUCCESS)
            ERR("Failed to create descriptor set layout!");
        
        else LOG("Successfully created pipeline layout for textured objects!");
    }


    /* Create pipeline layouts */
    void Renderer::initPipelineLayouts() {
        // Set up pipeline layout info struct
        VkPipelineLayoutCreateInfo local_pipeline_layout_createinfo{};
        local_pipeline_layout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        local_pipeline_layout_createinfo.setLayoutCount = 1;
        local_pipeline_layout_createinfo.pushConstantRangeCount = 0;
        local_pipeline_layout_createinfo.pSetLayouts = &this->m_unmapped_descriptor_set_layout;

        // Create pipeline layout for unmapped assets
        if(vkCreatePipelineLayout(this->m_device, &local_pipeline_layout_createinfo, nullptr, &this->m_unmapped_pipeline_layout) != VK_SUCCESS)
            ERR("Failed to create pipeline layout for unmapped objects!");

        // Create pipeline layout for texture mapped assets
        local_pipeline_layout_createinfo.pSetLayouts = &this->m_texture_mapped_descriptor_set_layout;
        if(vkCreatePipelineLayout(this->m_device, &local_pipeline_layout_createinfo, nullptr, &this->m_texture_mapped_pipeline_layout) != VK_SUCCESS)
            ERR("Failed to create pipeline layout for textured objects!");
    }
    

    /* Create commandpool for commandbuffers */
    void Renderer::initCommandPool() {
        // Set up commandpool createinfo
        VkCommandPoolCreateInfo local_commandpool_createinfo{};
        local_commandpool_createinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        local_commandpool_createinfo.queueFamilyIndex = this->m_queue_families.getGraphicsFamily();

        // Create commandpool
        if(vkCreateCommandPool(this->m_device, &local_commandpool_createinfo, nullptr, &this->m_commandpool) != VK_SUCCESS) {
            ERR("Failed to create command pool!");
        }
        
        else LOG("Successfully created commandpool!");
    }


    /* Create graphics pipelines for differently textured assets */
    void Renderer::initGraphicsPipelines() {
        uint32_t total_texture_mapped_vertices_count = 0;
        uint32_t total_unmapped_vertices_count = 0;
        size_t index;

        /* Count the amount of vertices for each type of assets */
        for(index = 0; index < this->m_p_game_objects->size(); index++) {
            if((*this->m_p_game_objects)[index].vertices.vertices_type)
                total_texture_mapped_vertices_count += static_cast<uint32_t>((*this->m_p_game_objects)[index].vertices.size);
            
            else if(!(*this->m_p_game_objects)[index].vertices.vertices_type)
                total_unmapped_vertices_count += static_cast<uint32_t>((*this->m_p_game_objects)[index].vertices.size);
        }
        
        /* Specify the pipiline type and layout */
        this->m_pipelines[0].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED;
        this->m_pipelines[1].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;
        this->m_pipelines[0].p_pipeline_layout = &this->m_unmapped_pipeline_layout;
        this->m_pipelines[1].p_pipeline_layout = &this->m_texture_mapped_pipeline_layout;

        // this->m_pipelines[1].p_vertices_buffer = &this->m_buffers.grid_buffer;

        PipelineCreator unmapped_pipeline(&this->m_pipelines[0], &this->m_device, &this->m_extent, &this->m_renderpass);
        PipelineCreator texture_mapped_pipeline(&this->m_pipelines[1], &this->m_device, &this->m_extent, &this->m_renderpass);

        VkGraphicsPipelineCreateInfo local_unmapped_pipeline_createinfo = unmapped_pipeline.getGraphicsPipelineInfo("shaders/bin/deng/specified_vert.spv", "shaders/bin/deng/specified_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true, false, 0);
        VkGraphicsPipelineCreateInfo local_texture_mapped_pipeline_createinfo = texture_mapped_pipeline.getGraphicsPipelineInfo("shaders/bin/deng/object_vert.spv", "shaders/bin/deng/object_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true, false, 0);
        
        LOG("Pipeline createinfos created!");

        std::array<VkGraphicsPipelineCreateInfo, DENG_PIPELINE_COUNT> local_pipeline_createinfos;
        std::array<VkPipeline, DENG_PIPELINE_COUNT> local_pipelines;

        local_pipeline_createinfos = {local_unmapped_pipeline_createinfo, local_texture_mapped_pipeline_createinfo};

        if(vkCreateGraphicsPipelines(this->m_device, VK_NULL_HANDLE, static_cast<uint32_t>(local_pipeline_createinfos.size()), local_pipeline_createinfos.data(), nullptr, local_pipelines.data()) != VK_SUCCESS)
            ERR("Failed to create graphics pipelines!");

        else {
            size_t index;
            for(index = 0; index < local_pipelines.size(); index++)
                this->m_pipelines[index].pipeline = local_pipelines[index];

            LOG("Graphics pipelines created successfully!");
        }
    }

    void Renderer::initDepthResources() {
        VkMemoryRequirements local_memory_requirements = BufferHandler::makeImage(&this->m_device, &this->m_gpu, &this->m_depth_image, this->m_extent.width, this->m_extent.height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_depth_image_memory, local_memory_requirements.size, local_memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkBindImageMemory(this->m_device, this->m_depth_image, this->m_depth_image_memory, 0);

        VkImageViewCreateInfo local_image_view_createinfo = this->getImageViewInfo(this->m_depth_image, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);

        if(vkCreateImageView(this->m_device, &local_image_view_createinfo, nullptr, &this->m_depth_image_view) != VK_SUCCESS)
            ERR("Failed to create depth image view!");
    }

    void Renderer::initFrameBuffers() {
        this->m_swapchain_framebuffers.resize(this->m_swapchain_image_views.size());

        for(size_t i = 0; i < this->m_swapchain_image_views.size(); i++) {
            std::array<VkImageView, 2> local_attachments = {this->m_swapchain_image_views[i], this->m_depth_image_view};

            VkFramebufferCreateInfo local_framebuffer_createinfo{};
            local_framebuffer_createinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            local_framebuffer_createinfo.renderPass = this->m_renderpass;
            local_framebuffer_createinfo.attachmentCount = local_attachments.size();
            local_framebuffer_createinfo.pAttachments = local_attachments.data();
            local_framebuffer_createinfo.width = this->m_extent.width;
            local_framebuffer_createinfo.height = this->m_extent.height;
            local_framebuffer_createinfo.layers = 1;

            if(vkCreateFramebuffer(this->m_device, &local_framebuffer_createinfo, nullptr, &this->m_swapchain_framebuffers[i]) != VK_SUCCESS)
                ERR("Failed to create framebuffer!");
            else
                LOG("Successfully created framebuffer!");
        }
    }


    /* Create texture image for each asset */
    // This will soon be deprecated and textures will be separate from asset! 
    void Renderer::initTextureImage() {
        size_t index;
        VkMemoryRequirements memory_requirements;

        // Iterate through assets an check if it is texture mapped
        for(index = 0; index < this->m_texture_data.size(); index++) {
            if((*this->m_p_game_objects)[index].vertices.vertices_type) {
                // Create new staging buffer and allocate memory for it
                memory_requirements =  BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &(*this->m_p_game_objects)[index].pixel_data.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &this->m_buffer_data.staging_buffer);
                BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.staging_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
                vkBindBufferMemory(this->m_device, this->m_buffer_data.staging_buffer, this->m_buffer_data.staging_buffer_memory, 0);
                BufferHandler::populateBufferMem(&this->m_device, &memory_requirements.size, (void*) (*this->m_p_game_objects)[index].pixel_data.p_pixel_data, &this->m_buffer_data.staging_buffer_memory, 0);

                // Create new image and populate the memory for it
                BufferHandler::makeImage(&this->m_device, &this->m_gpu, &this->m_texture_data[index].image, (uint32_t) this->m_texture_data[index].pixel_data.width, (uint32_t) this->m_texture_data[index].pixel_data.height, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
                BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_texture_data[index].image_mem, this->m_texture_data[index].pixel_data.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                vkBindImageMemory(this->m_device, this->m_texture_data[index].image, this->m_texture_data[index].image_mem, 0);

                // Copy data from staging buffer to texture image
                BufferHandler::transitionImageLayout(&this->m_device, &this->m_texture_data[index].image, &this->m_commandpool, &this->m_queues.graphics_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
                BufferHandler::copyBufferToImage(&this->m_device, &this->m_commandpool, &this->m_queues.graphics_queue, &this->m_buffer_data.staging_buffer, &this->m_texture_data[index].image, this->m_texture_data[index].pixel_data.width, this->m_texture_data[index].pixel_data.height);
                BufferHandler::transitionImageLayout(&this->m_device, &this->m_texture_data[index].image, &this->m_commandpool, &this->m_queues.graphics_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                
                // Clean the staging buffer
                vkDestroyBuffer(this->m_device, this->m_buffer_data.staging_buffer, nullptr);
                vkFreeMemory(this->m_device, this->m_buffer_data.staging_buffer_memory, nullptr);
                
                VkImageViewCreateInfo local_viewinfo = this->getImageViewInfo(this->m_texture_data[index].image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
                if(vkCreateImageView(this->m_device, &local_viewinfo, nullptr, &this->m_texture_data[index].image_view) != VK_SUCCESS)
                    ERR("Failed to create texture image view!");

                LOG("Successfully created texture image view!");
            }
        }
    }


    /* Create texture sampler */
    void Renderer::initTextureSampler() {
        // Set up texture sampler createinfo
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
        
        // Create texture sampler 
        if(vkCreateSampler(this->m_device, &local_samplerInfo, nullptr, &this->m_texture_sampler) != VK_SUCCESS)
            ERR("Failed to create texture sampler!");
    }
    

    /* Create and allocate buffers */
    void Renderer::initBuffers() {
        size_t index;
        LOG("Initialising main buffer!");
        VkDeviceSize total_size = 0, data_size = 0;
        VkMemoryRequirements memory_requirements;

        // Count total amount of bytes needed to allocate for assets 
        for(index = 0; index < this->m_p_game_objects->size(); index++) {
            LOG("Counting model bytes of model: " + std::string((*this->m_p_game_objects)[index].name));
            switch ((*this->m_p_game_objects)[index].vertices.vertices_type)
            {
            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
                total_size += (*this->m_p_game_objects)[index].vertices.size * sizeof((*this->m_p_game_objects)[index].vertices.p_mapped_vertices[0]);
                break;

            case DENG_PIPELINE_TYPE_UNMAPPED:
                LOG("Detected unmapped vertices!");
                total_size += (*this->m_p_game_objects)[index].vertices.size * sizeof((*this->m_p_game_objects)[index].vertices.p_unmapped_vertices[0]);
                break;
            
            default:
                break;
            }

            total_size += (*this->m_p_game_objects)[index].indices.size * sizeof((*this->m_p_game_objects)[index].indices.p_indices[0]);
        }
        
        LOG("Buffer size is: " + std::to_string(total_size));
        
        // Create staging buffer
        memory_requirements = BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &total_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &this->m_buffer_data.staging_buffer);
        BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.staging_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        vkBindBufferMemory(this->m_device, this->m_buffer_data.staging_buffer, this->m_buffer_data.staging_buffer_memory, 0);
        
        // Assign coorect offsets for buffers and populate buffer memory
        for(index = 0, total_size = 0; index < this->m_p_game_objects->size(); index++) {
            // Populate staging buffer memory with vertices data
            switch ((*this->m_p_game_objects)[index].vertices.vertices_type)
            {
            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
                data_size = (*this->m_p_game_objects)[index].vertices.size * sizeof((*this->m_p_game_objects)[index].vertices.p_mapped_vertices[0]);
                BufferHandler::populateBufferMem(&this->m_device, &data_size, (void*) (*this->m_p_game_objects)[index].vertices.p_mapped_vertices, &this->m_buffer_data.staging_buffer_memory, total_size);
                (*this->m_p_game_objects)[index].vertices.memory_offset = total_size;
                total_size += data_size;
                break;

            case DENG_PIPELINE_TYPE_UNMAPPED:
                data_size = (*this->m_p_game_objects)[index].vertices.size * sizeof((*this->m_p_game_objects)[index].vertices.p_unmapped_vertices[0]);
                BufferHandler::populateBufferMem(&this->m_device, &data_size, (void*) (*this->m_p_game_objects)[index].vertices.p_unmapped_vertices, &this->m_buffer_data.staging_buffer_memory, total_size);
                (*this->m_p_game_objects)[index].vertices.memory_offset = total_size;
                total_size += data_size;
                break;
            
            default:
                break;
            }

            // Populate staging memory with indices data
            data_size = (*this->m_p_game_objects)[index].indices.size * sizeof((*this->m_p_game_objects)[index].indices.p_indices[0]);
            BufferHandler::populateBufferMem(&this->m_device, &data_size, (*this->m_p_game_objects)[index].indices.p_indices, &this->m_buffer_data.staging_buffer_memory, total_size);
            (*this->m_p_game_objects)[index].indices.memory_offset = total_size;
            total_size += data_size;
        }

        // Push data from staging buffer to main buffer
        memory_requirements = BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &total_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, &this->m_buffer_data.main_buffer);
        BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.main_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkBindBufferMemory(this->m_device, this->m_buffer_data.main_buffer, this->m_buffer_data.main_buffer_memory, 0);
        BufferHandler::copyBufferToBuffer(&this->m_device, &this->m_commandpool, &this->m_queues.graphics_queue, &this->m_buffer_data.staging_buffer, &this->m_buffer_data.main_buffer, &total_size, 0);

        // Perform staging buffer cleanup
        vkDestroyBuffer(this->m_device, this->m_buffer_data.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, this->m_buffer_data.staging_buffer_memory, nullptr);


        total_size = sizeof(dengMath::UniformBufferData);

        this->m_buffer_data.uniform_buffers.resize(this->m_swapchain_images.size());
        this->m_buffer_data.uniform_buffers_memory.resize(this->m_swapchain_images.size());
        
        // Allocate uniform buffers
        for(index = 0; index < this->m_swapchain_images.size(); index++) { 
            memory_requirements = BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &total_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &this->m_buffer_data.uniform_buffers[index]);
            BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.uniform_buffers_memory[index], memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            vkBindBufferMemory(this->m_device, this->m_buffer_data.uniform_buffers[index], this->m_buffer_data.uniform_buffers_memory[index], 0);
        }
    }


    /* Create descriptor pool */
    void Renderer::initDescriptorPool() {
        // Create and set up descriptor pool size struct for uniform buffers
        std::vector<VkDescriptorPoolSize> local_descriptor_pool_sizes{};
        local_descriptor_pool_sizes.resize(1);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());

        // Set up descriptor pool createinfo 
        VkDescriptorPoolCreateInfo local_desc_pool_createInfo{};
        local_desc_pool_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        local_desc_pool_createInfo.poolSizeCount = static_cast<uint32_t>(local_descriptor_pool_sizes.size());
        local_desc_pool_createInfo.pPoolSizes = local_descriptor_pool_sizes.data();
        local_desc_pool_createInfo.maxSets = static_cast<uint32_t>(this->m_swapchain_images.size());

        // Create descriptor pool for unmapped assets 
        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_unmapped_descriptor_pool) != VK_SUCCESS)
            ERR("Failed to create descriptor pool!");

        // Set up descriptor pool size struct for uniform buffers and combined image sampler
        local_descriptor_pool_sizes.resize(2);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());
        local_descriptor_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        local_descriptor_pool_sizes[1].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());

        // Modify descriptor pool createinfo
        local_desc_pool_createInfo.poolSizeCount = static_cast<uint32_t>(local_descriptor_pool_sizes.size());
        local_desc_pool_createInfo.pPoolSizes = local_descriptor_pool_sizes.data();

        // Create descriptor pool for texture mapped assets
        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_texture_mapped_descriptor_pool) != VK_SUCCESS)
            ERR("Failed to create descriptor pool!");
    }


    /* Create descriptor sets */
    void Renderer::initDescriptorSets(bool init_texture_mapped_descriptor_sets) {
        size_t index, texture_index;
        // Set up multiple info structures
        VkDescriptorBufferInfo local_bufferinfo{};
        std::vector<VkWriteDescriptorSet> local_description_writes{};
        VkDescriptorImageInfo local_desc_imageinfo{};
        VkDescriptorSetAllocateInfo local_allocinfo{};
        local_allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

        // Create descriptor sets for depth image
        if(!init_texture_mapped_descriptor_sets) {
            std::vector<VkDescriptorSetLayout> local_descriptor_set_layouts(this->m_swapchain_images.size(), this->m_unmapped_descriptor_set_layout);
            // Set up descriptor set allocate info struct for unmapped vertices
            local_allocinfo.descriptorPool = this->m_unmapped_descriptor_pool;
            local_allocinfo.descriptorSetCount = static_cast<uint32_t>(local_descriptor_set_layouts.size());
            local_allocinfo.pSetLayouts = local_descriptor_set_layouts.data();
            this->m_unmapped_descriptor_sets.resize(local_descriptor_set_layouts.size());

            // Allocate descriptor sets for unmapped vertices
            if(vkAllocateDescriptorSets(this->m_device, &local_allocinfo, this->m_unmapped_descriptor_sets.data()) != VK_SUCCESS)
                ERR("Failed to allocate descriptor sets!");

            else LOG("Successfully allocated unmapped descriptor sets!");

            // Update every unmapped descriptor set 
            for(index = 0; index < this->m_swapchain_images.size(); index++) {
                // Set up descriptor buffer info struct
                local_bufferinfo.buffer = this->m_buffer_data.uniform_buffers[index];
                local_bufferinfo.offset = 0;
                local_bufferinfo.range = sizeof(dengMath::UniformBufferData);

                // Set up descriptor set write 
                local_description_writes.resize(1);
                local_description_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                local_description_writes[0].dstSet = this->m_unmapped_descriptor_sets[index];
                local_description_writes[0].dstBinding = 0;
                local_description_writes[0].dstArrayElement = 0;
                local_description_writes[0].descriptorCount = 1;
                local_description_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                local_description_writes[0].pBufferInfo = &local_bufferinfo;

                // Update descriptor sets
                vkUpdateDescriptorSets(this->m_device, local_description_writes.size(), local_description_writes.data(), 0, nullptr);
            }
        }

        // Create descriptor sets for texture image
        else {
            // Set up descriptor set allocate info struct for texture mapped vertices 
            std::vector<VkDescriptorSetLayout> local_descriptor_set_layouts(this->m_swapchain_images.size(), this->m_texture_mapped_descriptor_set_layout);
            local_allocinfo.descriptorPool = this->m_texture_mapped_descriptor_pool;
            local_allocinfo.descriptorSetCount = static_cast<uint32_t>(local_descriptor_set_layouts.size());
            local_allocinfo.pSetLayouts = local_descriptor_set_layouts.data();
            
            // Iterate through every game asset and 
            for(texture_index = 0; texture_index < this->m_texture_data.size(); texture_index++) {
                this->m_texture_data[texture_index].descriptor_sets.resize(this->m_swapchain_images.size());

                // Allocate descriptor sets
                if(vkAllocateDescriptorSets(this->m_device, &local_allocinfo, this->m_texture_data[texture_index].descriptor_sets.data()) != VK_SUCCESS)
                    ERR("Failed to allocate descriptor sets!");

                else LOG("Successfully allocated texture mapped descriptor sets for model !");

                // Iterate through every descriptor set and update it
                for(size_t index = 0; index < this->m_texture_data[texture_index].descriptor_sets.size(); index++) {
                    // Set up descriptor buffer info 
                    local_bufferinfo.buffer = this->m_buffer_data.uniform_buffers[index];
                    local_bufferinfo.offset = 0;
                    local_bufferinfo.range = sizeof(dengMath::UniformBufferData);

                    // Set up descriptor image info 
                    local_desc_imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    local_desc_imageinfo.imageView = this->m_texture_data[texture_index].image_view;
                    local_desc_imageinfo.sampler = this->m_texture_sampler;

                    // Set up descriptor writes structs for texture mapped assets
                    local_description_writes.resize(2);
                    local_description_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    local_description_writes[0].dstSet = this->m_texture_data[texture_index].descriptor_sets[index];
                    local_description_writes[0].dstBinding = 0;
                    local_description_writes[0].dstArrayElement = 0;
                    local_description_writes[0].descriptorCount = 1;
                    local_description_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    local_description_writes[0].pBufferInfo = &local_bufferinfo;

                    local_description_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    local_description_writes[1].dstSet = this->m_texture_data[texture_index].descriptor_sets[index];
                    local_description_writes[1].dstBinding = 1;
                    local_description_writes[1].dstArrayElement = 0;
                    local_description_writes[1].descriptorCount = 1;
                    local_description_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    local_description_writes[1].pImageInfo = &local_desc_imageinfo;

                    // Update texture mapped descriptor sets
                    vkUpdateDescriptorSets(this->m_device, local_description_writes.size(), local_description_writes.data(), 0, nullptr);
                }
            }
        }
    }


    /* Create commandbuffers */
    void Renderer::initCommandBuffers() {
        size_t model_index, commandbuffer_index;
        LOG("Framebuffer size: " + std::to_string(this->m_swapchain_framebuffers.size()));

        this->m_commandbuffers.resize(this->m_swapchain_framebuffers.size());

        // Set up commandbuffer allocate info
        VkCommandBufferAllocateInfo local_commandbuffer_allocation_info{};
        local_commandbuffer_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocation_info.commandPool = this->m_commandpool;
        local_commandbuffer_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocation_info.commandBufferCount = static_cast<uint32_t>(this->m_commandbuffers.size());

        // Allocate command buffers
        if(vkAllocateCommandBuffers(this->m_device, &local_commandbuffer_allocation_info, this->m_commandbuffers.data()))
            ERR("Failed to allocate command buffers!");
        
        else LOG("Successfully allocated command buffers!");

        // Start iterating through commandbuffers to submit draw calls
        for(commandbuffer_index = 0; commandbuffer_index < this->m_commandbuffers.size(); commandbuffer_index++) {
            VkCommandBufferBeginInfo local_commandbuffer_begininfo{};
            local_commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            // Begin recording command buffer
            if(vkBeginCommandBuffer(this->m_commandbuffers[commandbuffer_index], &local_commandbuffer_begininfo) != VK_SUCCESS)
                ERR("Failed to begin recording command buffers!");

            else LOG("Successfully begun to record command buffers!");

            // Set up renderpass begin info
            VkRenderPassBeginInfo local_renderpass_begininfo{};
            local_renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            local_renderpass_begininfo.renderPass = this->m_renderpass;
            local_renderpass_begininfo.framebuffer = this->m_swapchain_framebuffers[commandbuffer_index];
            local_renderpass_begininfo.renderArea.offset = {0, 0};
            local_renderpass_begininfo.renderArea.extent = this->m_extent;

            // Set up clear values
            std::array<VkClearValue, 2> local_clear_values;
            local_clear_values[0].color = {0.1411765f, 0.0431372f, 0.2313725f, 1.0f};
            local_clear_values[1].depthStencil = {1.0f, 0};

            // Add clear values to renderpass begin info
            local_renderpass_begininfo.clearValueCount = local_clear_values.size();
            local_renderpass_begininfo.pClearValues = local_clear_values.data();
            
            // Start a new render pass
            vkCmdBeginRenderPass(this->m_commandbuffers[commandbuffer_index], &local_renderpass_begininfo, VK_SUBPASS_CONTENTS_INLINE);
                LOG("Successfully began renderpass!");
                
                // Iterate through every asset and submit a draw call
                for(model_index = 0; model_index < this->m_p_game_objects->size(); model_index++) {
                    LOG("Vertices offset for model " + std::string((*this->m_p_game_objects)[model_index].name) + " is: " + std::to_string((*this->m_p_game_objects)[model_index].vertices.memory_offset));
                    LOG("Indices offset for model " + std::string((*this->m_p_game_objects)[model_index].name) + " is: " + std::to_string((*this->m_p_game_objects)[model_index].indices.memory_offset));
                    
                    // Bind buffers with appropriate offsets
                    vkCmdBindVertexBuffers(this->m_commandbuffers[commandbuffer_index], 0, 1, &this->m_buffer_data.main_buffer, &(*this->m_p_game_objects)[model_index].vertices.memory_offset);
                    vkCmdBindIndexBuffer(this->m_commandbuffers[commandbuffer_index], this->m_buffer_data.main_buffer, (*this->m_p_game_objects)[model_index].indices.memory_offset, VK_INDEX_TYPE_UINT32);
                    
                    // Bind pipelines and descriptor sets according to the vertices type 
                    if((*this->m_p_game_objects)[model_index].vertices.vertices_type) {
                        vkCmdBindPipeline(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipelines[1].pipeline);   
                        vkCmdBindDescriptorSets(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_pipelines[1].p_pipeline_layout, 0, 
                        1, &this->m_texture_data[(*this->m_p_game_objects)[model_index].texture_index].descriptor_sets[commandbuffer_index], 0, nullptr);                    
                    }

                    else {
                        vkCmdBindPipeline(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipelines[0].pipeline);
                        vkCmdBindDescriptorSets(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_pipelines[0].p_pipeline_layout, 0, 1, &this->m_unmapped_descriptor_sets[commandbuffer_index], 0, nullptr);
                    }

                    // Draw assets
                    vkCmdDrawIndexed(this->m_commandbuffers[commandbuffer_index], (*this->m_p_game_objects)[model_index].indices.size, 1, 0, 0, 0);
                }

            // End render pass
            vkCmdEndRenderPass(this->m_commandbuffers[commandbuffer_index]);
            LOG("Ended renderPass!");

            // Stop recording commandbuffer
            if(vkEndCommandBuffer(this->m_commandbuffers[commandbuffer_index]) != VK_SUCCESS)
                ERR("Failed to end recording command buffer!");
        }
    }
    

    /* Create semaphores and fences for synchronisation */
    void Renderer::initSemaphores() {
        this->m_image_available_semaphore_set.resize(this->m_max_frames_in_flight);
        this->m_render_finished_semaphore_set.resize(this->m_max_frames_in_flight);
        this->m_flight_fences.resize(this->m_max_frames_in_flight);

        VkSemaphoreCreateInfo local_semaphore_info{};
        local_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo local_fence_createInfo{};
        local_fence_createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        local_fence_createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(int32_t i = 0; i < this->m_max_frames_in_flight; i++) {
            if(vkCreateSemaphore(this->m_device, &local_semaphore_info, nullptr, &this->m_image_available_semaphore_set[i]) != VK_SUCCESS ||
            vkCreateSemaphore(this->m_device, &local_semaphore_info, nullptr, &this->m_render_finished_semaphore_set[i]) != VK_SUCCESS ||
            vkCreateFence(this->m_device, &local_fence_createInfo, nullptr, &this->m_flight_fences[i])) {
                ERR("Failed to create semaphores and/or fences!");
            }
        }

        LOG("Successfully initialised semaphores and fences!");
    }


    /* Frame update functions */
    void Renderer::makeFrame() {
        // Call Vulkan fence waiter method
        vkWaitForFences(this->m_device, 1, &this->m_flight_fences[this->m_current_frame], VK_TRUE, UINT64_MAX);
        // Call Vulkan next image acquire method
        uint32_t image_index;
        VkResult result = vkAcquireNextImageKHR(this->m_device, this->m_swapchain, UINT64_MAX, this->m_image_available_semaphore_set[this->m_current_frame], VK_NULL_HANDLE, &image_index);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            LOG("Acquiring new image from swap chain timed out!");
            return;
        }
        
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            ERR("Error acquiring swap chain image!");

        // Update uniform data buffer
        this->updateUniformBufferData(image_index);


        VkSemaphore local_wait_semaphores[] = {this->m_image_available_semaphore_set[this->m_current_frame]};
        VkSemaphore local_signalSemaphores[] = {this->m_render_finished_semaphore_set[this->m_current_frame]};

        VkSubmitInfo local_submitinfo{};
        local_submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkPipelineStageFlags local_wait_stages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        local_submitinfo.waitSemaphoreCount = 1;
        local_submitinfo.pWaitSemaphores = local_wait_semaphores;
        local_submitinfo.pWaitDstStageMask = local_wait_stages;
        local_submitinfo.commandBufferCount = 1;
        local_submitinfo.pCommandBuffers = &this->m_commandbuffers[image_index];
        local_submitinfo.signalSemaphoreCount = 1;
        local_submitinfo.pSignalSemaphores = local_signalSemaphores;

        vkResetFences(this->m_device, 1, &this->m_flight_fences[this->m_current_frame]);

        if(vkQueueSubmit(this->m_queues.graphics_queue, 1, &local_submitinfo, this->m_flight_fences[this->m_current_frame]) != VK_SUCCESS)
            ERR("Failed to submit draw command buffer!"); 

        VkPresentInfoKHR local_presentInfo{};
        local_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        local_presentInfo.waitSemaphoreCount = 1;
        local_presentInfo.pWaitSemaphores = local_signalSemaphores;
        local_presentInfo.pImageIndices = &image_index;

        VkSwapchainKHR swapChains[] = {this->m_swapchain};
        local_presentInfo.swapchainCount = 1;
        local_presentInfo.pSwapchains = swapChains;

        vkQueuePresentKHR(this->m_queues.present_queue, &local_presentInfo);

        this->m_current_frame = (m_current_frame + 1) % this->m_max_frames_in_flight;
    }

    void Renderer::updateUniformBufferData(const uint32_t &current_image) {
        dengMath::UniformBufferData local_ubo;
        
        this->m_p_camera->view_matrix.getViewMatrix(&local_ubo.view);
        this->m_p_camera->p_projection_matrix->getProjectionMatrix(&local_ubo.projection);

        void *data;
        vkMapMemory(this->m_device, this->m_buffer_data.uniform_buffers_memory[current_image], 0, sizeof(local_ubo), 0, &data);
            memcpy(data, &local_ubo, sizeof(local_ubo));
        vkUnmapMemory(this->m_device, this->m_buffer_data.uniform_buffers_memory[current_image]);

    }
    
    VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data) {
        std::cerr << "Validation layer: " << p_callback_data->pMessage << std::endl;
        return VK_FALSE;
    }

    VkResult Renderer::makeDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT *p_createinfo) {
        auto fun = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(this->m_instance, "vkCreateDebugUtilsMessengerEXT");
        if(fun == nullptr) {
            WARNME("Couldn't find vkCreateDebugUtilsMessengerEXT locations!");
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
        else {
            fun(this->m_instance, p_createinfo, nullptr, &this->m_debug_messenger);
            return VK_SUCCESS;
        }
    }

    bool Renderer::checkValidationLayerSupport() {
        uint32_t local_layer_count;
        vkEnumerateInstanceLayerProperties(&local_layer_count, nullptr);

        std::vector<VkLayerProperties> local_available_layers(local_layer_count);
        vkEnumerateInstanceLayerProperties(&local_layer_count, local_available_layers.data());
        bool isLayer = false;

        for(const VkLayerProperties &properties : local_available_layers) {
            if(strcmp(this->m_p_validation_layer, properties.layerName) == 0) {
                isLayer = true;
                break;
            }
        }

        return isLayer;
    }

    VkImageViewCreateInfo Renderer::getImageViewInfo(VkImage &image, const VkFormat &format, const VkImageAspectFlags &aspect_flags) {
            VkImageViewCreateInfo local_createInfo{};
            local_createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            local_createInfo.image = image;
            local_createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            local_createInfo.format = format;

            local_createInfo.subresourceRange.aspectMask = aspect_flags;
            local_createInfo.subresourceRange.baseMipLevel = 0;
            local_createInfo.subresourceRange.levelCount = 1;
            local_createInfo.subresourceRange.baseArrayLayer = 0;
            local_createInfo.subresourceRange.layerCount = 1;
            return local_createInfo;
    }

    /* Cleanup methods */
    void Renderer::deleteFrameBuffers() {
        for(VkFramebuffer &frameBuffer : this->m_swapchain_framebuffers) {
            vkDestroyFramebuffer(this->m_device, frameBuffer, nullptr);
        }
    }

    void Renderer::deleteCommandBuffers() {
        vkFreeCommandBuffers(this->m_device, this->m_commandpool, static_cast<uint32_t>(this->m_commandbuffers.size()), this->m_commandbuffers.data());
    }

    void Renderer::deletePipelines() {
        for(PipelineData &pipeline_data : this->m_pipelines)
            vkDestroyPipeline(this->m_device, pipeline_data.pipeline, nullptr);

        vkDestroyPipelineLayout(this->m_device, this->m_unmapped_pipeline_layout, nullptr);
        vkDestroyPipelineLayout(this->m_device, this->m_texture_mapped_pipeline_layout, nullptr);
    }

    void Renderer::deleteRenderPass() {
        vkDestroyRenderPass(this->m_device, this->m_renderpass, nullptr);
    }

    void Renderer::deleteImageViews() {
        for(VkImageView &imageView : this->m_swapchain_image_views) {
            vkDestroyImageView(this->m_device, imageView, nullptr);
        }
    }

    void Renderer::deleteSwapChain() {
        vkDestroySwapchainKHR(this->m_device, this->m_swapchain, nullptr);

        for(size_t i = 0; i < this->m_swapchain_images.size(); i++) {
            vkDestroyBuffer(this->m_device, this->m_buffer_data.uniform_buffers[i], nullptr);
            vkFreeMemory(this->m_device, this->m_buffer_data.uniform_buffers_memory[i], nullptr);
        }

        vkDestroyDescriptorPool(this->m_device, this->m_unmapped_descriptor_pool, nullptr);
        vkDestroyDescriptorPool(this->m_device, this->m_texture_mapped_descriptor_pool, nullptr);
    }

    void Renderer::deleteTextureImage() {
        vkDestroySampler(this->m_device, this->m_texture_sampler, nullptr);

        for(size_t index = 0; index < this->m_texture_data.size(); index++) {
            vkDestroyImageView(this->m_device, this->m_texture_data[index].image_view, nullptr);
            vkDestroyImage(this->m_device, this->m_texture_data[index].image, nullptr);
            vkFreeMemory(this->m_device, this->m_texture_data[index].image_mem, nullptr);
        }
    }

    void Renderer::deleteDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(this->m_device, this->m_unmapped_descriptor_set_layout, nullptr);
        vkDestroyDescriptorSetLayout(this->m_device, this->m_texture_mapped_descriptor_set_layout, nullptr);
    }

    void Renderer::deleteBuffers() {
        vkDestroyBuffer(this->m_device, this->m_buffer_data.main_buffer, nullptr);
        vkFreeMemory(this->m_device, this->m_buffer_data.main_buffer_memory, nullptr);
    }

    void Renderer::deleteDepthImageData() {
        vkDestroyImageView(this->m_device, this->m_depth_image_view, nullptr);
        vkDestroyImage(this->m_device, this->m_depth_image, nullptr);
        vkFreeMemory(this->m_device, this->m_depth_image_memory, nullptr);
    }

    void Renderer::deleteSemaphores() {
        for(int32_t i = 0; i < this->m_max_frames_in_flight; i++) {
            vkDestroySemaphore(this->m_device, this->m_image_available_semaphore_set[i], nullptr);
            vkDestroySemaphore(this->m_device, this->m_render_finished_semaphore_set[i], nullptr);
            vkDestroyFence(this->m_device, this->m_flight_fences[i], nullptr);
        }
    }

    void Renderer::deleteCommandPool() {
        vkDestroyCommandPool(this->m_device, this->m_commandpool, nullptr);
    }

    void Renderer::deleteDevice() {
        vkDestroyDevice(this->m_device, nullptr);
    }

    void Renderer::deleteSurface() {
        vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
    }

    void Renderer::deleteDebugMessenger() {
        auto fun = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(this->m_instance, "vkDestroyDebugUtilsMessengerEXT");
        if(fun != nullptr) {
            fun(this->m_instance, this->m_debug_messenger, nullptr);
        }
    }

    void Renderer::deleteInstance() { vkDestroyInstance(this->m_instance, nullptr); }

    void Renderer::run() {
        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;
        // Finish renderer setup
        this->initGraphicsPipelines();
        this->initDepthResources();
        this->initFrameBuffers();
        this->initTextureSampler();

        this->initTextureImage();
        this->initBuffers();
        this->initDescriptorPool();

        this->initDescriptorSets(false);
        this->initDescriptorSets(true);

        this->initCommandBuffers();
        this->initSemaphores();
        
        // Time points for calculating fps
        if(this->m_count_fps)
            begin = std::chrono::steady_clock::now();
        
        // Set input type as movement
        this->m_p_window_wrap->setInputMode(DENG_INPUT_MOVEMENT);
        
        uint32_t fps = 0;
        while(is_running(this->m_p_window_wrap->getWindow())) {
            if(this->m_count_fps) end = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() >= 1000) {
                LOG("FPS " + std::to_string(fps));
                begin = std::chrono::steady_clock::now();
                fps = 0;
            }

            else fps++;
            update_window(this->m_p_window_wrap->getWindow());
            this->m_p_ev->update();
            // this->m_p_dengui_window->update();
            this->makeFrame();
        }
        vkDeviceWaitIdle(this->m_device);
        destroy_window(this->m_p_window_wrap->getWindow());
    }
} 