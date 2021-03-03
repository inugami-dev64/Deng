#ifndef __VULKAN_PIPELINES_H
#define __VULKAN_PIPELINES_H

#ifdef __REND_HELPERS_CPP
    #include <vector>
    #include <array>
    #include <vulkan/vulkan.h>

    #include <deng/forward_dec.h>
    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
#endif

namespace deng {
    namespace vulkan {
        /* 
         * Struct for all pipeline createinfo specifier objects 
         * This struct functions as a parent struct for class PipelineCreator
         */
        struct __PipelineCreateinfoSpecifiers {
            std::array<VkPipelineShaderStageCreateInfo, 2> m_shader_stage_createinfos{};
            std::array<VkShaderModule, 2> m_shader_modules{};
            VkVertexInputBindingDescription m_input_binding_desc{};
            std::vector<VkVertexInputAttributeDescription> m_input_attr_descs{};

            VkViewport m_viewport{};
            VkRect2D m_scissor{};

            VkPipelineVertexInputStateCreateInfo m_vert_input_create_info{};
            VkPipelineInputAssemblyStateCreateInfo m_input_asm_createinfo{};
            VkPipelineViewportStateCreateInfo m_viewport_state_createinfo{};
            VkPipelineRasterizationStateCreateInfo m_rasterization_createinfo{};
            VkPipelineMultisampleStateCreateInfo m_multisample_createinfo{};
            VkPipelineColorBlendAttachmentState m_colorblend_attachment{};
            VkPipelineDepthStencilStateCreateInfo m_depth_stencil{};
            VkPipelineColorBlendStateCreateInfo m_colorblend_state_createinfo{};
        };


        /* 
         * Handle pipeline creation 
         * This class is used to simplify the process of creating pipelines
         * in Vulkan
         */
        class PipelineCreator : private __VulkanDeviceInfo, private __PipelineCreateinfoSpecifiers {
        private:
            VkDevice m_device;
            PipelineData *m_p_pipeline_data;
            VkRenderPass m_renderpass;
            VkExtent2D m_extent;

        private:
            /*
             * Create new shader module from SPIR-V binaries
             * This method is used to create new shader module from shader binaries 
             * provided as a parameter
             */
            VkShaderModule __mkShaderModule(std::vector<char> &shader_bins);
            /*
             * Get binding description info in VkVertexInputBindingDescription instance
             * This method checks for pipeline usage and returns VkVertexInputBindingDescription
             * instance according to usage
             */
            VkVertexInputBindingDescription __getBindingDesc();
            /*
             * Get attribute description info in VkVertexInputAttributeDescription instances
             * This method is used to recieve all input descriptions required by shaders
             */
            std::vector<VkVertexInputAttributeDescription> __getAttributeDescs();
            
        public:
            PipelineCreator (
                PipelineData *p_pipeline_data, 
                VkDevice device, 
                VkExtent2D extent, 
                VkRenderPass renderpass
            );
            ~PipelineCreator();

            /* 
             * Make createinfo instance for graphics pipeline
             * This method is used to set up VkGraphicsPipelineCreateInfo, while defaulting
             * some options that are not needed to be customised by different DENG pipelines
             */
            VkGraphicsPipelineCreateInfo mkGraphicsPipelineInfo (
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
            );
        };
    }
}

#endif
