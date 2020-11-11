#include "deng_core.h"

namespace deng
{
    Renderer::Renderer(Window &win) {
        //Required extensions vector initialisation
        this->m_required_extensions_name.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        this->loadDataFromConf(DENG_TRUE, DENG_TRUE, DENG_TRUE);
        this->m_p_window = &win;

        this->m_p_grid_manager = new dengUtils::GridManager(&this->m_grid, this->m_far_plane + 5, &this->m_environment_conf.grid_height, &this->m_environment_conf.grid_width, &this->m_environment_conf.grid_line_color_r, &this->m_environment_conf.grid_line_color_g, &this->m_environment_conf.grid_line_color_b);
        this->m_p_camera = new Camera({this->m_camera_conf.movement_x, this->m_camera_conf.movement_y, this->m_camera_conf.movement_z}, {this->m_camera_conf.mouse_movement_x, this->m_camera_conf.mouse_movement_y}, this->m_camera_conf.fov, this->m_near_plane, this->m_far_plane, this->m_p_window);
        this->m_p_ev = new dengMath::Events(this->m_p_window, this->m_p_camera);

        this->initObjects(this->m_sample_object, "objects/obj1.obj", "textures/obj1.tga", DENG_COORDINATE_MODE_DEFAULT);
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
        this->initGraphicsPipelines();
        this->initDepthResources();
        this->initFrameBuffers();
        this->initTextureImage(this->m_sample_object); 
        this->initTextureSampler(this->m_sample_object);
        this->initBuffers(this->m_sample_object);
        this->initDescriptorPool();
        this->initDescriptorSets();
        this->initCommandBuffers();
        this->initSemaphores();
    }

    Renderer::~Renderer() {
        delete this->m_p_grid_manager;
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
        this->deleteTextureImage(this->m_sample_object);
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
        if(load_camera_conf == DENG_TRUE) {
            this->m_camera_conf.movement_x = this->m_fm.getConfVal<float>("movement_x", "config/engine_camera.conf");
            this->m_camera_conf.movement_y = this->m_fm.getConfVal<float>("movement_y", "config/engine_camera.conf");
            this->m_camera_conf.movement_z = this->m_fm.getConfVal<float>("movement_z", "config/engine_camera.conf");

            this->m_camera_conf.mouse_movement_x = this->m_fm.getConfVal<float>("mouse_movement_x", "config/engine_camera.conf");
            this->m_camera_conf.mouse_movement_y = this->m_fm.getConfVal<float>("mouse_movement_y", "config/engine_camera.conf");
            this->m_camera_conf.fov = this->m_fm.getConfVal<float>("fov", "config/engine_camera.conf");
        }
        
        if(load_environment_conf == DENG_TRUE) {
            this->m_environment_conf.environment_color_r = this->m_fm.getConfVal<float>("environment_color_r", "config/engine_editor_environment.conf");
            this->m_environment_conf.environment_color_g = this->m_fm.getConfVal<float>("environment_color_g", "config/engine_editor_environment.conf");
            this->m_environment_conf.environment_color_b = this->m_fm.getConfVal<float>("environment_color_b", "config/engine_editor_environment.conf");

            this->m_environment_conf.show_grid = static_cast<dengBool>(this->m_fm.getConfVal<bool>("show_grid", "config/engine_editor_environment.conf"));
            this->m_environment_conf.grid_height = this->m_fm.getConfVal<float>("grid_height", "config/engine_editor_environment.conf");
            this->m_environment_conf.grid_width = this->m_fm.getConfVal<float>("grid_width", "config/engine_editor_environment.conf");
            this->m_environment_conf.grid_line_color_r = this->m_fm.getConfVal<float>("grid_line_color_r", "config/engine_editor_environment.conf");
            this->m_environment_conf.grid_line_color_g = this->m_fm.getConfVal<float>("grid_line_color_g", "config/engine_editor_environment.conf");
            this->m_environment_conf.grid_line_color_b = this->m_fm.getConfVal<float>("grid_line_color_b", "config/engine_editor_environment.conf");
        }

        if(load_dengUI_conf == DENG_TRUE) {
            this->m_dengui_conf.dengui_window_color_r = this->m_fm.getConfVal<float>("dengui_window_color_r", "config/dengui.conf");
            this->m_dengui_conf.dengui_window_color_g = this->m_fm.getConfVal<float>("dengui_window_color_g", "config/dengui.conf");
            this->m_dengui_conf.dengui_window_color_b = this->m_fm.getConfVal<float>("dengui_window_color_b", "config/dengui.conf");
            this->m_dengui_conf.dengui_window_color_a = this->m_fm.getConfVal<float>("dengui_window_color_a", "config/dengui.conf");

            this->m_dengui_conf.dengui_border_thickness = this->m_fm.getConfVal<float>("dengui_border_thickness", "config/dengui.conf");
            this->m_dengui_conf.dengui_titlebar_height = this->m_fm.getConfVal<float>("dengui_titlebar_height", "config/dengui.conf");
            this->m_dengui_conf.dengui_border_color_r = this->m_fm.getConfVal<float>("dengui_border_color_r", "config/dengui.conf");
            this->m_dengui_conf.dengui_border_color_g = this->m_fm.getConfVal<float>("dengui_border_color_g", "config/dengui.conf");
            this->m_dengui_conf.dengui_border_color_b = this->m_fm.getConfVal<float>("dengui_border_color_b", "config/dengui.conf");
            this->m_dengui_conf.dengui_border_color_a = this->m_fm.getConfVal<float>("dengui_border_color_a", "config/dengui.conf");

            this->m_dengui_conf.dengui_minimizing_triangle_color_r = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_r", "config/dengui.conf");
            this->m_dengui_conf.dengui_minimizing_triangle_color_g = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_g", "config/dengui.conf");
            this->m_dengui_conf.dengui_minimizing_triangle_color_b = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_b", "config/dengui.conf");
            this->m_dengui_conf.dengui_minimizing_triangle_color_b = this->m_fm.getConfVal<float>("dengui_minimizing_triangle_color_a", "config/dengui.conf");
        }
    }

