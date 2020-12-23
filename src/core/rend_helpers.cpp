#include "api_core.h"

namespace deng {

    /* Check for Vulkan extension support by hardware */
    bool HardwareSpecs::getExtensionSupport(const VkPhysicalDevice &gpu, const char *p_extenstion_name) {
        // Count total amount of supported extensions  
        LOG("Finding support for extension " + std::string(p_extenstion_name));
        uint32_t local_extension_count;
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &local_extension_count, nullptr);
        
        // Get extensions by names
        std::vector<VkExtensionProperties> local_all_extensions(local_extension_count);
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &local_extension_count, local_all_extensions.data());
        
        // Iterate through all extension to find matching extension
        for(VkExtensionProperties &extension : local_all_extensions) {
            const char *local_current_extenstion_name = extension.extensionName;
            LOG("Supported: " + std::string(local_current_extenstion_name));
            
            if(std::string(local_current_extenstion_name) == std::string(p_extenstion_name)) {
                LOG("Detected support for " + std::string(local_current_extenstion_name));
                return true;
            }
        }

        return false;
    }


    /* Find appropriate memory type */
    uint32_t HardwareSpecs::getMemoryType(VkPhysicalDevice *p_gpu, const uint32_t &type_filter, const VkMemoryPropertyFlags &properties) {
        // Get all device memory properties
        VkPhysicalDeviceMemoryProperties local_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(*p_gpu, &local_memory_properties);

        for(uint32_t index = 0; index < local_memory_properties.memoryTypeCount; index++) {
            if(type_filter & (1 << index) && (local_memory_properties.memoryTypes[index].propertyFlags & properties)) {
                LOG("Buffer mem type: " + std::to_string(index));
                return index;
            }
        }

        ERR("Failed to find suitable memory type");
    }


    /* Get a score for GPUs based on the properties and features */
    uint32_t HardwareSpecs::getDeviceScore(VkPhysicalDevice *p_gpu, std::vector<const char*> &required_extenstions) {
        uint32_t score = 0;
        VkPhysicalDeviceFeatures local_device_features;
        VkPhysicalDeviceProperties local_device_properties;

        vkGetPhysicalDeviceProperties(*p_gpu, &local_device_properties);
        vkGetPhysicalDeviceFeatures(*p_gpu, &local_device_features);

        if(local_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
        
        if(local_device_features.fillModeNonSolid) score += 500;

        score += local_device_properties.limits.maxImageDimension2D;
        score += local_device_properties.limits.maxImageDimension3D;
        score += local_device_properties.limits.maxMemoryAllocationCount;
        score += local_device_properties.limits.maxVertexOutputComponents;

        if(!local_device_features.geometryShader) return 0;
        if(!local_device_features.samplerAnisotropy) return 0;

        // Check if all the extension are supported by the device  
        for(const char* extenstion_name : required_extenstions) {
            if(!HardwareSpecs::getExtensionSupport(*p_gpu, extenstion_name)) {
                LOG("Required extension: " + std::string(extenstion_name) + " is not supported!");
                return 0;
            }
        }

        return score;
    }


    /* Find correct graphics queue family */
    bool QueueFamilies::findGraphicsFamily(VkPhysicalDevice &gpu) {
        uint32_t index;
        // Get the total count of queue families
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        
        // Get properties of all queue families
        std::vector<VkQueueFamilyProperties> queue_family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, queue_family_properties.data());

        // Check which family is graphics family
        for(index = 0; index < family_count; index++) {
            if(queue_family_properties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphics_family_index = index;
                LOG("Graphics queue family index: " + std::to_string(m_graphics_family_index));
                return true;
            }
        }

        return false;
    }


    /* Graphics queuefamily getter method */
    uint32_t QueueFamilies::getGraphicsFamilyIndex() { return m_graphics_family_index; }


    /* Find correct present queue family */
    bool QueueFamilies::findPresentSupportFamily(VkPhysicalDevice &gpu, VkSurfaceKHR &surface) {
        uint32_t index;
        // Get the total count of queue families
        uint32_t family_count = 0;
        VkBool32 support = false;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        
        // Get properties of all queue families
        std::vector<VkQueueFamilyProperties> family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, family_properties.data());

        for(index = 0; index < family_count; index++) {
            vkGetPhysicalDeviceSurfaceSupportKHR(gpu, index, surface, &support);
            if(support && m_graphics_family_index != index) {
                m_present_family_index = index; 
                LOG("Presentation queue family index: " + m_present_family_index);
                return true;
            }
        }
        return false;
    }


    /* Present queue family getter method */
    uint32_t QueueFamilies::getPresentFamilyIndex() { return m_present_family_index; }


    /* Initialize private variables */
    PipelineCreator::PipelineCreator(PipelineData *p_pipeline_data, VkDevice *device, VkExtent2D *extent, VkRenderPass *renderpass) {
        m_p_pipeline_data = p_pipeline_data;
        m_p_device = device;
        m_p_extent = extent;
        m_p_renderpass = renderpass;
    }


    /* Create shader module and return it */
    VkShaderModule PipelineCreator::getShaderModule(std::vector<char> &shader_bins) {
        VkShaderModuleCreateInfo local_createinfo{};
        local_createinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        local_createinfo.codeSize = (uint32_t) shader_bins.size();
        local_createinfo.pCode = reinterpret_cast<const uint32_t*>(shader_bins.data());
        VkShaderModule local_shader_module;

        if(vkCreateShaderModule(*m_p_device, &local_createinfo, nullptr, &local_shader_module) != VK_SUCCESS)
            ERR("Failed to create shader module!");

        return local_shader_module;
    }

    /* Get vertex input binding description info*/ 
    VkVertexInputBindingDescription PipelineCreator::getBindingDesc() {
        VkVertexInputBindingDescription local_input_binding_desc;
        local_input_binding_desc.binding = 0;
        local_input_binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        switch (m_p_pipeline_data->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_UNMAPPED_3D:
            local_input_binding_desc.stride = sizeof(VERT_UNMAPPED);
            break;
        
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D:
            local_input_binding_desc.stride = sizeof(VERT_MAPPED);
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED_2D:
            local_input_binding_desc.stride = sizeof(VERT_UNMAPPED_2D);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D:
            local_input_binding_desc.stride = sizeof(VERT_MAPPED_2D);
            break;
            
        default:
            break;
        }

        return local_input_binding_desc;
    } 

    /* Get vertex input attribute description info */
    std::vector<VkVertexInputAttributeDescription> PipelineCreator::getAttributeDesc() {
        std::vector<VkVertexInputAttributeDescription> local_input_attr_desc{};
        local_input_attr_desc.resize(2);

        switch (m_p_pipeline_data->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_UNMAPPED_3D:
            local_input_attr_desc[0].binding = 0;
            local_input_attr_desc[0].location = 0;
            local_input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            local_input_attr_desc[0].offset = offsetof(VERT_UNMAPPED, vert_data);

            local_input_attr_desc[1].binding = 0;
            local_input_attr_desc[1].location = 1;
            local_input_attr_desc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            local_input_attr_desc[1].offset = offsetof(VERT_UNMAPPED, color_data);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D:
            local_input_attr_desc[0].binding = 0;
            local_input_attr_desc[0].location = 0;
            local_input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            local_input_attr_desc[0].offset = offsetof(VERT_MAPPED, vert_data);

            local_input_attr_desc[1].binding = 0;
            local_input_attr_desc[1].location = 1;
            local_input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            local_input_attr_desc[1].offset = offsetof(VERT_MAPPED, tex_data);
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED_2D:
            local_input_attr_desc[0].binding = 0;
            local_input_attr_desc[0].location = 0;
            local_input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
            local_input_attr_desc[0].offset = offsetof(VERT_UNMAPPED_2D, vert_data);

            local_input_attr_desc[1].binding = 0;
            local_input_attr_desc[1].location = 1;
            local_input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            local_input_attr_desc[1].offset = offsetof(VERT_UNMAPPED_2D, color_data);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D:
            local_input_attr_desc[0].binding = 0;
            local_input_attr_desc[0].location = 0;
            local_input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
            local_input_attr_desc[0].offset = offsetof(VERT_MAPPED_2D, vert_data);

            local_input_attr_desc[1].binding = 0;
            local_input_attr_desc[1].location = 1;
            local_input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            local_input_attr_desc[1].offset = offsetof(VERT_MAPPED_2D, tex_data);
            break;

        default:
            break;
        }

        return local_input_attr_desc;
    }

    /* Create graphics pipeline createinfo */
    VkGraphicsPipelineCreateInfo PipelineCreator::getGraphicsPipelineInfo(const std::string &vert_shader, const std::string &frag_shader, const char *p_shader_module_name,
    const VkPolygonMode &polygon_mode, const VkCullModeFlagBits &cull_mode, const VkFrontFace &front_face, const VkPrimitiveTopology &primitive_topology, const bool &add_depth_stencil, const bool &add_color_blend, const uint32_t &subpass_index) {
        FILE *file;
        long file_size;

        /* Get vertex shader binary data */
        std::vector<char> vert_shader_binary_vector;        
        file = fopen(vert_shader.c_str(), "rb");
        
        if(!file) ERR("Incorrect vertex shader '" + vert_shader + "'!");
        
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        vert_shader_binary_vector.resize(file_size);
        fread(vert_shader_binary_vector.data(), sizeof(char), file_size, file);
        fclose(file);
        
        if(!file) ERR("Incorrect fragment shader '" + frag_shader + "'!");

        /* Get frag shader binary data */
        std::vector<char> frag_shader_binary_vector;
        file = fopen(frag_shader.c_str(), "rb");
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        frag_shader_binary_vector.resize(file_size);
        fread(frag_shader_binary_vector.data(), sizeof(char), file_size, file);

        /* Call shader module handler */
        m_shader_modules[0] = getShaderModule(vert_shader_binary_vector);
        m_shader_modules[1] = getShaderModule(frag_shader_binary_vector);

        /* Create vertex shader stage createinfo */
        VkPipelineShaderStageCreateInfo local_vertex_shader_stage_createinfo{};
        local_vertex_shader_stage_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        local_vertex_shader_stage_createinfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        local_vertex_shader_stage_createinfo.module = m_shader_modules[0];
        local_vertex_shader_stage_createinfo.pName = p_shader_module_name;

        /* Create fragment shader stage createinfo */
        VkPipelineShaderStageCreateInfo local_frag_shader_stage_createinfo{};
        local_frag_shader_stage_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        local_frag_shader_stage_createinfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        local_frag_shader_stage_createinfo.module = m_shader_modules[1];
        local_frag_shader_stage_createinfo.pName = p_shader_module_name;
        m_shader_stage_createinfos = {local_vertex_shader_stage_createinfo, local_frag_shader_stage_createinfo};

        /* Get descriptions */
        m_input_binding_descriptor = getBindingDesc();
        m_input_attribute_descriptors = getAttributeDesc();

        /* Set up vertex input createinfo object */
        m_vertex_input_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_vertex_input_createinfo.vertexBindingDescriptionCount = 1;
        m_vertex_input_createinfo.vertexAttributeDescriptionCount = (uint32_t) (m_input_attribute_descriptors.size());
        m_vertex_input_createinfo.pVertexBindingDescriptions = &m_input_binding_descriptor;
        m_vertex_input_createinfo.pVertexAttributeDescriptions = m_input_attribute_descriptors.data();

        /* Set up input assembly createinfo object */
        m_input_assembly_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_input_assembly_createinfo.topology = primitive_topology;
        m_input_assembly_createinfo.primitiveRestartEnable = VK_FALSE;

        /* Set viewport values */
        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.width = static_cast<float>(m_p_extent->width);
        m_viewport.height = static_cast<float>(m_p_extent->height);
        m_viewport.minDepth = 0.0f;
        m_viewport.maxDepth = 1.0f;

        /* Set scissor values */
        m_scissor.offset = {0, 0};
        m_scissor.extent = *m_p_extent;

        /* Set up viewport state createinfo object */
        m_viewport_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        m_viewport_state_createinfo.viewportCount = 1;
        m_viewport_state_createinfo.pViewports = &m_viewport;
        m_viewport_state_createinfo.scissorCount = 1;
        m_viewport_state_createinfo.pScissors = &m_scissor;

        /* Set up rasterization create info */
        m_rasterization_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        m_rasterization_createinfo.depthClampEnable = VK_FALSE;
        m_rasterization_createinfo.rasterizerDiscardEnable = VK_FALSE;
        m_rasterization_createinfo.polygonMode = polygon_mode;
        m_rasterization_createinfo.lineWidth = 1.0f;
        m_rasterization_createinfo.cullMode = cull_mode;

        if(cull_mode != VK_CULL_MODE_NONE) {
            m_rasterization_createinfo.frontFace = front_face;
            m_rasterization_createinfo.depthBiasEnable = VK_TRUE;
        }


        /* Set up multisampling createinfo */
        m_multisample_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_multisample_createinfo.sampleShadingEnable = VK_FALSE;
        m_multisample_createinfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        /* Set colorblend options */
        m_colorblend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        if(add_color_blend) m_colorblend_attachment.blendEnable = VK_TRUE;
        else m_colorblend_attachment.blendEnable = VK_FALSE;
        m_colorblend_attachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;
        m_colorblend_attachment.blendEnable = VK_TRUE;
        m_colorblend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        m_colorblend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        m_colorblend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
        m_colorblend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        m_colorblend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        m_colorblend_attachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;
        
        /* Set depth stencil */
        m_depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        if(add_depth_stencil) {
            m_depth_stencil.depthTestEnable = VK_TRUE;
            m_depth_stencil.depthWriteEnable = VK_TRUE;
            m_depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
            m_depth_stencil.depthBoundsTestEnable = VK_FALSE;
            m_depth_stencil.stencilTestEnable = VK_FALSE;
        }
        else {
            m_depth_stencil.depthTestEnable = VK_FALSE;
            m_depth_stencil.depthWriteEnable = VK_FALSE;
            m_depth_stencil.depthBoundsTestEnable = VK_FALSE;
            m_depth_stencil.stencilTestEnable = VK_FALSE;
        }
        
        /* Set up colorblend state createinfo */
        m_colorblend_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_colorblend_state_createinfo.logicOpEnable = VK_FALSE;
        m_colorblend_state_createinfo.attachmentCount = 1;
        m_colorblend_state_createinfo.pAttachments = &m_colorblend_attachment;

        /* Set up graphics pipeline createinfo */
        VkGraphicsPipelineCreateInfo local_graphics_pipeline_createinfo{};
        local_graphics_pipeline_createinfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        local_graphics_pipeline_createinfo.stageCount = (uint32_t) m_shader_stage_createinfos.size();
        local_graphics_pipeline_createinfo.pStages = m_shader_stage_createinfos.data();
        local_graphics_pipeline_createinfo.pVertexInputState = &m_vertex_input_createinfo;
        local_graphics_pipeline_createinfo.pInputAssemblyState = &m_input_assembly_createinfo;
        local_graphics_pipeline_createinfo.pViewportState = &m_viewport_state_createinfo;
        local_graphics_pipeline_createinfo.pColorBlendState = &m_colorblend_state_createinfo;
        local_graphics_pipeline_createinfo.pRasterizationState = &m_rasterization_createinfo;
        local_graphics_pipeline_createinfo.pMultisampleState = &m_multisample_createinfo;
        local_graphics_pipeline_createinfo.pDepthStencilState = &m_depth_stencil;
        local_graphics_pipeline_createinfo.layout = *m_p_pipeline_data->p_pipeline_layout;

        local_graphics_pipeline_createinfo.renderPass = *m_p_renderpass;
        local_graphics_pipeline_createinfo.subpass = subpass_index;
        local_graphics_pipeline_createinfo.basePipelineHandle = VK_NULL_HANDLE;

        return local_graphics_pipeline_createinfo;
    }


    /* Generic cleanup */
    PipelineCreator::~PipelineCreator() {
        vkDestroyShaderModule(*m_p_device, m_shader_modules[0], nullptr);
        vkDestroyShaderModule(*m_p_device, m_shader_modules[1], nullptr);
    }


    SwapChainDetails::SwapChainDetails(VkPhysicalDevice &gpu, VkSurfaceKHR &surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &m_capabilities);
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);

        if(!format_count) ERR("No surface formats available!");

        m_formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, m_formats.data());
        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, nullptr);

        if(!present_mode_count) ERR("No surface present modes available!");

        m_present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, m_present_modes.data());
    }

    VkSurfaceCapabilitiesKHR SwapChainDetails::getCapabilities() {
        return m_capabilities;
    }

    std::vector<VkSurfaceFormatKHR> SwapChainDetails::getFormats() {
        return m_formats;
    }

    std::vector<VkPresentModeKHR> SwapChainDetails::getPresentModes() {
        return m_present_modes;
    }


    /* Generic memory allocation method */
    void BufferCreator::allocateMemory(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceMemory *p_memory, const VkDeviceSize &size, uint32_t mem_type_bits, VkMemoryPropertyFlags properties) {
        VkMemoryAllocateInfo local_allocinfo{};
        local_allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_allocinfo.allocationSize = size;
        local_allocinfo.memoryTypeIndex = HardwareSpecs::getMemoryType(p_gpu, mem_type_bits, properties);

        if(vkAllocateMemory(*p_device, &local_allocinfo, nullptr, p_memory) != VK_SUCCESS)
            ERR("Failed to allocate buffer memory!");
    }

    /* VkImage related functions */
    VkMemoryRequirements BufferCreator::makeImage(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkImage *p_image, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage) {
        // Set up image createinfo
        VkImageCreateInfo local_image_createInfo{};
        local_image_createInfo.sType  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        local_image_createInfo.imageType = VK_IMAGE_TYPE_2D;
        local_image_createInfo.extent.width = width;
        local_image_createInfo.extent.height = height;
        local_image_createInfo.extent.depth = 1;
        local_image_createInfo.mipLevels = 1;
        local_image_createInfo.arrayLayers = 1;
        local_image_createInfo.format = format;
        local_image_createInfo.tiling = tiling;
        local_image_createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        local_image_createInfo.usage = usage;
        local_image_createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        local_image_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Create image
        if(vkCreateImage(*p_device, &local_image_createInfo, nullptr, p_image) != VK_SUCCESS) 
            ERR("Failed to create image!");

        // Get memory requirements for this image
        VkMemoryRequirements local_memory_requirement;
        vkGetImageMemoryRequirements(*p_device, *p_image, &local_memory_requirement);
        
        return local_memory_requirement;
    }


    /* Returns image filled view createinfo struct */
    VkImageViewCreateInfo BufferCreator::getImageViewInfo(VkImage &image, const VkFormat &format, const VkImageAspectFlags &aspect_flags) {
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
    

    /* Transition image layout into new layout */
    void BufferCreator::transitionImageLayout(VkDevice *p_device, VkImage *p_image, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout) {
        // Begin recording commandbuffer
        VkCommandBuffer tmp_commandbuffer;
        CommandBufferRecorder::beginCommandBufferSingleCommand(p_device, p_commandpool, tmp_commandbuffer);

        // Set up image memory barrier struct
        VkImageMemoryBarrier local_memory_barrier{};
        local_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        local_memory_barrier.oldLayout = old_layout;
        local_memory_barrier.newLayout = new_layout;
        local_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        local_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        local_memory_barrier.image = *p_image;
        local_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        local_memory_barrier.subresourceRange.baseMipLevel = 0;
        local_memory_barrier.subresourceRange.levelCount = 1;
        local_memory_barrier.subresourceRange.baseArrayLayer = 0;
        local_memory_barrier.subresourceRange.layerCount = 1;

        // Set up pipeline stage flags
        VkPipelineStageFlags local_src_stage;
        VkPipelineStageFlags local_dst_stage;
                
        if(old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            local_memory_barrier.srcAccessMask = 0;
            local_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            local_src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            local_dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        else if(old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            local_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            local_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            local_src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            local_dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        else ERR("Invalid layout transitions!");
    
        // Change image layout
        vkCmdPipelineBarrier(tmp_commandbuffer, local_src_stage, local_dst_stage, 0, 0, nullptr, 0, nullptr, 1, &local_memory_barrier);

        CommandBufferRecorder::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, tmp_commandbuffer);
    }


    /* Copy buffer to image */
    void BufferCreator::copyBufferToImage(VkDevice *p_device, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkBuffer *p_src_buffer, VkImage *p_dst_image, const uint32_t &width, const uint32_t &height) {
        // Begin recording commandbuffer
        VkCommandBuffer tmp_commandbuffer;
        CommandBufferRecorder::beginCommandBufferSingleCommand(p_device, p_commandpool, tmp_commandbuffer);

        // Set up buffer image copy struct
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
        
        // Call Vulkan buffer to image copy handler
        vkCmdCopyBufferToImage(tmp_commandbuffer, *p_src_buffer, *p_dst_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &local_copy_region);

        // End recording commandbuffer
        CommandBufferRecorder::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, tmp_commandbuffer);
    }
    

    /* VkBuffer related functions */
    /* Create new buffer object */
    VkMemoryRequirements BufferCreator::makeBuffer(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceSize *p_size, const VkBufferUsageFlags &usage, VkBuffer *p_buffer) {
        // Set up buffer createinfo struct 
        VkBufferCreateInfo local_buffer_createInfo{};
        local_buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_buffer_createInfo.size = *p_size;
        local_buffer_createInfo.usage = usage;
        local_buffer_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Call Vulkan buffer creation handler 
        if(vkCreateBuffer(*p_device, &local_buffer_createInfo, nullptr, p_buffer) != VK_SUCCESS)
            ERR("Failed to create a buffer!");

        // Get memory requirements for the buffer
        VkMemoryRequirements local_memory_requirement;
        vkGetBufferMemoryRequirements(*p_device, *p_buffer, &local_memory_requirement);

        return local_memory_requirement;
    }
    

    /* Populate allocated buffer memory */
    void BufferCreator::populateBufferMem(VkDevice *p_device, VkDeviceSize *p_size, const void *p_src_data, VkDeviceMemory *p_buffer_memory, VkDeviceSize offset) {
        LOG("Populating buffer memory!");
        void *local_data;
        vkMapMemory(*p_device, *p_buffer_memory, offset, *p_size, 0, &local_data);
            memcpy(local_data, p_src_data, static_cast<size_t>(*p_size));
        vkUnmapMemory(*p_device, *p_buffer_memory);
    }
    

    /* Copy buffer to buffer */
    void BufferCreator::copyBufferToBuffer(VkDevice *p_device, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkBuffer *p_src_buffer, VkBuffer *p_dst_buffer, VkDeviceSize *p_size, const VkDeviceSize &offset) {
        // Begin recording commandbuffer
        VkCommandBuffer local_commandbuffer;
        CommandBufferRecorder::beginCommandBufferSingleCommand(p_device, p_commandpool, local_commandbuffer);

        // Set up copy region 
        VkBufferCopy local_copy_region{};
        local_copy_region.srcOffset = 0;
        local_copy_region.dstOffset = offset;
        local_copy_region.size = *p_size;
        
        // Call Vulkan buffer copy handler
        vkCmdCopyBuffer(local_commandbuffer, *p_src_buffer, *p_dst_buffer, 1, &local_copy_region);
        // End recording commandbuffer
        CommandBufferRecorder::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, local_commandbuffer);
    }


    /* Single commandbuffer command recorder methods */
    void CommandBufferRecorder::beginCommandBufferSingleCommand(VkDevice *p_device, VkCommandPool *p_commandpool, VkCommandBuffer &commandbuffer) {
        // Set up commandbuffer allocation info
        VkCommandBufferAllocateInfo local_commandbuffer_allocinfo{};
        local_commandbuffer_allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        local_commandbuffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        local_commandbuffer_allocinfo.commandPool = *p_commandpool;
        local_commandbuffer_allocinfo.commandBufferCount = 1;

        // Allocate commandbuffers
        vkAllocateCommandBuffers(*p_device, &local_commandbuffer_allocinfo, &commandbuffer);
        
        // Set up command buffer begin info
        VkCommandBufferBeginInfo local_commandBuffer_beginInfo{};
        local_commandBuffer_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        local_commandBuffer_beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        // Begin commandbuffers
        if(vkBeginCommandBuffer(commandbuffer, &local_commandBuffer_beginInfo) != VK_SUCCESS)
            ERR("Failed to start command recording buffer!");
    }
    

    /* End recording commandbuffer */
    void CommandBufferRecorder::endCommandBufferSingleCommand(VkDevice *device, VkQueue *graphics_queue, VkCommandPool *commandpool, VkCommandBuffer &commandBuffer) {
        // Call Vulkan command buffer end handler
        vkEndCommandBuffer(commandBuffer);

        // Set up submit info 
        VkSubmitInfo local_submitInfo{};
        local_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        local_submitInfo.commandBufferCount = 1;
        local_submitInfo.pCommandBuffers = &commandBuffer;
        
        // Submit to the graphics queue
        vkQueueSubmit(*graphics_queue, 1, &local_submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(*graphics_queue);

        // Cleanup
        vkFreeCommandBuffers(*device, *commandpool, 1, &commandBuffer);
    }
}