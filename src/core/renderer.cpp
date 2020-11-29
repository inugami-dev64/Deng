#include "deng_core.h"

namespace deng
{
    Renderer::Renderer(Window &win) {
        //Required extensions vector initialisation
        this->m_required_extensions_name.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        this->loadDataFromConf(DENG_TRUE, DENG_TRUE, DENG_FALSE);
        this->m_p_window = &win;

        // this->m_p_grid_manager = new dengUtils::GridManager(&this->m_grid, this->m_far_plane + 5, &this->m_environment_conf.grid_height, &this->m_environment_conf.grid_width, &this->m_environment_conf.grid_line_color_r, &this->m_environment_conf.grid_line_color_g, &this->m_environment_conf.grid_line_color_b);
        this->m_p_camera = new Camera({this->m_camera_conf.movement_x, this->m_camera_conf.movement_y, this->m_camera_conf.movement_z}, {this->m_camera_conf.mouse_movement_x, this->m_camera_conf.mouse_movement_y}, this->m_camera_conf.fov, this->m_near_plane, this->m_far_plane, this->m_p_window);
        this->m_p_ev = new dengMath::Events(this->m_p_window, this->m_p_camera);

        // this->initObjects(this->m_sample_object, "objects/obj1.obj", "textures/obj1.1.tga", DENG_COORDINATE_MODE_DEFAULT);
        
        // Grid is disabled for now :(
        // this->initGrid();
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
        #if !DISABLE_DENGUI
            this->initDengUI();
        #endif
    }

    void Renderer::initObjects(std::vector<dengUtils::GameObject> *p_game_objects) {
        this->m_p_game_objects = p_game_objects;
        
        this->initGraphicsPipelines();
        this->initDepthResources();
        this->initFrameBuffers();
        this->initTextureSampler();

        this->initTextureImage();
        this->initBuffers();
        this->initDescriptorPool();

        this->initDescriptorSets(DENG_FALSE);
        this->initDescriptorSets(DENG_TRUE);

        this->initCommandBuffers();
        this->initSemaphores();
    }

