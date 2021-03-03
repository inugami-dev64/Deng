#ifndef __VULKAN_RENDERER_H
#define __VULKAN_RENDERER_H

#ifdef __VULKAN_RENDERER_CPP
    #include <iostream>
    #include <stdexcept>
    #include <map>
    #include <vector>
    #include <array>
    #include <math.h>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>

    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <deng/vulkan/vulkan_handler.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <deng/vulkan/vulkan_pipelines.h>
    #include <utils/timer.h>
    #include <deng/camera.h>

    #define DENG_PIPELINE_COUNT 6

    /* Pipeline indices */
    #define UM3D_UNOR_I     0
    #define UM3D_NOR_I      1
    #define TM3D_UNOR_I     2
    #define TM3D_NOR_I      3
    #define UM2D_I          4
    #define TM2D_I          5

    /* Shader binary file names */
    #define UNMAPPED_VERT_SHADER_2D             "shaders/bin/2d_unmapped.spv"
    #define TEXTURE_MAPPED_VERT_SHADER_2D       "shaders/bin/2d_tex_mapped.spv"
    #define UNMAPPED_VERT_SHADER_3D             "shaders/bin/3d_unmapped.spv"
    #define UNMAPPED_NORM_VERT_SHADER_3D        "shaders/bin/3d_unmapped_nor.spv"
    #define TEXTURE_MAPPED_VERT_SHADER_3D       "shaders/bin/3d_tex_mapped.spv"
    #define TEXTURE_MAPPPED_NORM_VERT_SHADER_3D "shaders/bin/3d_tex_mapped_nor.spv"
    #define TEXTURE_MAPPED_FRAG_SHADER          "shaders/bin/tex_mapped_frag.spv"
    #define UNMAPPED_FRAG_SHADER                "shaders/bin/unmapped_frag.spv"

    /* Generic default settings definitions */
#endif

#define DENG_DEFAULT_NEAR_PLANE 0.1f
#define DENG_DEFAULT_FAR_PLANE 25.0f
#define DENG_MAX_FRAMES_IN_FLIGHT 3

namespace deng {
    namespace vulkan {
        /* 
         * Class for picking appropriate physical device, creating 
         * logical device and creating new instance for Vulkan 
         */
        class InstanceCreator : private __VulkanInstanceInfo, private __VulkanDeviceInfo {
        private:
            // Supported device properties flags
            deng_bool_t m_tex_linear_filtering_support;

            // Required vulkan extensions
            std::vector<const char*> m_required_extension_names;
            VkSampleCountFlagBits m_max_sample_count;
            const char *m_p_validation_layer = "VK_LAYER_KHRONOS_validation";

        private:
            void __mkInstance(deng_bool_t &enable_validation_layers);
            deng_bool_t __checkValidationLayerSupport();
            void __findSupportedProperties();
            void __mkDebugMessenger();
            void __selectPhysicalDevice();
            void __mkLogicalDevice(deng_bool_t &enable_validation_layers);
            void __mkWindowSurface();

            // Debug messenger
            VkResult __mkDebugMessenger (
                const VkDebugUtilsMessengerCreateInfoEXT *p_messenger_createinfo
            );

            static VKAPI_ATTR VkBool32 VKAPI_CALL __debugCallback (
                VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
                VkDebugUtilsMessageTypeFlagsEXT message_type, 
                const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, 
                void *p_user_data
            );

        public:
            InstanceCreator (
                WindowWrap *p_window_wrap, 
                deng_bool_t enable_validation_layers
            );

            static void destroyDebugUtils (
                VkInstance instance,
                VkDebugUtilsMessengerEXT messenger
            );

        public:
            deng_bool_t getLFSupport();
            VkInstance getIns();
            VkDevice getDev();
            VkSampleCountFlagBits getMaxSampleCount();
            VkPhysicalDevice getGpu();
            VkSurfaceKHR getSu();
            QueueManager getQFF();
            VkDebugUtilsMessengerEXT getDMEXT();
        };


        /* 
         * Class for creating swapchains and swapchain image views 
         */
        class SwapChainCreator : private __VulkanSwapChainInfo, private __VulkanDeviceInfo {
        private:
            QueueManager m_qff;
            VkSampleCountFlagBits m_msaa_sample_c;
        