    void Renderer::initObjects(dengUtils::GameObject &obj, const std::string &obj_filepath, const std::string &texture_filepath, const dengCoordinateMode &coordinate_mode) {
        dengUtils::ObjLoader obj_loader(obj_filepath, coordinate_mode);
        obj.origin = {0.0f, 0.6f, 0.0f};
        obj_loader.getObjVerticesAndIndices(obj);

        dengTextureFormat local_tex_format = dengUtils::getTexFileFormat(texture_filepath);
        dengUtils::ObjRawTextureData texture_data;

        switch (local_tex_format)
        {
        case DENG_TEXTURE_FORMAT_BMP: {
            dengUtils::TextureLoaderBMP local_tex_loader(texture_filepath);
            local_tex_loader.getTextureDetails(texture_data.p_width, texture_data.p_height, texture_data.p_texture_size, texture_data.texture_pixels_data);
            break;
        }

        case DENG_TEXTURE_FORMAT_TGA: {
            dengUtils::TextureLoaderTGA local_tex_loader(texture_filepath);
            local_tex_loader.getTextureDetails(texture_data.p_width, texture_data.p_height, texture_data.p_texture_size, texture_data.texture_pixels_data);
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

        obj.raw_texture_data = texture_data;

        obj.model_matrix.setRotation(0, 0, 0);
        obj.model_matrix.setScale(1, 1, 1);
        obj.model_matrix.setTransformation(0, 0, 0);
    }

    void Renderer::initGrid() {
        LOG("initGrid camera view matrix position is: {" + std::to_string(this->m_p_camera->view_matrix.getPosition().first) + ";" + std::to_string(this->m_p_camera->view_matrix.getPosition().second) + ";" + std::to_string(this->m_p_camera->view_matrix.getPosition().third) + "}");
        this->m_p_grid_manager->generateVertices(this->m_p_camera->view_matrix.getPosition());
    }

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

        //get count of required GLFW extensions
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
            uint32_t score = HardwareSpecs::getDeviceScore(local_devices[i], this->m_required_extension_names);
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
        this->m_descriptor_set_layouts.first.first = DENG_PIPELINE_TYPE_SPECIFIED;
        this->m_descriptor_set_layouts.second.first = DENG_PIPELINE_TYPE_OBJECT_BASED;

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
        local_bindings = {local_ubo_layout_binding};

        VkDescriptorSetLayoutCreateInfo local_layout_createInfo{};
        local_layout_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_descriptor_set_layouts.first.second) != VK_SUCCESS) {
            ERR("Failed to create descriptor set layout!");
        }

        else {
            LOG("Successfully created pipeline layout for specific pipelines!");
        }

        local_bindings.resize(2);
        local_bindings = {local_ubo_layout_binding, local_sampler_layout_binding};
        
        local_layout_createInfo.bindingCount = local_bindings.size();
        local_layout_createInfo.pBindings = local_bindings.data();

        if(vkCreateDescriptorSetLayout(this->m_device, &local_layout_createInfo, nullptr, &this->m_descriptor_set_layouts.second.second)) {
            ERR("Failed to create descriptor set layout!");
        }
        