    Renderer::~Renderer() {
        // delete this->m_p_grid_manager;
        delete this->m_p_ev;
        #if !DISABLE_DENGUI
            delete this->m_p_dengui_window;
        #endif
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

    void Renderer::loadDataFromConf(const dengBool &load_camera_conf, const dengBool &load_environment_conf, const dengBool &load_dengUI_conf) {
        if(load_camera_conf) {
            this->m_camera_conf.movement_x = this->m_fm.getConfVal<float>("movement_x", "config/engine_camera.conf", nullptr, DENG_TRUE);
            this->m_camera_conf.movement_y = this->m_fm.getConfVal<float>("movement_y", "config/engine_camera.conf", nullptr, DENG_TRUE);
            this->m_camera_conf.movement_z = this->m_fm.getConfVal<float>("movement_z", "config/engine_camera.conf", nullptr, DENG_TRUE);

            this->m_camera_conf.mouse_movement_x = this->m_fm.getConfVal<float>("mouse_movement_x", "config/engine_camera.conf", nullptr, DENG_TRUE);
            this->m_camera_conf.mouse_movement_y = this->m_fm.getConfVal<float>("mouse_movement_y", "config/engine_camera.conf", nullptr, DENG_TRUE);
            this->m_camera_conf.fov = this->m_fm.getConfVal<float>("fov", "config/engine_camera.conf", nullptr, DENG_TRUE);
        }
        
        if(load_environment_conf) {
            this->m_environment_conf.environment_color_r = this->m_fm.getConfVal<float>("environment_color_r", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
            this->m_environment_conf.environment_color_g = this->m_fm.getConfVal<float>("environment_color_g", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
            this->m_environment_conf.environment_color_b = this->m_fm.getConfVal<float>("environment_color_b", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);

            this->m_environment_conf.show_grid = static_cast<dengBool>(this->m_fm.getConfVal<bool>("show_grid", "config/engine_editor_environment.conf", nullptr, DENG_TRUE));
            this->m_environment_conf.grid_height = this->m_fm.getConfVal<float>("grid_height", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
            this->m_environment_conf.grid_width = this->m_fm.getConfVal<float>("grid_width", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
            this->m_environment_conf.grid_line_color_r = this->m_fm.getConfVal<float>("grid_line_color_r", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
            this->m_environment_conf.grid_line_color_g = this->m_fm.getConfVal<float>("grid_line_color_g", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
            this->m_environment_conf.grid_line_color_b = this->m_fm.getConfVal<float>("grid_line_color_b", "config/engine_editor_environment.conf", nullptr, DENG_TRUE);
        }

        // if(load_dengUI_conf) {
        //     this->m_dengui_conf.dengui_window_color_r = this->m_fm.getConfVal<float>("dengui_window_color_r", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_window_color_g = this->m_fm.getConfVal<float>("dengui_window_color_g", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_window_color_b = this->m_fm.getConfVal<float>("dengui_window_color_b", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_window_color_a = this->m_fm.getConfVal<float>("dengui_window_color_a", "config/dengui.conf", nullptr, DENG_TRUE), nullptr, DENG_TRUE;

        //     this->m_dengui_conf.dengui_border_thickness = this->m_fm.getConfVal<float>("dengui_border_thickness", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_titlebar_height = this->m_fm.getConfVal<float>("dengui_titlebar_height", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_border_color_r = this->m_fm.getConfVal<float>("dengui_border_color_r", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_border_color_g = this->m_fm.getConfVal<float>("dengui_border_color_g", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_border_color_b = this->m_fm.getConfVal<float>("dengui_border_color_b", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_border_color_a = this->m_fm.getConfVal<float>("dengui_border_color_a", "config/dengui.conf", nullptr, DENG_TRUE);

        //     this->m_dengui_conf.dengui_minimizing_triangle_color_r = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_r", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_minimizing_triangle_color_g = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_g", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_minimizing_triangle_color_b = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_b", "config/dengui.conf", nullptr, DENG_TRUE);
        //     this->m_dengui_conf.dengui_minimizing_triangle_color_b = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_a", "config/dengui.conf", nullptr, DENG_TRUE);
        // }
    }

    // void Renderer::initGrid() {
    //     LOG("initGrid camera view matrix position is: {" + std::to_string(this->m_p_camera->view_matrix.getPosition().first) + ";" + std::to_string(this->m_p_camera->view_matrix.getPosition().second) + ";" + std::to_string(this->m_p_camera->view_matrix.getPosition().third) + "}");
    //     this->m_p_grid_manager->generateVertices(this->m_p_camera->view_matrix.getPosition());
    // }

    void Renderer::initInstance() {
        //initialise appinfo
        VkApplicationInfo local_appinfo{};
        local_appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        local_appinfo.pApplicationName = this->m_p_window->getTitle();
        local_appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        local_appinfo.pEngineName = "Deng";
        local_appinfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        local_appinfo.apiVersion = VK_API_VERSION_1_0;

        // LOG(local_appinfo.pApplicationName);

        //initialise create info
        VkInstanceCreateInfo local_instance_createInfo{}; 
        local_instance_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        local_instance_createInfo.pApplicationInfo = &local_appinfo;

        uint32_t local_extension_count;
        const char **local_extensions = get_required_surface_extensions(this->m_p_window->getWindow(), &local_extension_count, static_cast<int>(enable_validation_layers));
        local_instance_createInfo.enabledExtensionCount = local_extension_count;
        local_instance_createInfo.ppEnabledExtensionNames = local_extensions;

        LOG("Required extensions count is: " + std::to_string(local_instance_createInfo.enabledExtensionCount));

        VkDebugUtilsMessengerCreateInfoEXT local_debug_createInfo{};

        if(enable_validation_layers && !this->checkValidationLayerSupport()) {
            ERR("No validation layers available!");
        }
        else if(enable_validation_layers && this->checkValidationLayerSupport()) {
            local_instance_createInfo.enabledLayerCount = 1;
            local_instance_createInfo.ppEnabledLayerNames = &this->m_p_validation_layer;

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
        
        LOG("seg test!");
        if(vkCreateInstance(&local_instance_createInfo, nullptr, &this->m_instance) != VK_SUCCESS) {
            ERR("Failed to create an instance!");
        }

        else LOG("Successfully created an instance");
        LOG("seg test!");
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
        if(init_surface(this->m_p_window->getWindow(), &this->m_instance, &this->m_surface) != VK_SUCCESS) {
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
        local_logical_device_createinfo.enabledExtensionCount = this->m_required_extensions_name.size();
        local_logical_device_createinfo.ppEnabledExtensionNames = this->m_required_extensions_name.data();

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
            ERRME("Didn't find suitable surface format! Using first format!");
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
            ERRME("Didn't find suitable present mode! Using immediate instead!");
            this->m_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        if(this->m_p_device_swapchain_details->getCapabilities().currentExtent.width != UINT32_MAX && this->m_p_device_swapchain_details->getCapabilities().currentExtent.height != UINT32_MAX) {
            this->m_extent = this->m_p_device_swapchain_details->getCapabilities().currentExtent;
        }
        else {
            this->m_extent.width = this->m_p_window->getSize().first;
            this->m_extent.height = this->m_p_window->getSize().second;
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
            if(vkCreateImageView(this->m_device, &local_createInfo, nullptr, &this->m_swapchain_image_views[i]) != VK_SUCCESS) {
                ERR("Failed to create image views!");
            }
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


        if(vkCreateRenderPass(this->m_device, &local_renderpass_createinfo, nullptr, &this->m_renderpass) != VK_SUCCESS) {
            LOG("Failed to create render pass!");
        }
    }

    void Renderer::initDescriptorSetLayouts() {
        this->m_descriptor_set_layouts.first.first = DENG_PIPELINE_TYPE_UNMAPPED;
        this->m_descriptor_set_layouts.second.first = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;

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
        
        std::vector<VkDescriptorSetLayoutBinding> local_bindings;
        local_bindings.resize(1);
        local_bindings[0] = local_ubo_layout_binding;

        VkDescriptorSetLayoutCreateInfo local_layout_createInfo{};
        local_layout_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_descriptor_set_layouts.first.second) != VK_SUCCESS)
            ERR("Failed to create descriptor set layout!");

        else LOG("Successfully created pipeline layout for specific pipelines!");
        

        local_bindings.resize(2);
        local_bindings[0] = local_ubo_layout_binding;
        local_bindings[1] = local_sampler_layout_binding;
        
        local_layout_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_descriptor_set_layouts.second.second) != VK_SUCCESS)
            ERR("Failed to create descriptor set layout!");
        
        else LOG("Successfully created pipeline layout for textured objects!");
    }

    void Renderer::initPipelineLayouts() {
        this->m_pipeline_layouts.first.first = DENG_PIPELINE_TYPE_UNMAPPED;
        this->m_pipeline_layouts.second.first = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;

        VkPipelineLayoutCreateInfo local_pipeline_layout_createinfo{};
        local_pipeline_layout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        local_pipeline_layout_createinfo.setLayoutCount = 1;
        local_pipeline_layout_createinfo.pushConstantRangeCount = 0;
        local_pipeline_layout_createinfo.pSetLayouts = &this->m_descriptor_set_layouts.first.second;

        if(vkCreatePipelineLayout(this->m_device, &local_pipeline_layout_createinfo, nullptr, &this->m_pipeline_layouts.first.second) != VK_SUCCESS)
            ERR("Failed to create pipeline layout for unmapped objects!");

        local_pipeline_layout_createinfo.pSetLayouts = &this->m_descriptor_set_layouts.second.second;

        if(vkCreatePipelineLayout(this->m_device, &local_pipeline_layout_createinfo, nullptr, &this->m_pipeline_layouts.second.second) != VK_SUCCESS)
            ERR("Failed to create pipeline layout for textured objects!");
    }

    void Renderer::initCommandPool() {
        VkCommandPoolCreateInfo local_commandpool_createinfo{};
        local_commandpool_createinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        local_commandpool_createinfo.queueFamilyIndex = this->m_queue_families.getGraphicsFamily();
        // local_commandpool_createinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if(vkCreateCommandPool(this->m_device, &local_commandpool_createinfo, nullptr, &this->m_commandpool) != VK_SUCCESS) {
            ERR("Failed to create command pool!");
        }
        else {
            LOG("Successfully created commandpool!");
        }
    }

    #if !DISABLE_DENGUI
    void Renderer::initDengUI() {
        dengUI::WindowInfo local_window_info{};
        local_window_info.position = {0.0f, 0.0f};
        local_window_info.origin = {0.0f, 0.0f};
        local_window_info.vert_shader_path = "shaders/bin/dengui/ui_vert.spv";
        local_window_info.frag_shader_path = "shaders/bin/dengui/ui_frag.spv";
        local_window_info.p_gpu = &this->m_gpu;
        local_window_info.p_device = &this->m_device;
        local_window_info.p_file_manager = &this->m_fm;
        local_window_info.p_window = this->m_p_window;

        dengUI::BufferInfo local_bufferinfo{};
        local_bufferinfo.p_buffer_create_func = Renderer::makeBuffer;
        local_bufferinfo.p_buffer_memory_populate_func = Renderer::populateBufferMem;
        local_bufferinfo.p_buffer_copy_func = Renderer::copyBufferToBuffer;
        local_bufferinfo.p_commandpool = &this->m_commandpool;
        local_bufferinfo.p_graphics_queue = &this->m_queues.graphics_queue;
        local_bufferinfo.p_indices_size = &this->m_pipelines[DENGUI_PIPELINE_INDEX].indices_size;
        local_bufferinfo.p_staging_buffer = &this->m_buffers.staging_buffer;
        local_bufferinfo.p_staging_buffer_memory = &this->m_buffers.staging_buffer_memory;
        local_bufferinfo.p_vertices_buffer = &this->m_buffers.window_buffer;
        local_bufferinfo.p_vertices_buffer_memory = &this->m_buffers.window_buffer_memory;
        local_bufferinfo.p_indices_buffer = &this->m_buffers.window_index_buffer;
        local_bufferinfo.p_indices_buffer_memory = &this->m_buffers.window_index_buffer_memory;

        this->m_p_dengui_window = new dengUI::Window(local_window_info, local_bufferinfo);
    }
    #endif

    void Renderer::initGraphicsPipelines() {
        uint32_t total_texture_mapped_vertices_count = 0;
        uint32_t total_unmapped_vertices_count = 0;
        size_t index;

        for(index = 0; index < this->m_p_game_objects->size(); index++) {
            if((*this->m_p_game_objects)[index].pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED)
                total_texture_mapped_vertices_count += static_cast<uint32_t>((*this->m_p_game_objects)[index].texture_mapped_vertices_data.size());
            
            else if((*this->m_p_game_objects)[index].pipeline_type == DENG_PIPELINE_TYPE_UNMAPPED)
                total_unmapped_vertices_count += static_cast<uint32_t>((*this->m_p_game_objects)[index].unmapped_vertices_data.size());
        }

        this->m_pipelines[0].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;
        this->m_pipelines[1].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED;

        this->m_pipelines[0].pipeline_draw_mode = DENG_PIPELINE_DRAW_MODE_INDEXED;
        this->m_pipelines[1].pipeline_draw_mode = DENG_PIPELINE_DRAW_MODE_INDEXED;

        this->m_pipelines[0].p_pipeline_layout = &this->m_pipeline_layouts.second.second;
        this->m_pipelines[1].p_pipeline_layout = &this->m_pipeline_layouts.first.second;

        // this->m_pipelines[1].p_vertices_buffer = &this->m_buffers.grid_buffer;

        PipelineCreator texture_mapped_pipeline(&this->m_pipelines[0], &this->m_device, &this->m_fm, &this->m_extent, &this->m_renderpass);
        PipelineCreator unmapped_pipeline(&this->m_pipelines[1], &this->m_device, &this->m_fm, &this->m_extent, &this->m_renderpass);

        VkGraphicsPipelineCreateInfo local_texture_mapped_pipeline_createinfo = texture_mapped_pipeline.getGraphicsPipelineInfo("shaders/bin/deng/object_vert.spv", "shaders/bin/deng/object_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, DENG_TRUE, DENG_FALSE, 0);
        VkGraphicsPipelineCreateInfo local_unmapped_pipeline_createinfo = unmapped_pipeline.getGraphicsPipelineInfo("shaders/bin/deng/specified_vert.spv", "shaders/bin/deng/specified_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, DENG_TRUE, DENG_FALSE, 0);
        
        LOG("Pipeline createinfos created!");

        std::array<VkGraphicsPipelineCreateInfo, DENG_PIPELINE_COUNT> local_pipeline_createinfos;
        std::array<VkPipeline, DENG_PIPELINE_COUNT> local_pipelines;

        #if !DISABLE_DENGUI
            this->m_pipelines[1].pipeline_type = DENG_PIPELINE_TYPE_UI;
            this->m_pipelines[1].pipeline_draw_mode = DENG_PIPELINE_DRAW_MODE_INDEXED;
            this->m_pipelines[1].p_pipeline_layout = &this->m_pipeline_layouts.first.second;
            this->m_pipelines[1].p_descriptor_sets = &this->m_descriptor_sets.first.second;
            PipelineCreator local_ui_pipeline_creator(&this->m_pipelines[1], &this->m_device, &this->m_fm, &this->m_extent, &this->m_renderpass);
            VkGraphicsPipelineCreateInfo local_ui_pipeline_createinfo = local_ui_pipeline_creator.getGraphicsPipelineInfo("shaders/bin/dengui/ui_vert.spv", "shaders/bin/dengui/ui_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, DENG_TRUE, DENG_FALSE, 0);
            local_pipeline_createinfos = {local_main_pipeline_createinfo, local_ui_pipeline_createinfo};
        #else
            local_pipeline_createinfos = {local_texture_mapped_pipeline_createinfo, local_unmapped_pipeline_createinfo};
        #endif

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

    void Renderer::initTextureImage() {
        VkMemoryRequirements memory_requirements;
        size_t gen_index;

        for(gen_index = 0; gen_index < this->m_p_game_objects->size(); gen_index++) {
            LOG("model gen_index: " + gen_index);
            if((*this->m_p_game_objects)[gen_index].pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED) {
                memory_requirements =  BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->texture_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &this->m_buffer_data.staging_buffer);
                BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.staging_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
                vkBindBufferMemory(this->m_device, this->m_buffer_data.staging_buffer, this->m_buffer_data.staging_buffer_memory, 0);
                BufferHandler::populateBufferMem(&this->m_device, &memory_requirements.size, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->texture_pixels_data.data(), &this->m_buffer_data.staging_buffer_memory, 0);
            
                BufferHandler::makeImage(&this->m_device, &this->m_gpu, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->image, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->height, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->width, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
                BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->image_memory, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->texture_size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                vkBindImageMemory(this->m_device, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->image, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->image_memory, 0);

                BufferHandler::transitionImageLayout(&this->m_device, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->image, &this->m_commandpool, &this->m_queues.graphics_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
                BufferHandler::copyBufferToImage(&this->m_device, &this->m_commandpool, &this->m_queues.graphics_queue, &this->m_buffer_data.staging_buffer, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->image, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->width, (*this->m_p_game_objects)[gen_index].p_raw_texture_data->height);
                BufferHandler::transitionImageLayout(&this->m_device, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->image, &this->m_commandpool, &this->m_queues.graphics_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                
                vkDestroyBuffer(this->m_device, this->m_buffer_data.staging_buffer, nullptr);
                vkFreeMemory(this->m_device, this->m_buffer_data.staging_buffer_memory, nullptr);
                LOG("seg test");
                
                VkImageViewCreateInfo local_viewinfo = this->getImageViewInfo((*this->m_p_game_objects)[gen_index].p_raw_texture_data->image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
                if(vkCreateImageView(this->m_device, &local_viewinfo, nullptr, &(*this->m_p_game_objects)[gen_index].p_raw_texture_data->image_view) != VK_SUCCESS)
                    ERR("Failed to create texture image view for model: " + (*this->m_p_game_objects)[gen_index].object_name);

                LOG("Successfully created texture image for model: " + (*this->m_p_game_objects)[gen_index].object_name);
            }
        }
    }

    void Renderer::initTextureSampler() {
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

        if(vkCreateSampler(this->m_device, &local_samplerInfo, nullptr, &this->m_sampler) != VK_SUCCESS)
            ERR("Failed to create texture sampler!");
    }

    void Renderer::initBuffers() {
        // main object buffer
        size_t index;
        LOG("Initialising main buffer!");
        VkDeviceSize total_size = 0, data_size = 0;
        VkMemoryRequirements memory_requirements;

        for(index = 0; index < this->m_p_game_objects->size(); index++) {
            LOG("Counting model bytes of model: " + (*this->m_p_game_objects)[index].object_name);
            switch ((*this->m_p_game_objects)[index].pipeline_type)
            {
            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
                total_size += (*this->m_p_game_objects)[index].texture_mapped_vertices_data.size() * sizeof((*this->m_p_game_objects)[index].texture_mapped_vertices_data[0]);
                break;

            case DENG_PIPELINE_TYPE_UNMAPPED:
                total_size += (*this->m_p_game_objects)[index].unmapped_vertices_data.size() * sizeof((*this->m_p_game_objects)[index].unmapped_vertices_data[0]);
                break;
            
            default:
                break;
            }

            total_size += (*this->m_p_game_objects)[index].indices_data.size() * sizeof((*this->m_p_game_objects)[index].indices_data[0]);
        }

        LOG("Buffer size is: " + std::to_string(total_size));
        memory_requirements = BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &total_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &this->m_buffer_data.staging_buffer);
        BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.staging_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        vkBindBufferMemory(this->m_device, this->m_buffer_data.staging_buffer, this->m_buffer_data.staging_buffer_memory, 0);
        
        for(index = 0, total_size = 0; index < this->m_p_game_objects->size(); index++) {
            switch ((*this->m_p_game_objects)[index].pipeline_type)
            {
            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
                data_size = (*this->m_p_game_objects)[index].texture_mapped_vertices_data.size() * sizeof((*this->m_p_game_objects)[index].texture_mapped_vertices_data[0]);
                BufferHandler::populateBufferMem(&this->m_device, &data_size, (*this->m_p_game_objects)[index].texture_mapped_vertices_data.data(), &this->m_buffer_data.staging_buffer_memory, total_size);
                (*this->m_p_game_objects)[index].vertices_buffer_memory_offset = total_size;
                total_size += data_size;
                break;

            case DENG_PIPELINE_TYPE_UNMAPPED:
                data_size = (*this->m_p_game_objects)[index].unmapped_vertices_data.size() * sizeof((*this->m_p_game_objects)[index].unmapped_vertices_data[0]);
                BufferHandler::populateBufferMem(&this->m_device, &data_size, (*this->m_p_game_objects)[index].unmapped_vertices_data.data(), &this->m_buffer_data.staging_buffer_memory, total_size);
                (*this->m_p_game_objects)[index].vertices_buffer_memory_offset = total_size;
                total_size += data_size;
                break;
            
            default:
                break;
            }

            data_size = (*this->m_p_game_objects)[index].indices_data.size() * sizeof((*this->m_p_game_objects)[index].indices_data[0]);
            BufferHandler::populateBufferMem(&this->m_device, &data_size, (*this->m_p_game_objects)[index].indices_data.data(), &this->m_buffer_data.staging_buffer_memory, total_size);
            (*this->m_p_game_objects)[index].indices_buffer_memory_offset = total_size;
            total_size += data_size;
        }

        memory_requirements = BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &total_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, &this->m_buffer_data.main_buffer);
        BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.main_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkBindBufferMemory(this->m_device, this->m_buffer_data.main_buffer, this->m_buffer_data.main_buffer_memory, 0);
        BufferHandler::copyBufferToBuffer(&this->m_device, &this->m_commandpool, &this->m_queues.graphics_queue, &this->m_buffer_data.staging_buffer, &this->m_buffer_data.main_buffer, &total_size, 0);

        vkDestroyBuffer(this->m_device, this->m_buffer_data.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, this->m_buffer_data.staging_buffer_memory, nullptr);


        // LOG("Successfully allocated memory for main buffer!");
        // if(this->m_environment_conf.show_grid == DENG_TRUE) {
        //     // grid buffer
        //     local_size = sizeof(this->m_grid.vertex_data[0]) * this->m_grid.vertex_data.size();

        //     this->makeBuffer(&this->m_device, &this->m_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &this->m_buffers.staging_buffer, &this->m_buffers.staging_buffer_memory, nullptr);
        //     this->populateBufferMem(&this->m_device, &this->m_gpu, &local_size, this->m_grid.vertex_data.data(), &this->m_buffers.staging_buffer, &this->m_buffers.staging_buffer_memory);

        //     this->makeBuffer(&this->m_device, &this->m_gpu, &local_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &this->m_buffers.grid_buffer, &this->m_buffers.grid_buffer_memory, nullptr);
        //     this->copyBufferToBuffer(&this->m_device, &this->m_commandpool, &this->m_queues.graphics_queue, &this->m_buffers.staging_buffer, &this->m_buffers.grid_buffer, &local_size);

        //     vkDestroyBuffer(this->m_device, this->m_buffers.staging_buffer, nullptr);
        //     vkFreeMemory(this->m_device, this->m_buffers.staging_buffer_memory, nullptr);
        // }

        
        total_size = sizeof(dengMath::UniformBufferData);

        this->m_buffer_data.uniform_buffers.resize(this->m_swapchain_images.size());
        this->m_buffer_data.uniform_buffers_memory.resize(this->m_swapchain_images.size());

        for(index = 0; index < this->m_swapchain_images.size(); index++) { 
            memory_requirements = BufferHandler::makeBuffer(&this->m_device, &this->m_gpu, &total_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &this->m_buffer_data.uniform_buffers[index]);
            BufferHandler::allocateMemory(&this->m_device, &this->m_gpu, &this->m_buffer_data.uniform_buffers_memory[index], memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            vkBindBufferMemory(this->m_device, this->m_buffer_data.uniform_buffers[index], this->m_buffer_data.uniform_buffers_memory[index], 0);
        }
    }

    void Renderer::initDescriptorPool() {
        this->m_descriptor_pool_sets.first.first = DENG_PIPELINE_TYPE_UNMAPPED;
        this->m_descriptor_pool_sets.second.first = DENG_PIPELINE_TYPE_TEXTURE_MAPPED;

        std::vector<VkDescriptorPoolSize> local_descriptor_pool_sizes{};
        local_descriptor_pool_sizes.resize(1);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());

        VkDescriptorPoolCreateInfo local_desc_pool_createInfo{};
        local_desc_pool_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        local_desc_pool_createInfo.poolSizeCount = static_cast<uint32_t>(local_descriptor_pool_sizes.size());
        local_desc_pool_createInfo.pPoolSizes = local_descriptor_pool_sizes.data();
        local_desc_pool_createInfo.maxSets = static_cast<uint32_t>(this->m_swapchain_images.size());

        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_descriptor_pool_sets.first.second) != VK_SUCCESS)
            ERR("Failed to create descriptor pool!");

        local_descriptor_pool_sizes.resize(2);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());
        local_descriptor_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        local_descriptor_pool_sizes[1].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());

        local_desc_pool_createInfo.poolSizeCount = static_cast<uint32_t>(local_descriptor_pool_sizes.size());
        local_desc_pool_createInfo.pPoolSizes = local_descriptor_pool_sizes.data();

        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_descriptor_pool_sets.second.second) != VK_SUCCESS)
            ERR("Failed to create descriptor pool!");
    }

    void Renderer::initDescriptorSets(dengBool init_texture_mapped_descriptor_sets) {
        size_t index, model_index;
        VkDescriptorBufferInfo local_bufferinfo{};
        std::vector<VkWriteDescriptorSet> local_description_writes{};
        VkDescriptorImageInfo local_desc_imageInfo{};
        
        VkDescriptorSetAllocateInfo local_allocinfo{};
        local_allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

        if(!init_texture_mapped_descriptor_sets) {
            std::vector<VkDescriptorSetLayout> local_descriptor_set_layouts(this->m_swapchain_images.size(), this->m_descriptor_set_layouts.first.second);
            local_allocinfo.descriptorPool = this->m_descriptor_pool_sets.first.second;
            local_allocinfo.descriptorSetCount = static_cast<uint32_t>(local_descriptor_set_layouts.size());
            local_allocinfo.pSetLayouts = local_descriptor_set_layouts.data();
            this->m_unmapped_descriptor_sets.resize(local_descriptor_set_layouts.size());

            if(vkAllocateDescriptorSets(this->m_device, &local_allocinfo, this->m_unmapped_descriptor_sets.data()) != VK_SUCCESS)
                ERR("Failed to allocate descriptor sets!");

            else LOG("Successfully allocated unmapped descriptor sets!");

            for(index = 0; index < this->m_swapchain_images.size(); index++) {
                local_bufferinfo.buffer = this->m_buffer_data.uniform_buffers[index];
                local_bufferinfo.offset = 0;
                local_bufferinfo.range = sizeof(dengMath::UniformBufferData);

                local_description_writes.resize(1);
                local_description_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                local_description_writes[0].dstSet = this->m_unmapped_descriptor_sets[index];
                local_description_writes[0].dstBinding = 0;
                local_description_writes[0].dstArrayElement = 0;
                local_description_writes[0].descriptorCount = 1;
                local_description_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                local_description_writes[0].pBufferInfo = &local_bufferinfo;

                vkUpdateDescriptorSets(this->m_device, local_description_writes.size(), local_description_writes.data(), 0, nullptr);
            }
        }

        else {
            std::vector<VkDescriptorSetLayout> local_descriptor_set_layouts(this->m_swapchain_images.size(), this->m_descriptor_set_layouts.second.second);
            local_allocinfo.descriptorPool = this->m_descriptor_pool_sets.second.second;
            local_allocinfo.descriptorSetCount = static_cast<uint32_t>(local_descriptor_set_layouts.size());
            local_allocinfo.pSetLayouts = local_descriptor_set_layouts.data();
            
            for(model_index = 0; model_index < this->m_p_game_objects->size(); model_index++) {
                if((*this->m_p_game_objects)[model_index].pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED) {                
                    (*this->m_p_game_objects)[model_index].p_raw_texture_data->descriptor_sets.resize(this->m_swapchain_images.size());

                    if(vkAllocateDescriptorSets(this->m_device, &local_allocinfo, (*this->m_p_game_objects)[model_index].p_raw_texture_data->descriptor_sets.data()) != VK_SUCCESS)
                        ERR("Failed to allocate descriptor sets!");

                    else LOG("Successfully allocated texture mapped descriptor sets for model !");

                    for(size_t index = 0; index < this->m_swapchain_images.size(); index++) {
                        local_bufferinfo.buffer = this->m_buffer_data.uniform_buffers[index];
                        local_bufferinfo.offset = 0;
                        local_bufferinfo.range = sizeof(dengMath::UniformBufferData);

                        local_desc_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        local_desc_imageInfo.imageView = (*this->m_p_game_objects)[model_index].p_raw_texture_data->image_view;
                        local_desc_imageInfo.sampler = this->m_sampler;

                        local_description_writes.resize(2);
                        local_description_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        local_description_writes[0].dstSet = (*this->m_p_game_objects)[model_index].p_raw_texture_data->descriptor_sets[index];
                        local_description_writes[0].dstBinding = 0;
                        local_description_writes[0].dstArrayElement = 0;
                        local_description_writes[0].descriptorCount = 1;
                        local_description_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        local_description_writes[0].pBufferInfo = &local_bufferinfo;

                        local_description_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        local_description_writes[1].dstSet = (*this->m_p_game_objects)[model_index].p_raw_texture_data->descriptor_sets[index];
                        local_description_writes[1].dstBinding = 1;
                        local_description_writes[1].dstArrayElement = 0;
                        local_description_writes[1].descriptorCount = 1;
                        local_description_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        local_description_writes[1].pImageInfo = &local_desc_imageInfo;

                        vkUpdateDescriptorSets(this->m_device, local_description_writes.size(), local_description_writes.data(), 0, nullptr);
                    }
                }
            }
        }
    }

    void Renderer::initCommandBuffers() {
        size_t model_index, commandbuffer_index;
        LOG("Framebuffer size: " + std::to_string(this->m_swapchain_framebuffers.size()));

        this->m_commandbuffers.resize(this->m_swapchain_framebuffers.size());

        VkCommandBufferAllocateInfo local_commandbuffer_allocation_info{};
        local_commandbuffer_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocation_info.commandPool = this->m_commandpool;
        local_commandbuffer_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocation_info.commandBufferCount = static_cast<uint32_t>(this->m_commandbuffers.size());

        if(vkAllocateCommandBuffers(this->m_device, &local_commandbuffer_allocation_info, this->m_commandbuffers.data()))
            ERR("Failed to allocate command buffers!");
        
        else LOG("Successfully allocated command buffers!");
    

        for(commandbuffer_index = 0; commandbuffer_index < this->m_commandbuffers.size(); commandbuffer_index++) {
            VkCommandBufferBeginInfo local_commandbuffer_begininfo{};
            local_commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(this->m_commandbuffers[commandbuffer_index], &local_commandbuffer_begininfo) != VK_SUCCESS)
                ERR("Failed to begin recording command buffers!");

            else LOG("Successfully begun to record command buffers!");

            VkRenderPassBeginInfo local_renderpass_begininfo{};
            local_renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            local_renderpass_begininfo.renderPass = this->m_renderpass;
            local_renderpass_begininfo.framebuffer = this->m_swapchain_framebuffers[commandbuffer_index];
            local_renderpass_begininfo.renderArea.offset = {0, 0};
            local_renderpass_begininfo.renderArea.extent = this->m_extent;

            std::array<VkClearValue, 2> local_clear_values;

            local_clear_values[0].color = {this->m_environment_conf.environment_color_r, this->m_environment_conf.environment_color_g, this->m_environment_conf.environment_color_b, 1.0f};
            local_clear_values[1].depthStencil = {1.0f, 0};

            local_renderpass_begininfo.clearValueCount = local_clear_values.size();
            local_renderpass_begininfo.pClearValues = local_clear_values.data();
            
            vkCmdBeginRenderPass(this->m_commandbuffers[commandbuffer_index], &local_renderpass_begininfo, VK_SUBPASS_CONTENTS_INLINE);
                LOG("Successfully began renderpass!");
                
                for(model_index = 0; model_index < this->m_p_game_objects->size(); model_index++) {
                    LOG("Vertices offset for model " + (*this->m_p_game_objects)[model_index].object_name + " is: " + std::to_string((*this->m_p_game_objects)[model_index].vertices_buffer_memory_offset));
                    LOG("Indices offset for model " + (*this->m_p_game_objects)[model_index].object_name + " is: " + std::to_string((*this->m_p_game_objects)[model_index].indices_buffer_memory_offset));
                    
                    vkCmdBindVertexBuffers(this->m_commandbuffers[commandbuffer_index], 0, 1, &this->m_buffer_data.main_buffer, &(*this->m_p_game_objects)[model_index].vertices_buffer_memory_offset);
                    vkCmdBindIndexBuffer(this->m_commandbuffers[commandbuffer_index], this->m_buffer_data.main_buffer, (*this->m_p_game_objects)[model_index].indices_buffer_memory_offset, VK_INDEX_TYPE_UINT32);
                    
                    if((*this->m_p_game_objects)[model_index].pipeline_type == DENG_PIPELINE_TYPE_TEXTURE_MAPPED) {
                        vkCmdBindPipeline(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipelines[0].pipeline);   
                        vkCmdBindDescriptorSets(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_pipelines[0].p_pipeline_layout, 0, 1, &(*this->m_p_game_objects)[model_index].p_raw_texture_data->descriptor_sets[commandbuffer_index], 0, nullptr);                    
                    }

                    else if((*this->m_p_game_objects)[model_index].pipeline_type == DENG_PIPELINE_TYPE_UNMAPPED) {
                        vkCmdBindPipeline(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipelines[1].pipeline);
                        vkCmdBindDescriptorSets(this->m_commandbuffers[commandbuffer_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_pipelines[1].p_pipeline_layout, 0, 1, &this->m_unmapped_descriptor_sets[commandbuffer_index], 0, nullptr);
                        // vkCmdDraw(this->m_commandbuffers[commandbuffer_index], (*this->m_p_game_objects)[model_index].unmapped_vertices_data->size(), 1, 0, 0);                
                    }

                    LOG("Indices size check before draw command: " + std::to_string((*this->m_p_game_objects)[model_index].indices_data.size()));
                    vkCmdDrawIndexed(this->m_commandbuffers[commandbuffer_index], (*this->m_p_game_objects)[model_index].indices_data.size(), 1, 0, 0, 0);
                }

            vkCmdEndRenderPass(this->m_commandbuffers[commandbuffer_index]);
            LOG("Ended renderPass!");

            if(vkEndCommandBuffer(this->m_commandbuffers[commandbuffer_index]) != VK_SUCCESS)
                ERR("Failed to end recording command buffer!");
        }
    }

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


    /* frame update functions */
    void Renderer::makeFrame() {
        vkWaitForFences(this->m_device, 1, &this->m_flight_fences[this->m_current_frame], VK_TRUE, UINT64_MAX);

        uint32_t image_index;
        VkResult result = vkAcquireNextImageKHR(this->m_device, this->m_swapchain, UINT64_MAX, this->m_image_available_semaphore_set[this->m_current_frame], VK_NULL_HANDLE, &image_index);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            LOG("Acquiring new image from swap chain timed out!");
            return;
        }
        
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            ERR("Error acquiring swap chain image!");

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
        std::cerr << "validation layer: " << p_callback_data->pMessage << std::endl;
        return VK_FALSE;
    }

    VkResult Renderer::makeDebugMessenger(const VkDebugUtilsMessengerCreateInfoEXT *p_createinfo) {
        auto fun = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(this->m_instance, "vkCreateDebugUtilsMessengerEXT");
        if(fun == nullptr) {
            ERRME("Couldn't find vkCreateDebugUtilsMessengerEXT locations!");
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

        vkDestroyPipelineLayout(this->m_device, this->m_pipeline_layouts.first.second, nullptr);
        vkDestroyPipelineLayout(this->m_device, this->m_pipeline_layouts.second.second, nullptr);
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

        vkDestroyDescriptorPool(this->m_device, this->m_descriptor_pool_sets.first.second, nullptr);
        vkDestroyDescriptorPool(this->m_device, this->m_descriptor_pool_sets.second.second, nullptr);
    }

    void Renderer::deleteTextureImage() {
        vkDestroySampler(this->m_device, this->m_sampler, nullptr);

        for(size_t index = 0; index < this->m_p_game_objects->size(); index++) {
            vkDestroyImageView(this->m_device, (*this->m_p_game_objects)[index].p_raw_texture_data->image_view, nullptr);
            vkDestroyImage(this->m_device, (*this->m_p_game_objects)[index].p_raw_texture_data->image, nullptr);
            vkFreeMemory(this->m_device, (*this->m_p_game_objects)[index].p_raw_texture_data->image_memory, nullptr);
        }
    }

    void Renderer::deleteDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(this->m_device, this->m_descriptor_set_layouts.first.second, nullptr);
        vkDestroyDescriptorSetLayout(this->m_device, this->m_descriptor_set_layouts.second.second, nullptr);
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

    void Renderer::deleteInstance() {
        vkDestroyInstance(this->m_instance, nullptr);
    }

    VkDeviceSize Renderer::getBufferMemoryOffset() { return this->m_maximum_offset; }

    void Renderer::run() {
        this->m_p_window->setInputMode(DENG_INPUT_MOVEMENT);
        while(1) {
            update_window(this->m_p_window->getWindow());
            this->m_p_ev->update();
            // this->m_p_dengui_window->update();
            this->makeFrame();
        }
        vkDeviceWaitIdle(this->m_device);
        destroy_window(this->m_p_window->getWindow());
    }
} 