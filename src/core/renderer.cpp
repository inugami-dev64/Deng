#include "api_core.h"

namespace deng
{

    /************ InstanceCreator class *************/

    InstanceCreator::InstanceCreator(WindowWrap *p_window_wrap, bool enable_validation_layers) {
        m_required_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        m_p_window_wrap = p_window_wrap;

        mkInstance(enable_validation_layers);
        mkWindowSurface();
        if(enable_validation_layers)
            mkDebugMessenger();
        
        selectPhysicalDevice();
        mkLogicalDevice(enable_validation_layers);
    }


    /* Create new vulkan instance */
    void InstanceCreator::mkInstance(bool &enable_validation_layers) {
        // Set up Vulkan application info
        VkApplicationInfo local_appinfo{};
        local_appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        local_appinfo.pApplicationName = m_p_window_wrap->getTitle();
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
        const char **local_extensions = get_required_surface_extensions(m_p_window_wrap->getWindow(), &local_extension_count, static_cast<int>(enable_validation_layers));
        local_instance_createInfo.enabledExtensionCount = local_extension_count;
        local_instance_createInfo.ppEnabledExtensionNames = local_extensions;
        LOG("Required extensions count is: " + std::to_string(local_instance_createInfo.enabledExtensionCount));
        
        VkDebugUtilsMessengerCreateInfoEXT local_debug_createInfo{};
        
        // Check for validatation layer support
        if(enable_validation_layers && !checkValidationLayerSupport())
            ERR("Vulkan validation layers usage specified, but none are available!");

        else if(enable_validation_layers && checkValidationLayerSupport()) {
            // Set up instance info to support validation layers
            local_instance_createInfo.enabledLayerCount = 1;
            local_instance_createInfo.ppEnabledLayerNames = &m_p_validation_layer;
            local_instance_createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &local_debug_createInfo;

            // Set up debug messenger createinfo
            local_debug_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            local_debug_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            local_debug_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            local_debug_createInfo.pfnUserCallback = InstanceCreator::debugCallback;
        }

        else if(!enable_validation_layers) {
            // Set up instance info to not support validation layers
            LOG("Vulkan validation layers are disabled");
            local_instance_createInfo.enabledLayerCount = 0;
            local_instance_createInfo.pNext = nullptr;
        }
        
        // Create new Vulkan instance
        if(vkCreateInstance(&local_instance_createInfo, nullptr, &m_instance) != VK_SUCCESS)
            ERR("Failed to create an instance!");

        else LOG("Successfully created an instance");
    }


    /* Check if Vulkan validation layers are available */
    bool InstanceCreator::checkValidationLayerSupport() {
        uint32_t local_layer_count;
        vkEnumerateInstanceLayerProperties(&local_layer_count, nullptr);

        std::vector<VkLayerProperties> local_available_layers(local_layer_count);
        vkEnumerateInstanceLayerProperties(&local_layer_count, local_available_layers.data());
        bool isLayer = false;

        for(const VkLayerProperties &properties : local_available_layers) {
            if(strcmp(m_p_validation_layer, properties.layerName) == 0) {
                isLayer = true;
                break;
            }
        }

        return isLayer;
    }


    /* Make debug messenger for Vulkan validation layer */
    void InstanceCreator::mkDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT local_messenger_createinfo{};
        local_messenger_createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        local_messenger_createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        local_messenger_createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        local_messenger_createinfo.pfnUserCallback = InstanceCreator::debugCallback;
        
        auto debugUtilsMessengerCreator_fun = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

        if(!debugUtilsMessengerCreator_fun)
            ERR("Couldn't create debug messenger for Vulkan validation layers!");

