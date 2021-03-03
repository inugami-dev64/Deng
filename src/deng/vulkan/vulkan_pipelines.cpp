#define __REND_HELPERS_CPP
#include <deng/vulkan/vulkan_pipelines.h>

namespace deng {
    namespace vulkan {
        /**********************************/
        /******** PipelineCreator *********/
        /**********************************/

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


        /*
         * Create new shader module from SPIR-V binaries
         * This method is used to create new shader module from shader binaries 
         * provided as a parameter
         */
        VkShaderModule PipelineCreator::__mkShaderModule(std::vector<char> &shader_bins) {
            VkShaderModuleCreateInfo createinfo{};
            createinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createinfo.codeSize = (deng_ui32_t) shader_bins.size();
            createinfo.pCode = reinterpret_cast<const deng_ui32_t*>(shader_bins.data());
            VkShaderModule shader_module;

            if(vkCreateShaderModule(m_device, &createinfo, nullptr, &shader_module) != VK_SUCCESS)
                VK_PIPELINEC_ERR("failed to create shader module!");

            return shader_module;
        }

        
        /*
         * Get binding description info in VkVertexInputBindingDescription instance
         * This method checks for pipeline usage and returns VkVertexInputBindingDescription
         * instance according to usage
         */
        VkVertexInputBindingDescription PipelineCreator::__getBindingDesc() {
            VkVertexInputBindingDescription input_binding_desc;
            input_binding_desc.binding = 0;
            input_binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            switch (m_p_pipeline_data->pipeline_type)
            {
            case DENG_PIPELINE_TYPE_UNMAPPED_3D:
                input_binding_desc.stride = sizeof(VERT_UNMAPPED_UNOR);
                break;

            case DENG_PIPELINE_TYPE_UNMAPPED_3D_NORM:
                input_binding_desc.stride = sizeof(VERT_UNMAPPED_NOR);
                break;

            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D:
                input_binding_desc.stride = sizeof(VERT_MAPPED_UNOR);
                break;

            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_3D_NORM:
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


        /*
         * Get attribute description info in VkVertexInputAttributeDescription instances
         * This method is used to recieve all input descriptions required by shaders
         */
        std::vector<VkVertexInputAttributeDescription> PipelineCreator::__getAttributeDescs() {
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
                input_attr_desc.resize(3);
                input_attr_desc[0].binding = 0;
                input_attr_desc[0].location = 0;
                input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
                input_attr_desc[0].offset = offsetof(VERT_UNMAPPED_2D, vert_data);

                input_attr_desc[1].binding = 0;
                input_attr_desc[1].location = 1;
                input_attr_desc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
                input_attr_desc[1].offset = offsetof(VERT_UNMAPPED_2D, color_data);

                input_attr_desc[2].binding = 0;
                input_attr_desc[2].location = 2;
                input_attr_desc[2].format = VK_FORMAT_R32_UINT;
                input_attr_desc[2].offset = offsetof(VERT_UNMAPPED_2D, z_hier_level);
                break;

            case DENG_PIPELINE_TYPE_TEXTURE_MAPPED_2D:
                input_attr_desc.resize(3);
                input_attr_desc[0].binding = 0;
                input_attr_desc[0].location = 0;
                input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
                input_attr_desc[0].offset = offsetof(VERT_MAPPED_2D, vert_data);

                input_attr_desc[1].binding = 0;
                input_attr_desc[1].location = 1;
                input_attr_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
                input_attr_desc[1].offset = offsetof(VERT_MAPPED_2D, tex_data);

                input_attr_desc[2].binding = 0;
                input_attr_desc[2].location = 2;
                input_attr_desc[2].format = VK_FORMAT_R32_UINT;
                input_attr_desc[2].offset = offsetof(VERT_MAPPED_2D, z_hier_level);
                break;

            default:
                break;
            }

            return input_attr_desc;
        }

        /* 
         * Make createinfo instance for graphics pipeline
         * This method is used to set up VkGraphicsPipelineCreateInfo, while defaulting
         * some options that are not needed to be customised by different DENG pipelines
         */
        VkGraphicsPipelineCreateInfo PipelineCreator::mkGraphicsPipelineInfo (
            std::string vert_shader, 
            std::string frag_shader, 
            char *shader_module_name,
            VkPolygonMode polygon_mode, 
            VkCullModeFlagBits cull_mode, 
            VkFrontFace front_face, 
            VkPrimitiveTopology primitive_topology, 
            deng_bool_t add_depth_stencil, 
            deng_bool_t add_color_blend, 
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
            m_shader_modules[0] = __mkShaderModule(vert_shader_binary_vector);
            m_shader_modules[1] = __mkShaderModule(frag_shader_binary_vector);

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
            m_input_binding_desc = __getBindingDesc();
            m_input_attr_descs = __getAttributeDescs();

            /* Set up vertex input createinfo object */ m_vert_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
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
    }
}
