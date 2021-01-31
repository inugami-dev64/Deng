#include "../../headers/deng/api_core.h"
#include <string>
#include <vulkan/vulkan_core.h>

namespace deng {

    /* Check for Vulkan extension support by hardware */
    bool HardwareSpecs::getExtensionSupport (
        const VkPhysicalDevice &gpu, 
        const char *p_extenstion_name
    ) {
        // Count total amount of supported extensions  
        LOG("Finding support for extension " + std::string(p_extenstion_name));
        deng_ui32_t extension_count;
        vkEnumerateDeviceExtensionProperties (
            gpu, 
            nullptr, 
            &extension_count, 
            nullptr
        );
        
        // Get extensions by names
        std::vector<VkExtensionProperties> all_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties (
            gpu, 
            nullptr, 
            &extension_count, 
            all_extensions.data()
        );
        
        // Iterate through all extension to find matching extension
        for(VkExtensionProperties &extension : all_extensions) {
            const char *current_extenstion_name = extension.extensionName;
            LOG("Supported: " + std::string(current_extenstion_name));
            
            if(std::string(current_extenstion_name) == std::string(p_extenstion_name)) {
                LOG("Detected support for " + std::string(current_extenstion_name));
                return true;
            }
        }

        return false;
    }


    /* Find appropriate memory type */
    deng_ui32_t HardwareSpecs::getMemoryType (
        VkPhysicalDevice *p_gpu, 
        const deng_ui32_t &type_filter, 
        const VkMemoryPropertyFlags &properties
    ) {
        // Get all device memory properties
        VkPhysicalDeviceMemoryProperties memory_properties;
        
        vkGetPhysicalDeviceMemoryProperties (
            *p_gpu, 
            &memory_properties
        );

        for(deng_ui32_t index = 0; index < memory_properties.memoryTypeCount; index++) {
            if(type_filter & (1 << index) && (memory_properties.memoryTypes[index].propertyFlags & properties))
                return index;
        }

        VK_BUFFER_ERR("failed to find suitable memory type");
    }


