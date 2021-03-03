#ifndef __VULKAN_REND_INFOS_H
#define __VULKAN_REND_INFOS_H

#define __DENG_PIPELINE_COUNT 6

namespace deng {
    namespace vulkan {
        /* 
         * Struct for storing all vulkan device related information 
         */
        struct __VulkanDeviceInfo {
            VkDevice m_device;
            WindowWrap *m_p_win;
            VkPhysicalDevice m_gpu;
        };


        /* 
         * Struct for storing all Vulkan instance objects 
         */
        struct __VulkanInstanceInfo {
            QueueManager m_qff;
            VkInstance m_instance;
            VkSurfaceKHR m_surface;
            VkDebugUtilsMessengerEXT m_debug_mes;
        };


        /* 
         * Struct for storing all Vulkan swapchain related objects 
         */
        struct __VulkanSwapChainInfo {
            VkRenderPass m_renderpass;
            VkSwapchainKHR m_swapchain;
            VkPresentModeKHR m_present_mode;
            VkExtent2D m_ext;
            VkFormat m_format;
            VkSurfaceFormatKHR m_surface_format;
            std::vector<VkImage> m_swapchain_images;
            std::vector<VkImageView> m_swapchain_image_views;
            SwapChainDetails *m_p_sc_details;
        };


        /* 
         * Contains information about certain pipeline used in DENG 
         */
        struct PipelineData {
            VkPipeline pipeline;
            deng_PipelineType pipeline_type;
            VkPipelineLayout *p_pipeline_layout;
        };


        /* 
         * Struct for storing all Vulkan descriptor related objects 
         */
        struct __VulkanDescriptorInfo {
            std::array<PipelineData, __DENG_PIPELINE_COUNT> m_pipelines;
            std::vector<VkDescriptorSet> m_unmapped_descriptor_sets;
            VkPipelineLayout m_unmapped_pl;
            VkPipelineLayout m_tex_mapped_pl;
            VkDescriptorPool m_unmapped_desc_pool;
            VkDescriptorPool m_tex_mapped_desc_pool;
            VkDescriptorSetLayout m_unmapped_desc_set_layout;
            VkDescriptorSetLayout m_tex_mapped_desc_set_layout;
        };


        /* 
         * Struct for storing all Vulkan resource allocation related objects 
         */
        struct __VulkanResourceInfo {
            std::vector<VkFramebuffer> m_framebuffers;
            VkImage m_depth_image;
            VkDeviceMemory m_depth_image_mem;
            VkImageView m_depth_image_view;

            VkSampleCountFlagBits m_sample_count;
            VkImage m_color_image;
            VkDeviceMemory m_color_image_mem;
            VkImageView m_color_image_view;

            BufferData m_buffer_data;
        };
    }
}

#endif
