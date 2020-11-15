#include "deng_core.h"

namespace deng {
    
    dengBool HardwareSpecs::getExtensionSupport(const VkPhysicalDevice &gpu, const char *p_extenstion_name) {
        LOG("Finding support for extension " + std::string(p_extenstion_name));
        uint32_t local_extension_count;
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &local_extension_count, nullptr);
        
        std::vector<VkExtensionProperties> local_all_extensions(local_extension_count);
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &local_extension_count, local_all_extensions.data());
        
        for(VkExtensionProperties &extension : local_all_extensions) {
            const char *local_current_extenstion_name = extension.extensionName;
            LOG("Supported: " + std::string(local_current_extenstion_name));
            if(std::string(local_current_extenstion_name) == std::string(p_extenstion_name)) {
                LOG("Detected support for " + std::string(local_current_extenstion_name));
                return DENG_TRUE;
            }
        }

        return DENG_FALSE;
    }

    uint32_t HardwareSpecs::getMemoryType(VkPhysicalDevice *p_gpu, const uint32_t &type_filter, const VkMemoryPropertyFlags &properties) {
        VkPhysicalDeviceMemoryProperties local_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(*p_gpu, &local_memory_properties);

        for(uint32_t i = 0; i < local_memory_properties.memoryTypeCount; i++) {
            if(type_filter & (1 << i) && (local_memory_properties.memoryTypes[i].propertyFlags & properties)) {
                LOG("Vertex buffer mem type: " + std::to_string(i));
                return i;
            }
        }

        ERR("Failed to find suitable memory type");
    }

    uint32_t HardwareSpecs::getDeviceScore(VkPhysicalDevice *p_gpu, std::vector<const char*> &required_extenstions) {
        uint32_t score = 0;
        VkPhysicalDeviceFeatures local_device_features;
        VkPhysicalDeviceProperties local_device_properties;

        vkGetPhysicalDeviceProperties(*p_gpu, &local_device_properties);
        vkGetPhysicalDeviceFeatures(*p_gpu, &local_device_features);

        if(local_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
        
        if(local_device_features.fillModeNonSolid == true) score += 500;

        score += local_device_properties.limits.maxImageDimension2D;
        score += local_device_properties.limits.maxImageDimension3D;
        score += local_device_properties.limits.maxMemoryAllocationCount;
        score += local_device_properties.limits.maxVertexOutputComponents;

        if(!local_device_features.geometryShader) return 0;
        if(!local_device_features.samplerAnisotropy) return 0;

        for(const char* extenstion_name : required_extenstions) {
            if(HardwareSpecs::getExtensionSupport(*p_gpu, extenstion_name) != DENG_TRUE) {
                LOG("Required extension: " + std::string(extenstion_name) + " is not supported!");
                return 0;
            }
        }

        return score;
    }

    bool QueueFamilies::findGraphicsFamily(VkPhysicalDevice &gpu) {
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, queue_family_properties.data());

        for(uint32_t i = 0; i < family_count; i++) {
            if(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                this->m_graphics_family_index = i;
                LOG("Graphics queue family index: " + std::to_string(this->getGraphicsFamily()));
                return true;
            }
        }

        return false;
    }

    uint32_t QueueFamilies::getGraphicsFamily() {
        return this->m_graphics_family_index;
    }

    bool QueueFamilies::findPresentSupportFamily(VkPhysicalDevice &gpu, VkSurfaceKHR &surface) {
        uint32_t family_count = 0;
        VkBool32 support = false;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, family_properties.data());

        for(uint32_t i = 0; i < family_count; i++) {
            vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &support);
            if(support) {
                if(this->m_graphics_family_index != i) {
                    this->m_present_family_index = i; 
                    LOG("Presentation queue family index: " + std::to_string(this->m_present_family_index));
                    return true;
                }
            }
        }
        return false;
    }

    uint32_t QueueFamilies::getPresentFamily() {
        return this->m_present_family_index;
    }


    PipelineCreator::PipelineCreator(PipelineData *p_pipeline_data, VkDevice *device, dengUtils::FileManager *filemanager, VkExtent2D *extent, VkRenderPass *renderpass) {
        this->m_p_pipeline_data = p_pipeline_data;
        this->m_p_device = device;
        this->m_p_fm = filemanager;
        this->m_p_extent = extent;
        this->m_p_renderpass = renderpass;
    }

    VkShaderModule PipelineCreator::getShaderModule(std::vector<char> &shader_bins) {
        VkShaderModuleCreateInfo local_createinfo{};
        local_createinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        local_createinfo.codeSize = shader_bins.size();
        local_createinfo.pCode = reinterpret_cast<const uint32_t*>(shader_bins.data());
        VkShaderModule local_shader_module;
        if(vkCreateShaderModule(*this->m_p_device, &local_createinfo, nullptr, &local_shader_module) != VK_SUCCESS) {
            ERR("Failed to create shader module!");
        }

        return local_shader_module;
    }

    VkVertexInputBindingDescription PipelineCreator::getBindingDesc() {
        VkVertexInputBindingDescription local_input_binding_desc{};
        local_input_binding_desc.binding = 0;

        switch (this->m_p_pipeline_data->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_UNMAPPED:
            local_input_binding_desc.stride = sizeof(dengUtils::UnmappedVerticesData);
            break;
        
        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
            local_input_binding_desc.stride = sizeof(dengMath::vec3<float>);
            break;
        
        default:
            break;
        }

        return local_input_binding_desc;
    } 

    std::vector<VkVertexInputAttributeDescription> PipelineCreator::getAttributeDesc() {
        std::vector<VkVertexInputAttributeDescription> local_input_attr_desc{};
        local_input_attr_desc.resize(2);

        switch (this->m_p_pipeline_data->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_UNMAPPED:
            local_input_attr_desc[0].binding = 0;
            local_input_attr_desc[0].location = 0;
            local_input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            local_input_attr_desc[0].offset = offsetof(dengUtils::UnmappedVerticesData, position_vec);

            local_input_attr_desc[1].binding = 0;
            local_input_attr_desc[1].location = 1;
            local_input_attr_desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            local_input_attr_desc[1].offset = offsetof(dengUtils::UnmappedVerticesData, color_vec);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED:
            local_input_attr_desc[0].binding = 0;
            local_input_attr_desc[0].location = 0;
            local_input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            local_input_attr_desc[0].offset = offsetof(dengUtils::TextureMappedVerticesData, position_vec);

            local_input_attr_desc[1].binding = 0;
            local_input_attr_desc[1].location = 1;
            local_input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            local_input_attr_desc[1].offset = offsetof(dengUtils::TextureMappedVerticesData, texture_vec);
            break;

        default:
            break;
        }

        return local_input_attr_desc;
    }

    VkGraphicsPipelineCreateInfo PipelineCreator::getGraphicsPipelineInfo(const std::string &vert_shader, const std::string &frag_shader, const char *p_shader_module_name,
    const VkPolygonMode &polygon_mode, const VkCullModeFlagBits &cull_mode, const VkFrontFace &front_face, const VkPrimitiveTopology &primitive_topology, const dengBool &add_depth_stencil, const dengBool &add_color_blend, const uint32_t &subpass_index) {
        
        std::vector<char> vert_shader_binary_vector;
        std::vector<char> frag_shader_binary_vector;
        this->m_p_fm->getFileContents(vert_shader, &vert_shader_binary_vector, nullptr);
        this->m_p_fm->getFileContents(frag_shader, &frag_shader_binary_vector, nullptr);

        this->m_shader_modules[0] = this->getShaderModule(vert_shader_binary_vector);
        this->m_shader_modules[1] = this->getShaderModule(frag_shader_binary_vector);

        VkPipelineShaderStageCreateInfo local_vertex_shader_stage_createinfo{};
        local_vertex_shader_stage_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        local_vertex_shader_stage_createinfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        local_vertex_shader_stage_createinfo.module = this->m_shader_modules[0];
        local_vertex_shader_stage_createinfo.pName = p_shader_module_name;

        VkPipelineShaderStageCreateInfo local_frag_shader_stage_createinfo{};
        local_frag_shader_stage_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        local_frag_shader_stage_createinfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        local_frag_shader_stage_createinfo.module = this->m_shader_modules[1];
        local_frag_shader_stage_createinfo.pName = p_shader_module_name;

        this->m_shader_stage_createinfos = {local_vertex_shader_stage_createinfo, local_frag_shader_stage_createinfo};

        this->m_input_binding_descriptor = this->getBindingDesc();
        this->m_input_attribute_descriptors = this->getAttributeDesc();

        this->m_vertex_input_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        this->m_vertex_input_createinfo.vertexBindingDescriptionCount = 1;
        this->m_vertex_input_createinfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(this->m_input_attribute_descriptors.size());
        this->m_vertex_input_createinfo.pVertexBindingDescriptions = &this->m_input_binding_descriptor;
        this->m_vertex_input_createinfo.pVertexAttributeDescriptions = this->m_input_attribute_descriptors.data();

        this->m_input_assembly_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        this->m_input_assembly_createinfo.topology = primitive_topology;
        this->m_input_assembly_createinfo.primitiveRestartEnable = VK_FALSE;

        this->m_viewport.x = 0.0f;
        this->m_viewport.y = 0.0f;
        this->m_viewport.width = static_cast<float>(this->m_p_extent->width);
        this->m_viewport.height = static_cast<float>(this->m_p_extent->height);
        this->m_viewport.minDepth = 0.0f;
        this->m_viewport.maxDepth = 1.0f;

        this->m_scissor.offset = {0, 0};
        this->m_scissor.extent = *this->m_p_extent;

        this->m_viewport_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        this->m_viewport_state_createinfo.viewportCount = 1;
        this->m_viewport_state_createinfo.pViewports = &this->m_viewport;
        this->m_viewport_state_createinfo.scissorCount = 1;
        this->m_viewport_state_createinfo.pScissors = &this->m_scissor;

        this->m_rasterization_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        this->m_rasterization_createinfo.depthClampEnable = VK_FALSE;
        this->m_rasterization_createinfo.rasterizerDiscardEnable = VK_FALSE;

        this->m_rasterization_createinfo.polygonMode = polygon_mode;
        this->m_rasterization_createinfo.lineWidth = 1.0f;
        this->m_rasterization_createinfo.cullMode = cull_mode;
        if(cull_mode != VK_CULL_MODE_NONE) {
            this->m_rasterization_createinfo.frontFace = front_face;
            this->m_rasterization_createinfo.depthBiasEnable = VK_TRUE;
        }

        this->m_multisample_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        this->m_multisample_createinfo.sampleShadingEnable = VK_FALSE;
        this->m_multisample_createinfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        this->m_colorblend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        if(add_color_blend == DENG_TRUE) this->m_colorblend_attachment.blendEnable =  VK_TRUE;
        else this->m_colorblend_attachment.blendEnable = VK_FALSE;
        
        this->m_depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        if(add_depth_stencil == DENG_TRUE) {
            this->m_depth_stencil.depthTestEnable = VK_TRUE;
            this->m_depth_stencil.depthWriteEnable = VK_TRUE;
            this->m_depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
            this->m_depth_stencil.depthBoundsTestEnable = VK_FALSE;
            this->m_depth_stencil.stencilTestEnable = VK_FALSE;
        }
        else {
            this->m_depth_stencil.depthTestEnable = VK_FALSE;
            this->m_depth_stencil.depthWriteEnable = VK_FALSE;
            this->m_depth_stencil.depthBoundsTestEnable = VK_FALSE;
            this->m_depth_stencil.stencilTestEnable = VK_FALSE;
        }
        
        this->m_colorblend_state_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        this->m_colorblend_state_createinfo.logicOpEnable = VK_FALSE;
        this->m_colorblend_state_createinfo.attachmentCount = 1;
        this->m_colorblend_state_createinfo.pAttachments = &this->m_colorblend_attachment;

        VkGraphicsPipelineCreateInfo local_graphics_pipeline_createinfo{};
        local_graphics_pipeline_createinfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        local_graphics_pipeline_createinfo.stageCount = this->m_shader_stage_createinfos.size();
        local_graphics_pipeline_createinfo.pStages = this->m_shader_stage_createinfos.data();
        local_graphics_pipeline_createinfo.pVertexInputState = &this->m_vertex_input_createinfo;
        local_graphics_pipeline_createinfo.pInputAssemblyState = &this->m_input_assembly_createinfo;
        local_graphics_pipeline_createinfo.pViewportState = &this->m_viewport_state_createinfo;
        local_graphics_pipeline_createinfo.pColorBlendState = &this->m_colorblend_state_createinfo;
        local_graphics_pipeline_createinfo.pRasterizationState = &this->m_rasterization_createinfo;
        local_graphics_pipeline_createinfo.pMultisampleState = &this->m_multisample_createinfo;
        local_graphics_pipeline_createinfo.pDepthStencilState = &this->m_depth_stencil;
        local_graphics_pipeline_createinfo.layout = *this->m_p_pipeline_data->p_pipeline_layout;
        
        local_graphics_pipeline_createinfo.renderPass = *this->m_p_renderpass;
        local_graphics_pipeline_createinfo.subpass = subpass_index;
        local_graphics_pipeline_createinfo.basePipelineHandle = VK_NULL_HANDLE;

        return local_graphics_pipeline_createinfo;
    }

    PipelineCreator::~PipelineCreator() {
        vkDestroyShaderModule(*this->m_p_device, this->m_shader_modules[0], nullptr);
        vkDestroyShaderModule(*this->m_p_device, this->m_shader_modules[1], nullptr);
    }


    SwapChainDetails::SwapChainDetails(VkPhysicalDevice &gpu, VkSurfaceKHR &surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &this->m_capabilities);

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);

        if(format_count != 0) {
            this->m_formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, this->m_formats.data());
        }

        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, nullptr);

        if(present_mode_count != 0) {
            this->m_present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, this->m_present_modes.data());
        }
        else {
            ERR("No present modes available!");
        }
    }

    VkSurfaceCapabilitiesKHR SwapChainDetails::getCapabilities() {
        return this->m_capabilities;
    }

    std::vector<VkSurfaceFormatKHR> SwapChainDetails::getFormats() {
        return this->m_formats;
    }

    std::vector<VkPresentModeKHR> SwapChainDetails::getPresentModes() {
        return this->m_present_modes;
    }


    /* Generic memory allocation method */
    void BufferHandler::allocateMemory(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceMemory *p_memory, const VkDeviceSize &size, uint32_t mem_type_bits, VkMemoryPropertyFlags properties) {
        VkMemoryAllocateInfo local_allocinfo{};
        local_allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_allocinfo.allocationSize = size;
        local_allocinfo.memoryTypeIndex = HardwareSpecs::getMemoryType(p_gpu, mem_type_bits, properties);

        if(vkAllocateMemory(*p_device, &local_allocinfo, nullptr, p_memory) != VK_SUCCESS)
            ERR("Failed to allocate buffer memory!");
    }

    /* VkImage related functions */
    VkMemoryRequirements BufferHandler::makeImage(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkImage *p_image, uint32_t &width, uint32_t &height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize offset) {
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

        if(vkCreateImage(*p_device, &local_image_createInfo, nullptr, p_image) != VK_SUCCESS) 
            ERR("Failed to create image!");

        VkMemoryRequirements local_memory_requirement;
        vkGetImageMemoryRequirements(*p_device, *p_image, &local_memory_requirement);
        
        return local_memory_requirement;
    }

    void BufferHandler::transitionImageLayout(VkDevice *p_device, VkImage *p_image, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout) {
        VkCommandBuffer local_commandbuffer;
        BufferHandler::beginCommandBufferSingleCommand(p_device, p_commandpool, local_commandbuffer);

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
        else {
            ERR("Invalid layout transitions!");
        }

        vkCmdPipelineBarrier(local_commandbuffer, local_src_stage, local_dst_stage, 0, 0, nullptr, 0, nullptr, 1, &local_memory_barrier);

        BufferHandler::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, local_commandbuffer);
    }

    void BufferHandler::copyBufferToImage(VkDevice *p_device, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkBuffer &src_buffer, VkImage &dst_image, const uint32_t &width, const uint32_t &height) {
        VkCommandBuffer local_commandbuffer;
        BufferHandler::beginCommandBufferSingleCommand(p_device, p_commandpool, local_commandbuffer);

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

        BufferHandler::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, local_commandbuffer);
    }
    
    /* VkBuffer related functions */
    VkMemoryRequirements BufferHandler::makeBuffer(VkDevice *p_device, VkPhysicalDevice *p_gpu, VkDeviceSize *p_size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkBuffer *p_buffer, VkDeviceMemory *p_buffer_memory, VkDeviceSize offset) {
        
        VkBufferCreateInfo local_buffer_createInfo{};
        local_buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_buffer_createInfo.size = *p_size;
        local_buffer_createInfo.usage = usage;
        local_buffer_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(*p_device, &local_buffer_createInfo, nullptr, p_buffer) != VK_SUCCESS)
            ERR("Failed to create a buffer!");

        VkMemoryRequirements local_memory_requirement;
        vkGetBufferMemoryRequirements(*p_device, *p_buffer, &local_memory_requirement);

        return local_memory_requirement;
    }

    void BufferHandler::populateBufferMem(VkDevice *p_device, VkDeviceSize *p_size, const void *p_src_data, VkDeviceMemory *p_buffer_memory, VkDeviceSize offset) {
        LOG("Populating buffer memory!");
        void *local_data;
        vkMapMemory(*p_device, *p_buffer_memory, offset, *p_size, 0, &local_data);
            memcpy(local_data, p_src_data, static_cast<size_t>(*p_size));
        vkUnmapMemory(*p_device, *p_buffer_memory);
    }

    void BufferHandler::copyBufferToBuffer(VkDevice *p_device, VkCommandPool *p_commandpool, VkQueue *p_graphics_queue, VkBuffer *p_src_buffer, VkBuffer *p_dst_buffer, VkDeviceSize *p_size, const VkDeviceSize &offset) {
        VkCommandBuffer local_commandbuffer;
        BufferHandler::beginCommandBufferSingleCommand(p_device, p_commandpool, local_commandbuffer);

        VkBufferCopy local_copy_region{};
        local_copy_region.srcOffset = 0;
        local_copy_region.dstOffset = offset;
        local_copy_region.size = *p_size;
        
        vkCmdCopyBuffer(local_commandbuffer, *p_src_buffer, *p_dst_buffer, 1, &local_copy_region);
        BufferHandler::endCommandBufferSingleCommand(p_device, p_graphics_queue, p_commandpool, local_commandbuffer);
    }

    /* single commandbuffer command recorder functions */
    void BufferHandler::beginCommandBufferSingleCommand(VkDevice *device, VkCommandPool *commandpool, VkCommandBuffer &commandBuffer) {
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

    void BufferHandler::endCommandBufferSingleCommand(VkDevice *device, VkQueue *graphics_queue, VkCommandPool *commandpool, VkCommandBuffer &commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo local_submitInfo{};
        local_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        local_submitInfo.commandBufferCount = 1;
        local_submitInfo.pCommandBuffers = &commandBuffer;
        
        vkQueueSubmit(*graphics_queue, 1, &local_submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(*graphics_queue);

        vkFreeCommandBuffers(*device, *commandpool, 1, &commandBuffer);
    }
}