        debugUtilsMessengerCreator_fun(m_instance, &local_messenger_createinfo, nullptr, &m_debug_messenger);
        LOG("Debug messenger created successfully!");
    }


    /* Callback method for Vulkan validation layer debugging */
    VKAPI_ATTR VkBool32 VKAPI_CALL InstanceCreator::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, 
    const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data) {
        std::cerr << "VL: " << p_callback_data->pMessage << std::endl;
        return VK_FALSE;
    }

    
    /* Select appropriate graphics device */
    void InstanceCreator::selectPhysicalDevice() {
        uint32_t local_device_count;
        uint32_t score;
        vkEnumeratePhysicalDevices(m_instance, &local_device_count, nullptr);

        if(local_device_count == 0)
            ERR("Failed to find graphics cards!");

        std::vector<VkPhysicalDevice> local_devices(local_device_count);
        std::multimap<uint32_t, VkPhysicalDevice> local_device_candidates;
        VkResult result = vkEnumeratePhysicalDevices(m_instance, &local_device_count, local_devices.data());
        if(result != VK_SUCCESS) ERR("Failed to count physical GPUs!");

        // Iterate through every potential gpu device
        for(uint32_t i = 0; i < local_device_count; i++) {
            score = HardwareSpecs::getDeviceScore(&local_devices[i], m_required_extension_names);
            SwapChainDetails swapchain_details(local_devices[i], m_surface);
            
            if(!swapchain_details.getFormats().empty() && !swapchain_details.getPresentModes().empty())
                local_device_candidates.insert(std::make_pair(score, local_devices[i]));
        }

        if(!local_device_candidates.empty() && local_device_candidates.rbegin()->first > 0)
            m_gpu = local_device_candidates.rbegin()->second;

        else ERR("Failed to find suitable GPU!");

        LOG("Found suitable physical device");
    }

    
    /* Create logical Vulkan device */ 
    void InstanceCreator::mkLogicalDevice(bool &enable_validation_layers) {
        if(!m_queue_families.findGraphicsFamily(m_gpu) || !m_queue_families.findPresentSupportFamily(m_gpu, m_surface))
            ERR("Queue supporting GPU not found!");

        std::array<VkDeviceQueueCreateInfo, 2> local_queue_createinfos;
        std::array<uint32_t, 2> queue_families_indexes = {m_queue_families.getGraphicsFamilyIndex(), m_queue_families.getPresentFamilyIndex()};

        float queue_priority = 1.0f;

        // Create device queue creatinfos for present and graphics queues
        for(uint32_t i = 0; i < 2; i++) {
            VkDeviceQueueCreateInfo dev_queue_createinfo{};
            dev_queue_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            dev_queue_createinfo.queueFamilyIndex = queue_families_indexes[i];
            dev_queue_createinfo.queueCount = 1;
            dev_queue_createinfo.pQueuePriorities = &queue_priority;
            local_queue_createinfos[i] = dev_queue_createinfo;                
        } 

        VkPhysicalDeviceFeatures local_device_features{};
        local_device_features.samplerAnisotropy = VK_TRUE;

        // Create device createinfo
        VkDeviceCreateInfo local_logical_device_createinfo{};
        local_logical_device_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        local_logical_device_createinfo.queueCreateInfoCount = static_cast<uint32_t>(local_queue_createinfos.size());
        local_logical_device_createinfo.pQueueCreateInfos = local_queue_createinfos.data();
        local_logical_device_createinfo.pEnabledFeatures = &local_device_features;
        local_logical_device_createinfo.enabledExtensionCount = m_required_extension_names.size();
        local_logical_device_createinfo.ppEnabledExtensionNames = m_required_extension_names.data();

        if(enable_validation_layers) {
            local_logical_device_createinfo.enabledLayerCount = 1;
            local_logical_device_createinfo.ppEnabledLayerNames = &m_p_validation_layer;
        }

        else local_logical_device_createinfo.enabledLayerCount = 0;

        if(vkCreateDevice(m_gpu, &local_logical_device_createinfo, nullptr, &m_device) != VK_SUCCESS)
            ERR("Failed to create logical device!");

        vkGetDeviceQueue(m_device, m_queue_families.getGraphicsFamilyIndex(), 0, &m_queues.graphics_queue);
        LOG("Device graphics queue recieved successfully!");
        vkGetDeviceQueue(m_device, m_queue_families.getPresentFamilyIndex(), 0, &m_queues.present_queue);
        LOG("Device present queue recieved successfully!");
    }

    
    /* Create window surface with deng surface library */
    void InstanceCreator::mkWindowSurface() {
        LOG("Initialising window surface!");
        if(init_surface(m_p_window_wrap->getWindow(), &m_instance, &m_surface) != VK_SUCCESS)
            ERR("Failed to create window surface!");

        LOG("Window surface created successfully!");
    }


    /* InstanceCreator getter methods */
    VkInstance InstanceCreator::getIns() { return m_instance; }
    VkDevice InstanceCreator::getDev() { return m_device; }
    VkPhysicalDevice InstanceCreator::getGpu() { return m_gpu; }
    VkSurfaceKHR InstanceCreator::getSu() { return m_surface; }
    VkQueue InstanceCreator::getGQ() { return m_queues.graphics_queue; }
    VkQueue InstanceCreator::getPQ() { return m_queues.present_queue; }
    uint32_t InstanceCreator::getGQi() { return m_queue_families.getGraphicsFamilyIndex(); }
    uint32_t InstanceCreator::getPQi() { return m_queue_families.getPresentFamilyIndex(); } 
    VkDebugUtilsMessengerEXT InstanceCreator::getDMEXT() { return m_debug_messenger; }


    /************ SwapChainCreator class *************/
    SwapChainCreator::SwapChainCreator(WindowWrap *p_window_wrap, VkDevice device, VkPhysicalDevice gpu, VkSurfaceKHR surface, uint32_t g_queue_i, uint32_t p_queue_i) {        
        m_p_device_swapchain_details = new SwapChainDetails(gpu, surface);
        m_device = device;
        m_p_window_wrap = p_window_wrap;

        mkSwapChainSettings();
        mkSwapChain(surface, g_queue_i, p_queue_i);
        mkSCImageViews();
        mkRenderPass();
    }


    /* Initialise swapchain settings in order to create swapchain */
    void SwapChainCreator::mkSwapChainSettings() {
        bool found_suitable_format = false;
        for(const VkSurfaceFormatKHR &surface_format : m_p_device_swapchain_details->getFormats()) {
            if(surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                m_surface_format = surface_format;
                found_suitable_format = true;
                LOG("Found suitable surface format!");
            }
        }
        
        if(!found_suitable_format) {
            WARNME("Didn't find suitable surface format! Using first format!");
            m_surface_format = m_p_device_swapchain_details->getFormats()[0];
        }

        bool foundSuitablePresentMode = false;
        for(const VkPresentModeKHR &presentFormat : m_p_device_swapchain_details->getPresentModes()) {
            // Check which present modes are available
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
                m_present_mode = presentFormat;
                foundSuitablePresentMode = true;
                LOG("Found suitable present mode for triple buffering!");
            }
        }

        if(!foundSuitablePresentMode) {
            WARNME("Didn't find suitable present mode! Using vSync instead!");
            m_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        if(m_p_device_swapchain_details->getCapabilities().currentExtent.width != UINT32_MAX && 
        m_p_device_swapchain_details->getCapabilities().currentExtent.height != UINT32_MAX) 
            m_extent = m_p_device_swapchain_details->getCapabilities().currentExtent;

        else {
            m_extent.width = m_p_window_wrap->getSize().first;
            m_extent.height = m_p_window_wrap->getSize().second;
        }

        LOG("Successfully initialised swap chain settings!");
    }


    /* Create swapchain */
    void SwapChainCreator::mkSwapChain(VkSurfaceKHR &surface, uint32_t g_queue_i, uint32_t p_queue_i) {
        uint32_t local_min_image_count = m_p_device_swapchain_details->getCapabilities().minImageCount + 1;
        
        // Verify that the maximum image count is not exceeded
        if(m_p_device_swapchain_details->getCapabilities().maxImageCount > 0 && 
        local_min_image_count > m_p_device_swapchain_details->getCapabilities().maxImageCount)
            local_min_image_count = m_p_device_swapchain_details->getCapabilities().maxImageCount;


        VkSwapchainCreateInfoKHR local_swapchain_createinfo{};
        local_swapchain_createinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        local_swapchain_createinfo.surface = surface;
        local_swapchain_createinfo.minImageCount = local_min_image_count;
        local_swapchain_createinfo.imageFormat = m_surface_format.format;
        local_swapchain_createinfo.imageColorSpace = m_surface_format.colorSpace;
        local_swapchain_createinfo.imageExtent = m_extent;
        local_swapchain_createinfo.imageArrayLayers = 1;
        local_swapchain_createinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // Check if present queue and graphics queue are the same and if needed then synchronise the image sharing mode
        std::array<uint32_t, 2> local_queue_family_indices = {g_queue_i, p_queue_i};
        if(g_queue_i != p_queue_i) {
            local_swapchain_createinfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            local_swapchain_createinfo.queueFamilyIndexCount = 2;
            local_swapchain_createinfo.pQueueFamilyIndices = local_queue_family_indices.data();
        }

        else local_swapchain_createinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        

        local_swapchain_createinfo.preTransform = m_p_device_swapchain_details->getCapabilities().currentTransform;
        local_swapchain_createinfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
        local_swapchain_createinfo.presentMode = m_present_mode;
        local_swapchain_createinfo.clipped = VK_TRUE;

        if(vkCreateSwapchainKHR(m_device, &local_swapchain_createinfo, nullptr, &m_swapchain) != VK_SUCCESS)
            ERR("Failed to create swap chain!");

        else {
            LOG("Swapchain created successfully!");
            uint32_t local_image_count;
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &local_image_count, nullptr);
            LOG("Currently available " + std::to_string(local_image_count) + " swapchain images!");
            m_swapchain_images.resize(local_image_count);
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &local_image_count, m_swapchain_images.data());
        }
    }


    /* Create render pass */
    void SwapChainCreator::mkRenderPass() {
        VkAttachmentDescription local_color_attachment{};
        local_color_attachment.format = m_surface_format.format;
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


        if(vkCreateRenderPass(m_device, &local_renderpass_createinfo, nullptr, &m_renderpass) != VK_SUCCESS)
            LOG("Failed to create render pass!");
    }


    /* Create swapchain image views */
    void SwapChainCreator::mkSCImageViews() {
        m_swapchain_image_views.resize(m_swapchain_images.size());

        for(uint32_t i = 0; i < m_swapchain_image_views.size(); i++) {
            VkImageViewCreateInfo local_createInfo = 
            BufferCreator::getImageViewInfo(m_swapchain_images[i], m_surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT);
            if(vkCreateImageView(m_device, &local_createInfo, nullptr, &m_swapchain_image_views[i]) != VK_SUCCESS)
                ERR("Failed to create image views!");
        }
    }


    /* Remake the swapchain (needed for resizing the window) */
    void SwapChainCreator::remkSwapChain(VkSurfaceKHR surface, VkSurfaceFormatKHR s_format, uint32_t g_queue_i, uint32_t p_queue_i) {
        vkDeviceWaitIdle(m_device);

        SCCleanup();
        mkSwapChainSettings();
        mkSwapChain(surface, g_queue_i, p_queue_i);
        mkRenderPass();
        mkSCImageViews();
    }


    /* Clean all swap chain related structs */
    void SwapChainCreator::SCCleanup() {
        size_t index = 0;
        for(index = 0; index < m_swapchain_images.size(); index++) {
            vkDestroyImage(m_device, m_swapchain_images[index], nullptr);
            vkDestroyImageView(m_device, m_swapchain_image_views[index], nullptr);
        }

        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
    }


    SwapChainCreator::~SwapChainCreator() {
        SCCleanup();
        delete m_p_device_swapchain_details;
    }

    
    /* SwapChainCreator getters */
    VkRenderPass SwapChainCreator::getRp() { return m_renderpass; }
    VkExtent2D SwapChainCreator::getExt() { return m_extent; }
    VkSwapchainKHR SwapChainCreator::getSC() { return m_swapchain; }
    std::vector<VkImage> SwapChainCreator::getSCImg() { return m_swapchain_images; }
    std::vector<VkImageView> SwapChainCreator::getSCImgViews() { return m_swapchain_image_views; }


    
    /********** DescriptorCreator class ************/
    DescriptorCreator::DescriptorCreator(VkDevice device, VkExtent2D extent, VkRenderPass renderpass, std::vector<DENGAsset> *p_assets, std::vector<TextureImageData> *p_textures, size_t sc_img_size) {
        m_p_assets = p_assets;
        m_p_textures = p_textures;
        mkDescriptorSetLayouts(device);   
        mkPipelineLayouts(device);
        mkGraphicsPipelines(device, extent, renderpass);
        mkDescriptorPools(device, sc_img_size);
    }


    /* Create descriptor set layouts */
    void DescriptorCreator::mkDescriptorSetLayouts(VkDevice &device) {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        VkDescriptorSetLayoutCreateInfo layout_createinfo{};

        // Set up descriptor set layout bindings for uniform buffers
        VkDescriptorSetLayoutBinding ubo_matrix_layout_binding{};
        ubo_matrix_layout_binding.binding = 0;
        ubo_matrix_layout_binding.descriptorCount = 1;
        ubo_matrix_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        ubo_matrix_layout_binding.pImmutableSamplers = nullptr;
        ubo_matrix_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        // Set up descriptor set layout bindings for image sampler
        VkDescriptorSetLayoutBinding sampler_layout_binding{};
        sampler_layout_binding.binding = 1;
        sampler_layout_binding.descriptorCount = 1;
        sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        sampler_layout_binding.pImmutableSamplers = nullptr;
        sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;        
        
        // Use only uniform buffers for unmapped assets
        bindings.resize(1);
        bindings[0] = ubo_matrix_layout_binding;

        // Set up descriptor set layout info for unmapped assets
        layout_createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layout_createinfo.bindingCount = bindings.size();
        layout_createinfo.pBindings = bindings.data();
        if(vkCreateDescriptorSetLayout(device, &layout_createinfo, nullptr, &m_unmapped_descriptor_set_layout))
            ERR("Failed to create descriptor set layout for unmapped assets!");

        else LOG("Successfully created pipeline layout for unmapped assets");

        // Use both bindings for texture mapped vertices
        bindings.resize(2);
        bindings[0] = ubo_matrix_layout_binding;
        bindings[1] = sampler_layout_binding;
        
        VkDescriptorSetLayoutCreateInfo texture_mapped_layout_createinfo{};

        // Set up descriptor set layout info for texture mapped assets
        texture_mapped_layout_createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        texture_mapped_layout_createinfo.bindingCount = bindings.size();
        texture_mapped_layout_createinfo.pBindings = bindings.data();

        // Create descriptor set layout for texture 
        if(vkCreateDescriptorSetLayout(device, &texture_mapped_layout_createinfo, nullptr, &m_texture_mapped_descriptor_set_layout) != VK_SUCCESS)
            ERR("Failed to create descriptor set layout for texture mapped assets!");
        
        else LOG("Successfully created pipeline layout for textured assets");
    }


    /* Create pipeline layouts */
    void DescriptorCreator::mkPipelineLayouts(VkDevice &device) {
        // Set up pipeline layout info struct
        VkPipelineLayoutCreateInfo local_pipeline_layout_createinfo{};
        local_pipeline_layout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        local_pipeline_layout_createinfo.setLayoutCount = 1;
        local_pipeline_layout_createinfo.pushConstantRangeCount = 0;
        local_pipeline_layout_createinfo.pSetLayouts = &m_unmapped_descriptor_set_layout;

        // Create pipeline layout for unmapped assets
        if(vkCreatePipelineLayout(device, &local_pipeline_layout_createinfo, nullptr, &m_unmapped_pipeline_layout) != VK_SUCCESS)
            ERR("Failed to create pipeline layout for unmapped objects!");

        // Create pipeline layout for texture mapped assets
        local_pipeline_layout_createinfo.pSetLayouts = &m_texture_mapped_descriptor_set_layout;
        if(vkCreatePipelineLayout(device, &local_pipeline_layout_createinfo, nullptr, &m_texture_mapped_pipeline_layout) != VK_SUCCESS)
            ERR("Failed to create pipeline layout for textured objects!");
    }


    /* Create graphics pipelines for differently textured assets */
    void DescriptorCreator::mkGraphicsPipelines(VkDevice &device, VkExtent2D &extent, VkRenderPass &renderpass) {
        // Vertices sizes
        uint32_t tex_mapped_3d_vert_count = 0;
        uint32_t unmapped_3d_vert_count = 0;
        uint32_t tex_mapped_2d_vert_count = 0;
        uint32_t unmapped_2d_vert_count = 0;
        size_t index;

        // Count the amount of vertices for each type of assets
        for(index = 0; index < m_p_assets->size(); index++) {
            switch ((*m_p_assets)[index].asset_mode)
            {
            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                tex_mapped_3d_vert_count += static_cast<uint32_t>((*m_p_assets)[index].vertices.size);
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED:
                unmapped_3d_vert_count += static_cast<uint32_t>((*m_p_assets)[index].vertices.size);
                break;
            
            case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                tex_mapped_3d_vert_count += static_cast<uint32_t>((*m_p_assets)[index].vertices.size);
                break;

            case DENG_ASSET_MODE_2D_UNMAPPED:
                unmapped_2d_vert_count += static_cast<uint32_t>((*m_p_assets)[index].vertices.size);
                break;

            default:
                break;
            }
        }

        // Make sure that vertices count isn't zero
        if(!tex_mapped_3d_vert_count) tex_mapped_3d_vert_count++;
        if(!unmapped_3d_vert_count) unmapped_3d_vert_count++;
        if(!tex_mapped_2d_vert_count) tex_mapped_2d_vert_count++;
        if(!unmapped_2d_vert_count) unmapped_2d_vert_count++;
        
        // Specify the pipiline type and layout
        m_pipelines[0].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED_3D;
        m_pipelines[1].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D;
        m_pipelines[2].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED_2D;
        m_pipelines[3].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D;

        m_pipelines[0].p_pipeline_layout = &m_unmapped_pipeline_layout;
        m_pipelines[1].p_pipeline_layout = &m_texture_mapped_pipeline_layout;
        m_pipelines[2].p_pipeline_layout = &m_unmapped_pipeline_layout;
        m_pipelines[3].p_pipeline_layout = &m_texture_mapped_pipeline_layout;

        PipelineCreator unmapped_pipeline_3d(&m_pipelines[0], &device, &extent, &renderpass);
        PipelineCreator texture_mapped_pipeline_3d(&m_pipelines[1], &device, &extent, &renderpass);
        PipelineCreator unmapped_pipeline_2d(&m_pipelines[2], &device, &extent, &renderpass);
        PipelineCreator texture_mapped_pipeline_2d(&m_pipelines[3], &device, &extent, &renderpass);

        std::array<VkGraphicsPipelineCreateInfo, DENG_PIPELINE_COUNT> pipeline_infos{};
        pipeline_infos[0] = unmapped_pipeline_3d.getGraphicsPipelineInfo("shaders/bin/3d_unmapped_vert.spv", "shaders/bin/3d_unmapped_frag.spv", 
        "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true, false, 0);
        pipeline_infos[1] = texture_mapped_pipeline_3d.getGraphicsPipelineInfo("shaders/bin/3d_tex_mapped_vert.spv", "shaders/bin/3d_tex_mapped_frag.spv", 
        "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true, false, 0);

        pipeline_infos[2] = unmapped_pipeline_2d.getGraphicsPipelineInfo("shaders/bin/2d_unmapped_vert.spv", "shaders/bin/2d_unmapped_frag.spv", 
        "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false, false, 0);
        pipeline_infos[3] = texture_mapped_pipeline_2d.getGraphicsPipelineInfo("shaders/bin/2d_tex_mapped_vert.spv", "shaders/bin/2d_tex_mapped_frag.spv", 
        "main", VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true, false, 0);

        std::array<VkPipeline, DENG_PIPELINE_COUNT> local_pipelines;
        if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, pipeline_infos.size(), pipeline_infos.data(), nullptr, local_pipelines.data()) != VK_SUCCESS)
            ERR("Failed to create graphics pipelines!");

        else {
            for(index = 0; index < local_pipelines.size(); index++)
                m_pipelines[index].pipeline = local_pipelines[index];

            LOG("Graphics pipelines created successfully!");
        }
    }


    /* Create descriptor pool */
    void DescriptorCreator::mkDescriptorPools(VkDevice &device, size_t sc_img_size) {
        // Create and set up descriptor pool size struct for uniform buffers
        std::vector<VkDescriptorPoolSize> local_descriptor_pool_sizes{};
        VkDescriptorPoolCreateInfo local_desc_pool_createinfo{};

        local_descriptor_pool_sizes.resize(1);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[0].descriptorCount = (uint32_t) sc_img_size;
        local_desc_pool_createinfo.maxSets = (uint32_t) sc_img_size;

        // Set up descriptor pool createinfo 
        local_desc_pool_createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        local_desc_pool_createinfo.poolSizeCount = (uint32_t) local_descriptor_pool_sizes.size();
        local_desc_pool_createinfo.pPoolSizes = local_descriptor_pool_sizes.data();

        // Create descriptor pool for unmapped assets 
        if(vkCreateDescriptorPool(device, &local_desc_pool_createinfo, nullptr, &m_unmapped_descriptor_pool) != VK_SUCCESS)
            ERR("Failed to create descriptor pool!");

        // Set up descriptor pool size struct for uniform buffers and combined image sampler
        local_descriptor_pool_sizes.resize(2);
        local_descriptor_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        local_descriptor_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        
        if(m_p_textures->size() != 0)
            m_texture_descriptor_size = (uint32_t) (*m_p_textures).size() * sc_img_size; 
        
        else 
            m_texture_descriptor_size = 1;

        LOG("Texture descriptor pool size: " + std::to_string(m_texture_descriptor_size));
        local_descriptor_pool_sizes[0].descriptorCount = m_texture_descriptor_size;
        local_descriptor_pool_sizes[1].descriptorCount = m_texture_descriptor_size;
        local_desc_pool_createinfo.maxSets = m_texture_descriptor_size;

        // Modify descriptor pool createinfo
        local_desc_pool_createinfo.poolSizeCount = (uint32_t) local_descriptor_pool_sizes.size();
        local_desc_pool_createinfo.pPoolSizes = local_descriptor_pool_sizes.data();

        // Create descriptor pool for texture mapped assets
        if(vkCreateDescriptorPool(device, &local_desc_pool_createinfo, nullptr, &m_texture_mapped_descriptor_pool) != VK_SUCCESS)
            ERR("Failed to create descriptor pool!");
    }


    /* Create unmapped descriptor sets */
    void DescriptorCreator::mkUnmappedDS(VkDevice device, size_t sc_img_size, BufferData buffer_data) {
        size_t index;
        // Set up multiple descriptor layout structures
        std::vector<VkDescriptorSetLayout> tmp_descriptor_set_layouts;
        tmp_descriptor_set_layouts.resize(sc_img_size);

        VkDescriptorBufferInfo local_bufferinfo;
        std::vector<VkWriteDescriptorSet> local_descriptor_writes{};
        VkDescriptorSetAllocateInfo local_allocinfo{};
        local_allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

        // Set up descriptor set allocate info struct for unmapped vertices
        m_unmapped_descriptor_sets.resize(sc_img_size);
        for(index = 0; index < tmp_descriptor_set_layouts.size(); index++) 
            tmp_descriptor_set_layouts[index] = m_unmapped_descriptor_set_layout;
        
        local_allocinfo.descriptorPool = m_unmapped_descriptor_pool;
        local_allocinfo.descriptorSetCount = (uint32_t) tmp_descriptor_set_layouts.size();
        local_allocinfo.pSetLayouts = tmp_descriptor_set_layouts.data();

        /* Allocate descriptor sets for unmapped 3D vertices */
        if(vkAllocateDescriptorSets(device, &local_allocinfo, m_unmapped_descriptor_sets.data()) != VK_SUCCESS)
            ERR("Failed to allocate descriptor set!");

        for(index = 0; index < sc_img_size; index++) {
            // Set up descriptor buffer info struct
            local_bufferinfo.buffer = buffer_data.mat_uniform_buffers[index];
            local_bufferinfo.offset = 0;
            local_bufferinfo.range = sizeof(dengMath::UniformData);

            // Set up descriptor set write
            local_descriptor_writes.resize(1);
            local_descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            local_descriptor_writes[0].dstSet = m_unmapped_descriptor_sets[index];
            local_descriptor_writes[0].dstBinding = 0;
            local_descriptor_writes[0].dstArrayElement = 0;
            local_descriptor_writes[0].descriptorCount = 1;
            local_descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            local_descriptor_writes[0].pBufferInfo = &local_bufferinfo;

            // Update descriptor sets
            vkUpdateDescriptorSets(device, (uint32_t) local_descriptor_writes.size(), local_descriptor_writes.data(), 0, nullptr);
            LOG("Successfully created descriptor sets for unmapped assets");
        }
    }
    

    /* Create texture mapped descriptor sets */
    void DescriptorCreator::mkTexMappedDS(VkDevice device, size_t sc_img_size, VkSampler tex_sampler, BufferData buffer_data) {
        size_t l_index, r_index;
        // Set up multiple info structures
        std::vector<VkDescriptorSetLayout> tmp_descriptor_set_layouts;
        tmp_descriptor_set_layouts.resize(sc_img_size);

        VkDescriptorBufferInfo local_bufferinfo;
        std::vector<VkWriteDescriptorSet> local_descriptor_writes{};
        VkDescriptorImageInfo local_desc_imageinfo{};
        VkDescriptorSetAllocateInfo local_allocinfo{};
        local_allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

        for(l_index = 0; l_index < tmp_descriptor_set_layouts.size(); l_index++)
                tmp_descriptor_set_layouts[l_index] = m_texture_mapped_descriptor_set_layout;

        local_allocinfo.descriptorPool = m_texture_mapped_descriptor_pool;
        local_allocinfo.descriptorSetCount = tmp_descriptor_set_layouts.size();   
        local_allocinfo.pSetLayouts = tmp_descriptor_set_layouts.data();
        
        // Iterate through every game asset and 
        for(l_index = 0; l_index < m_p_textures->size(); l_index++) {
            (*m_p_textures)[l_index].descriptor_sets.resize(sc_img_size);

            // Allocate descriptor sets
            LOG("Attempting to allocate descriptor sets");
            if(vkAllocateDescriptorSets(device, &local_allocinfo, (*m_p_textures)[l_index].descriptor_sets.data()) != VK_SUCCESS)
                ERR("Failed to allocate descriptor sets!");

            else LOG("Successfully allocated texture mapped descriptor sets!");

            // Iterate through every descriptor set and update it
            for(r_index = 0; r_index < sc_img_size; r_index++) {
                // Set up descriptor buffer info 
                local_bufferinfo.buffer = buffer_data.mat_uniform_buffers[r_index];
                local_bufferinfo.offset = 0;
                local_bufferinfo.range = sizeof(dengMath::UniformData);

                // Set up descriptor image info 
                local_desc_imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                local_desc_imageinfo.imageView = (*m_p_textures)[l_index].image_view;
                local_desc_imageinfo.sampler = tex_sampler;

                // Set up descriptor writes structs for texture mapped assets
                local_descriptor_writes.resize(2);
                local_descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                local_descriptor_writes[0].dstSet = (*m_p_textures)[l_index].descriptor_sets[r_index];
                local_descriptor_writes[0].dstBinding = 0;
                local_descriptor_writes[0].dstArrayElement = 0;
                local_descriptor_writes[0].descriptorCount = 1;
                local_descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                local_descriptor_writes[0].pBufferInfo = &local_bufferinfo;

                local_descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                local_descriptor_writes[1].dstSet = (*m_p_textures)[l_index].descriptor_sets[r_index];
                local_descriptor_writes[1].dstBinding = 1;
                local_descriptor_writes[1].dstArrayElement = 0;
                local_descriptor_writes[1].descriptorCount = 1;
                local_descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                local_descriptor_writes[1].pImageInfo = &local_desc_imageinfo;

                // Update texture mapped descriptor sets
                vkUpdateDescriptorSets(device, (uint32_t) local_descriptor_writes.size(), local_descriptor_writes.data(), 0, nullptr);
                LOG("Successfully created descriptor sets for texture mapped assets"); 
            }
        }
    }


    /* DescriptorCreator getters */
    std::array<PipelineData, DENG_PIPELINE_COUNT> DescriptorCreator::getPipelines() { return m_pipelines; }
    std::vector<VkDescriptorSet> DescriptorCreator::getUnmappedDS() { return m_unmapped_descriptor_sets; }
    VkDescriptorSetLayout DescriptorCreator::getUnmappedDSL() { return m_unmapped_descriptor_set_layout; }
    VkDescriptorSetLayout DescriptorCreator::getTexMappedDSL() { return m_texture_mapped_descriptor_set_layout; }
    VkDescriptorPool DescriptorCreator::getUnmappedDP() { return m_unmapped_descriptor_pool; }
    VkDescriptorPool DescriptorCreator::getTexMappedDP() { return m_texture_mapped_descriptor_pool; }



    /********** ResourceAllocator class methods **************/

    ResourceAllocator::ResourceAllocator(VkDevice device, VkPhysicalDevice gpu, VkExtent2D extent, VkRenderPass renderpass, std::vector<VkImageView> sc_img_views) {
        mkUniformBuffers(device, gpu, sc_img_views.size());
        mkTextureSampler(device);
        mkDepthResources(device, gpu, extent);
        mkFrameBuffers(device, renderpass, extent, sc_img_views);
    }


    /* Create uniform buffers, but not populate them */
    void ResourceAllocator::mkUniformBuffers(VkDevice &device, VkPhysicalDevice &gpu, size_t sc_img_size) {
        size_t index;
        size_t data_size;
        VkMemoryRequirements memory_requirements;

        // Resize buffer vectors 
        m_buffer_data.mat_uniform_buffers.resize(sc_img_size);
        m_buffer_data.mat_uniform_buffer_mem.resize(sc_img_size);

        // Allocate space for perspective uniform data
        data_size = sizeof(dengMath::UniformData);
        for(index = 0; index < sc_img_size; index++) {
            memory_requirements = BufferCreator::makeBuffer(&device, &gpu, &data_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &m_buffer_data.mat_uniform_buffers[index]);
            BufferCreator::allocateMemory(&device, &gpu, &m_buffer_data.mat_uniform_buffer_mem[index], memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            vkBindBufferMemory(device, m_buffer_data.mat_uniform_buffers[index], m_buffer_data.mat_uniform_buffer_mem[index], 0);
        }
    }


    /* Create texture sampler */
    void ResourceAllocator::mkTextureSampler(VkDevice &device) {
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
        if(vkCreateSampler(device, &local_samplerInfo, nullptr, &m_texture_sampler) != VK_SUCCESS)
            ERR("Failed to create texture sampler!");
    }
    

    /* Create framebuffers */
    void ResourceAllocator::mkFrameBuffers(VkDevice &device, VkRenderPass &renderpass, VkExtent2D &extent, std::vector<VkImageView> &sc_img_views) {
        size_t index;
        m_framebuffers.resize(sc_img_views.size());
        std::array<VkImageView, 2> local_attachments;

        for(index = 0; index < sc_img_views.size(); index++) {
            local_attachments = {sc_img_views[index], m_depth_image_view};

            VkFramebufferCreateInfo local_framebuffer_createinfo{};
            local_framebuffer_createinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            local_framebuffer_createinfo.renderPass = renderpass;
            local_framebuffer_createinfo.attachmentCount = local_attachments.size();
            local_framebuffer_createinfo.pAttachments = local_attachments.data();
            local_framebuffer_createinfo.width = extent.width;
            local_framebuffer_createinfo.height = extent.height;
            local_framebuffer_createinfo.layers = 1;

            if(vkCreateFramebuffer(device, &local_framebuffer_createinfo, nullptr, &m_framebuffers[index]) != VK_SUCCESS)
                ERR("Failed to create framebuffer!");
            
            else LOG("Framebuffer successfully created");
        }
    }


    /* Create depth resources for depth buffering */
    void ResourceAllocator::mkDepthResources(VkDevice &device, VkPhysicalDevice &gpu, VkExtent2D &extent) {
        VkMemoryRequirements local_memory_requirements = BufferCreator::makeImage(&device, &gpu, &m_depth_image, extent.width, extent.height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        BufferCreator::allocateMemory(&device, &gpu, &m_depth_image_memory, local_memory_requirements.size, local_memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkBindImageMemory(device, m_depth_image, m_depth_image_memory, 0);

        VkImageViewCreateInfo local_image_view_createinfo = BufferCreator::getImageViewInfo(m_depth_image, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);

        if(vkCreateImageView(device, &local_image_view_createinfo, nullptr, &m_depth_image_view) != VK_SUCCESS)
            ERR("Failed to create depth image view!");
    }


    /* Set assets and textures data */
    void ResourceAllocator::setAssetsData(std::vector<DENGAsset> *p_assets, std::vector<TextureImageData> *p_textures) {
        m_p_assets = p_assets;
        m_p_textures = p_textures;
    } 


    /* Create image buffers for every texture */
    void ResourceAllocator::mkTextureImages(VkDevice device, VkPhysicalDevice gpu, VkCommandPool command_pool, VkQueue g_queue, size_t sc_img_size) {
        size_t index;
        VkMemoryRequirements memory_requirements;

        // Iterate through assets an check if it is texture mapped
        for(index = 0; index < m_p_textures->size(); index++) {
            (*m_p_textures)[index].descriptor_sets.resize(sc_img_size);
            // Create new staging buffer and allocate memory for it
            memory_requirements =  BufferCreator::makeBuffer(&device, &gpu, &(*m_p_textures)[index].texture.pixel_data.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &m_buffer_data.staging_buffer);
            BufferCreator::allocateMemory(&device, &gpu, &m_buffer_data.staging_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            vkBindBufferMemory(device, m_buffer_data.staging_buffer, m_buffer_data.staging_buffer_memory, 0);
            BufferCreator::populateBufferMem(&device, &memory_requirements.size, (void*) (*m_p_textures)[index].texture.pixel_data.p_pixel_data, &m_buffer_data.staging_buffer_memory, 0);

            // Create new image and populate the memory for it
            BufferCreator::makeImage(&device, &gpu, &(*m_p_textures)[index].image, (uint32_t) (*m_p_textures)[index].texture.pixel_data.width, (uint32_t) (*m_p_textures)[index].texture.pixel_data.height, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
            BufferCreator::allocateMemory(&device, &gpu, &(*m_p_textures)[index].image_mem, (*m_p_textures)[index].texture.pixel_data.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            vkBindImageMemory(device, (*m_p_textures)[index].image, (*m_p_textures)[index].image_mem, 0);

            // Copy data from staging buffer to texture image
            BufferCreator::transitionImageLayout(&device, &(*m_p_textures)[index].image, &command_pool, &g_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            BufferCreator::copyBufferToImage(&device, &command_pool, &g_queue, &m_buffer_data.staging_buffer, &(*m_p_textures)[index].image, (*m_p_textures)[index].texture.pixel_data.width, (*m_p_textures)[index].texture.pixel_data.height);
            BufferCreator::transitionImageLayout(&device, &(*m_p_textures)[index].image, &command_pool, &g_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            
            // Clean the staging buffer
            vkDestroyBuffer(device, m_buffer_data.staging_buffer, nullptr);
            vkFreeMemory(device, m_buffer_data.staging_buffer_memory, nullptr);
                
            VkImageViewCreateInfo local_viewinfo = BufferCreator::getImageViewInfo((*m_p_textures)[index].image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
            if(vkCreateImageView(device, &local_viewinfo, nullptr, &(*m_p_textures)[index].image_view) != VK_SUCCESS)
                ERR("Failed to create texture image view!");

            LOG("Successfully created texture image view!");
        }
    }


    /* Create and populate main data buffer */
    void ResourceAllocator::mkBuffers(VkDevice device, VkPhysicalDevice gpu, VkCommandPool command_pool, VkQueue g_queue) {
        size_t l_index, r_index;
        LOG("Initialising main buffer!");
        VkDeviceSize total_size = 0, data_size = 0;
        VkMemoryRequirements memory_requirements;

        // Count total amount of bytes needed to allocate for assets 
        for(l_index = 0; l_index < m_p_assets->size(); l_index++) {
            LOG("Counting model bytes of model: " + std::string((*m_p_assets)[l_index].name));
            
            (*m_p_assets)[l_index].vertices.memory_offset = total_size;

            switch ((*m_p_assets)[l_index].asset_mode)
            {
            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED);
                break;
 
            case DENG_ASSET_MODE_3D_UNMAPPED: {
                total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED);
                break;
            }

            case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_2D);
                break;

            case DENG_ASSET_MODE_2D_UNMAPPED:
                total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_2D);
                break;
            
            default:
                break;
            }

            // Add indices to the buffer byte count
            (*m_p_assets)[l_index].indices.memory_offset = total_size;
            total_size += (*m_p_assets)[l_index].indices.size * sizeof(uint32_t);

            LOG("Vertices offset for asset " + std::string((*m_p_assets)[l_index].name) + ": " + 
            std::to_string((*m_p_assets)[l_index].vertices.memory_offset));
            LOG("Indices offset for asset " + std::string((*m_p_assets)[l_index].name) + ": " +
            std::to_string((*m_p_assets)[l_index].indices.memory_offset));
        }

        LOG("Main buffer size is: " + std::to_string(total_size));
        
        // Create staging buffer
        memory_requirements = BufferCreator::makeBuffer(&device, &gpu, &total_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &m_buffer_data.staging_buffer);
        BufferCreator::allocateMemory(&device, &gpu, &m_buffer_data.staging_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        vkBindBufferMemory(device, m_buffer_data.staging_buffer, m_buffer_data.staging_buffer_memory, 0);
        
        // Assign correct offsets for buffers and populate buffer memory
        for(l_index = 0, total_size = 0; l_index < m_p_assets->size(); l_index++) {
            // Populate staging buffer memory with vertices data
            switch ((*m_p_assets)[l_index].asset_mode)
            {
            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED);
                BufferCreator::populateBufferMem(&device, &data_size, (void*) (*m_p_assets)[l_index].vertices.p_texture_mapped_vert_data, 
                &m_buffer_data.staging_buffer_memory, total_size);
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED: {
                // Copy data into VERT_UNMAPPED vector for coloring purposes
                std::vector<VERT_UNMAPPED> tmp_data;
                tmp_data.resize((*m_p_assets)[l_index].vertices.size);
                
                for(r_index = 0; r_index < (*m_p_assets)[l_index].vertices.size; r_index++) {
                    tmp_data[r_index].vert_data= (*m_p_assets)[l_index].vertices.p_unmapped_vert_data[r_index];
                    tmp_data[r_index].color_data = (*m_p_assets)[l_index].solid_fill_color;
                }

                data_size = tmp_data.size() * sizeof(VERT_UNMAPPED);
                BufferCreator::populateBufferMem(&device, &data_size, tmp_data.data(), 
                &m_buffer_data.staging_buffer_memory, total_size);
                break;
            }

            case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_2D);
                BufferCreator::populateBufferMem(&device, &data_size, (void*) (*m_p_assets)[l_index].vertices.p_texture_mapped_vert_data_2d, 
                &m_buffer_data.staging_buffer_memory, total_size);
                break;

            case DENG_ASSET_MODE_2D_UNMAPPED: {
                std::vector<VERT_UNMAPPED_2D> tmp_data;
                tmp_data.resize((*m_p_assets)[l_index].vertices.size);

                for(r_index = 0; r_index < tmp_data.size(); r_index++) {
                    tmp_data[r_index].vert_data = (*m_p_assets)[l_index].vertices.p_unmapped_vert_data_2d[r_index];
                    tmp_data[r_index].color_data = (*m_p_assets)[l_index].solid_fill_color;
                }

                data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_2D);
                BufferCreator::populateBufferMem(&device, &data_size, (void*) (*m_p_assets)[l_index].vertices.p_texture_mapped_vert_data_2d,
                &m_buffer_data.staging_buffer_memory, total_size);
                break;
            }

            default:
                break;
            }

            total_size += data_size;

            // Populate staging memory with indices data
            data_size = (*m_p_assets)[l_index].indices.size * sizeof(uint32_t);
            BufferCreator::populateBufferMem(&device, &data_size, (*m_p_assets)[l_index].indices.p_indices, &m_buffer_data.staging_buffer_memory, total_size);
            total_size += data_size;
        }

        // Push data from staging buffer to main buffer
        memory_requirements = BufferCreator::makeBuffer(&device, &gpu, &total_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | 
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, &m_buffer_data.main_buffer);
        BufferCreator::allocateMemory(&device, &gpu, &m_buffer_data.main_buffer_memory, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkBindBufferMemory(device, m_buffer_data.main_buffer, m_buffer_data.main_buffer_memory, 0);
        BufferCreator::copyBufferToBuffer(&device, &command_pool, &g_queue, &m_buffer_data.staging_buffer, &m_buffer_data.main_buffer, &total_size, 0);

        // Perform staging buffer cleanup
        vkDestroyBuffer(device, m_buffer_data.staging_buffer, nullptr);
        vkFreeMemory(device, m_buffer_data.staging_buffer_memory, nullptr);
    }


    /* Update uniform data buffer */
    void ResourceAllocator::updateUniformBufferData(VkDevice device, const uint32_t current_image, Camera *p_camera, dengCameraUniformFlagBits flag_bits) {
        dengMath::UniformData local_ubo;
        local_ubo.cam_flag_bits = flag_bits;
        
        p_camera->view_matrix.getViewMatrix(&local_ubo.view);
        p_camera->p_projection_matrix->getProjectionMatrix(&local_ubo.projection);

        local_ubo.cam_flag_bits = DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_2D | DENG_CAMERA_UNIFORM_PERSPECTIVE_CAMERA_MODE_3D;

        void *data;
        vkMapMemory(device, m_buffer_data.mat_uniform_buffer_mem[current_image], 0, sizeof(dengMath::UniformData), 0, &data);
            memcpy(data, (void*) &local_ubo, sizeof(dengMath::UniformData));
        vkUnmapMemory(device, m_buffer_data.mat_uniform_buffer_mem[current_image]);
    }


    /* ResourceAllocator class getters */
    BufferData ResourceAllocator::getBD() { return m_buffer_data; }
    VkSampler ResourceAllocator::getSamp() { return m_texture_sampler; }
    std::vector<VkFramebuffer> ResourceAllocator::getFB() { return m_framebuffers; }
    VkImage ResourceAllocator::getDepImg() { return m_depth_image; }
    VkDeviceMemory ResourceAllocator::getDepImgMem() { return m_depth_image_memory; }
    VkImageView ResourceAllocator::getDepImgView() { return m_depth_image_view; }



    /********** DrawCaller class methods **********/
    DrawCaller::DrawCaller(VkDevice device, uint32_t g_queue_i) {
        mkCommandPool(device, g_queue_i);
        mkSynchronisation(device);
    }


    /* Create command pool */
    void DrawCaller::mkCommandPool(VkDevice &device, uint32_t &g_queue_i) {
        VkCommandPoolCreateInfo local_commandpool_createinfo{};
        local_commandpool_createinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        local_commandpool_createinfo.queueFamilyIndex = g_queue_i;

        // Create commandpool
        if(vkCreateCommandPool(device, &local_commandpool_createinfo, nullptr, &m_commandpool) != VK_SUCCESS)
            ERR("Failed to create command pool!");
        
        else LOG("Successfully created commandpool!");
    }


    /* Create semaphores and fences for synchronising frames */
    void DrawCaller::mkSynchronisation(VkDevice &device) {
        // Resize semaphores 
        image_available_semaphore_set.resize(DENG_MAX_FRAMES_IN_FLIGHT);
        render_finished_semaphore_set.resize(DENG_MAX_FRAMES_IN_FLIGHT);
        flight_fences.resize(DENG_MAX_FRAMES_IN_FLIGHT);
        VkSemaphoreCreateInfo local_semaphore_info{};
        local_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo local_fence_createInfo{};
        local_fence_createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        local_fence_createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(int32_t i = 0; i < DENG_MAX_FRAMES_IN_FLIGHT; i++) {
            if(vkCreateSemaphore(device, &local_semaphore_info, nullptr, &image_available_semaphore_set[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &local_semaphore_info, nullptr, &render_finished_semaphore_set[i]) != VK_SUCCESS ||
            vkCreateFence(device, &local_fence_createInfo, nullptr, &flight_fences[i])) {
                ERR("Failed to create semaphores and/or fences!");
            }
        }

        LOG("Successfully initialised semaphores and fences!");
    }


    /* Set the correct pointer to assets and textures */
    void DrawCaller::setAssetsData(std::vector<DENGAsset> *p_assets, std::vector<TextureImageData> *p_textures) {
        m_p_assets = p_assets;
        m_p_textures = p_textures;
    };


    /* Set miscellanious data arrays */
    void DrawCaller::setMiscData(std::array<PipelineData, DENG_PIPELINE_COUNT> pl_data, std::vector<VkFramebuffer> fb, std::vector<VkDescriptorSet> unmapped_ds, BufferData bd) {
        m_pl_data = pl_data;
        m_framebuffers = fb;
        m_unmapped_ds = unmapped_ds;
        m_buffer_data = bd;
    }


    /* Record draw command buffers for assets */ 
    void DrawCaller::recordDrawCommands(VkDevice device, VkQueue g_queue, VkRenderPass renderpass, VkExtent2D extent) {
        size_t l_index, r_index;

        m_commandbuffers.resize(m_framebuffers.size());

        // Set up commandbuffer allocate info
        VkCommandBufferAllocateInfo local_commandbuffer_allocation_info{};
        local_commandbuffer_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocation_info.commandPool = m_commandpool;
        local_commandbuffer_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocation_info.commandBufferCount = static_cast<uint32_t>(m_commandbuffers.size());

        // Allocate command buffers
        if(vkAllocateCommandBuffers(device, &local_commandbuffer_allocation_info, m_commandbuffers.data()))
            ERR("Failed to allocate command buffers!");
        
        else LOG("Successfully allocated command buffers!");

        // Start iterating through commandbuffers to submit draw calls
        for(l_index = 0; l_index < m_commandbuffers.size(); l_index++) {
            VkCommandBufferBeginInfo local_commandbuffer_begininfo{};
            local_commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            // Begin recording command buffer
            if(vkBeginCommandBuffer(m_commandbuffers[l_index], &local_commandbuffer_begininfo) != VK_SUCCESS)
                ERR("Failed to begin recording command buffers!");

            else LOG("Successfully begun to record command buffers!");

            // Set up renderpass begin info
            VkRenderPassBeginInfo local_renderpass_begininfo{};
            local_renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            local_renderpass_begininfo.renderPass = renderpass;
            local_renderpass_begininfo.framebuffer = m_framebuffers[l_index];
            local_renderpass_begininfo.renderArea.offset = {0, 0};
            local_renderpass_begininfo.renderArea.extent = extent;

            // Set up clear values
            std::array<VkClearValue, 2> local_clear_values;
            local_clear_values[0].color = {0.1411765f, 0.0431372f, 0.2313725f, 1.0f};
            local_clear_values[1].depthStencil = {1.0f, 0};

            // Add clear values to renderpass begin info
            local_renderpass_begininfo.clearValueCount = (uint32_t) local_clear_values.size();
            local_renderpass_begininfo.pClearValues = local_clear_values.data();
            
            // Start a new render pass
            vkCmdBeginRenderPass(m_commandbuffers[l_index], &local_renderpass_begininfo, VK_SUBPASS_CONTENTS_INLINE);
                LOG("Successfully began renderpass!");
                
                // Iterate through every asset and submit a draw call
                for(r_index = 0; r_index < m_p_assets->size(); r_index++) {
                    LOG("Vertices offset for model " + std::string((*m_p_assets)[r_index].name) + " is: " + std::to_string((*m_p_assets)[r_index].vertices.memory_offset));
                    LOG("Indices offset for model " + std::string((*m_p_assets)[r_index].name) + " is: " + std::to_string((*m_p_assets)[r_index].indices.memory_offset));
                    
                    vkCmdBindVertexBuffers(m_commandbuffers[l_index], 0, 1, &m_buffer_data.main_buffer, &(*m_p_assets)[r_index].vertices.memory_offset);
                    vkCmdBindIndexBuffer(m_commandbuffers[l_index], m_buffer_data.main_buffer, (*m_p_assets)[r_index].indices.memory_offset, VK_INDEX_TYPE_UINT32);

                    // Bind pipelines and descriptor sets according to the vertices type 
                    switch((*m_p_assets)[r_index].asset_mode) 
                    {
                    case DENG_ASSET_MODE_3D_UNMAPPED:
                        vkCmdBindPipeline(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pl_data[0].pipeline);
                        vkCmdBindDescriptorSets(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pl_data[0].p_pipeline_layout, 0, 1, 
                        &m_unmapped_ds[l_index], 0, nullptr);
                        break;

                    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                        vkCmdBindPipeline(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pl_data[1].pipeline);   
                        vkCmdBindDescriptorSets(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pl_data[1].p_pipeline_layout, 0, 1,
                        &(*m_p_textures)[(*m_p_assets)[r_index].fragment_index].descriptor_sets[l_index], 0, nullptr);                    
                        break;

                    case DENG_ASSET_MODE_2D_UNMAPPED:
                        vkCmdBindPipeline(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pl_data[2].pipeline);
                        vkCmdBindDescriptorSets(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pl_data[2].p_pipeline_layout, 0, 1, 
                        &m_unmapped_ds[l_index], 0, nullptr);
                        break;

                    case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                        vkCmdBindPipeline(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pl_data[3].pipeline);
                        vkCmdBindDescriptorSets(m_commandbuffers[l_index], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pl_data[3].p_pipeline_layout, 0, 1, 
                        &(*m_p_textures)[(*m_p_assets)[r_index].fragment_index].descriptor_sets[l_index], 0, nullptr);
                        break;
                    }

                    // Draw assets
                    vkCmdDrawIndexed(m_commandbuffers[l_index], (*m_p_assets)[r_index].indices.size, 1, 0, 0, 0);
                }

            // End render pass
            vkCmdEndRenderPass(m_commandbuffers[l_index]);
            LOG("Ended renderPass!");

            // Stop recording commandbuffer
            if(vkEndCommandBuffer(m_commandbuffers[l_index]) != VK_SUCCESS)
                ERR("Failed to end recording command buffer!");
        }
    }


    /* DrawCaller getter methods */
    VkCommandPool DrawCaller::getComPool() { return m_commandpool; }
    std::vector<VkCommandBuffer> *DrawCaller::getComBufs() { return &m_commandbuffers; }


    /********** Renderer class methods ***********/


    /* Submit assets for drawing */
    void Renderer::submitAssets(DENGAsset *p_game_objects, size_t size) {
        size_t l_index = m_assets.size(); 
        size_t r_index;
        m_assets.resize(l_index + size);
        
        for(r_index = 0; r_index < size; l_index++, r_index++)
            m_assets[l_index] = p_game_objects[r_index];
    } 


    /* Submit textures for mapping assets */
    void Renderer::submitTextures(DENGTexture *p_textures, size_t size) {
        size_t l_index = m_textures.size();
        size_t r_index;
        m_textures.resize(l_index + size);

        for(r_index = 0; r_index < size; l_index++, r_index++)
            m_textures[l_index].texture = p_textures[r_index];
    }


    /* Submit text instances to renderer and make them into DENGAsset objects */ 
    void Renderer::submitRendStr(dengUtils::dengRendStr *rend_strs, size_t size) {
        size_t asset_index = m_assets.size();
        size_t tex_index = m_textures.size();
        size_t r_index;

        m_assets.resize(asset_index + size);
        m_textures.resize(tex_index + size);

        for(r_index = 0; r_index < size; asset_index++, tex_index++, r_index++) {
            // Create new asset for text box
            m_assets[asset_index].asset_mode = DENG_ASSET_MODE_2D_TEXTURE_MAPPED;
            m_assets[asset_index].name = (char*) "Text instance";
            m_assets[asset_index].description = (char*) "Text instance";
            m_assets[asset_index].time_point = time(NULL);
            m_assets[asset_index].fragment_index = tex_index;
            m_assets[asset_index].vertices.p_texture_mapped_vert_data_2d = rend_strs[r_index].vert_pos.data();
            m_assets[asset_index].vertices.size = rend_strs[r_index].vert_pos.size();
            m_assets[asset_index].indices.p_indices = rend_strs[r_index].vert_indices.data();
            m_assets[asset_index].indices.size = rend_strs[r_index].vert_indices.size();
 
            // Create new texture for text box
            m_textures[tex_index].texture.name = (char*) "Text instance";
            m_textures[tex_index].texture.descritpion = (char*) "Text instance";
            m_textures[tex_index].texture.pixel_data.width = rend_strs[r_index].box_size.first;
            m_textures[tex_index].texture.pixel_data.height = rend_strs[r_index].box_size.second;
            m_textures[tex_index].texture.pixel_data.p_pixel_data = rend_strs[r_index].tex_data.data();
            m_textures[tex_index].texture.pixel_data.size = rend_strs[r_index].tex_data.size();
        }
    }

    /* Set renderer hints */
    void Renderer::setHints(dengRendererHintBits hints) {
        // Check for renderer usage hint
        if((hints & DENG_RENDERER_HINT_ASSET_EDITOR) == DENG_RENDERER_HINT_ASSET_EDITOR)
            m_usage_mode = DENG_RENDERER_USAGE_ASSET_EDITOR;
        else if((hints & DENG_RENDERER_HINT_MAP_EDITOR) == DENG_RENDERER_HINT_MAP_EDITOR)
            m_usage_mode = DENG_RENDERER_USAGE_MAP_EDITOR;
        else if((hints & DENG_RENDERER_HINT_GAME_MODE) == DENG_RENDERER_HINT_GAME_MODE)
            m_usage_mode = DENG_RENDERER_USAGE_GAME_MODE;
        else m_usage_mode = DENG_RENDERER_USAGE_GAME_MODE;

        
        // Miscelanious hints
        if((hints & DENG_RENDERER_HINT_ENABLE_VSYNC) == DENG_RENDERER_HINT_ENABLE_VSYNC)
            m_enable_vsync = true;
        else m_enable_vsync = false;

        if((hints & DENG_RENDERER_HINT_SHOW_FPS_COUNTER) == DENG_RENDERER_HINT_SHOW_FPS_COUNTER)
            m_count_fps = true;
        else m_count_fps = false;

        if((hints & DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS) == DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS)
            m_enable_validation_layers = true;
        else m_enable_validation_layers = false;
    }

    
    /* Setup renderer for new rendering task */
    void Renderer::initRenderer(WindowWrap *p_ww) {
        m_p_ww = p_ww;
        
        m_p_camera = new Camera({1.2f, 1.2f, 1.2f}, {0.2f, 0.2f}, 65.0f, DENG_DEFAULT_NEAR_PLANE, DENG_DEFAULT_FAR_PLANE, m_p_ww);
        m_p_ev = new dengMath::Events(m_p_ww, m_p_camera);

        // Create new renderer creator objects
        m_p_ic = new InstanceCreator(m_p_ww, m_enable_validation_layers);
        m_p_scc = new SwapChainCreator(m_p_ww, m_p_ic->getDev(), m_p_ic->getGpu(), m_p_ic->getSu(), m_p_ic->getGQi(), m_p_ic->getPQi());
        m_p_desc_c = new DescriptorCreator(m_p_ic->getDev(), m_p_scc->getExt(), m_p_scc->getRp(), &m_assets, &m_textures, m_p_scc->getSCImg().size());
        m_p_ra = new ResourceAllocator(m_p_ic->getDev(), m_p_ic->getGpu(), m_p_scc->getExt(), m_p_scc->getRp(), m_p_scc->getSCImgViews());
        m_p_dc = new DrawCaller(m_p_ic->getDev(), m_p_ic->getGQi());

        // Create buffer resources
        m_p_ra->setAssetsData(&m_assets, &m_textures);
        m_p_ra->mkTextureImages(m_p_ic->getDev(), m_p_ic->getGpu(), m_p_dc->getComPool(), m_p_ic->getGQ(), m_p_scc->getSCImg().size());
        m_p_ra->mkBuffers(m_p_ic->getDev(), m_p_ic->getGpu(), m_p_dc->getComPool(), m_p_ic->getGQ());

        // Make descriptor sets
        m_p_desc_c->mkUnmappedDS(m_p_ic->getDev(), m_p_scc->getSCImg().size(), m_p_ra->getBD());
        m_p_desc_c->mkTexMappedDS(m_p_ic->getDev(), m_p_scc->getSCImg().size(), m_p_ra->getSamp(), m_p_ra->getBD());

        // Record draw calls to command buffers 
        m_p_dc->setAssetsData(&m_assets, &m_textures);
        m_p_dc->setMiscData(m_p_desc_c->getPipelines(), m_p_ra->getFB(), m_p_desc_c->getUnmappedDS(), m_p_ra->getBD());
        m_p_dc->recordDrawCommands(m_p_ic->getDev(), m_p_ic->getGQ(), m_p_scc->getRp(), m_p_scc->getExt());
    }


    /* Renderer run method */
    void Renderer::run() {
        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;
        
        // Time points for calculating fps
        if(m_count_fps)
            begin = std::chrono::steady_clock::now();
        
        // Set input type as movement
        m_p_ww->setInputMode(DENG_INPUT_MOVEMENT);
        
        uint32_t fps = 0;
        while(is_running(m_p_ww->getWindow())) {
            if(m_count_fps) end = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() >= 1000) {
                LOG("FPS " + std::to_string(fps));
                begin = std::chrono::steady_clock::now();
                fps = 0;
            }

            else fps++;
            update_window(m_p_ww->getWindow());
            m_p_ev->update();
            // m_p_dengui_window->update();
            makeFrame();
        }
        vkDeviceWaitIdle(m_p_ic->getDev());
        destroy_window(m_p_ww->getWindow());
    }


    /* Frame update functions */
    void Renderer::makeFrame() {
        // Call Vulkan fence waiter method
        vkWaitForFences(m_p_ic->getDev(), 1, &m_p_dc->flight_fences[m_p_dc->current_frame], VK_TRUE, UINT64_MAX);
        // Call Vulkan next image acquire method
        uint32_t image_index;
        VkResult result = vkAcquireNextImageKHR(m_p_ic->getDev(), m_p_scc->getSC(), UINT64_MAX, m_p_dc->image_available_semaphore_set[m_p_dc->current_frame], VK_NULL_HANDLE, &image_index);
        
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            LOG("Image acquiring timed out!");
            return;
        }
        
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            ERR("Error acquiring swap chain image!");

        // Update uniform data buffer
        m_p_ra->updateUniformBufferData(m_p_ic->getDev(), m_p_dc->current_frame, m_p_camera, DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_2D | DENG_CAMERA_UNIFORM_PERSPECTIVE_CAMERA_MODE_3D);

        VkSemaphore local_wait_semaphores[] = {m_p_dc->image_available_semaphore_set[m_p_dc->current_frame]};
        VkSemaphore local_signal_semaphores[] = {m_p_dc->render_finished_semaphore_set[m_p_dc->current_frame]};

        VkSubmitInfo local_submitinfo{};
        local_submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkPipelineStageFlags local_wait_stages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        local_submitinfo.waitSemaphoreCount = 1;
        local_submitinfo.pWaitSemaphores = local_wait_semaphores;
        local_submitinfo.pWaitDstStageMask = local_wait_stages;
        local_submitinfo.commandBufferCount = 1;
        local_submitinfo.pCommandBuffers = &(*m_p_dc->getComBufs())[image_index];
        local_submitinfo.signalSemaphoreCount = 1;
        local_submitinfo.pSignalSemaphores = local_signal_semaphores;

        vkResetFences(m_p_ic->getDev(), 1, &m_p_dc->flight_fences[m_p_dc->current_frame]);

        if(vkQueueSubmit(m_p_ic->getGQ(), 1, &local_submitinfo, m_p_dc->flight_fences[m_p_dc->current_frame]) != VK_SUCCESS)
            ERR("Failed to submit draw command buffer!"); 

        VkPresentInfoKHR local_present_info{};
        local_present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        local_present_info.waitSemaphoreCount = 1;
        local_present_info.pWaitSemaphores = local_signal_semaphores;
        local_present_info.pImageIndices = &image_index;
        
        VkSwapchainKHR swapchains[] = {m_p_scc->getSC()};
        local_present_info.swapchainCount = 1;
        local_present_info.pSwapchains = swapchains;

        vkQueuePresentKHR(m_p_ic->getPQ(), &local_present_info);

        m_p_dc->current_frame = (m_p_dc->current_frame + 1) % DENG_MAX_FRAMES_IN_FLIGHT;
    }


    /* Cleanup the renderer before destruction */
    void Renderer::cleanup() {
        size_t index;
        // Clean framebuffers
        for(index = 0; index < m_p_ra->getFB().size(); index++) 
            vkDestroyFramebuffer(m_p_ic->getDev(), m_p_ra->getFB()[index], nullptr);

        // Clean commandbuffers and commandpools
        vkFreeCommandBuffers(m_p_ic->getDev(), m_p_dc->getComPool(), m_p_dc->getComBufs()->size(), m_p_dc->getComBufs()->data());
        vkDestroyCommandPool(m_p_ic->getDev(), m_p_dc->getComPool(), nullptr);

        // Clean pipeline related data
        std::array<PipelineData, DENG_PIPELINE_COUNT> local_pipeline_data = m_p_desc_c->getPipelines();
        for(index = 0; index < local_pipeline_data.size(); index++) { 
            vkDestroyPipeline(m_p_ic->getDev(), local_pipeline_data[index].pipeline, nullptr);
            if(local_pipeline_data[index].p_pipeline_layout) {
                vkDestroyPipelineLayout(m_p_ic->getDev(), *local_pipeline_data[index].p_pipeline_layout, nullptr);
                local_pipeline_data[index].p_pipeline_layout = nullptr;
            }
        }

        // Clean swapchain information
        vkDestroyRenderPass(m_p_ic->getDev(), m_p_scc->getRp(), nullptr);
        vkDestroyDescriptorPool(m_p_ic->getDev(), m_p_desc_c->getUnmappedDP(), nullptr);
        vkDestroyDescriptorPool(m_p_ic->getDev(), m_p_desc_c->getTexMappedDP(), nullptr);
        delete m_p_scc;

        // Clean texture images
        vkDestroySampler(m_p_ic->getDev(), m_p_ra->getSamp(), nullptr);
        for(index = 0; index < m_textures.size(); index++) {
            vkDestroyImageView(m_p_ic->getDev(), m_textures[index].image_view, nullptr);
            vkDestroyImage(m_p_ic->getDev(), m_textures[index].image, nullptr);
            vkFreeMemory(m_p_ic->getDev(), m_textures[index].image_mem, nullptr);
        }
        
        // Clean descriptor set layouts
        vkDestroyDescriptorSetLayout(m_p_ic->getDev(), m_p_desc_c->getUnmappedDSL(), nullptr);
        vkDestroyDescriptorSetLayout(m_p_ic->getDev(), m_p_desc_c->getTexMappedDSL(), nullptr);

        // Clean main buffer data
        vkDestroyBuffer(m_p_ic->getDev(), m_p_ra->getBD().main_buffer, nullptr);
        vkFreeMemory(m_p_ic->getDev(), m_p_ra->getBD().main_buffer_memory, nullptr);

        // Clean depth image resources
        vkDestroyImageView(m_p_ic->getDev(), m_p_ra->getDepImgView(), nullptr);
        vkDestroyImage(m_p_ic->getDev(), m_p_ra->getDepImg(), nullptr);
        vkFreeMemory(m_p_ic->getDev(), m_p_ra->getDepImgMem(), nullptr);

        // Clean semaphores and fences
        for(index = 0; index < DENG_MAX_FRAMES_IN_FLIGHT; index++) {
            vkDestroySemaphore(m_p_ic->getDev(), m_p_dc->image_available_semaphore_set[index], nullptr);
            vkDestroySemaphore(m_p_ic->getDev(), m_p_dc->render_finished_semaphore_set[index], nullptr);
            vkDestroyFence(m_p_ic->getDev(), m_p_dc->flight_fences[index], nullptr);
        }

        // Clean instance and devices
        vkDestroyDevice(m_p_ic->getDev(), nullptr);
        vkDestroySurfaceKHR(m_p_ic->getIns(), m_p_ic->getSu(), nullptr);
        auto mesFun = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_p_ic->getIns(), "vkDestroyDebugUtilsMessengerEXT");
        if(mesFun) mesFun(m_p_ic->getIns(), m_p_ic->getDMEXT(), nullptr);
        vkDestroyInstance(m_p_ic->getIns(), nullptr);
    }
} 