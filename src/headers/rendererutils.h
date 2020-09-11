#ifndef RENDERERUTILS_H
#define RENDERERUTILS_H
#include "files.h"

namespace deng {

    class QueueFamilies {
    private:
        uint32_t m_graphics_family_index;
        uint32_t m_present_family_index;
    
    public:
        bool findGraphicsFamily(VkPhysicalDevice &gpu);
        uint32_t getGraphicsFamily();

        bool findPresentSupportFamily(VkPhysicalDevice &gpu, VkSurfaceKHR &surface);
        uint32_t getPresentFamily();
    };

    class Queues {
    public:
        VkQueue graphics_queue;
        VkQueue present_queue;
    };

    class PipelineCreator {
    private:
        dengPipelineType m_pipeline_type;
        VkDevice *m_p_device;
        FileManager *m_p_fm;
        VkExtent2D *m_p_extent;
        VkRenderPass *m_p_renderpass;

        std::array<VkPipelineShaderStageCreateInfo, 2> m_shader_stage_createinfos{};
        VkVertexInputBindingDescription m_input_binding_descriptor{};
        std::vector<VkVertexInputAttributeDescription> m_input_attribute_descriptor{};
        std::array<VkShaderModule, 2> m_shader_modules{};

        VkPipelineVertexInputStateCreateInfo m_vertex_input_createinfo{};
        VkPipelineInputAssemblyStateCreateInfo m_input_assembly_createinfo{};
        VkViewport m_viewport{};
        VkRect2D m_scissor{};
        VkPipelineViewportStateCreateInfo m_viewport_state_createinfo{};
        VkPipelineRasterizationStateCreateInfo m_rasterization_createinfo{};
        VkPipelineMultisampleStateCreateInfo m_multisample_createinfo{};
        VkPipelineColorBlendAttachmentState m_colorblend_attachment{};
        VkPipelineDepthStencilStateCreateInfo m_depth_stencil{};
        VkPipelineColorBlendStateCreateInfo m_colorblend_state_createinfo{};

    private:
        VkShaderModule getShaderModule(std::vector<char> &shader_bins);

        VkVertexInputBindingDescription getBindingDesc();
        std::vector<VkVertexInputAttributeDescription> getAttributeDesc();
        
    public:
        PipelineCreator(const dengPipelineType &pipeline_type, VkDevice *device, FileManager *filemanager, VkExtent2D *extent, VkRenderPass *renderpass);
        ~PipelineCreator();
        VkGraphicsPipelineCreateInfo getGraphicsPipeline(const std::string &vert_shader, const std::string &frag_shader, const char *p_shader_module_name,
        const VkPolygonMode &polygon_mode, const VkFrontFace &front_face, const VkPrimitiveTopology &primitive_topology, const dengBool &add_depth_stencil, const dengBool &add_color_blend, const VkPipelineLayout &layout, const uint32_t &subpass_index);

    };

    struct PipelineData {
        dengPipelineType pipeline_type;
        VkDescriptorSetLayout descriptor_set_layout;
        VkDescriptorPool descriptor_pool;
        std::vector<VkDescriptorSet> descriptor_sets;
        VkPipelineLayout pipeline_layout;
        VkPipeline pipeline;
    };

    class SwapChainDetails {
    private:
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_present_modes;

    public:
        SwapChainDetails(VkPhysicalDevice &gpu, VkSurfaceKHR &surface);
        VkSurfaceCapabilitiesKHR getCapabilities();
        std::vector<VkSurfaceFormatKHR> getFormats();
        std::vector<VkPresentModeKHR> getPresentModes();

    };
}

#endif