#define __VULKAN_RENDERER_CPP
#include <deng/vulkan/vulkan_renderer.h>

namespace deng {
    namespace vulkan {
        /******************************************/
        /******************************************/
        /************ InstanceCreator *************/
        /******************************************/
        /******************************************/

        InstanceCreator::InstanceCreator (
            WindowWrap *p_window_wrap, 
            deng_bool_t enable_validation_layers
        ) {
            m_required_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            m_p_win = p_window_wrap;

            __mkInstance(enable_validation_layers);
            __mkWindowSurface();
            if(enable_validation_layers)
                __mkDebugMessenger();
            
            __selectPhysicalDevice();
            __findSupportedProperties();
            __mkLogicalDevice(enable_validation_layers);
        }


        /* Create new vulkan instance for renderer */
        void InstanceCreator::__mkInstance(deng_bool_t &enable_validation_layers) {
            // Set up Vulkan application info
            VkApplicationInfo appinfo{};
            appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appinfo.pApplicationName = m_p_win->getTitle();
            appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appinfo.pEngineName = "DENG";
            appinfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
            appinfo.apiVersion = VK_API_VERSION_1_0;

            // Set up instance create info
            VkInstanceCreateInfo instance_createinfo{}; 
            instance_createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instance_createinfo.pApplicationInfo = &appinfo;

            // Get all required extensions
            deng_ui32_t extension_count;
            
            const char **extensions;
            deng_GetRequiredVKSurfaceExt (
                m_p_win->getWindow(), 
                (char***) &extensions,
                &extension_count, 
                static_cast<int>(enable_validation_layers)
            );

            instance_createinfo.enabledExtensionCount = extension_count;
            instance_createinfo.ppEnabledExtensionNames = extensions;
            LOG (
                "Required extensions count is: " + 
                std::to_string(instance_createinfo.enabledExtensionCount)
            );
            
            VkDebugUtilsMessengerCreateInfoEXT debug_createinfo{};
            
            // Check for validatation layer support
            if(enable_validation_layers && !__checkValidationLayerSupport())
                VK_INSTANCE_ERR("validation layers usage specified, but none are available!");

            else if(enable_validation_layers && __checkValidationLayerSupport()) {
                // Set up instance info to support validation layers
                instance_createinfo.enabledLayerCount = 1;
                instance_createinfo.ppEnabledLayerNames = &m_p_validation_layer;
                instance_createinfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_createinfo;

                // Set up debug messenger createinfo
                debug_createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debug_createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debug_createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debug_createinfo.pfnUserCallback = InstanceCreator::__debugCallback;
            }

            else if(!enable_validation_layers) {
                // Set up instance info to not support validation layers
                LOG("Vulkan validation layers are disabled");
                instance_createinfo.enabledLayerCount = 0;
                instance_createinfo.pNext = nullptr;
            }
            
            // Create new Vulkan instance
            if(vkCreateInstance(&instance_createinfo, nullptr, &m_instance) != VK_SUCCESS)
                VK_INSTANCE_ERR("failed to create an instance!");
        }


        /* Check if Vulkan validation layers are available */
        deng_bool_t InstanceCreator::__checkValidationLayerSupport() {
            deng_ui32_t layer_count;
            vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

            std::vector<VkLayerProperties> available_layers(layer_count);
            vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
            deng_bool_t is_layer = false;

            for(const VkLayerProperties &properties : available_layers) {
                if(!strcmp(m_p_validation_layer, properties.layerName)) {
                    is_layer = true;
                    break;
                }
            }

            return is_layer;
        }


        /* Make debug messenger for Vulkan validation layer */
        void InstanceCreator::__mkDebugMessenger() {
            VkDebugUtilsMessengerCreateInfoEXT messenger_createinfo{};
            messenger_createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            messenger_createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            messenger_createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            messenger_createinfo.pfnUserCallback = InstanceCreator::__debugCallback;
            
            auto debugUtilsMessengerCreator_fun = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

            if(!debugUtilsMessengerCreator_fun)
                VK_GEN_ERR("could not create debug messenger!");

            debugUtilsMessengerCreator_fun (
                m_instance, 
                &messenger_createinfo, 
                nullptr, 
                &m_debug_mes
            );
        }


        /* Callback method for Vulkan validation layer debugging */
        VKAPI_ATTR VkBool32 VKAPI_CALL InstanceCreator::__debugCallback (
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
            VkDebugUtilsMessageTypeFlagsEXT message_type, 
            const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, 
            void *p_user_data
        ) {
            VK_VAL_LAYER(p_callback_data->pMessage);
            return VK_FALSE;
        }

        
        /* Select appropriate graphics device */
        void InstanceCreator::__selectPhysicalDevice() {
            deng_ui32_t device_count;
            deng_ui32_t score;
            vkEnumeratePhysicalDevices (
                m_instance, 
                &device_count, 
                nullptr
            );

            if(device_count == 0)
                VK_INSTANCE_ERR("failed to find graphics cards!");

            std::vector<VkPhysicalDevice> devices(device_count);
            std::multimap<deng_ui32_t, VkPhysicalDevice> device_candidates;
            VkResult result = vkEnumeratePhysicalDevices (
                m_instance, 
                &device_count, 
                devices.data()
            );
            
            if(result != VK_SUCCESS) 
                VK_INSTANCE_ERR("no physical devices found!");

            // Iterate through every potential gpu device
            for(deng_ui32_t i = 0; i < device_count; i++) {
                score = HardwareSpecs::getDeviceScore (
                    devices[i], 
                    m_required_extension_names
                );
                SwapChainDetails swapchain_details (
                    devices[i], 
                    m_surface
                );
                
                if(!swapchain_details.getFormats().empty() && !swapchain_details.getPresentModes().empty())
                    device_candidates.insert(std::make_pair(score, devices[i]));
            }

            if(!device_candidates.empty() && device_candidates.rbegin()->first > 0)
                m_gpu = device_candidates.rbegin()->second;

            else 
                VK_INSTANCE_ERR("failed to find suitable GPU!");

            LOG("Found suitable physical device");
        }

        
        /* Create logical Vulkan device */ 
        void InstanceCreator::__mkLogicalDevice(deng_bool_t &enable_validation_layers) {
            if (
                !m_qff.findGraphicsFamily(m_gpu) || 
                !m_qff.findPresentSupportFamily(m_gpu, m_surface)
            ) VK_INSTANCE_ERR("queue supporting GPU not found!");

            std::array<VkDeviceQueueCreateInfo, 2> queue_createinfos;
            std::array<deng_ui32_t, 2> queue_families_indexes = {
                m_qff.getGraphicsQFIndex(), 
                m_qff.getPresentQFIndex()
            };

            deng_vec_t queue_priority = 1.0f;

            // Create device queue creatinfos for present and graphics queues
            for(deng_ui32_t i = 0; i < 2; i++) {
                VkDeviceQueueCreateInfo dev_queue_createinfo{};
                dev_queue_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                dev_queue_createinfo.queueFamilyIndex = queue_families_indexes[i];
                dev_queue_createinfo.queueCount = 1;
                dev_queue_createinfo.pQueuePriorities = &queue_priority;
                queue_createinfos[i] = dev_queue_createinfo;                
            } 

            VkPhysicalDeviceFeatures device_features{};
            device_features.samplerAnisotropy = VK_TRUE;

            // Create device createinfo
            VkDeviceCreateInfo logical_device_createinfo{};
            logical_device_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            logical_device_createinfo.queueCreateInfoCount = static_cast<deng_ui32_t>(queue_createinfos.size());
            logical_device_createinfo.pQueueCreateInfos = queue_createinfos.data();
            logical_device_createinfo.pEnabledFeatures = &device_features;
            logical_device_createinfo.enabledExtensionCount = m_required_extension_names.size();
            logical_device_createinfo.ppEnabledExtensionNames = m_required_extension_names.data();

            if(enable_validation_layers) {
                logical_device_createinfo.enabledLayerCount = 1;
                logical_device_createinfo.ppEnabledLayerNames = &m_p_validation_layer;
            }

            else logical_device_createinfo.enabledLayerCount = 0;

            if(vkCreateDevice(m_gpu, &logical_device_createinfo, nullptr, &m_device) != VK_SUCCESS)
                VK_INSTANCE_ERR("failed to create logical device!");

            vkGetDeviceQueue (
                m_device, 
                m_qff.getGraphicsQFIndex(), 
                0, 
                &m_qff.graphics_queue
            );
            
            vkGetDeviceQueue (
                m_device, 
                m_qff.getPresentQFIndex(), 
                0, 
                &m_qff.present_queue
            );
        }

        
        /* Create window surface with deng surface library */
        void InstanceCreator::__mkWindowSurface() {
            if(deng_InitVKSurface(m_p_win->getWindow(), &m_instance, &m_surface) != VK_SUCCESS)
                VK_INSTANCE_ERR("failed to create window surface!");
        }