    /* Get a score for GPUs based on the properties and features */
    deng_ui32_t HardwareSpecs::getDeviceScore (
        VkPhysicalDevice *p_gpu, 
        std::vector<const char*> &required_extenstions
    ) {
        deng_ui32_t score = 0;
        VkPhysicalDeviceFeatures device_features;
        VkPhysicalDeviceProperties device_properties;

        vkGetPhysicalDeviceProperties (
            *p_gpu, 
            &device_properties
        );
        
        vkGetPhysicalDeviceFeatures (
            *p_gpu, 
            &device_features
        );

        if(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
        
        if(device_features.fillModeNonSolid) score += 500;

        score += device_properties.limits.maxImageDimension2D;
        score += device_properties.limits.maxImageDimension3D;
        score += device_properties.limits.maxMemoryAllocationCount;
        score += device_properties.limits.maxVertexOutputComponents;

        if(!device_features.geometryShader) return 0;
        if(!device_features.samplerAnisotropy) return 0;

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
    bool QueueFamilyFinder::findGraphicsFamily(VkPhysicalDevice gpu) {
        deng_ui32_t index;
        // Get the total count of queue families
        deng_ui32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties (
            gpu, 
            &family_count, 
            nullptr
        );
        
        // Get properties of all queue families
        std::vector<VkQueueFamilyProperties> queue_family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties (
            gpu, 
            &family_count, 
            queue_family_properties.data()
        );

        // Check which family is graphics family
        for(index = 0; index < family_count; index++) {
            if(queue_family_properties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphics_family_index = index;
                return true;
            }
        }

        return false;
    }


    /* Find correct present queue family */
    bool QueueFamilyFinder::findPresentSupportFamily (
        VkPhysicalDevice gpu, 
        VkSurfaceKHR surface
    ) {
        deng_ui32_t index;
        // Get the total count of queue families
        deng_ui32_t family_count = 0;
        VkBool32 support = false;
        vkGetPhysicalDeviceQueueFamilyProperties (
            gpu, 
            &family_count, 
            nullptr
        );
        
        // Get properties of all queue families
        std::vector<VkQueueFamilyProperties> family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties (
            gpu, 
            &family_count, 
            family_properties.data()
        );

        for(index = 0; index < family_count; index++) {
            vkGetPhysicalDeviceSurfaceSupportKHR (
                gpu, 
                index, 
                surface, 
                &support
            );

            if(support && m_graphics_family_index != index) {
                m_present_family_index = index; 
                return true;
            }
        }
        return false;
    }


    /* Graphics queuefamily getter method */
    deng_ui32_t QueueFamilyFinder::getGraphicsQFIndex() { return m_graphics_family_index; }
    /* Present queue family getter method */
    deng_ui32_t QueueFamilyFinder::getPresentQFIndex() { return m_present_family_index; }


    /* Initialize private variables */
    PipelineCreator::PipelineCreator (
        PipelineData *p_pipeline_data, 
        VkDevice device, 
        VkExtent2D extent, 
        VkRenderPass renderpass
    ) {
        m_p_pipeline_data = p_pipeline_data;
        m_device = device;
        m_extent = extent;
        m_renderpass = renderpass;
    }


    /* Create shader module and return it */
    VkShaderModule PipelineCreator::getShaderModule(std::vector<char> &shader_bins) {
        VkShaderModuleCreateInfo createinfo{};
        createinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createinfo.codeSize = (deng_ui32_t) shader_bins.size();
        createinfo.pCode = reinterpret_cast<const deng_ui32_t*>(shader_bins.data());
        VkShaderModule shader_module;

        if(vkCreateShaderModule(m_device, &createinfo, nullptr, &shader_module) != VK_SUCCESS)
            VK_PIPELINEC_ERR("failed to create shader module!");

        return shader_module;
    }

    /* Get vertex input binding description info*/ 
    VkVertexInputBindingDescription PipelineCreator::getBindingDesc() {
        VkVertexInputBindingDescription input_binding_desc;
        input_binding_desc.binding = 0;
        input_binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        switch (m_p_pipeline_data->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_UNMAPPED_3D:
            LOG("Pipeline type unmapped unnorm");
            input_binding_desc.stride = sizeof(VERT_UNMAPPED_UNOR);
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED_3D_NORM:
            LOG("Pipeline type unmapped unnorm");
            input_binding_desc.stride = sizeof(VERT_UNMAPPED_NOR);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D:
            LOG("Pipeline type tex mapped unnorm");
            input_binding_desc.stride = sizeof(VERT_MAPPED_UNOR);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D_NORM:
            LOG("Pipeline type tex mapped norm");
            input_binding_desc.stride = sizeof(VERT_MAPPED_NOR);
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED_2D:
            input_binding_desc.stride = sizeof(VERT_UNMAPPED_2D);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D:
            input_binding_desc.stride = sizeof(VERT_MAPPED_2D);
            break;
            
        default:
            break;
        }

        return input_binding_desc;
    } 

    /* Get vertex input attribute description info */
    std::vector<VkVertexInputAttributeDescription> PipelineCreator::getAttributeDescs() {
        std::vector<VkVertexInputAttributeDescription> input_attr_desc{};

        switch (m_p_pipeline_data->pipeline_type)
        {
        case DENG_PIPELINE_TYPE_UNMAPPED_3D:
            input_attr_desc.resize(2);
            input_attr_desc[0].binding = 0;
            input_attr_desc[0].location = 0;
            input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            input_attr_desc[0].offset = offsetof(VERT_UNMAPPED_UNOR, vert_data);

            input_attr_desc[1].binding = 0;
            input_attr_desc[1].location = 1;
            input_attr_desc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            input_attr_desc[1].offset = offsetof(VERT_UNMAPPED_UNOR, color_data);
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED_3D_NORM:
            input_attr_desc.resize(3);
            input_attr_desc[0].binding = 0;
            input_attr_desc[0].location = 0;
            input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            input_attr_desc[0].offset = offsetof(VERT_UNMAPPED_NOR, vert_data);

            input_attr_desc[1].binding = 0;
            input_attr_desc[1].location = 1;
            input_attr_desc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            input_attr_desc[1].offset = offsetof(VERT_UNMAPPED_NOR, color_data);

            input_attr_desc[2].binding = 0;
            input_attr_desc[2].location = 2;
            input_attr_desc[2].format = VK_FORMAT_R32G32B32_SFLOAT;
            input_attr_desc[2].offset = offsetof(VERT_UNMAPPED_NOR, norm_data);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D:
            input_attr_desc.resize(2);
            input_attr_desc[0].binding = 0;
            input_attr_desc[0].location = 0;
            input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            input_attr_desc[0].offset = offsetof(VERT_MAPPED_UNOR, vert_data);

            input_attr_desc[1].binding = 0;
            input_attr_desc[1].location = 1;
            input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            input_attr_desc[1].offset = offsetof(VERT_MAPPED_UNOR, tex_data);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D_NORM:
            input_attr_desc.resize(3);
            input_attr_desc[0].binding = 0;
            input_attr_desc[0].location = 0;
            input_attr_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            input_attr_desc[0].offset = offsetof(VERT_MAPPED_NOR, vert_data);

            input_attr_desc[1].binding = 0;
            input_attr_desc[1].location = 1;
            input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            input_attr_desc[1].offset = offsetof(VERT_MAPPED_NOR, tex_data);

            input_attr_desc[2].binding = 0;
            input_attr_desc[2].location = 2;
            input_attr_desc[2].format = VK_FORMAT_R32G32B32_SFLOAT;
            input_attr_desc[2].offset = offsetof(VERT_MAPPED_NOR, norm_data);
            break;

        case DENG_PIPELINE_TYPE_UNMAPPED_2D:
            input_attr_desc.resize(2);
            input_attr_desc[0].binding = 0;
            input_attr_desc[0].location = 0;
            input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
            input_attr_desc[0].offset = offsetof(VERT_UNMAPPED_2D, vert_data);

            input_attr_desc[1].binding = 0;
            input_attr_desc[1].location = 1;
            input_attr_desc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            input_attr_desc[1].offset = offsetof(VERT_UNMAPPED_2D, color_data);
            break;

        case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D:
            input_attr_desc.resize(2);
            input_attr_desc[0].binding = 0;
            input_attr_desc[0].location = 0;
            input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
            input_attr_desc[0].offset = offsetof(VERT_MAPPED_2D, vert_data);

            input_attr_desc[1].binding = 0;
            input_attr_desc[1].location = 1;
            input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
            input_attr_desc[1].offset = offsetof(VERT_MAPPED_2D, tex_data);
            break;

        default:
            break;
        }

        return input_attr_desc;
    }

    /* Create graphics pipeline createinfo */
    VkGraphicsPipelineCreateInfo PipelineCreator::getGraphicsPipelineInfo (
        std::string vert_shader, 
        std::string frag_shader, 
        char *shader_module_name,
        VkPolygonMode polygon_mode, 
        VkCullModeFlagBits cull_mode, 
        VkFrontFace front_face, 
        VkPrimitiveTopology primitive_topology, 
        bool add_depth_stencil, 
        bool add_color_blend, 
        VkSampleCountFlagBits sample_c,
        deng_ui32_t subpass_index
    ) {
        FILE *file;
        size_t res = 0;
        long file_size;

        /* Get vertex shader binary data */
        std::vector<char> vert_shader_binary_vector;        
        file = fopen(vert_shader.c_str(), "rb");
        
        if(!file) 
            VK_PIPELINEC_ERR("incorrect vertex shader '" + vert_shader + "'!");
        
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        vert_shader_binary_vector.resize(file_size);
        res = fread (
            vert_shader_binary_vector.data(), 
            sizeof(char), 
            file_size, 
            file
        );

        if(res != (size_t) file_size)
            MEM_ERR("Failed to read " + std::to_string(file_size) + "members");

        fclose(file);

        /* Get frag shader binary data */
        std::vector<char> frag_shader_binary_vector;
        file = fopen(frag_shader.c_str(), "rb");
        
        if(!file) 
            VK_PIPELINEC_ERR("incorrect fragment shader '" + frag_shader + "'!");

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        frag_shader_binary_vector.resize(file_size);
        res = fread (
            frag_shader_binary_vector.data(), 
            sizeof(char), 
            file_size, 
            file
        );

        if(res != (size_t) file_size)
            MEM_ERR("Failed to read " + std::to_string(file_size) + " members");

        /* Call shader module handler */
        m_shader_modules[0] = getShaderModule(vert_shader_binary_vector);
        m_shader_modules[1] = getShaderModule(frag_shader_binary_vector);

        /* Create vertex shader stage createinfo */
        VkPipelineShaderStageCreateInfo vertex_shader_stage_createinfo{};
        vertex_shader_stage_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_shader_stage_createinfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_shader_stage_createinfo.module = m_shader_modules[0];
        vertex_shader_stage_createinfo.pName = shader_module_name;

        /* Create fragment shader stage createinfo */
        VkPipelineShaderStageCreateInfo frag_shader_stage_createinfo{};
        frag_shader_stage_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_shader_stage_createinfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_shader_stage_createinfo.module = m_shader_modules[1];
        frag_shader_stage_createinfo.pName = shader_module_name;
        
        m_shader_stage_createinfos = {vertex_shader_stage_createinfo, frag_shader_stage_createinfo};

        /* Get descriptions */
        m_input_binding_desc = getBindingDesc();
        m_input_attr_descs = getAttributeDescs();

        /* Set up vertex input createinfo object */
        m_vert_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_vert_input_create_info.vertexBindingDescriptionCount = 1;
        m_vert_input_create_info.vertexAttributeDescriptionCount = (deng_ui32_t) (m_input_attr_descs.size());
        m_vert_input_create_info.pVertexBindingDescriptions = &m_input_binding_desc;
        m_vert_input_create_info.pVertexAttributeDescriptions = m_input_attr_descs.data();

        /* Set up input assembly createinfo object */
        m_input_asm_createinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_input_asm_createinfo.topology = primitive_topology;
        m_input_asm_createinfo.primitiveRestartEnable = VK_FALSE;

        /* Set viewport values */
        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.width = static_cast<float>(m_extent.width);
        m_viewport.height = static_cast<float>(m_extent.height);
        m_viewport.minDepth = 0.0f;
        m_viewport.maxDepth = 1.0f;

        /* Set scissor values */
        m_scissor.offset = {0, 0};
        m_scissor.extent = m_extent;

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
        m_multisample_createinfo.rasterizationSamples = sample_c;

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
        VkGraphicsPipelineCreateInfo graphics_pipeline_createinfo{};
        graphics_pipeline_createinfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_createinfo.stageCount = (deng_ui32_t) m_shader_stage_createinfos.size();
        graphics_pipeline_createinfo.pStages = m_shader_stage_createinfos.data();
        graphics_pipeline_createinfo.pVertexInputState = &m_vert_input_create_info;
        graphics_pipeline_createinfo.pInputAssemblyState = &m_input_asm_createinfo;
        graphics_pipeline_createinfo.pViewportState = &m_viewport_state_createinfo;
        graphics_pipeline_createinfo.pColorBlendState = &m_colorblend_state_createinfo;
        graphics_pipeline_createinfo.pRasterizationState = &m_rasterization_createinfo;
        graphics_pipeline_createinfo.pMultisampleState = &m_multisample_createinfo;
        graphics_pipeline_createinfo.pDepthStencilState = &m_depth_stencil;
        graphics_pipeline_createinfo.layout = *m_p_pipeline_data->p_pipeline_layout;

        graphics_pipeline_createinfo.renderPass = m_renderpass;
        graphics_pipeline_createinfo.subpass = subpass_index;
        graphics_pipeline_createinfo.basePipelineHandle = VK_NULL_HANDLE;

        fclose(file);
        return graphics_pipeline_createinfo;
    }


    /* Generic cleanup */
    PipelineCreator::~PipelineCreator() {
        vkDestroyShaderModule (
            m_device, 
            m_shader_modules[0],
            nullptr
        );
        
        vkDestroyShaderModule (
            m_device, 
            m_shader_modules[1], 
            nullptr
        );
    }


    SwapChainDetails::SwapChainDetails (
        VkPhysicalDevice &gpu, 
        VkSurfaceKHR &surface
    ) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &m_capabilities);
        deng_ui32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);