        private:
            void __mkSwapChainSettings();
            void __mkSwapChain (
                VkSurfaceKHR &surface, 
                deng_ui32_t g_queue_i, 
                deng_ui32_t p_queue_i
            );
            void __mkRenderPass();
            void __mkSCImageViews();
        
        public:
            SwapChainCreator (
                VkDevice device, 
                WindowWrap *p_win,
                VkPhysicalDevice gpu, 
                VkSurfaceKHR surface, 
                QueueManager qff,
                VkSampleCountFlagBits sample_c
            );
            ~SwapChainCreator();
            void SCCleanup();

            /* System that checks for window resizing should be made */
            void remkSwapChain (
                VkDevice device,
                WindowWrap *p_win,
                VkSurfaceKHR surface, 
                VkSurfaceFormatKHR s_format
            );

        public:
            VkRenderPass getRp();
            VkExtent2D getExt();
            VkSwapchainKHR getSC();
            VkFormat getSF();
            std::vector<VkImage> getSCImg();
            std::vector<VkImageView> getSCImgViews();
        };  


        /* 
         * Class for creating descriptor set layouts, pipelines, descriptor sets 
         */
        class DescriptorCreator : private __VulkanDescriptorInfo {
        private:
            std::vector<deng_Asset> *m_p_assets = NULL;
            std::vector<TextureImage> *m_p_textures = NULL;
            deng_ui32_t m_tex_cap = 0;

        private:
            void __mkDescriptorSetLayouts(VkDevice &device);
            void __mkPipelineLayouts(VkDevice &device);
            void __mkGraphicsPipelines (
                VkDevice &device, 
                VkExtent2D &ext, 
                VkRenderPass &renderpass,
                VkSampleCountFlagBits sample_c
            );
            void __mkUnmappedDescPool (
                const VkDevice &device, 
                deng_ui32_t sc_img_c
            );

            void __mkTexMappedDescPool (
                const VkDevice &device,
                deng_ui32_t sc_img_c
            );

        public:
            DescriptorCreator (
                VkDevice device,
                VkExtent2D ext,
                VkRenderPass renderpass, 
                std::vector<deng_Asset> *p_assets, 
                std::vector<TextureImage> *p_textures, 
                size_t sc_img_size,
                VkSampleCountFlagBits sample_c
            );
            
            void mkUnmappedDS (
                VkDevice device, 
                size_t sc_img_size, 
                BufferData buffer_data
            );
            
            void mkTexMappedDS (
                const VkDevice &device, 
                size_t sc_img_size, 
                const dengMath::vec2<deng_ui32_t> &tex_bounds,
                BufferData *p_bd
            );

            void updateTexDescriptors (
                const VkDevice &device,
                size_t sc_img_c,
                deng_bool_t new_ds,
                dengMath::vec2<deng_ui32_t> *p_tex_bounds,
                BufferData *p_bd
            );

            void syncTextures (
                const VkDevice &device,
                size_t sc_img_c,
                BufferData *p_bd
            );

        public:
            std::array<PipelineData, __DENG_PIPELINE_COUNT> getPipelines();
            VkPipelineLayout getUnmappedPL();
            VkPipelineLayout getTexMappedPL();
            std::vector<VkDescriptorSet> *getUnmappedDS();
            VkDescriptorSetLayout getUnmappedDSL();
            VkDescriptorSetLayout getTexMappedDSL();
            VkDescriptorPool getUnmappedDP();
            VkDescriptorPool getTexMappedDP();
        };


        /* 
         * Class for allocating buffer and image resources 
         */ 
        class ResourceAllocator : private __VulkanResourceInfo {
        private:
            std::vector<deng_Asset> *m_p_assets;
            std::vector<TextureImage> *m_p_textures;

        private:
            void __mkUniformBuffers (
                VkDevice &device, 
                VkPhysicalDevice &gpu, 
                size_t sc_img_size
            );

            void __mkTextureSampler (
                VkDevice &device,
                VkSampler &sampler,
                deng_ui32_t mip_levels
            );

            void __mkFrameBuffers (
                VkDevice &device, 
                VkRenderPass &renderpass, 
                VkExtent2D &ext, 
                std::vector<VkImageView> &sc_img_views
            );