        /* Find the maximum supported sample count for anti-aliasing (MSAA) */
        void InstanceCreator::__findSupportedProperties() {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(m_gpu, &props);

            // Find maximum multisample count
            VkSampleCountFlags counts = 
            props.limits.framebufferColorSampleCounts &
            props.limits.framebufferDepthSampleCounts;

            if(counts & VK_SAMPLE_COUNT_64_BIT) {
                m_max_sample_count = VK_SAMPLE_COUNT_64_BIT;
                LOG("Maximum sample count 64bit");
            } 
            else if(counts & VK_SAMPLE_COUNT_32_BIT) { 
                m_max_sample_count = VK_SAMPLE_COUNT_32_BIT;
                LOG("Maximum sample count 32bit");
            }
            else if(counts & VK_SAMPLE_COUNT_16_BIT) { 
                m_max_sample_count = VK_SAMPLE_COUNT_16_BIT;
                LOG("Maximum sample count 16bit");
            }
            else if(counts & VK_SAMPLE_COUNT_8_BIT) { 
                m_max_sample_count = VK_SAMPLE_COUNT_8_BIT;
                LOG("Maximum sample count 8bit");
            }
            else if(counts & VK_SAMPLE_COUNT_4_BIT) { 
                m_max_sample_count = VK_SAMPLE_COUNT_4_BIT;
                LOG("Maximum sample count 4bit");
            }
            else if(counts & VK_SAMPLE_COUNT_2_BIT) { 
                m_max_sample_count = VK_SAMPLE_COUNT_2_BIT;
                LOG("Maximum sample count 2bit");
            }
            else if(counts & VK_SAMPLE_COUNT_1_BIT) { 
                m_max_sample_count = VK_SAMPLE_COUNT_1_BIT;
                LOG("Maximum sample count 1bit");
            }
            else 
                m_max_sample_count = VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;

            // Find linear filtering support needed for mipmapping
            VkFormatProperties format_props;
            vkGetPhysicalDeviceFormatProperties (
                m_gpu,
                VK_FORMAT_B8G8R8A8_SRGB,
                &format_props
            );

            if(!(format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
                m_tex_linear_filtering_support = false;
                LOG("Texture linear filtering is not supported by hardware");
            }
            else {
                m_tex_linear_filtering_support = true;
                LOG("Texture linear filtering is supported by hardware");
            }
        }   

        
        /* Destroy debug messenger */
        void InstanceCreator::destroyDebugUtils (
            VkInstance instance,
            VkDebugUtilsMessengerEXT messenger
        ) {
            auto messengerDestroyer = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr (
                instance,
                "vkDestroyDebugUtilsMessengerEXT"
            );

            if(messengerDestroyer)
                messengerDestroyer (
                    instance,
                    messenger,
                    nullptr
                );
            else
                WARNME("Failed to destroy Vulkan debug utils");
        }



        /* InstanceCreator getter methods */
        VkInstance InstanceCreator::getIns() { return m_instance; }
        VkDevice InstanceCreator::getDev() { return m_device; }
        VkPhysicalDevice InstanceCreator::getGpu() { return m_gpu; }
        VkSurfaceKHR InstanceCreator::getSu() { return m_surface; }
        QueueManager InstanceCreator::getQFF() { return m_qff; } 
        VkDebugUtilsMessengerEXT InstanceCreator::getDMEXT() { return m_debug_mes; }
        deng_bool_t InstanceCreator::getLFSupport() { return m_tex_linear_filtering_support; }
        VkSampleCountFlagBits InstanceCreator::getMaxSampleCount() { return m_max_sample_count; }

        
        /*************************************************/
        /*************************************************/
        /************ SwapChainCreator class *************/
        /*************************************************/
        /*************************************************/
        SwapChainCreator::SwapChainCreator (
            VkDevice device,
            WindowWrap *p_win,
            VkPhysicalDevice gpu, 
            VkSurfaceKHR surface, 
            QueueManager qff,
            VkSampleCountFlagBits sample_c
        ) { 
            m_msaa_sample_c = sample_c;       
            m_device = device;
            m_gpu = gpu;
            m_p_win = p_win;
            m_qff = qff;
            
            m_p_sc_details = new SwapChainDetails(gpu, surface);

            __mkSwapChainSettings();
            __mkSwapChain (
                surface, 
                m_qff.getGraphicsQFIndex(), 
                m_qff.getPresentQFIndex()
            );
            __mkSCImageViews();
            __mkRenderPass();
        }


        /* Initialise swapchain settings in order to create swapchain */
        void SwapChainCreator::__mkSwapChainSettings() {
            deng_bool_t found_suitable_format = false;
            for(const VkSurfaceFormatKHR &surface_format : m_p_sc_details->getFormats()) {
                if 
                (
                    surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && 
                    surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
                ) {
                    m_surface_format = surface_format;
                    found_suitable_format = true;
                }
            }
            
            if(!found_suitable_format) {
                WARNME("Didn't find suitable surface format! Trying to use first available one!");
                m_surface_format = m_p_sc_details->getFormats()[0];
            }

            deng_bool_t foundSuitablePresentMode = false;
            for(const VkPresentModeKHR &presentFormat : m_p_sc_details->getPresentModes()) {
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

            if
            (
                m_p_sc_details->getCapabilities().currentExtent.width != UINT32_MAX && 
                m_p_sc_details->getCapabilities().currentExtent.height != UINT32_MAX
            ) m_ext = m_p_sc_details->getCapabilities().currentExtent;

            else {
                m_ext.width = m_p_win->getSize().first;
                m_ext.height = m_p_win->getSize().second;
            }
        }


        /* Create swapchain */
        void SwapChainCreator::__mkSwapChain (
            VkSurfaceKHR &surface, 
            deng_ui32_t g_queue_i, 
            deng_ui32_t p_queue_i
        ) {
            deng_ui32_t min_image_count = m_p_sc_details->getCapabilities().minImageCount + 1;
            
            // Verify that the maximum image count is not exceeded
            if
            (
                m_p_sc_details->getCapabilities().maxImageCount > 0 && 
                min_image_count > m_p_sc_details->getCapabilities().maxImageCount
            ) min_image_count = m_p_sc_details->getCapabilities().maxImageCount;


            VkSwapchainCreateInfoKHR swapchain_createinfo{};
            swapchain_createinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchain_createinfo.surface = surface;
            swapchain_createinfo.minImageCount = min_image_count;
            swapchain_createinfo.imageFormat = m_surface_format.format;
            swapchain_createinfo.imageColorSpace = m_surface_format.colorSpace;
            swapchain_createinfo.imageExtent = m_ext;
            swapchain_createinfo.imageArrayLayers = 1;
            swapchain_createinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            // Check if present queue and graphics queue are the same and if needed then synchronise the image sharing mode
            std::array<deng_ui32_t, 2> queue_family_indices = {g_queue_i, p_queue_i};
            if(g_queue_i != p_queue_i) {
                swapchain_createinfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchain_createinfo.queueFamilyIndexCount = 2;
                swapchain_createinfo.pQueueFamilyIndices = queue_family_indices.data();
            }

            else swapchain_createinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            

            swapchain_createinfo.preTransform = m_p_sc_details->getCapabilities().currentTransform;
            swapchain_createinfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
            swapchain_createinfo.presentMode = m_present_mode;
            swapchain_createinfo.clipped = VK_TRUE;

            if(vkCreateSwapchainKHR(m_device, &swapchain_createinfo, nullptr, &m_swapchain) != VK_SUCCESS)
                VK_SWAPCHAIN_ERR("failed to create create a swap chain!");

            else {
                deng_ui32_t image_count;
                vkGetSwapchainImagesKHR (
                    m_device, 
                    m_swapchain, 
                    &image_count, 
                    nullptr
                );
                m_swapchain_images.resize(image_count);
                vkGetSwapchainImagesKHR (
                    m_device, 
                    m_swapchain, 
                    &image_count, 
                    m_swapchain_images.data()
                );
            }
        }


        /* Create render pass */
        void SwapChainCreator::__mkRenderPass() {
            VkAttachmentDescription color_attachment{};
            color_attachment.format = m_surface_format.format;
            color_attachment.samples = m_msaa_sample_c;
            color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription depth_attachment{};
            depth_attachment.format = VK_FORMAT_D32_SFLOAT;
            depth_attachment.samples = m_msaa_sample_c;
            depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference color_attachment_reference{};
            color_attachment_reference.attachment = 0;
            color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            VkAttachmentReference depth_attachment_reference{};
            depth_attachment_reference.attachment = 1;
            depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            // Color attachment description for resolving multisampled images
            VkAttachmentDescription color_attachment_resolve{};
            color_attachment_resolve.format = m_surface_format.format;
            color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
            color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference color_attachment_resolve_ref{};
            color_attachment_resolve_ref.attachment = 2;
            color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass_desc{};
            subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass_desc.colorAttachmentCount = 1;
            subpass_desc.pColorAttachments = &color_attachment_reference;
            subpass_desc.pDepthStencilAttachment = &depth_attachment_reference;
            subpass_desc.pResolveAttachments = &color_attachment_resolve_ref;

            VkSubpassDependency subpass_dependency{};
            subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependency.srcAccessMask = 0;

            subpass_dependency.dstSubpass = 0;
            subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            std::array<VkAttachmentDescription, 3> attachments = {color_attachment, depth_attachment, color_attachment_resolve};
            VkRenderPassCreateInfo renderpass_createinfo{};
            renderpass_createinfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderpass_createinfo.attachmentCount = static_cast<deng_ui32_t>(attachments.size());
            renderpass_createinfo.pAttachments = attachments.data();
            renderpass_createinfo.subpassCount = 1;
            renderpass_createinfo.pSubpasses = &subpass_desc;
            renderpass_createinfo.dependencyCount = 1;
            renderpass_createinfo.pDependencies = &subpass_dependency;


            if(vkCreateRenderPass(m_device, &renderpass_createinfo, nullptr, &m_renderpass) != VK_SUCCESS)
                VK_SWAPCHAIN_ERR("failed to create render pass!");
        }


        /* Create swapchain image views */
        void SwapChainCreator::__mkSCImageViews() {
            m_swapchain_image_views.resize(m_swapchain_images.size());

            for(deng_ui32_t i = 0; i < m_swapchain_image_views.size(); i++) {
                VkImageViewCreateInfo createInfo = 
                ImageCreator::getImageViewInfo (
                    m_swapchain_images[i], 
                    m_surface_format.format, 
                    VK_IMAGE_ASPECT_COLOR_BIT,
                    1
                );
                if(vkCreateImageView(m_device, &createInfo, nullptr, &m_swapchain_image_views[i]) != VK_SUCCESS)
                    VK_SWAPCHAIN_ERR("failed to create image views!");
            }
        }


        /* Remake the swapchain (needed for resizing the window) */
        // Not tested
        void SwapChainCreator::remkSwapChain (
            VkDevice device,
            WindowWrap *p_win,
            VkSurfaceKHR surface,
            VkSurfaceFormatKHR s_format
        ) {
            vkDeviceWaitIdle(m_device);

            SCCleanup();
            __mkSwapChainSettings();
            __mkSwapChain (
                surface, 
                m_qff.getGraphicsQFIndex(),
                m_qff.getPresentQFIndex() 
            );
            __mkRenderPass();
            __mkSCImageViews();
        }


        /* Clean all swap chain related structs */
        void SwapChainCreator::SCCleanup() {
            size_t index = 0;
            for(index = 0; index < m_swapchain_images.size(); index++) {
                vkDestroyImageView (
                    m_device, 
                    m_swapchain_image_views[index], 
                    nullptr
                );
            }

            vkDestroySwapchainKHR (
                m_device, 
                m_swapchain, 
                nullptr
            );
        }


        SwapChainCreator::~SwapChainCreator() {
            delete m_p_sc_details;
        }

        
        /* SwapChainCreator getters */
        VkRenderPass SwapChainCreator::getRp() { return m_renderpass; }
        VkExtent2D SwapChainCreator::getExt() { return m_ext; }
        VkSwapchainKHR SwapChainCreator::getSC() { return m_swapchain; }
        VkFormat SwapChainCreator::getSF() { return m_surface_format.format; }
        std::vector<VkImage> SwapChainCreator::getSCImg() { return m_swapchain_images; }
        std::vector<VkImageView> SwapChainCreator::getSCImgViews() { return m_swapchain_image_views; }

        
        /***********************************************/
        /***********************************************/
        /********** DescriptorCreator class ************/
        /***********************************************/
        /***********************************************/

        DescriptorCreator::DescriptorCreator (
            VkDevice device, 
            VkExtent2D extent, 
            VkRenderPass renderpass, 
            std::vector<deng_Asset> *p_assets, 
            std::vector<TextureImage> *p_textures, 
            size_t sc_img_size,
            VkSampleCountFlagBits sample_c
        ) {
            m_p_assets = p_assets;
            m_p_textures = p_textures;
            __mkDescriptorSetLayouts(device);   
            __mkPipelineLayouts(device);
            __mkGraphicsPipelines (
                device, 
                extent, 
                renderpass, 
                sample_c
            );

            __mkUnmappedDescPool (
                device, 
                (deng_ui32_t) sc_img_size
            );

            __mkTexMappedDescPool (
                device, 
                (deng_ui32_t) sc_img_size
            );
        }


        /* Create descriptor set layouts */
        void DescriptorCreator::__mkDescriptorSetLayouts(VkDevice &device) {
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
            if
            (
                vkCreateDescriptorSetLayout (
                    device, 
                    &layout_createinfo, 
                    nullptr, 
                    &m_unmapped_desc_set_layout
                )
            ) VK_DESC_ERR("failed to create descriptor set layout for unmapped assets!");

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
            if
            (
                vkCreateDescriptorSetLayout (
                    device, 
                    &texture_mapped_layout_createinfo, 
                    nullptr, 
                    &m_tex_mapped_desc_set_layout
                ) != VK_SUCCESS
            ) VK_DESC_ERR("failed to create descriptor set layout for texture mapped assets!");
        }


        /* Create pipeline layouts */
        void DescriptorCreator::__mkPipelineLayouts(VkDevice &device) {
            // Set up pipeline layout info struct
            VkPipelineLayoutCreateInfo pipeline_layout_createinfo{};
            pipeline_layout_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipeline_layout_createinfo.setLayoutCount = 1;
            pipeline_layout_createinfo.pushConstantRangeCount = 0;
            pipeline_layout_createinfo.pSetLayouts = &m_unmapped_desc_set_layout;

            // Create pipeline layout for unmapped assets
            if
            (
                vkCreatePipelineLayout (
                    device, 
                    &pipeline_layout_createinfo, 
                    nullptr, 
                    &m_unmapped_pl
                ) != VK_SUCCESS
            ) VK_DESC_ERR("failed to create pipeline layout for unmapped objects!");

            // Create pipeline layout for texture mapped assets
            pipeline_layout_createinfo.pSetLayouts = &m_tex_mapped_desc_set_layout;
            if
            (
                vkCreatePipelineLayout (
                    device, 
                    &pipeline_layout_createinfo, 
                    nullptr, 
                    &m_tex_mapped_pl
                ) != VK_SUCCESS
            ) VK_DESC_ERR("failed to create pipeline layout for textured objects!");
        }


        /* Create graphics pipelines for different assets */
        void DescriptorCreator::__mkGraphicsPipelines (
            VkDevice &device, 
            VkExtent2D &extent, 
            VkRenderPass &renderpass,
            VkSampleCountFlagBits sample_c
        ) {
            size_t index;
            
            // Specify the pipiline type and layout
            m_pipelines[UM3D_UNOR_I].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED_3D;
            m_pipelines[UM3D_NOR_I].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED_3D_NORM;
            m_pipelines[TM3D_UNOR_I].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D;
            m_pipelines[TM3D_NOR_I].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D_NORM;
            m_pipelines[UM2D_I].pipeline_type = DENG_PIPELINE_TYPE_UNMAPPED_2D;
            m_pipelines[TM2D_I].pipeline_type = DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D;

            m_pipelines[UM3D_UNOR_I].p_pipeline_layout = &m_unmapped_pl;
            m_pipelines[UM3D_NOR_I].p_pipeline_layout = &m_unmapped_pl;
            m_pipelines[TM3D_UNOR_I].p_pipeline_layout = &m_tex_mapped_pl;
            m_pipelines[TM3D_NOR_I].p_pipeline_layout = &m_tex_mapped_pl;
            m_pipelines[UM2D_I].p_pipeline_layout = &m_unmapped_pl;
            m_pipelines[TM2D_I].p_pipeline_layout = &m_tex_mapped_pl;

            PipelineCreator unmapped_pipeline_3d (
                &m_pipelines[UM3D_UNOR_I],
                device, 
                extent, 
                renderpass
            );

            PipelineCreator unmapped_nor_pipeline_3d (
                &m_pipelines[UM3D_NOR_I],
                device, 
                extent, 
                renderpass
            );

            PipelineCreator texture_mapped_pipeline_3d (
                &m_pipelines[TM3D_UNOR_I], 
                device, 
                extent, 
                renderpass
            );

            PipelineCreator texture_mapped_nor_pipeline_3d (
                &m_pipelines[TM3D_NOR_I], 
                device, 
                extent, 
                renderpass
            );

            PipelineCreator unmapped_pipeline_2d (
                &m_pipelines[UM2D_I], 
                device, 
                extent, 
                renderpass
            );

            PipelineCreator texture_mapped_pipeline_2d (
                &m_pipelines[TM2D_I], 
                device, 
                extent, 
                renderpass
            );

            std::array<VkGraphicsPipelineCreateInfo, DENG_PIPELINE_COUNT> pipeline_infos{};
            pipeline_infos[UM3D_UNOR_I] = unmapped_pipeline_3d.mkGraphicsPipelineInfo (
                UNMAPPED_VERT_SHADER_3D, 
                UNMAPPED_FRAG_SHADER, 
                (char*) "main", 
                VK_POLYGON_MODE_FILL, 
                VK_CULL_MODE_NONE, 
                VK_FRONT_FACE_COUNTER_CLOCKWISE, 
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 
                true, 
                false, 
                sample_c,
                0
            );

            pipeline_infos[UM3D_NOR_I] = unmapped_nor_pipeline_3d.mkGraphicsPipelineInfo (
                UNMAPPED_NORM_VERT_SHADER_3D, 
                UNMAPPED_FRAG_SHADER, 
                (char*) "main", 
                VK_POLYGON_MODE_FILL, 
                VK_CULL_MODE_NONE, 
                VK_FRONT_FACE_COUNTER_CLOCKWISE, 
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 
                true, 
                false, 
                sample_c,
                0
            );

            pipeline_infos[TM3D_UNOR_I] = texture_mapped_pipeline_3d.mkGraphicsPipelineInfo (
                TEXTURE_MAPPED_VERT_SHADER_3D, 
                TEXTURE_MAPPED_FRAG_SHADER, 
                (char*) "main", 
                VK_POLYGON_MODE_FILL, 
                VK_CULL_MODE_NONE, 
                VK_FRONT_FACE_COUNTER_CLOCKWISE, 
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 
                true, 
                false,
                sample_c, 
                0
            );

            pipeline_infos[TM3D_NOR_I] = texture_mapped_nor_pipeline_3d.mkGraphicsPipelineInfo (
                TEXTURE_MAPPPED_NORM_VERT_SHADER_3D, 
                TEXTURE_MAPPED_FRAG_SHADER, 
                (char*) "main", 
                VK_POLYGON_MODE_FILL, 
                VK_CULL_MODE_NONE, 
                VK_FRONT_FACE_COUNTER_CLOCKWISE, 
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 
                true, 
                false,
                sample_c, 
                0
            );

            pipeline_infos[UM2D_I] = unmapped_pipeline_2d.mkGraphicsPipelineInfo (
                UNMAPPED_VERT_SHADER_2D, 
                UNMAPPED_FRAG_SHADER, 
                (char*) "main", 
                VK_POLYGON_MODE_FILL, 
                VK_CULL_MODE_NONE, 
                VK_FRONT_FACE_CLOCKWISE, 
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 
                false, 
                false, 
                sample_c,
                0
            );

            pipeline_infos[TM2D_I] = texture_mapped_pipeline_2d.mkGraphicsPipelineInfo (
                TEXTURE_MAPPED_VERT_SHADER_2D, 
                TEXTURE_MAPPED_FRAG_SHADER, 
                (char*) "main", 
                VK_POLYGON_MODE_FILL, 
                VK_CULL_MODE_NONE, 
                VK_FRONT_FACE_CLOCKWISE, 
                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 
                true, 
                false, 
                sample_c,
                0
            );

            LOG("Test");

            std::array<VkPipeline, DENG_PIPELINE_COUNT> pipelines;
            if
            (
                vkCreateGraphicsPipelines (
                    device, 
                    VK_NULL_HANDLE, 
                    pipeline_infos.size(), 
                    pipeline_infos.data(), 
                    nullptr, 
                    pipelines.data()
                ) != VK_SUCCESS
            ) VK_DESC_ERR("failed to create graphics pipelines!");

            else {
                for(index = 0; index < pipelines.size(); index++)
                    m_pipelines[index].pipeline = pipelines[index];
            }
            LOG("Test");
        }


        /* Create descriptor pool for descriptor sets without texture sampling */
        void DescriptorCreator::__mkUnmappedDescPool (
            const VkDevice &device,
            deng_ui32_t sc_img_c
        ) {
            VkDescriptorPoolSize desc_pool_size;
            VkDescriptorPoolCreateInfo desc_pool_createinfo{};

            desc_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_size.descriptorCount = sc_img_c;

            // Set up descriptor pool createinfo 
            desc_pool_createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            desc_pool_createinfo.poolSizeCount = 1;
            desc_pool_createinfo.pPoolSizes = &desc_pool_size;
            desc_pool_createinfo.maxSets = sc_img_c;

            // Create descriptor pool for unmapped assets 
            if(vkCreateDescriptorPool(device, &desc_pool_createinfo, nullptr, &m_unmapped_desc_pool) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor pool for unmapped assets");
        }


        
        /* Create descriptor pool for descriptor sets without texture sampling */
        void DescriptorCreator::__mkTexMappedDescPool (
            const VkDevice &device,
            deng_ui32_t sc_img_c
        ) {
            std::array<VkDescriptorPoolSize, 2> desc_pool_sizes;
            VkDescriptorPoolCreateInfo desc_pool_createinfo{};
            m_tex_cap += 2 * m_p_textures->size();

            desc_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[0].descriptorCount = m_tex_cap * sc_img_c;
            desc_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            desc_pool_sizes[1].descriptorCount = m_tex_cap * sc_img_c;

            desc_pool_createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            desc_pool_createinfo.poolSizeCount = (deng_ui32_t) desc_pool_sizes.size();
            desc_pool_createinfo.pPoolSizes = desc_pool_sizes.data();
            desc_pool_createinfo.maxSets = m_tex_cap * sc_img_c;
            desc_pool_createinfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            
            if(vkCreateDescriptorPool(device, &desc_pool_createinfo, NULL, &m_tex_mapped_desc_pool) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor pool for texture mapped assets");
        }


        void DescriptorCreator::updateTexDescriptors (
            const VkDevice &device,
            size_t sc_img_c,
            deng_bool_t new_ds,
            dengMath::vec2<deng_ui32_t> *p_tex_bounds,
            BufferData *p_bd
        ) {
            if(m_tex_cap <= m_p_textures->size()) {
                vkDestroyDescriptorPool (
                    device, 
                    m_tex_mapped_desc_pool, 
                    nullptr
                );

                __mkTexMappedDescPool (
                    device,
                    sc_img_c
                );

                mkTexMappedDS (
                    device,
                    sc_img_c,
                    {0, (deng_ui32_t) m_p_textures->size()},
                    p_bd
                );
            }

            if(new_ds && p_tex_bounds->second - p_tex_bounds->first) {
                mkTexMappedDS (
                    device,
                    sc_img_c,
                    *p_tex_bounds,
                    p_bd
                );
            }
        }


        /* Syncronise descriptor pool with textures count */
        void DescriptorCreator::syncTextures (
            const VkDevice &device, 
            size_t sc_img_c, 
            BufferData *p_bd
        ) {
            vkResetDescriptorPool (
                device,
                m_tex_mapped_desc_pool,
                0
            );

            mkTexMappedDS (
                device,
                sc_img_c,
                {0, (deng_ui32_t) m_p_textures->size()},
                p_bd
            );
        }


        /* Create unmapped descriptor sets */
        void DescriptorCreator::mkUnmappedDS (
            VkDevice device, 
            size_t sc_img_size, 
            BufferData buffer_data
        ) {
            size_t index;
            // Set up multiple descriptor layout structures
            std::vector<VkDescriptorSetLayout> tmp_descriptor_set_layouts;
            tmp_descriptor_set_layouts.resize(sc_img_size);

            VkDescriptorBufferInfo bufferinfo;
            std::vector<VkWriteDescriptorSet> descriptor_writes{};
            VkDescriptorSetAllocateInfo allocinfo{};
            allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

            // Set up descriptor set allocate info struct for unmapped vertices
            m_unmapped_descriptor_sets.resize(sc_img_size);
            for(index = 0; index < tmp_descriptor_set_layouts.size(); index++) 
                tmp_descriptor_set_layouts[index] = m_unmapped_desc_set_layout;
            
            allocinfo.descriptorPool = m_unmapped_desc_pool;
            allocinfo.descriptorSetCount = (deng_ui32_t) tmp_descriptor_set_layouts.size();
            allocinfo.pSetLayouts = tmp_descriptor_set_layouts.data();

            /* Allocate descriptor sets for unmapped 3D vertices */
            if(vkAllocateDescriptorSets(device, &allocinfo, m_unmapped_descriptor_sets.data()) != VK_SUCCESS)
                VK_DESC_ERR("failed to allocate descriptor sets for unmapped assets");

            for(index = 0; index < sc_img_size; index++) {
                // Set up descriptor buffer info struct
                bufferinfo.buffer = buffer_data.mat_uniform_buffers[index];
                bufferinfo.offset = 0;
                bufferinfo.range = sizeof(dengMath::UniformData);

                // Set up descriptor set write
                descriptor_writes.resize(1);
                descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_writes[0].dstSet = m_unmapped_descriptor_sets[index];
                descriptor_writes[0].dstBinding = 0;
                descriptor_writes[0].dstArrayElement = 0;
                descriptor_writes[0].descriptorCount = 1;
                descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptor_writes[0].pBufferInfo = &bufferinfo;

                // Update descriptor sets
                vkUpdateDescriptorSets (
                    device, 
                    (deng_ui32_t) descriptor_writes.size(), 
                    descriptor_writes.data(), 
                    0, 
                    nullptr
                );
            }
        }
        

        /* Create texture mapped descriptor sets */
        void DescriptorCreator::mkTexMappedDS (
            const VkDevice &device, 
            size_t sc_img_size, 
            const dengMath::vec2<deng_ui32_t> &tex_bounds,
            BufferData *p_bd
        ) {
            size_t l_index, r_index;
            // Set up multiple info structures
            std::vector<VkDescriptorSetLayout> tmp_descriptor_set_layouts;
            tmp_descriptor_set_layouts.resize(sc_img_size);

            VkDescriptorBufferInfo bufferinfo;
            std::vector<VkWriteDescriptorSet> descriptor_writes{};
            VkDescriptorImageInfo desc_imageinfo{};
            VkDescriptorSetAllocateInfo allocinfo{};
            allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

            for(l_index = 0; l_index < tmp_descriptor_set_layouts.size(); l_index++)
                tmp_descriptor_set_layouts[l_index] = m_tex_mapped_desc_set_layout;

            allocinfo.descriptorPool = m_tex_mapped_desc_pool;
            allocinfo.descriptorSetCount = tmp_descriptor_set_layouts.size();   
            allocinfo.pSetLayouts = tmp_descriptor_set_layouts.data();
            
            // Iterate through every texture and create descritor sets for them
            for(l_index = tex_bounds.first; l_index < tex_bounds.second; l_index++) {
                (*m_p_textures)[l_index].descriptor_sets.resize(sc_img_size);

                // Allocate descriptor sets
                if(vkAllocateDescriptorSets(device, &allocinfo, (*m_p_textures)[l_index].descriptor_sets.data()) != VK_SUCCESS)
                    VK_DESC_ERR("failed to allocate descriptor sets for texture mapped assets");

                // Iterate through every descriptor set and update it
                for(r_index = 0; r_index < sc_img_size; r_index++) {
                    // Set up descriptor buffer info 
                    bufferinfo.buffer = p_bd->mat_uniform_buffers[r_index];
                    bufferinfo.offset = 0;
                    bufferinfo.range = sizeof(dengMath::UniformData);

                    // Set up descriptor image info 
                    desc_imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    desc_imageinfo.imageView = (*m_p_textures)[l_index].image_view;
                    desc_imageinfo.sampler = (*m_p_textures)[l_index].sampler;

                    // Set up descriptor writes structs for texture mapped assets
                    descriptor_writes.resize(2);
                    descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptor_writes[0].dstSet = (*m_p_textures)[l_index].descriptor_sets[r_index];
                    descriptor_writes[0].dstBinding = 0;
                    descriptor_writes[0].dstArrayElement = 0;
                    descriptor_writes[0].descriptorCount = 1;
                    descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptor_writes[0].pBufferInfo = &bufferinfo;

                    descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptor_writes[1].dstSet = (*m_p_textures)[l_index].descriptor_sets[r_index];
                    descriptor_writes[1].dstBinding = 1;
                    descriptor_writes[1].dstArrayElement = 0;
                    descriptor_writes[1].descriptorCount = 1;
                    descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descriptor_writes[1].pImageInfo = &desc_imageinfo;

                    // Update texture mapped descriptor sets
                    vkUpdateDescriptorSets (
                        device, 
                        (deng_ui32_t) descriptor_writes.size(), 
                        descriptor_writes.data(), 
                        0, 
                        nullptr
                    );
                }
            }
        }


        /* DescriptorCreator getters */
        std::array<PipelineData, DENG_PIPELINE_COUNT> DescriptorCreator::getPipelines() { return m_pipelines; }
        VkPipelineLayout DescriptorCreator::getUnmappedPL() { return m_unmapped_pl; }
        VkPipelineLayout DescriptorCreator::getTexMappedPL() { return m_tex_mapped_pl; }
        std::vector<VkDescriptorSet> *DescriptorCreator::getUnmappedDS() { return &m_unmapped_descriptor_sets; }
        VkDescriptorSetLayout DescriptorCreator::getUnmappedDSL() { return m_unmapped_desc_set_layout; }
        VkDescriptorSetLayout DescriptorCreator::getTexMappedDSL() { return m_tex_mapped_desc_set_layout; }
        VkDescriptorPool DescriptorCreator::getUnmappedDP() { return m_unmapped_desc_pool; }
        VkDescriptorPool DescriptorCreator::getTexMappedDP() { return m_tex_mapped_desc_pool; }


        /*********************************************************/
        /*********************************************************/
        /********** ResourceAllocator class methods **************/
        /*********************************************************/
        /*********************************************************/

        ResourceAllocator::ResourceAllocator (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkExtent2D extent,
            VkSampleCountFlagBits sample_c, 
            VkRenderPass renderpass, 
            std::vector<VkImageView> sc_img_views,
            VkFormat sc_color_format,
            deng_bool_t init_res
        ) {
            m_sample_count = sample_c;
            if(init_res) {
                __mkUniformBuffers (
                    device, 
                    gpu, 
                    sc_img_views.size()
                );
                
                __mkColorResources (
                    device, 
                    gpu, 
                    extent, 
                    sc_color_format
                );
                
                __mkDepthResources (
                    device, 
                    gpu, 
                    extent
                );

                __mkFrameBuffers (
                    device, 
                    renderpass, 
                    extent, 
                    sc_img_views
                );
            }
        }


        /* Create uniform buffers, but not populate them */
        void ResourceAllocator::__mkUniformBuffers (
            VkDevice &device, 
            VkPhysicalDevice &gpu, 
            size_t sc_img_size
        ) {
            size_t index;
            size_t data_size;
            VkMemoryRequirements memory_requirements;

            // Resize buffer vectors 
            m_buffer_data.mat_uniform_buffers.resize(sc_img_size);
            m_buffer_data.mat_uniform_buffer_mem.resize(sc_img_size);

            // Allocate space for perspective uniform data
            data_size = sizeof(dengMath::UniformData);
            for(index = 0; index < sc_img_size; index++) {
                memory_requirements = BufferCreator::makeBuffer (
                    device, 
                    gpu, 
                    data_size, 
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                    m_buffer_data.mat_uniform_buffers[index]
                );
                BufferCreator::allocateMemory (
                    device, 
                    gpu, 
                    memory_requirements.size, 
                    m_buffer_data.mat_uniform_buffer_mem[index], 
                    memory_requirements.memoryTypeBits, 
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
                );

                vkBindBufferMemory (
                    device, 
                    m_buffer_data.mat_uniform_buffers[index], 
                    m_buffer_data.mat_uniform_buffer_mem[index], 
                    0
                );
            }
        }


        /* Create texture sampler */
        void ResourceAllocator::__mkTextureSampler (
            VkDevice &device,
            VkSampler &sampler,
            deng_ui32_t mip_levels
        ) {
            // Set up texture sampler createinfo base
            VkSamplerCreateInfo sampler_info{};
            sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            sampler_info.magFilter = VK_FILTER_LINEAR;
            sampler_info.minFilter = VK_FILTER_LINEAR;
            sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            sampler_info.anisotropyEnable = VK_TRUE;
            sampler_info.maxAnisotropy = 16.0f;
            sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            sampler_info.unnormalizedCoordinates = VK_FALSE;
            sampler_info.compareEnable = VK_FALSE;
            sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            sampler_info.mipLodBias = 0.0f;
            sampler_info.minLod = 0.0f;
            sampler_info.maxLod = static_cast<deng_vec_t>(mip_levels);

            // Create texture sampler 
            if(vkCreateSampler(device, &sampler_info, nullptr, &sampler) != VK_SUCCESS)
                VK_RES_ERR("failed to create texture sampler!");
        }
        

        /* Create framebuffers */
        void ResourceAllocator::__mkFrameBuffers (
            VkDevice &device, 
            VkRenderPass &renderpass, 
            VkExtent2D &extent, 
            std::vector<VkImageView> &sc_img_views
        ) {
            size_t index;
            m_framebuffers.resize(sc_img_views.size());
            std::array<VkImageView, 3> attachments;

            for(index = 0; index < sc_img_views.size(); index++) {
                attachments = {m_color_image_view, m_depth_image_view, sc_img_views[index]};

                VkFramebufferCreateInfo framebuffer_createinfo{};
                framebuffer_createinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebuffer_createinfo.renderPass = renderpass;
                framebuffer_createinfo.attachmentCount = attachments.size();
                framebuffer_createinfo.pAttachments = attachments.data();
                framebuffer_createinfo.width = extent.width;
                framebuffer_createinfo.height = extent.height;
                framebuffer_createinfo.layers = 1;

                if(vkCreateFramebuffer(device, &framebuffer_createinfo, nullptr, &m_framebuffers[index]) != VK_SUCCESS)
                    VK_RES_ERR("failed to create framebuffer!");
                
                else LOG("Framebuffer successfully created");
            }
        }


        /* Create color resources for multisampling */
        void ResourceAllocator::__mkColorResources (
            VkDevice &device,
            VkPhysicalDevice &gpu,
            VkExtent2D &extent,
            VkFormat sc_color_format
        ) {
            VkMemoryRequirements mem_req = ImageCreator::makeImage (
                device,
                gpu,
                m_color_image,
                extent.width,
                extent.height,
                1,
                sc_color_format,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                m_sample_count
            );

            BufferCreator::allocateMemory (
                device,
                gpu,
                mem_req.size,
                m_color_image_mem,
                mem_req.memoryTypeBits,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

            vkBindImageMemory (
                device,
                m_color_image,
                m_color_image_mem,
                0
            );

            VkImageViewCreateInfo image_view_createinfo = ImageCreator::getImageViewInfo (
                m_color_image,
                sc_color_format,
                VK_IMAGE_ASPECT_COLOR_BIT,
                1
            );

            if(vkCreateImageView(device, &image_view_createinfo, nullptr, &m_color_image_view) != VK_SUCCESS)
                VK_RES_ERR("failed to create color image view");
        }


        /* Create depth resources for depth buffering */
        void ResourceAllocator::__mkDepthResources (
            VkDevice &device, 
            VkPhysicalDevice &gpu, 
            VkExtent2D &extent
        ) {
            VkMemoryRequirements memory_requirements = ImageCreator::makeImage (
                device, 
                gpu, 
                m_depth_image, 
                extent.width, 
                extent.height, 
                1,
                VK_FORMAT_D32_SFLOAT, 
                VK_IMAGE_TILING_OPTIMAL, 
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                m_sample_count
            );
            
            BufferCreator::allocateMemory (
                device, 
                gpu, 
                memory_requirements.size,
                m_depth_image_mem,  
                memory_requirements.memoryTypeBits, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

            vkBindImageMemory (
                device, 
                m_depth_image, 
                m_depth_image_mem, 
                0
            );

            VkImageViewCreateInfo image_view_createinfo = ImageCreator::getImageViewInfo (
                m_depth_image, 
                VK_FORMAT_D32_SFLOAT, 
                VK_IMAGE_ASPECT_DEPTH_BIT,
                1
            );

            if(vkCreateImageView(device, &image_view_createinfo, nullptr, &m_depth_image_view) != VK_SUCCESS)
                VK_RES_ERR("failed to create depth image view!");
        }


        /* Set assets and textures data */
        void ResourceAllocator::setAssetsData (
            std::vector<deng_Asset> *p_assets, 
            std::vector<TextureImage> *p_textures
        ) {
            m_p_assets = p_assets;
            m_p_textures = p_textures;
        } 


        void ResourceAllocator::__mkMipMaps (
            VkDevice &device,
            VkCommandPool &cmd_pool,
            VkImage image,
            VkQueue g_queue,
            deng_i32_t width,
            deng_i32_t height,
            deng_ui32_t mip_levels
        ) {
            // Generate all mipmaps
            VkCommandBuffer cmd_buf;
            CommandBufferRecorder::beginCommandBufferSingleCommand (
                device,
                cmd_pool,
                &cmd_buf
            );

            VkImageMemoryBarrier mem_barrier{};
            VkImageBlit blit{};
            mem_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            mem_barrier.image = image;
            mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            mem_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            mem_barrier.subresourceRange.baseArrayLayer = 0;
            mem_barrier.subresourceRange.layerCount = 1;
            mem_barrier.subresourceRange.levelCount = 1;

            deng_i32_t mip_width = width;
            deng_i32_t mip_height = height;
            deng_ui32_t index;
            for(index = 1; index < mip_levels; index++) {
                mem_barrier.subresourceRange.baseMipLevel = index - 1;
                mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                mem_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                mem_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                // Record pipeline barrier
                vkCmdPipelineBarrier (
                    cmd_buf,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    0,
                    0,
                    NULL,
                    0,
                    NULL,
                    1,
                    &mem_barrier
                );

                // Set blit image struct for mipmapping
                blit.srcOffsets[0] = {0, 0, 0};
                blit.srcOffsets[1] = {mip_width, mip_height, 1};
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.baseArrayLayer = 0;
                blit.srcSubresource.mipLevel = index - 1;
                blit.srcSubresource.layerCount = 1;
                blit.dstOffsets[0] = {0, 0, 0};
                blit.dstOffsets[1] = {mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1};
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.baseArrayLayer = 0;
                blit.dstSubresource.mipLevel = index;
                blit.dstSubresource.layerCount = 1;

                vkCmdBlitImage (
                    cmd_buf,
                    image,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    image, 
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &blit,
                    VK_FILTER_LINEAR
                );

                mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                mem_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                mem_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                vkCmdPipelineBarrier (
                    cmd_buf,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                    0,
                    0,
                    NULL,
                    0,
                    NULL,
                    1,
                    &mem_barrier
                );

                if(mip_width > 1) mip_width /= 2;
                if(mip_height > 1) mip_height /= 2;
            }

            // Final mip level transitioning
            mem_barrier.subresourceRange.baseMipLevel = mip_levels - 1;
            mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            mem_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            mem_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier (
                cmd_buf,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0,
                NULL,
                0,
                NULL,
                1,
                &mem_barrier
            );

            CommandBufferRecorder::endCommandBufferSingleCommand (
                device,
                g_queue,
                cmd_pool,
                &cmd_buf
            );
        }


        /* Create image buffers for every texture */
        void ResourceAllocator::mkTextureImages (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool command_pool,
            deng_bool_t is_lf, 
            dengMath::vec2<deng_ui32_t> tex_bounds,
            VkQueue g_queue, 
            size_t sc_img_size
        ) {
            size_t index;
            VkMemoryRequirements memory_requirements;
            deng_ui32_t mip_levels = 0;

            // Iterate through assets an check if it is texture mapped
            for(index = tex_bounds.first; index < tex_bounds.second; index++) {
                (*m_p_textures)[index].descriptor_sets.resize(sc_img_size);
                // Check if mipmapping is supported
                if(is_lf) {
                    mip_levels = (deng_i32_t) floor (
                        log2 (
                            std::max (
                                (*m_p_textures)[index].texture.pixel_data.width, 
                                (*m_p_textures)[index].texture.pixel_data.height
                            )
                        )
                    ) + 1;
                }
                
                else mip_levels = 1;

                // Create new staging buffer and allocate memory for it
                memory_requirements = BufferCreator::makeBuffer (
                    device, 
                    gpu, 
                    (*m_p_textures)[index].texture.pixel_data.size, 
                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                    m_buffer_data.staging_buffer
                );

                BufferCreator::allocateMemory (
                    device, 
                    gpu, 
                    memory_requirements.size,
                    m_buffer_data.staging_buffer_memory,  
                    memory_requirements.memoryTypeBits, 
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
                );
                vkBindBufferMemory (
                    device, 
                    m_buffer_data.staging_buffer, 
                    m_buffer_data.staging_buffer_memory, 
                    0
                );
                BufferCreator::cpyToBufferMem (
                    device, 
                    memory_requirements.size, 
                    (*m_p_textures)[index].texture.pixel_data.p_pixel_data, 
                    m_buffer_data.staging_buffer_memory, 
                    0
                );

                // Create new image and populate the memory for it
                memory_requirements = ImageCreator::makeImage (
                    device, 
                    gpu, 
                    (*m_p_textures)[index].image, 
                    (deng_ui32_t) (*m_p_textures)[index].texture.pixel_data.width, 
                    (deng_ui32_t) (*m_p_textures)[index].texture.pixel_data.height,
                    mip_levels, 
                    VK_FORMAT_B8G8R8A8_SRGB, 
                    VK_IMAGE_TILING_OPTIMAL, 
                    VK_IMAGE_USAGE_SAMPLED_BIT | 
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                    VK_SAMPLE_COUNT_1_BIT
                );

                BufferCreator::allocateMemory (
                    device, 
                    gpu, 
                    memory_requirements.size,
                    (*m_p_textures)[index].image_mem,  
                    memory_requirements.memoryTypeBits, 
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
                );
                vkBindImageMemory (
                    device, 
                    (*m_p_textures)[index].image, 
                    (*m_p_textures)[index].image_mem, 
                    0
                );

                // Copy data from staging buffer to texture image
                ImageCreator::transitionImageLayout (
                    device, 
                    (*m_p_textures)[index].image, 
                    command_pool, 
                    g_queue, 
                    VK_FORMAT_B8G8R8A8_SRGB, 
                    VK_IMAGE_LAYOUT_UNDEFINED, 
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    mip_levels
                );

                ImageCreator::copyBufferToImage (
                    device, 
                    command_pool, 
                    g_queue, 
                    m_buffer_data.staging_buffer, 
                    (*m_p_textures)[index].image, 
                    (*m_p_textures)[index].texture.pixel_data.width, 
                    (*m_p_textures)[index].texture.pixel_data.height
                );

                // Additional mipmap enable / disable flags should be implemented
                if(is_lf) {
                    __mkMipMaps (
                        device,
                        command_pool,
                        (*m_p_textures)[index].image,
                        g_queue,
                        (deng_i32_t) (*m_p_textures)[index].texture.pixel_data.width,
                        (deng_i32_t) (*m_p_textures)[index].texture.pixel_data.height,
                        mip_levels
                    );
                }

                else {
                    ImageCreator::transitionImageLayout (
                        device,
                        (*m_p_textures)[index].image,
                        command_pool,
                        g_queue,
                        VK_FORMAT_B8G8R8_SRGB,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                        mip_levels
                    );
                }

                // Clean the staging buffer
                vkDestroyBuffer (
                    device, 
                    m_buffer_data.staging_buffer, 
                    nullptr
                );
                vkFreeMemory (
                    device, 
                    m_buffer_data.staging_buffer_memory, 
                    nullptr
                );
                    
                VkImageViewCreateInfo viewinfo = ImageCreator::getImageViewInfo (
                    (*m_p_textures)[index].image, 
                    VK_FORMAT_B8G8R8A8_SRGB, 
                    VK_IMAGE_ASPECT_COLOR_BIT,
                    mip_levels
                );
                
                if
                (
                    vkCreateImageView (
                        device, 
                        &viewinfo, 
                        nullptr, 
                        &(*m_p_textures)[index].image_view
                    ) != VK_SUCCESS
                ) VK_RES_ERR("Failed to create texture image view!");

                // Create texture sampler for every texture
                __mkTextureSampler (
                    device, 
                    (*m_p_textures)[index].sampler,
                    mip_levels
                );
            }
        }


        /* Create and populate main data buffer */
        void ResourceAllocator::mkBuffers (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool command_pool, 
            VkQueue g_queue
        ) {
            size_t l_index;
            VkDeviceSize total_size = 0, data_size = 0;
            VkMemoryRequirements memory_requirements;

            // Count total amount of bytes needed to allocate for assets 
            for(l_index = 0; l_index < m_p_assets->size(); l_index++) {
                (*m_p_assets)[l_index].vertices.memory_offset = total_size;
                //LOG (
                    //"ASSET, OFFSET: " + 
                    //std::string((*m_p_assets)[l_index].id) + 
                    //", " + 
                    //std::to_string((*m_p_assets)[l_index].is_shown) +
                    //"; " +
                    //std::to_string((*m_p_assets)[l_index].vertices.memory_offset)
                //);

                switch ((*m_p_assets)[l_index].asset_mode)
                {
                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                    total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_UNOR);
                    break;

                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                    total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_NOR);
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED:
                    total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_UNOR);
                    break;
                
                case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                    total_size += (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_NOR);
                    break;

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
                total_size += (*m_p_assets)[l_index].indices.size * sizeof(deng_ui32_t);
            }

            LOG("Main buffer size is: " + std::to_string(total_size));
            
            // Create staging buffer
            memory_requirements = BufferCreator::makeBuffer (
                device, 
                gpu, 
                total_size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                m_buffer_data.staging_buffer
            );

            BufferCreator::allocateMemory (
                device, 
                gpu, 
                memory_requirements.size,
                m_buffer_data.staging_buffer_memory,  
                memory_requirements.memoryTypeBits, 
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            vkBindBufferMemory (
                device, 
                m_buffer_data.staging_buffer, 
                m_buffer_data.staging_buffer_memory, 
                0
            );
            
            // Assign correct offsets for buffers and populate buffer memory
            for(l_index = 0, total_size = 0; l_index < m_p_assets->size(); l_index++) {
                // Populate staging buffer memory with vertices data
                switch ((*m_p_assets)[l_index].asset_mode)
                {
                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                    data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_UNOR);
                    BufferCreator::cpyToBufferMem (
                        device, 
                        data_size, 
                        (*m_p_assets)[l_index].vertices.p_tex_mapped_unnormalized_vert, 
                        m_buffer_data.staging_buffer_memory, 
                        total_size
                    );
                    break;

                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                    data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_NOR);
                    BufferCreator::cpyToBufferMem (
                        device, 
                        data_size, 
                        (*m_p_assets)[l_index].vertices.p_tex_mapped_normalized_vert, 
                        m_buffer_data.staging_buffer_memory, 
                        total_size
                    );
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED:
                    data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_UNOR);
                    BufferCreator::cpyToBufferMem (
                        device, 
                        data_size, 
                        (*m_p_assets)[l_index].vertices.p_unmapped_unnormalized_vert, 
                        m_buffer_data.staging_buffer_memory, 
                        total_size
                    );
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                    data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_NOR);
                    BufferCreator::cpyToBufferMem (
                        device, 
                        data_size, 
                        (*m_p_assets)[l_index].vertices.p_unmapped_normalized_vert, 
                        m_buffer_data.staging_buffer_memory, 
                        total_size
                    );
                    break;

                case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                    data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_MAPPED_2D);
                    BufferCreator::cpyToBufferMem (
                        device, 
                        data_size, 
                        (*m_p_assets)[l_index].vertices.p_tex_mapped_vert_data_2d, 
                        m_buffer_data.staging_buffer_memory, 
                        total_size
                    );
                    break;

                case DENG_ASSET_MODE_2D_UNMAPPED: {
                    data_size = (*m_p_assets)[l_index].vertices.size * sizeof(VERT_UNMAPPED_2D);
                    BufferCreator::cpyToBufferMem (
                        device, 
                        data_size, 
                        (*m_p_assets)[l_index].vertices.p_unmapped_vert_data_2d,
                        m_buffer_data.staging_buffer_memory, 
                        total_size
                    );
                    break;
                }

                default:
                    break;
                }

                total_size += data_size;

                // Populate staging memory with indices data
                data_size = (*m_p_assets)[l_index].indices.size * sizeof(deng_ui32_t);
                BufferCreator::cpyToBufferMem (
                    device, 
                    data_size, 
                    (*m_p_assets)[l_index].indices.p_indices, 
                    m_buffer_data.staging_buffer_memory, 
                    total_size
                );

                total_size += data_size;
            }

            // Push data from staging buffer to main buffer
            memory_requirements = BufferCreator::makeBuffer (
                device, 
                gpu, 
                total_size, 
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | 
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT | 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
                m_buffer_data.main_buffer
            );

            BufferCreator::allocateMemory (
                device, 
                gpu, 
                memory_requirements.size, 
                m_buffer_data.main_buffer_memory, 
                memory_requirements.memoryTypeBits, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

            vkBindBufferMemory (
                device, 
                m_buffer_data.main_buffer, 
                m_buffer_data.main_buffer_memory, 
                0
            );

            BufferCreator::copyBufferToBuffer (
                device, 
                command_pool, 
                g_queue, 
                m_buffer_data.staging_buffer, 
                m_buffer_data.main_buffer, 
                total_size, 
                0
            );

            // Perform staging buffer cleanup
            vkDestroyBuffer(device, m_buffer_data.staging_buffer, nullptr);
            vkFreeMemory(device, m_buffer_data.staging_buffer_memory, nullptr);
        }


        /* Update uniform data buffer */
        void ResourceAllocator::updateUniformBufferData (
            VkDevice device, 
            const deng_ui32_t current_image, 
            void *p_camera, 
            deng_CameraType cam_type,
            deng_CameraUniformFlagBits flag_bits
        ) {
            dengMath::UniformData ubo;
            ubo.cam_flag_bits = flag_bits;
            
            switch(cam_type)
            {
            case DENG_CAMERA_FPP:
                ((FPPCamera*) p_camera)->view_matrix.getViewMatrix(&ubo.view);
                ((FPPCamera*) p_camera)->p_projection_matrix->getProjectionMatrix(&ubo.projection);
                break;

            case DENG_CAMERA_EDITOR:
                ((EditorCamera*) p_camera)->view_matrix.getViewMatrix(&ubo.view);
                ((EditorCamera*) p_camera)->p_projection_matrix->getProjectionMatrix(&ubo.projection);
                break;

            default:
                break;
            }

            ubo.cam_flag_bits = DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_2D | 
                                DENG_CAMERA_UNIFORM_PERSPECTIVE_CAMERA_MODE_3D;

            void *data;
            vkMapMemory (
                device, 
                m_buffer_data.mat_uniform_buffer_mem[current_image], 
                0, 
                sizeof(dengMath::UniformData), 
                0, 
                &data
            );
                memcpy (
                    data, 
                    &ubo, 
                    sizeof(dengMath::UniformData)
                );

            vkUnmapMemory (
                device, 
                m_buffer_data.mat_uniform_buffer_mem[current_image]
            );
        }


        /* Remap asset vertices to main buffer */
        void ResourceAllocator::remapAssetVerts (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            dengMath::vec2<deng_ui32_t> asset_bounds
        ) {
            // Verify asset bounds
            if(asset_bounds.first >= m_p_assets->size() || asset_bounds.second > m_p_assets->size()) 
                RUN_ERR("remapAssetVerts(): Invalid asset vertices bounds!");
            
            // Find maximum vertex memory usage
            VkDeviceSize max_mem = 0;
            for(size_t i = asset_bounds.first; i < asset_bounds.second; i++) {
                switch(m_p_assets->at(i).asset_mode) 
                {
                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                    if(max_mem < m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_UNOR))
                        max_mem = m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_UNOR);
                    break;

                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                    if(max_mem < m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_NOR))
                        max_mem = m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_NOR);
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED:
                    if(max_mem < m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_UNOR))
                        max_mem = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_UNOR);
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                    if(max_mem < m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_NOR))
                        max_mem = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_NOR);
                    break;

                case DENG_ASSET_MODE_2D_UNMAPPED:
                    if(max_mem < m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_2D))
                        max_mem = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_2D);
                    break;

                case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                    if(max_mem < m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_2D))
                        max_mem = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_2D);
                    break;

                default:
                    break;
                }
            }

            // Allocate memory for staging buffer
            VkMemoryRequirements mem_req;
            mem_req = BufferCreator::makeBuffer (
                device,
                gpu,
                max_mem,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                m_buffer_data.staging_buffer

            );

            BufferCreator::allocateMemory (
                device,
                gpu,
                mem_req.size,
                m_buffer_data.staging_buffer_memory,
                mem_req.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            vkBindBufferMemory (
                device,
                m_buffer_data.staging_buffer,
                m_buffer_data.staging_buffer_memory,
                0
            );

            // Update main buffer 
            VkDeviceSize offset = 0;
            VkDeviceSize size = 0;
            for(size_t i = asset_bounds.first; i < asset_bounds.second; i++) {
                switch(m_p_assets->at(i).asset_mode) 
                {
                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                    offset = m_p_assets->at(i).vertices.memory_offset;
                    size = m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_UNOR);

                    BufferCreator::cpyToBufferMem ( 
                        device,
                        size,
                        m_p_assets->at(i).vertices.p_tex_mapped_unnormalized_vert,
                        m_buffer_data.staging_buffer_memory,
                        0 
                    );
                    break;

                case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                    offset = m_p_assets->at(i).vertices.memory_offset;
                    size = m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_NOR);

                    BufferCreator::cpyToBufferMem ( 
                        device,
                        size,
                        m_p_assets->at(i).vertices.p_tex_mapped_normalized_vert,
                        m_buffer_data.staging_buffer_memory,
                        0 
                    );
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED:
                    offset = m_p_assets->at(i).vertices.memory_offset;
                    size = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_UNOR);

                    BufferCreator::cpyToBufferMem ( 
                        device,
                        size,
                        m_p_assets->at(i).vertices.p_unmapped_unnormalized_vert,
                        m_buffer_data.staging_buffer_memory,
                        0 
                    );
                    break;

                case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                    offset = m_p_assets->at(i).vertices.memory_offset;
                    size = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_NOR);

                    BufferCreator::cpyToBufferMem ( 
                        device,
                        size,
                        m_p_assets->at(i).vertices.p_unmapped_normalized_vert,
                        m_buffer_data.staging_buffer_memory,
                        0 
                    );
                    break;

                case DENG_ASSET_MODE_2D_UNMAPPED:
                    offset = m_p_assets->at(i).vertices.memory_offset;
                    size = m_p_assets->at(i).vertices.size * sizeof(VERT_UNMAPPED_2D);

                    BufferCreator::cpyToBufferMem ( 
                        device,
                        size,
                        m_p_assets->at(i).vertices.p_unmapped_vert_data_2d,
                        m_buffer_data.staging_buffer_memory,
                        0 
                    );
                    break;

                case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                    offset = m_p_assets->at(i).vertices.memory_offset;
                    size = m_p_assets->at(i).vertices.size * sizeof(VERT_MAPPED_2D);

                    BufferCreator::cpyToBufferMem ( 
                        device,
                        size,
                        m_p_assets->at(i).vertices.p_tex_mapped_vert_data_2d,
                        m_buffer_data.staging_buffer_memory,
                        0 
                    );
                    break;

                default:
                    break;
                }


                BufferCreator::copyBufferToBuffer (
                    device,
                    cmd_pool,
                    g_queue,
                    m_buffer_data.staging_buffer,
                    m_buffer_data.main_buffer,
                    size,
                    offset
                );
            }

            vkDestroyBuffer(device, m_buffer_data.staging_buffer, NULL);
            vkFreeMemory(device, m_buffer_data.staging_buffer_memory, NULL);
        }


        /* ResourceAllocator class getters */
        BufferData *ResourceAllocator::getBD() { return &m_buffer_data; }
        std::vector<VkFramebuffer> ResourceAllocator::getFB() { return m_framebuffers; }
        VkImage ResourceAllocator::getDepImg() { return m_depth_image; }
        VkDeviceMemory ResourceAllocator::getDepImgMem() { return m_depth_image_mem; }
        VkImageView ResourceAllocator::getDepImgView() { return m_depth_image_view; }
        VkImage ResourceAllocator::getColorImg() { return m_color_image; }
        VkDeviceMemory ResourceAllocator::getColorImgMem() { return m_color_image_mem; }
        VkImageView ResourceAllocator::getColorImgView() { return m_color_image_view; }


        /********** DrawCaller class methods **********/
        DrawCaller::DrawCaller (
            VkDevice device,
            QueueManager qff
        ) {
            m_qff = qff;
            __mkSynchronisation(device);
        }


        /* Create command pool */
        void DrawCaller::mkCommandPool(VkDevice device) {
            VkCommandPoolCreateInfo commandpool_createinfo{};
            commandpool_createinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandpool_createinfo.queueFamilyIndex = m_qff.getGraphicsQFIndex();
            commandpool_createinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            // Create commandpool
            if(vkCreateCommandPool(device, &commandpool_createinfo, nullptr, &m_cmd_pool) != VK_SUCCESS)
                VK_DRAWCMD_ERR("failed to create command pool");
        }


        /* Find correct texture by ID specified by the asset */
        TextureImage DrawCaller::__findTextureImageByID(char *id) {
            size_t l_index;
            for(l_index = 0; l_index < m_p_textures->size(); l_index++) {
                if(!strcmp(id, (*m_p_textures)[l_index].texture.id))
                    return (*m_p_textures)[l_index];    
            }

            // Error texture not found
            VK_DRAWCMD_ERR("could not find texture with id: " + std::string(id));
        }


        /* Create semaphores and fences for synchronising frames */
        void DrawCaller::__mkSynchronisation(VkDevice &device) {
            // Resize semaphores 
            image_available_semaphore_set.resize(DENG_MAX_FRAMES_IN_FLIGHT);
            render_finished_semaphore_set.resize(DENG_MAX_FRAMES_IN_FLIGHT);
            flight_fences.resize(DENG_MAX_FRAMES_IN_FLIGHT);
            VkSemaphoreCreateInfo semaphore_info{};
            semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fence_createInfo{};
            fence_createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for(deng_i32_t i = 0; i < DENG_MAX_FRAMES_IN_FLIGHT; i++) {
                if
                (
                    vkCreateSemaphore (
                        device, 
                        &semaphore_info, 
                        nullptr, 
                        &image_available_semaphore_set[i]
                    ) != VK_SUCCESS ||

                    vkCreateSemaphore (
                        device, 
                        &semaphore_info, 
                        nullptr, 
                        &render_finished_semaphore_set[i]
                    ) != VK_SUCCESS ||

                    vkCreateFence (
                        device, 
                        &fence_createInfo, 
                        nullptr, 
                        &flight_fences[i]
                    ) != VK_SUCCESS
                ) VK_DRAWCMD_ERR("Failed to create semaphores");
                else LOG("Successfully created semaphores");
            }
        }


        /* Set the correct pointer to assets and textures */
        void DrawCaller::setAssetsData (
            std::vector<deng_Asset> *p_assets, 
            std::vector<TextureImage> *p_textures
        ) {
            m_p_assets = p_assets;
            m_p_textures = p_textures;
        }


        /* Set miscellanious data arrays */
        void DrawCaller::setMiscData (
            std::array<PipelineData, DENG_PIPELINE_COUNT> pl_data, 
            std::vector<VkFramebuffer> fb, 
            std::vector<VkDescriptorSet> *p_unmapped_ds 
        ) {
            m_pl_data = pl_data;
            m_framebuffers = fb;
            m_p_unmapped_ds = p_unmapped_ds;
        }


        /* Record draw command buffers for assets */ 
        void DrawCaller::allocateMainCmdBuffers (
            VkDevice device, 
            VkQueue g_queue, 
            VkRenderPass renderpass, 
            VkExtent2D ext,
            dengMath::vec4<deng_vec_t> background,
            const BufferData &bd
        ) {
            m_cmd_bufs.resize(m_framebuffers.size());

            // Set up commandbuffer allocate info
            VkCommandBufferAllocateInfo commandbuffer_allocation_info{};
            commandbuffer_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandbuffer_allocation_info.commandPool = m_cmd_pool;
            commandbuffer_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandbuffer_allocation_info.commandBufferCount = static_cast<deng_ui32_t>(m_cmd_bufs.size());

            // Allocate command buffers
            if
            (
                vkAllocateCommandBuffers (
                    device, 
                    &commandbuffer_allocation_info, 
                    m_cmd_bufs.data()
                )
            ) VK_DRAWCMD_ERR("failed to allocate command buffers");
            
            recordMainCmdBuffers (
                renderpass,
                ext,
                background,
                bd
            );
        }


        /* Record main commandbuffers */
        void DrawCaller::recordMainCmdBuffers (
            VkRenderPass renderpass,
            VkExtent2D ext,
            dengMath::vec4<deng_vec_t> background,
            const BufferData &bd
        ) {
            size_t l_index, r_index;

            // Start iterating through commandbuffers to submit draw calls
            for(l_index = 0; l_index < m_cmd_bufs.size(); l_index++) {
                VkCommandBufferBeginInfo commandbuffer_begininfo{};
                commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

                // Begin recording command buffer
                if(vkBeginCommandBuffer(m_cmd_bufs[l_index], &commandbuffer_begininfo) != VK_SUCCESS)
                    VK_DRAWCMD_ERR("failed to begin recording command buffers");

                // Set up renderpass begin info
                VkRenderPassBeginInfo renderpass_begininfo{};
                renderpass_begininfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderpass_begininfo.renderPass = renderpass;
                renderpass_begininfo.framebuffer = m_framebuffers[l_index];
                renderpass_begininfo.renderArea.offset = {0, 0};
                renderpass_begininfo.renderArea.extent = ext;

                // Set up clear values
                std::array<VkClearValue, 2> clear_values;
                clear_values[0].color = (VkClearColorValue) {
                    background.first, 
                    background.second, 
                    background.third, 
                    background.fourth
                };
                clear_values[1].depthStencil = {1.0f, 0};

                // Add clear values to renderpass begin info
                renderpass_begininfo.clearValueCount = 
                (deng_ui32_t) clear_values.size();
                renderpass_begininfo.pClearValues = clear_values.data();
                
                // Start a new render pass
                vkCmdBeginRenderPass (
                    m_cmd_bufs[l_index], 
                    &renderpass_begininfo, 
                    VK_SUBPASS_CONTENTS_INLINE
                );
                    // Iterate through every asset and submit a draw call
                    for(r_index = 0; r_index < m_p_assets->size(); r_index++) {
                        if((*m_p_assets)[r_index].is_shown) {
                            vkCmdBindVertexBuffers (
                                m_cmd_bufs[l_index], 
                                0, 
                                1, 
                                &bd.main_buffer, 
                                &(*m_p_assets)[r_index].vertices.memory_offset
                            );

                            vkCmdBindIndexBuffer (
                                m_cmd_bufs[l_index], 
                                bd.main_buffer, 
                                (*m_p_assets)[r_index].indices.memory_offset, 
                                VK_INDEX_TYPE_UINT32
                            );

                            // Bind pipelines and descriptor sets according to the vertices type 
                            switch((*m_p_assets)[r_index].asset_mode) 
                            {
                            case DENG_ASSET_MODE_3D_UNMAPPED:
                                vkCmdBindPipeline (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    m_pl_data[UM3D_UNOR_I].pipeline
                                );
                                vkCmdBindDescriptorSets (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    *m_pl_data[UM3D_UNOR_I].p_pipeline_layout, 
                                    0, 
                                    1, 
                                    &m_p_unmapped_ds->at(l_index), 
                                    0, 
                                    nullptr
                                );
                                break;

                            case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                                vkCmdBindPipeline (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    m_pl_data[UM3D_NOR_I].pipeline
                                );
                                vkCmdBindDescriptorSets (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    *m_pl_data[UM3D_NOR_I].p_pipeline_layout, 
                                    0, 
                                    1, 
                                    &(*m_p_unmapped_ds)[l_index], 
                                    0, 
                                    nullptr
                                );
                                break;

                            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                                vkCmdBindPipeline (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    m_pl_data[TM3D_UNOR_I].pipeline
                                );   
                                vkCmdBindDescriptorSets (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    *m_pl_data[TM3D_UNOR_I].p_pipeline_layout, 
                                    0, 
                                    1,
                                    &__findTextureImageByID((*m_p_assets)[r_index].tex_id).descriptor_sets[l_index], 
                                    0, 
                                    nullptr
                                );                    
                                break;

                            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                                vkCmdBindPipeline (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    m_pl_data[TM3D_NOR_I].pipeline
                                );   
                                vkCmdBindDescriptorSets (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    *m_pl_data[TM3D_NOR_I].p_pipeline_layout, 
                                    0, 
                                    1,
                                    &__findTextureImageByID((*m_p_assets)[r_index].tex_id).descriptor_sets[l_index], 
                                    0, 
                                    nullptr
                                );                    
                                break;

                            case DENG_ASSET_MODE_2D_UNMAPPED:
                                vkCmdBindPipeline (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    m_pl_data[UM2D_I].pipeline
                                );

                                vkCmdBindDescriptorSets (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    *m_pl_data[UM2D_I].p_pipeline_layout, 
                                    0, 
                                    1, 
                                    &(*m_p_unmapped_ds)[l_index], 
                                    0, 
                                    nullptr
                                );

                                break;

                            case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                                vkCmdBindPipeline (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    m_pl_data[TM2D_I].pipeline
                                );
                                vkCmdBindDescriptorSets (
                                    m_cmd_bufs[l_index], 
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                    *m_pl_data[TM2D_I].p_pipeline_layout, 
                                    0, 
                                    1, 
                                    &__findTextureImageByID((*m_p_assets)[r_index].id).descriptor_sets[l_index], 
                                    0, 
                                    nullptr
                                );
                                break;

                            default:
                                break;
                            }

                            // Draw assets
                            vkCmdDrawIndexed (
                                m_cmd_bufs[l_index], 
                                (*m_p_assets)[r_index].indices.size, 
                                1, 
                                0, 
                                0, 
                                0
                            );
                        }
                    }

                // End render pass
                vkCmdEndRenderPass(m_cmd_bufs[l_index]);

                // Stop recording commandbuffer
                if(vkEndCommandBuffer(m_cmd_bufs[l_index]) != VK_SUCCESS)
                    VK_DRAWCMD_ERR("failed to end recording command buffer");
            }
        }


        /* DrawCaller getter methods */
        VkCommandPool DrawCaller::getComPool() { return m_cmd_pool; }
        const std::vector<VkCommandBuffer> &DrawCaller::getComBufs() { return m_cmd_bufs; }
    }
}