        if(!format_count) 
            VK_SWAPCHAIN_ERR("no surface formats available!");

        m_formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR (
            gpu, 
            surface, 
            &format_count, 
            m_formats.data()
        );
        
        deng_ui32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR (
            gpu, 
            surface, 
            &present_mode_count, 
            nullptr
        );

        if(!present_mode_count) 
            VK_SWAPCHAIN_ERR("No surface present modes available!");

        m_present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR (
            gpu, 
            surface, 
            &present_mode_count, 
            m_present_modes.data()
        );
    }

    VkSurfaceCapabilitiesKHR SwapChainDetails::getCapabilities() { return m_capabilities; }
    std::vector<VkSurfaceFormatKHR> SwapChainDetails::getFormats() { return m_formats; }
    std::vector<VkPresentModeKHR> SwapChainDetails::getPresentModes() { return m_present_modes; }

    /****************************************/
    /****************************************/
    /********* BufferCreator struct *********/
    /****************************************/
    /****************************************/

    /* Generic memory allocation method */
    void BufferCreator::allocateMemory (
        VkDevice device, 
        VkPhysicalDevice gpu, 
        VkDeviceSize size,
        VkDeviceMemory *p_memory,  
        deng_ui32_t mem_type_bits, 
        VkMemoryPropertyFlags properties
    ) {
        VkMemoryAllocateInfo allocinfo{};
        allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocinfo.allocationSize = size;
        
        allocinfo.memoryTypeIndex = HardwareSpecs::getMemoryType (
            &gpu, 
            mem_type_bits, 
            properties
        );

        if(vkAllocateMemory(device, &allocinfo, nullptr, p_memory) != VK_SUCCESS)
            VK_BUFFER_ERR("failed to allocate buffer memory!");
    }

    /* VkImage related functions */
    VkMemoryRequirements BufferCreator::makeImage (
        VkDevice device, 
        VkPhysicalDevice gpu, 
        VkImage &image, 
        deng_ui32_t width, 
        deng_ui32_t height, 
        deng_ui32_t mip_levels,
        VkFormat format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage,
        VkSampleCountFlagBits sample_c
    ) {
        // Set up image createinfo
        VkImageCreateInfo image_createInfo{};
        image_createInfo.sType  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_createInfo.imageType = VK_IMAGE_TYPE_2D;
        image_createInfo.extent.width = width;
        image_createInfo.extent.height = height;
        image_createInfo.extent.depth = 1;
        image_createInfo.mipLevels = mip_levels;
        image_createInfo.arrayLayers = 1;
        image_createInfo.format = format;
        image_createInfo.tiling = tiling;
        image_createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_createInfo.usage = usage;
        image_createInfo.samples = sample_c;
        image_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Create image
        if(vkCreateImage(device, &image_createInfo, nullptr, &image) != VK_SUCCESS) 
            VK_BUFFER_ERR("failed to create image!");

        // Get memory requirements for this image
        VkMemoryRequirements memory_requirement;
        vkGetImageMemoryRequirements (
            device, 
            image, 
            &memory_requirement
        );
        
        return memory_requirement;
    }


    /* Returns image filled view createinfo struct */
    VkImageViewCreateInfo BufferCreator::getImageViewInfo (
        VkImage &image, 
        VkFormat format, 
        VkImageAspectFlags aspect_flags,
        deng_ui32_t mip_levels
    ) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;

        createInfo.subresourceRange.aspectMask = aspect_flags;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = mip_levels;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        return createInfo;
    }
    

    /* Transition image layout into new layout */
    void BufferCreator::transitionImageLayout (
        VkDevice device, 
        VkImage &image, 
        VkCommandPool commandpool, 
        VkQueue g_queue, 
        VkFormat format, 
        VkImageLayout old_layout, 
        VkImageLayout new_layout,
        deng_ui32_t mip_levels
    ) {
        // Begin recording commandbuffer
        VkCommandBuffer tmp_commandbuffer;
        CommandBufferRecorder::beginCommandBufferSingleCommand (
            device, 
            commandpool, 
            &tmp_commandbuffer
        );

        // Set up image memory barrier struct
        VkImageMemoryBarrier memory_barrier{};
        memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        memory_barrier.oldLayout = old_layout;
        memory_barrier.newLayout = new_layout;
        memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memory_barrier.image = image;
        memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        memory_barrier.subresourceRange.baseMipLevel = 0;
        memory_barrier.subresourceRange.levelCount = mip_levels;
        memory_barrier.subresourceRange.baseArrayLayer = 0;
        memory_barrier.subresourceRange.layerCount = 1;

        // Set up pipeline stage flags
        VkPipelineStageFlags src_stage;
        VkPipelineStageFlags dst_stage;
                
        if(old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            memory_barrier.srcAccessMask = 0;
            memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        else if(old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        else 
            VK_BUFFER_ERR("invalid image layout transition");
    
        // Change image layout
        vkCmdPipelineBarrier (
            tmp_commandbuffer, 
            src_stage, 
            dst_stage, 
            0, 
            0, 
            nullptr, 
            0, 
            nullptr, 
            1, 
            &memory_barrier
        );

        CommandBufferRecorder::endCommandBufferSingleCommand (
            device, 
            g_queue, 
            commandpool, 
            &tmp_commandbuffer
        );
    }


    /* Copy buffer to image */
    void BufferCreator::copyBufferToImage (
        VkDevice device, 
        VkCommandPool commandpool, 
        VkQueue g_queue, 
        VkBuffer src_buffer, 
        VkImage dst_image, 
        deng_ui32_t width, 
        deng_ui32_t height
    ) {
        // Begin recording commandbuffer
        VkCommandBuffer tmp_commandbuffer;
        CommandBufferRecorder::beginCommandBufferSingleCommand (
            device, 
            commandpool, 
            &tmp_commandbuffer
        );

        // Set up buffer image copy struct
        VkBufferImageCopy copy_region{};
        copy_region.bufferOffset = 0;
        copy_region.bufferRowLength = 0;
        copy_region.bufferImageHeight = 0;
        copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copy_region.imageSubresource.mipLevel = 0;
        copy_region.imageSubresource.baseArrayLayer = 0;
        copy_region.imageSubresource.layerCount = 1;
        copy_region.imageOffset = {0, 0, 0};
        copy_region.imageExtent = {width, height, 1};
        
        // Call Vulkan buffer to image copy handler
        vkCmdCopyBufferToImage (
            tmp_commandbuffer, 
            src_buffer, 
            dst_image, 
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            1, 
            &copy_region
        );

        // End recording commandbuffer
        CommandBufferRecorder::endCommandBufferSingleCommand (
            device, 
            g_queue, 
            commandpool, 
            &tmp_commandbuffer
        );
    }
    

    /* VkBuffer related functions */
    /* Create new buffer object */
    VkMemoryRequirements BufferCreator::makeBuffer (
        VkDevice device, 
        VkPhysicalDevice gpu, 
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkBuffer *p_buffer
    ) {
        // Set up buffer createinfo struct 
        VkBufferCreateInfo buffer_createInfo{};
        buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_createInfo.size = size;
        buffer_createInfo.usage = usage;
        buffer_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Call Vulkan buffer creation handler 
        if(vkCreateBuffer(device, &buffer_createInfo, nullptr, p_buffer) != VK_SUCCESS)
            VK_BUFFER_ERR("Failed to create a buffer!");

        // Get memory requirements for the buffer
        VkMemoryRequirements memory_requirement;
        vkGetBufferMemoryRequirements (
            device, 
            *p_buffer, 
            &memory_requirement
        );

        return memory_requirement;
    }
    

    /* Populate allocated buffer memory */
    void BufferCreator::populateBufferMem (
        VkDevice device, 
        VkDeviceSize size, 
        void *src_data, 
        VkDeviceMemory buffer_memory, 
        VkDeviceSize offset
    ) {
        void *data;
        vkMapMemory (
            device, 
            buffer_memory, 
            offset, 
            size, 
            0, 
            &data
        );
            memcpy (
                data, 
                src_data, 
                size
            );
        vkUnmapMemory (
            device, 
            buffer_memory
        );
    }
    

    /* Copy buffer to buffer */
    void BufferCreator::copyBufferToBuffer (
        VkDevice device, 
        VkCommandPool commandpool, 
        VkQueue g_queue, 
        VkBuffer src_buffer, 
        VkBuffer dst_buffer, 
        VkDeviceSize size, 
        VkDeviceSize offset
    ) {
        // Begin recording commandbuffer
        VkCommandBuffer tmp_commandbuffer;
        CommandBufferRecorder::beginCommandBufferSingleCommand (
            device, 
            commandpool, 
            &tmp_commandbuffer
        );

        // Set up copy region 
        VkBufferCopy copy_region{};
        copy_region.srcOffset = 0;
        copy_region.dstOffset = offset;
        copy_region.size = size;
        
        // Call Vulkan buffer copy handler
        vkCmdCopyBuffer (
            tmp_commandbuffer, 
            src_buffer, 
            dst_buffer, 
            1, 
            &copy_region
        );
        
        // End recording commandbuffer
        CommandBufferRecorder::endCommandBufferSingleCommand (
            device, 
            g_queue, 
            commandpool, 
            &tmp_commandbuffer
        );
    }


    /* Single commandbuffer command recorder methods */
    void CommandBufferRecorder::beginCommandBufferSingleCommand (
        VkDevice device, 
        VkCommandPool commandpool, 
        VkCommandBuffer *p_commandbuffer
    ) {
        // Set up commandbuffer allocation info
        VkCommandBufferAllocateInfo commandbuffer_allocinfo{};
        commandbuffer_allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandbuffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandbuffer_allocinfo.commandPool = commandpool;
        commandbuffer_allocinfo.commandBufferCount = 1;

        // Allocate commandbuffers
        vkAllocateCommandBuffers (
            device, 
            &commandbuffer_allocinfo, 
            p_commandbuffer
        );
        
        // Set up command buffer begin info
        VkCommandBufferBeginInfo commandbuffer_begininfo{};
        commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandbuffer_begininfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        // Begin commandbuffers
        if(vkBeginCommandBuffer(*p_commandbuffer, &commandbuffer_begininfo) != VK_SUCCESS)
            VK_RES_ERR("failed to start command recording buffer!");
    }
    

    /* End recording commandbuffer */
    void CommandBufferRecorder::endCommandBufferSingleCommand (
        VkDevice device, 
        VkQueue g_queue, 
        VkCommandPool commandpool, 
        VkCommandBuffer *p_commandbuffer
    ) {
        // Call Vulkan command buffer end handler
        vkEndCommandBuffer(*p_commandbuffer);

        // Set up submit info 
        VkSubmitInfo submitinfo{};
        submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitinfo.commandBufferCount = 1;
        submitinfo.pCommandBuffers = p_commandbuffer;
        
        // Submit to the graphics queue
        vkQueueSubmit (
            g_queue, 
            1, 
            &submitinfo, 
            VK_NULL_HANDLE
        );
        vkQueueWaitIdle(g_queue);

        // Cleanup
        vkFreeCommandBuffers (
            device, 
            commandpool, 
            1, 
            p_commandbuffer
        );
    }
}