            void __mkColorResources (
                VkDevice &device,
                VkPhysicalDevice &gpu,
                VkExtent2D &ext,
                VkFormat sc_color_format
            );

            void __mkDepthResources (
                VkDevice &device, 
                VkPhysicalDevice &gpu, 
                VkExtent2D &ext
            );

            void __mkMipMaps (
                VkDevice &device,
                VkCommandPool &cmd_pool,
                VkImage image,
                VkQueue g_queue,
                deng_i32_t width,
                deng_i32_t height,
                deng_ui32_t mip_levels
            );
            
        public:
            ResourceAllocator (
                VkDevice device, 
                VkPhysicalDevice gpu, 
                VkExtent2D ext, 
                VkSampleCountFlagBits sample_c,
                VkRenderPass renderpass, 
                std::vector<VkImageView> sc_img_views,
                VkFormat sc_color_format,
                deng_bool_t init_res
            );

            
            void setAssetsData (
                std::vector<deng_Asset> *p_assets, 
                std::vector<TextureImage> *p_textures
            );

            void mkTextureImages (
                VkDevice device, 
                VkPhysicalDevice gpu, 
                VkCommandPool command_pool,
                deng_bool_t is_lf_supported, 
                dengMath::vec2<deng_ui32_t> tex_bounds,
                VkQueue g_queue, 
                size_t sc_img_size
            );

            void mkBuffers (
                VkDevice device, 
                VkPhysicalDevice gpu, 
                VkCommandPool command_pool, 
                VkQueue g_queue
            );
            
            void updateUniformBufferData (
                VkDevice device, 
                const deng_ui32_t current_image, 
                void *p_camera, 
                deng_CameraType cam_type,
                deng_CameraUniformFlagBits flag_bits
            );

            void remapAssetVerts (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue,
                dengMath::vec2<deng_ui32_t> asset_bounds
            );

        public:
            BufferData *getBD();
            std::vector<VkFramebuffer> getFB();
            VkImage getDepImg();
            VkDeviceMemory getDepImgMem();
            VkImageView getDepImgView();
            VkImage getColorImg();
            VkDeviceMemory getColorImgMem();
            VkImageView getColorImgView();
        };


        /* 
         * Class for making drawcalls and setting up proper synchronisation 
         */
        class DrawCaller {
        private:
            std::vector<deng_Asset> *m_p_assets = NULL;
            std::vector<TextureImage> *m_p_textures = NULL;
            std::vector<VkFramebuffer> m_framebuffers;
            std::array<PipelineData, __DENG_PIPELINE_COUNT> m_pl_data;
            std::vector<VkDescriptorSet> *m_p_unmapped_ds = NULL;
            QueueManager m_qff;

            // Commandpools and commandbuffers
            VkCommandPool m_cmd_pool;
            std::vector<VkCommandBuffer> m_cmd_bufs;

        private:
            void __mkSynchronisation(VkDevice &device);
            TextureImage __findTextureImageByID(char *id);

        public:
            // Needed for synchronising frames
            size_t current_frame = 0;
            std::vector<VkFence> flight_fences;
            std::vector<VkSemaphore> image_available_semaphore_set;
            std::vector<VkSemaphore> render_finished_semaphore_set;

        public:
            DrawCaller (
                VkDevice device,
                QueueManager qff
            );
            
            void setAssetsData (
                std::vector<deng_Asset> *p_assets, 
                std::vector<TextureImage> *p_textures
            );
            
            void setMiscData (
                std::array<PipelineData, __DENG_PIPELINE_COUNT> pl_data, 
                std::vector<VkFramebuffer> fb, 
                std::vector<VkDescriptorSet> *p_unmapped_ds
            );

            void mkCommandPool(VkDevice device);

            void allocateMainCmdBuffers (
                VkDevice device, 
                VkQueue g_queue, 
                VkRenderPass renderpass, 
                VkExtent2D ext,
                dengMath::vec4<deng_vec_t> background,
                const BufferData &bd
            );

            void recordMainCmdBuffers (
                VkRenderPass renderpass,
                VkExtent2D ext,
                dengMath::vec4<deng_vec_t> background,
                const BufferData &bd
            );
        
        public:
            VkCommandPool getComPool();
            const std::vector<VkCommandBuffer> &getComBufs();
        };
    }
}

#endif