        else {
            LOG("Successfully created pipeline layout for object based pipelines!");
        }
    }

    void Renderer::initPipelineLayouts() {
        this->m_pipeline_layouts.first.first = DENG_PIPELINE_TYPE_SPECIFIED;
        this->m_pipeline_layouts.second.first = DENG_PIPELINE_TYPE_OBJECT_BASED;

        VkPipelineLayoutCreateInfo local_pipeline_layout_createinfo{};
        local_pipeline_layout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        local_pipeline_layout_createinfo.setLayoutCount = 1;
        local_pipeline_layout_createinfo.pushConstantRangeCount = 0;
        local_pipeline_layout_createinfo.pSetLayouts = &this->m_descriptor_set_layouts.first.second;

        if(vkCreatePipelineLayout(this->m_device, &local_pipeline_layout_createinfo, nullptr, &this->m_pipeline_layouts.first.second) != VK_SUCCESS) {
            ERR("Failed to create pipeline layout!");
        }

        local_pipeline_layout_createinfo.pSetLayouts = &this->m_descriptor_set_layouts.second.second;

        if(vkCreatePipelineLayout(this->m_device, &local_pipeline_layout_createinfo, nullptr, &this->m_pipeline_layouts.second.second) != VK_SUCCESS) {
            ERR("Failed to create pipeline layout!");
        }
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
        local_bufferinfo.p_indices_size = &this->m_pipelines[1].indices_size;
        local_bufferinfo.p_staging_buffer = &this->m_buffers.staging_buffer;
        local_bufferinfo.p_staging_buffer_memory = &this->m_buffers.staging_buffer_memory;
        local_bufferinfo.p_vertices_buffer = &this->m_buffers.window_buffer;
        local_bufferinfo.p_vertices_buffer_memory = &this->m_buffers.window_buffer_memory;
        local_bufferinfo.p_indices_buffer = &this->m_buffers.window_index_buffer;
        local_bufferinfo.p_indices_buffer_memory = &this->m_buffers.window_index_buffer_memory;

        this->m_p_dengui_window = new dengUI::Window(local_window_info, local_bufferinfo);
    }

    void Renderer::initGraphicsPipelines() {
        this->m_pipelines[0].pipeline_type = DENG_PIPELINE_TYPE_OBJECT_BASED;
        // this->m_pipelines[1].pipeline_type = DENG_PIPELINE_TYPE_SPECIFIED;

        this->m_pipelines[0].pipeline_draw_mode = DENG_PIPELINE_DRAW_MODE_LINEAR;
        // this->m_pipelines[1].pipeline_draw_mode = DENG_PIPELINE_DRAW_MODE_LINEAR;

        this->m_pipelines[0].p_pipeline_layout = &this->m_pipeline_layouts.second.second;
        // this->m_pipelines[1].p_pipeline_layout = &this->m_pipeline_layouts.first.second;

        this->m_pipelines[0].p_descriptor_sets = &this->m_descriptor_sets.second.second;
        // this->m_pipelines[1].p_descriptor_sets = &this->m_descriptor_sets.first.second;

        this->m_pipelines[0].p_vertices_buffer = &this->m_buffers.vertex_buffer;
        // this->m_pipelines[1].p_vertices_buffer = &this->m_buffers.grid_buffer;

        this->m_pipelines[0].vertices_size = static_cast<uint32_t>(this->m_sample_object.vertex_data.size());
        // this->m_pipelines[1].vertices_size = static_cast<uint32_t>(this->m_grid.vertex_data.size());

        PipelineCreator local_main_pipeline_creator(&this->m_pipelines[0], &this->m_device, &this->m_fm, &this->m_extent, &this->m_renderpass);
        // PipelineCreator local_grid_pipeline_creator(&this->m_pipelines[1], &this->m_device, &this->m_fm, &this->m_extent, &this->m_renderpass);

        VkGraphicsPipelineCreateInfo local_main_pipeline_createinfo = local_main_pipeline_creator.getGraphicsPipelineInfo("shaders/bin/deng/object_vert.spv", "shaders/bin/deng/object_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, DENG_TRUE, DENG_FALSE, 0);
        // VkGraphicsPipelineCreateInfo local_grid_pipeline_createinfo = local_grid_pipeline_creator.getGraphicsPipelineInfo("shaders/bin/deng/specified_vert.spv", "shaders/bin/deng/specified_frag.spv", "main", VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, DENG_TRUE, DENG_FALSE, 0);

        LOG("Pipeline createinfos created!");

        std::array<VkGraphicsPipelineCreateInfo, DENG_PIPELINE_COUNT> local_pipeline_createinfos;
        std::array<VkPipeline, DENG_PIPELINE_COUNT> local_pipelines;

        #if !DISABLE_DENGUI
            this->m_pipelines[1].pipeline_type = DENG_PIPELINE_TYPE_UI;
            this->m_pipelines[1].pipeline_draw_mode = DENG_PIPELINE_DRAW_MODE_INDEXED;
            this->m_pipelines[1].p_pipeline_layout = &this->m_pipeline_layouts.first.second;
            this->m_pipelines[1].p_descriptor_sets = &this->m_descriptor_sets.first.second;
            this->m_pipelines[1].p_vertices_buffer = &this->m_buffers.window_buffer;
            this->m_pipelines[1].p_indices_buffer = &this->m_buffers.window_index_buffer; 
            PipelineCreator local_ui_pipeline_creator(&this->m_pipelines[1], &this->m_device, &this->m_fm, &this->m_extent, &this->m_renderpass);
            VkGraphicsPipelineCreateInfo local_ui_pipeline_createinfo = local_ui_pipeline_creator.getGraphicsPipelineInfo("shaders/bin/dengui/ui_vert.spv", "shaders/bin/dengui/ui_frag.spv", "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, DENG_TRUE, DENG_FALSE, 0);
            local_pipeline_createinfos = {local_main_pipeline_createinfo, local_ui_pipeline_createinfo};
        #else
            local_pipeline_createinfos = {local_main_pipeline_createinfo, local_grid_pipeline_createinfo};
        #endif

        if(vkCreateGraphicsPipelines(this->m_device, VK_NULL_HANDLE, static_cast<uint32_t>(local_pipeline_createinfos.size()), local_pipeline_createinfos.data(), nullptr, local_pipelines.data()) != VK_SUCCESS) {
            ERR("Failed to create graphics pipelines!");
        }

        else {
            size_t index;
            for(index = 0; index < local_pipelines.size(); index++)
                this->m_pipelines[index].pipeline = local_pipelines[index];

            LOG("Graphics pipelines created successfully!");
        }
    }

    void Renderer::initDepthResources() {
        this->makeImage(VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, nullptr, DENG_IMAGE_TYPE_DEPTH);
        
        VkImageViewCreateInfo local_imgage_view_createinfo = this->getImageViewInfo(this->m_depthimage_data.depthimage, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);

        if(vkCreateImageView(this->m_device, &local_imgage_view_createinfo, nullptr, &this->m_depthimage_data.depthimage_view) != VK_SUCCESS) {
            ERR("Failed to create depth image view!");
        }

    }

    void Renderer::initFrameBuffers() {
        this->m_swapchain_framebuffers.resize(this->m_swapchain_image_views.size());

        for(size_t i = 0; i < this->m_swapchain_image_views.size(); i++) {
            std::array<VkImageView, 2> local_attachments = {this->m_swapchain_image_views[i], this->m_depthimage_data.depthimage_view};

            VkFramebufferCreateInfo local_framebuffer_createinfo{};
            local_framebuffer_createinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            local_framebuffer_createinfo.renderPass = this->m_renderpass;
            local_framebuffer_createinfo.attachmentCount = local_attachments.size();
            local_framebuffer_createinfo.pAttachments = local_attachments.data();
            local_framebuffer_createinfo.width = this->m_extent.width;
            local_framebuffer_createinfo.height = this->m_extent.height;
            local_framebuffer_createinfo.layers = 1;

            if(vkCreateFramebuffer(this->m_device, &local_framebuffer_createinfo, nullptr, &this->m_swapchain_framebuffers[i]) != VK_SUCCESS) {
                ERR("Failed to create framebuffer!");
            }
            else {
                LOG("Successfully created framebuffer!");
            }
        }
    }

    void Renderer::initTextureImage(dengUtils::GameObject &obj) {
        Renderer::makeBuffer(&this->m_device, &this->m_gpu, obj.raw_texture_data.p_texture_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &this->m_buffers.staging_buffer, &this->m_buffers.staging_buffer_memory, nullptr);
        this->populateBufferMem(&this->m_device, &this->m_gpu, obj.raw_texture_data.p_texture_size, obj.raw_texture_data.texture_pixels_data.data(), &this->m_buffers.staging_buffer, &this->m_buffers.staging_buffer_memory);
        LOG("Successfully created texture staging buffer!");

        obj.raw_texture_data.cpyDims(obj.texture_data);
        obj.raw_texture_data.clear();

        this->makeImage(VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &obj, DENG_IMAGE_TYPE_TEXTURE);

        this->transitionImageLayout(obj.texture_data.texture_image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        this->copyBufferToImage(m_buffers.staging_buffer, obj.texture_data.texture_image, obj.texture_data.width, obj.texture_data.height);
        this->transitionImageLayout(obj.texture_data.texture_image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(this->m_device, m_buffers.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, m_buffers.staging_buffer_memory, nullptr);

        VkImageViewCreateInfo local_viewInfo = this->getImageViewInfo(obj.texture_data.texture_image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

        if(vkCreateImageView(this->m_device, &local_viewInfo, nullptr, &obj.texture_data.texture_image_view) != VK_SUCCESS) {
            ERR("Failed to create texture image view!");
        }
    }

    void Renderer::initTextureSampler(dengUtils::GameObject &obj) {
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

        if(vkCreateSampler(this->m_device, &local_samplerInfo, nullptr, &obj.texture_data.texture_sampler) != VK_SUCCESS) {
            ERR("Failed to create texture sampler!");
        }

    }

    void Renderer::initBuffers(dengUtils::GameObject &obj) {
        // main object buffer
        LOG("Initialising main buffer!");
        VkDeviceSize local_size = sizeof(obj.vertex_data[0]) * obj.vertex_data.size();

        this->makeBuffer(&this->m_device, &this->m_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &this->m_buffers.staging_buffer, &this->m_buffers.staging_buffer_memory, nullptr);
        this->populateBufferMem(&this->m_device, &this->m_gpu, &local_size, obj.vertex_data.data(), &this->m_buffers.staging_buffer, &this->m_buffers.staging_buffer_memory);

        this->makeBuffer(&this->m_device, &this->m_gpu, &local_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &this->m_buffers.vertex_buffer, &this->m_buffers.vertex_buffer_memory, nullptr);
        this->copyBufferToBuffer(&this->m_device, &this->m_commandpool, &this->m_queues.graphics_queue, &this->m_buffers.staging_buffer, &this->m_buffers.vertex_buffer, &local_size);

        vkDestroyBuffer(this->m_device, m_buffers.staging_buffer, nullptr);
        vkFreeMemory(this->m_device, m_buffers.staging_buffer_memory, nullptr);

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

        local_size = sizeof(dengMath::UniformBufferData);

        this->m_buffers.uniform_buffers.resize(this->m_swapchain_images.size());
        this->m_buffers.uniform_buffers_memory.resize(this->m_swapchain_images.size());

        for(size_t i = 0; i < this->m_swapchain_images.size(); i++) 
            this->makeBuffer(&this->m_device, &this->m_gpu, &local_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &this->m_buffers.uniform_buffers[i], &this->m_buffers.uniform_buffers_memory[i], &i);
    }

    void Renderer::initDescriptorPool() {
        this->m_descriptor_pool_sets.first.first = DENG_PIPELINE_TYPE_SPECIFIED;
        this->m_descriptor_pool_sets.second.first = DENG_PIPELINE_TYPE_OBJECT_BASED;

        std::vector<VkDescriptorPoolSize> local_descriptor_pool_sizes{};
        local_descriptor_pool_sizes.resize(1);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());

        VkDescriptorPoolCreateInfo local_desc_pool_createInfo{};
        local_desc_pool_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        local_desc_pool_createInfo.poolSizeCount = static_cast<uint32_t>(local_descriptor_pool_sizes.size());
        local_desc_pool_createInfo.pPoolSizes = local_descriptor_pool_sizes.data();
        local_desc_pool_createInfo.maxSets = static_cast<uint32_t>(this->m_swapchain_images.size());

        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_descriptor_pool_sets.first.second) != VK_SUCCESS) {
            ERR("Failed to create descriptor pool!");
        }

        local_descriptor_pool_sizes.resize(2);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());
        local_descriptor_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        local_descriptor_pool_sizes[1].descriptorCount = static_cast<uint32_t>(this->m_swapchain_images.size());

        local_desc_pool_createInfo.poolSizeCount = static_cast<uint32_t>(local_descriptor_pool_sizes.size());
        local_desc_pool_createInfo.pPoolSizes = local_descriptor_pool_sizes.data();

        if(vkCreateDescriptorPool(this->m_device, &local_desc_pool_createInfo, nullptr, &this->m_descriptor_pool_sets.second.second) != VK_SUCCESS) {
            ERR("Failed to create descriptor pool!");
        }
    }

    void Renderer::initDescriptorSets() {
        this->m_descriptor_sets.first.first = DENG_PIPELINE_TYPE_SPECIFIED;
        this->m_descriptor_sets.second.first = DENG_PIPELINE_TYPE_OBJECT_BASED;

        for(int i = 0; i < 2; i++) {
            VkDescriptorSetAllocateInfo local_allocinfo{};
            local_allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            std::vector<VkDescriptorSetLayout> *local_descriptor_layout;
            switch (i)
            {
            case 0: {
                local_descriptor_layout = new std::vector<VkDescriptorSetLayout>(this->m_swapchain_images.size(), this->m_descriptor_set_layouts.first.second);
                local_allocinfo.descriptorPool = this->m_descriptor_pool_sets.first.second;
                local_allocinfo.descriptorSetCount = static_cast<uint32_t>(local_descriptor_layout->size());
                local_allocinfo.pSetLayouts = local_descriptor_layout->data();
                this->m_descriptor_sets.first.second.resize(this->m_swapchain_images.size());

                if(vkAllocateDescriptorSets(this->m_device, &local_allocinfo, this->m_descriptor_sets.first.second.data()) != VK_SUCCESS) {
                    ERR("Failed to allocate descriptor sets!");
                }

                else {
                    LOG("Successfully allocated descriptor sets!");
                }

                break;
            }

            case 1: {
                local_descriptor_layout = new std::vector<VkDescriptorSetLayout>(this->m_swapchain_images.size(), this->m_descriptor_set_layouts.second.second);
                local_allocinfo.descriptorPool = this->m_descriptor_pool_sets.second.second;
                local_allocinfo.descriptorSetCount = static_cast<uint32_t>(local_descriptor_layout->size());
                local_allocinfo.pSetLayouts = local_descriptor_layout->data();
                this->m_descriptor_sets.second.second.resize(this->m_swapchain_images.size());

                if(vkAllocateDescriptorSets(this->m_device, &local_allocinfo, this->m_descriptor_sets.second.second.data()) != VK_SUCCESS) {
                    ERR("Failed to allocate descriptor sets!");
                }

                else {
                    LOG("Successfully allocated descriptor sets!");
                }

                break;
            }
            
            default:
                break;
            }

            for(size_t ii = 0; ii < this->m_swapchain_images.size(); ii++) {
                VkDescriptorBufferInfo local_bufferinfo{};
                local_bufferinfo.buffer = this->m_buffers.uniform_buffers[i];
                local_bufferinfo.offset = 0;
                local_bufferinfo.range = sizeof(dengMath::UniformBufferData);

                std::vector<VkWriteDescriptorSet> local_description_writes{};
                switch (i)
                {
                case 0:
                    local_description_writes.resize(1);
                    local_description_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    local_description_writes[0].dstSet = this->m_descriptor_sets.first.second[ii];
                    local_description_writes[0].dstBinding = 0;
                    local_description_writes[0].dstArrayElement = 0;
                    local_description_writes[0].descriptorCount = 1;
                    local_description_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    local_description_writes[0].pBufferInfo = &local_bufferinfo;

                    vkUpdateDescriptorSets(this->m_device, local_description_writes.size(), local_description_writes.data(), 0, nullptr);
                    break;

                case 1: {
                    VkDescriptorImageInfo local_desc_imageInfo{};
                    local_desc_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    local_desc_imageInfo.imageView = this->m_sample_object.texture_data.texture_image_view;
                    local_desc_imageInfo.sampler = this->m_sample_object.texture_data.texture_sampler;

                    local_description_writes.resize(2);
                    local_description_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    local_description_writes[0].dstSet = this->m_descriptor_sets.second.second[ii];
                    local_description_writes[0].dstBinding = 0;
                    local_description_writes[0].dstArrayElement = 0;
                    local_description_writes[0].descriptorCount = 1;
                    local_description_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    local_description_writes[0].pBufferInfo = &local_bufferinfo;

                    local_description_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    local_description_writes[1].dstSet = this->m_descriptor_sets.second.second[ii];
                    local_description_writes[1].dstBinding = 1;
                    local_description_writes[1].dstArrayElement = 0;
                    local_description_writes[1].descriptorCount = 1;
                    local_description_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    local_description_writes[1].pImageInfo = &local_desc_imageInfo;
                    
                    vkUpdateDescriptorSets(this->m_device, local_description_writes.size(), local_description_writes.data(), 0, nullptr);
                    break;
                }
                
                default:
                    break;
                }
            }

            delete local_descriptor_layout;
        }
        
    }

    void Renderer::initCommandBuffers() {
        LOG("Framebuffer size: " + std::to_string(this->m_swapchain_framebuffers.size()));

        this->m_commandbuffers.resize(this->m_swapchain_framebuffers.size());

        VkCommandBufferAllocateInfo local_commandbuffer_allocation_info{};
        local_commandbuffer_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocation_info.commandPool = this->m_commandpool;
        local_commandbuffer_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocation_info.commandBufferCount = static_cast<uint32_t>(this->m_commandbuffers.size());

        if(vkAllocateCommandBuffers(this->m_device, &local_commandbuffer_allocation_info, this->m_commandbuffers.data())) {
            ERR("Failed to allocate command buffers!");
        }
        else {
            LOG("Successfully allocated command buffers!");
        }

        for(size_t i = 0; i < this->m_commandbuffers.size(); i++) {
            VkCommandBufferBeginInfo local_commandbuffer_begininfo{};
            local_commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(this->m_commandbuffers[i], &local_commandbuffer_begininfo) != VK_SUCCESS) {
                ERR("Failed to begin recording command buffers!");
            }

            else {
                LOG("Successfully begun to record command buffers!");
            }

            VkRenderPassBeginInfo local_renderpass_begininfo{};
            local_renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            local_renderpass_begininfo.renderPass = this->m_renderpass;
            local_renderpass_begininfo.framebuffer = this->m_swapchain_framebuffers[i];
            local_renderpass_begininfo.renderArea.offset = {0, 0};
            local_renderpass_begininfo.renderArea.extent = this->m_extent;

            std::array<VkClearValue, 2> local_clear_values;
            local_clear_values[0].color = {this->m_environment_conf.environment_color_r, this->m_environment_conf.environment_color_g, this->m_environment_conf.environment_color_b, 1.0f};
            local_clear_values[1].depthStencil = {1.0f, 0};

            local_renderpass_begininfo.clearValueCount = local_clear_values.size();
            local_renderpass_begininfo.pClearValues = local_clear_values.data();

            vkCmdBeginRenderPass(this->m_commandbuffers[i], &local_renderpass_begininfo, VK_SUBPASS_CONTENTS_INLINE);
                LOG("Successfully began renderpass!");
                VkDeviceSize local_offsets[] = {0};

                for(size_t ii = 0; ii < this->m_pipelines.size(); ii++) {
                    vkCmdBindPipeline(this->m_commandbuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pipelines[ii].pipeline);
                    vkCmdBindVertexBuffers(this->m_commandbuffers[i], 0, 1, this->m_pipelines[ii].p_vertices_buffer, local_offsets);

                    if(this->m_pipelines[ii].pipeline_draw_mode == DENG_PIPELINE_DRAW_MODE_INDEXED) {
                        vkCmdBindIndexBuffer(this->m_commandbuffers[i], *this->m_pipelines[ii].p_indices_buffer, 0, VK_INDEX_TYPE_UINT16);
                        vkCmdBindDescriptorSets(this->m_commandbuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_pipelines[ii].p_pipeline_layout, 0, 1, &this->m_pipelines[ii].p_descriptor_sets->at(i), 0, nullptr);
                        vkCmdDrawIndexed(this->m_commandbuffers[i], this->m_pipelines[ii].indices_size, 1, 0, 0, 0);
                    }

                    else if(this->m_pipelines[ii].pipeline_draw_mode == DENG_PIPELINE_DRAW_MODE_LINEAR) {
                        vkCmdBindDescriptorSets(this->m_commandbuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_pipelines[ii].p_pipeline_layout, 0, 1, &this->m_pipelines[ii].p_descriptor_sets->at(i), 0, nullptr);
                        vkCmdDraw(this->m_commandbuffers[i], this->m_pipelines[ii].vertices_size, 1, 0, 0);
                    }
                }

            vkCmdEndRenderPass(this->m_commandbuffers[i]);
            LOG("Ended renderPass!");

            if(vkEndCommandBuffer(this->m_commandbuffers[i]) != VK_SUCCESS) {
                ERR("Failed to end recording command buffer!");
            }
        }
    }

    void Renderer::initSemaphores() {
        this->m_image_available_semaphore_set.resize(this->m_MAX_FRAMES_IN_FLIGHT);
        this->m_render_finished_semaphore_set.resize(this->m_MAX_FRAMES_IN_FLIGHT);
        this->m_flight_fences.resize(this->m_MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo local_semaphore_info{};
        local_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo local_fence_createInfo{};
        local_fence_createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        local_fence_createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(int32_t i = 0; i < this->m_MAX_FRAMES_IN_FLIGHT; i++) {
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
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            ERR("Error acquiring swap chain image!");
        }

        this->updateUniformBufferData(image_index, this->m_sample_object);

        VkSemaphore local_wait_semaphores[] = {this->m_image_available_semaphore_set[this->m_current_frame]};
        VkSemaphore local_signalSemaphores[] = {this->m_render_finished_semaphore_set[this->m_current_frame]};

        VkSubmitInfo local_submitinfo{};
        local_submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkPipelineStageFlags local_wait_stages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        local_submitinfo.waitSemaphoreCount = 1;
        local_submitinfo.pWaitSemaphores = local_wait_semaphores;
        local_submitinfo.pWaitDstStageMask = local_wait_stages;
        
        local_submitinfo.commandBufferCount = 1;
        
        // if(this->m_p_camera->view_matrix.getPosition().second >= 0.0f)
            local_submitinfo.pCommandBuffers = &this->m_commandbuffers[image_index];

        // else if(this->m_p_camera->view_matrix.getPosition().second < 0.0f) 
            // local_submitinfo.pCommandBuffers = &this->m_commandbuffers.second[image_index];

        local_submitinfo.signalSemaphoreCount = 1;
        local_submitinfo.pSignalSemaphores = local_signalSemaphores;

        vkResetFences(this->m_device, 1, &this->m_flight_fences[this->m_current_frame]);

        if(vkQueueSubmit(this->m_queues.graphics_queue, 1, &local_submitinfo, this->m_flight_fences[this->m_current_frame]) != VK_SUCCESS) {
            ERR("Failed to submit draw command buffer!"); 
        }

        VkPresentInfoKHR local_presentInfo{};
        local_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        local_presentInfo.waitSemaphoreCount = 1;
        local_presentInfo.pWaitSemaphores = local_signalSemaphores;
        local_presentInfo.pImageIndices = &image_index;

        VkSwapchainKHR swapChains[] = {this->m_swapchain};
        local_presentInfo.swapchainCount = 1;
        local_presentInfo.pSwapchains = swapChains;

        vkQueuePresentKHR(this->m_queues.present_queue, &local_presentInfo);

        this->m_current_frame = (m_current_frame + 1) % this->m_MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::updateUniformBufferData(const uint32_t &current_image, dengUtils::GameObject &obj) {
        dengMath::UniformBufferData local_ubo;
        obj.model_matrix.getModelMatrix(&local_ubo.model);
        
        this->m_p_camera->view_matrix.getViewMatrix(&local_ubo.view);
        this->m_p_camera->p_projection_matrix->getProjectionMatrix(&local_ubo.projection);

        void *data;
        vkMapMemory(this->m_device, this->m_buffers.uniform_buffers_memory[current_image], 0, sizeof(local_ubo), 0, &data);
            memcpy(data, &local_ubo, sizeof(local_ubo));
        vkUnmapMemory(this->m_device, this->m_buffers.uniform_buffers_memory[current_image]);

    }

    /* VkImage related functions */
    void Renderer::makeImage(const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, dengUtils::GameObject *p_obj, const dengImageType &image_type) {
        uint32_t local_width, local_height;
        VkImage *p_local_image;
        VkDeviceMemory *p_local_image_memory;

        switch (image_type)
        {
        case DENG_IMAGE_TYPE_TEXTURE:
            local_width = p_obj->texture_data.width;
            local_height = p_obj->texture_data.height;
            p_local_image = &p_obj->texture_data.texture_image;
            p_local_image_memory = &p_obj->texture_data.texture_image_memory;
            break;

        case DENG_IMAGE_TYPE_DEPTH: 
            local_width = this->m_extent.width;
            local_height = this->m_extent.height;
            p_local_image = &this->m_depthimage_data.depthimage;
            p_local_image_memory = &this->m_depthimage_data.depthimage_memory;
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

        if(vkCreateImage(this->m_device, &local_image_createInfo, nullptr, p_local_image) != VK_SUCCESS) {
            ERR("Failed to create image!");
        }

        VkMemoryRequirements local_memReq;
        vkGetImageMemoryRequirements(this->m_device, *p_local_image, &local_memReq);

        VkMemoryAllocateInfo local_allocInfo{};
        local_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_allocInfo.allocationSize = local_memReq.size;
        local_allocInfo.memoryTypeIndex = HardwareSpecs::getMemoryType(this->m_gpu, local_memReq.memoryTypeBits, properties);

        if(vkAllocateMemory(this->m_device, &local_allocInfo, nullptr, p_local_image_memory) != VK_SUCCESS) {
            ERR("Failed to allocate image memory!");
        }

        vkBindImageMemory(this->m_device, *p_local_image, *p_local_image_memory, 0);
    }

    void Renderer::transitionImageLayout(VkImage &image, const VkFormat &format, const VkImageLayout &oldLayout, const VkImageLayout &newLayout) {
        VkCommandBuffer local_commandbuffer;
        Renderer::beginCommandBufferSingleCommand(&this->m_device, &this->m_commandpool, local_commandbuffer);

        VkImageMemoryBarrier local_memory_barrier{};
        local_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        local_memory_barrier.oldLayout = oldLayout;
        local_memory_barrier.newLayout = newLayout;
        local_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        local_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        local_memory_barrier.image = image;
        local_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        local_memory_barrier.subresourceRange.baseMipLevel = 0;
        local_memory_barrier.subresourceRange.levelCount = 1;
        local_memory_barrier.subresourceRange.baseArrayLayer = 0;
        local_memory_barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags local_src_stage;
        VkPipelineStageFlags local_dst_stage;
        
        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            local_memory_barrier.srcAccessMask = 0;
            local_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            local_src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            local_dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            local_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            local_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            local_src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            local_dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            ERR("Invalid layout transitions!");
        }

        vkCmdPipelineBarrier(local_commandbuffer, local_src_stage, local_dst_stage, 0, 0, nullptr, 0, nullptr, 1, &local_memory_barrier);

        Renderer::endCommandBufferSingleCommand(&this->m_device, &this->m_queues.graphics_queue, &this->m_commandpool, local_commandbuffer);
    }

    void Renderer::copyBufferToImage(VkBuffer &src_buffer, VkImage &dst_image, const uint32_t &width, const uint32_t &height) {
        VkCommandBuffer local_commandbuffer;
        Renderer::beginCommandBufferSingleCommand(&this->m_device, &this->m_commandpool, local_commandbuffer);

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

        vkCmdCopyBufferToImage(local_commandbuffer, src_buffer, dst_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &local_copy_region);

        Renderer::endCommandBufferSingleCommand(&this->m_device, &this->m_queues.graphics_queue, &this->m_commandpool, local_commandbuffer);
    }
    
    /* VkBuffer related functions */
    void Renderer::makeBuffer(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceSize *p_size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkBuffer *p_buffer, VkDeviceMemory *p_buffer_memory, size_t *p_buffer_index) {
        
        VkBufferCreateInfo local_buffer_createInfo{};
        local_buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_buffer_createInfo.size = *p_size;
        local_buffer_createInfo.usage = usage;
        local_buffer_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(*p_device, &local_buffer_createInfo, nullptr, p_buffer) != VK_SUCCESS) {
            ERR("Failed to create a buffer!");
        }

        VkMemoryRequirements local_mem_req;
        vkGetBufferMemoryRequirements(*p_device, *p_buffer, &local_mem_req);

        VkMemoryAllocateInfo local_mem_allocInfo{};
        local_mem_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_mem_allocInfo.allocationSize = local_mem_req.size;
        local_mem_allocInfo.memoryTypeIndex = HardwareSpecs::getMemoryType(*p_gpu, local_mem_req.memoryTypeBits, properties);

        if(vkAllocateMemory(*p_device, &local_mem_allocInfo, nullptr, p_buffer_memory) != VK_SUCCESS) {
            ERR("Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(*p_device, *p_buffer, *p_buffer_memory, 0);
    }

    void Renderer::populateBufferMem(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceSize *p_size, const void *p_src_data, VkBuffer *p_buffer, VkDeviceMemory *p_buffer_memory) {
        LOG("Populating buffer memory!");
        void *local_data;
        vkMapMemory(*p_device, *p_buffer_memory, 0, *p_size, 0, &local_data);
            memcpy(local_data, p_src_data, static_cast<size_t>(*p_size));
        vkUnmapMemory(*p_device, *p_buffer_memory);
    }

    void Renderer::copyBufferToBuffer(VkDevice *p_device, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkBuffer *p_src_buffer, VkBuffer *p_dst_buffer, VkDeviceSize *p_size) {
        VkCommandBuffer local_commandbuffer;
        Renderer::beginCommandBufferSingleCommand(p_device, p_commandpool, local_commandbuffer);

        VkBufferCopy local_copy_region{};
        local_copy_region.size = *p_size;
        
        vkCmdCopyBuffer(local_commandbuffer, *p_src_buffer, *p_dst_buffer, 1, &local_copy_region);
        Renderer::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, local_commandbuffer);
    }

    /* single commandbuffer command recorder functions */
    void Renderer::beginCommandBufferSingleCommand(VkDevice *device, VkCommandPool *commandpool, VkCommandBuffer &commandBuffer) {
        VkCommandBufferAllocateInfo local_commandBuffer_allocInfo{};
        local_commandBuffer_allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandBuffer_allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandBuffer_allocInfo.commandPool = *commandpool;
        local_commandBuffer_allocInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(*device, &local_commandBuffer_allocInfo, &commandBuffer);
        
        VkCommandBufferBeginInfo local_commandBuffer_beginInfo{};
        local_commandBuffer_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        local_commandBuffer_beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if(vkBeginCommandBuffer(commandBuffer, &local_commandBuffer_beginInfo) != VK_SUCCESS) {
            ERR("Failed to begin command recording buffer!");
        }
    }

    void Renderer::endCommandBufferSingleCommand(VkDevice *device, VkQueue *graphics_queue, VkCommandPool *commandpool, VkCommandBuffer &commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo local_submitInfo{};
        local_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        local_submitInfo.commandBufferCount = 1;
        local_submitInfo.pCommandBuffers = &commandBuffer;
        
        vkQueueSubmit(*graphics_queue, 1, &local_submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(*graphics_queue);

        vkFreeCommandBuffers(*device, *commandpool, 1, &commandBuffer);
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
            vkDestroyBuffer(this->m_device, this->m_buffers.uniform_buffers[i], nullptr);
            vkFreeMemory(this->m_device, this->m_buffers.uniform_buffers_memory[i], nullptr);
        }

        vkDestroyDescriptorPool(this->m_device, this->m_descriptor_pool_sets.first.second, nullptr);
        vkDestroyDescriptorPool(this->m_device, this->m_descriptor_pool_sets.second.second, nullptr);
    }

    void Renderer::deleteTextureImage(dengUtils::GameObject &obj) {
        vkDestroySampler(this->m_device, obj.texture_data.texture_sampler, nullptr);
        vkDestroyImageView(this->m_device, obj.texture_data.texture_image_view, nullptr);
        vkDestroyImage(this->m_device, obj.texture_data.texture_image, nullptr);
        vkFreeMemory(this->m_device, obj.texture_data.texture_image_memory, nullptr);
    }

    void Renderer::deleteDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(this->m_device, this->m_descriptor_set_layouts.first.second, nullptr);
        vkDestroyDescriptorSetLayout(this->m_device, this->m_descriptor_set_layouts.second.second, nullptr);
    }

    void Renderer::deleteBuffers() {
        vkDestroyBuffer(this->m_device, this->m_buffers.vertex_buffer, nullptr);
        vkFreeMemory(this->m_device, this->m_buffers.vertex_buffer_memory, nullptr);
        vkDestroyBuffer(this->m_device, this->m_buffers.grid_buffer, nullptr);
        vkFreeMemory(this->m_device, this->m_buffers.grid_buffer_memory, nullptr);
    }

    void Renderer::deleteDepthImageData() {
        vkDestroyImageView(this->m_device, this->m_depthimage_data.depthimage_view, nullptr);
        vkDestroyImage(this->m_device, this->m_depthimage_data.depthimage, nullptr);
        vkFreeMemory(this->m_device, this->m_depthimage_data.depthimage_memory, nullptr);
    }

    void Renderer::deleteSemaphores() {
        for(int32_t i = 0; i < this->m_MAX_FRAMES_IN_FLIGHT; i++) {
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

    void Renderer::run() {
        this->m_p_window->setInputMode(DENG_INPUT_MOVEMENT);
        while(1) {
            update_window(this->m_p_window->getWindow());
            this->m_p_ev->update();
            this->m_p_dengui_window->update();
            this->makeFrame();
        }
        vkDeviceWaitIdle(this->m_device);
        destroy_window(this->m_p_window->getWindow());
    }
} 