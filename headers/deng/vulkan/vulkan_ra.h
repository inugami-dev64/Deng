/*    ____         ________     __      _        ______  
 *   |  _ \_      |  ______|   |  \    | |     _/  ____\
 *   | | \_ \_    |  |         |   \   | |    /  _/   
 *   | |   \  |   |  |_____    | |\ \  | |   |  |   _____ 
 *   | |    | |   |  ______|   | | \ \ | |   |  |  |___  | 
 *   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
 *   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
 *   |____/       |________|   |_|    \__|       \____/
 *               
 * __________________________________________________________
 * __________________________________________________________
 *                       Project DENG 
 *
 *
 * Copyright (C) 2020 - 2021
 * This Software is licensed under Apache License as described 
 * in the LICENSE.md file, which you should have recieved with 
 * this distribution.
 * 
 * You may reproduce and distribute copies of the
 * Work or Derivative Works thereof in any medium, with or without
 * modifications, and in Source or Object form, provided that You
 * meet the following conditions:
 *
 * (a) You must give any other recipients of the Work or
 *     Derivative Works a copy of this License; and
 *
 * (b) You must cause any modified files to carry prominent notices
 *     stating that You changed the files; and
 *
 * (c) You must retain, in the Source form of any Derivative Works
 *     that You distribute, all copyright, patent, trademark, and
 *     attribution notices from the Source form of the Work,
 *     excluding those notices that do not pertain to any part of
 *     the Derivative Works; and
 *
 * (d) If the Work includes a "NOTICE" text file as part of its
 *     distribution, then any Derivative Works that You distribute must
 *     include a readable copy of the attribution notices contained
 *     within such NOTICE file, excluding those notices that do not
 *     pertain to any part of the Derivative Works, in at least one
 *     of the following places: within a NOTICE text file distributed
 *     as part of the Derivative Works; within the Source form or
 *     documentation, if provided along with the Derivative Works; or,
 *     within a display generated by the Derivative Works, if and
 *     wherever such third-party notices normally appear. The contents
 *     of the NOTICE file are for informational purposes only and
 *     do not modify the License. You may add Your own attribution
 *     notices within Derivative Works that You distribute, alongside
 *     or as an addendum to the NOTICE text from the Work, provided
 *     that such additional attribution notices cannot be construed
 *     as modifying the License.
 *
 * You may add Your own copyright statement to Your modifications and
 * may provide additional or different license terms and conditions
 * for use, reproduction, or distribution of Your modifications, or
 * for any such Derivative Works as a whole, provided Your use,
 * reproduction, and distribution of the Work otherwise complies with
 * the conditions stated in this License.
 */ 


#ifndef __VULKAN_RA_H
#define __VULKAN_RA_H

#ifdef __VULKAN_RA_CPP
    #include <vector>
    #include <array>
    #include <mutex>
    #include <cmath>

    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
    #include <common/hashmap.h>
    #include <common/uuid.h>
    #include <common/err_def.h>
    #include <das/assets.h>

    #include <math/deng_math.h>
    #include <deng/window.h>
    #include <utils/timer.h>
    #include <deng/camera.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
#endif

#define __DEFAULT_ASSET_HM_CAP  64
#define __DEFAULT_TEX_HM_CAP    64
#define __DEFAULT_TEX_WIDTH     512
#define __DEFAULT_TEX_HEIGHT    512
#define __DEFAULT_TEX_SIZE      1048576 // bytes
#define __DEFAULT_TEX_MEM_CAP   8388608 // bytes

#define __DEFAULT_ASSET_HM_CAP  64
#define __DEFAULT_TEX_HM_CAP    64
#define __DEFAULT_TEX_WIDTH     512
#define __DEFAULT_TEX_HEIGHT    512
#define __DEFAULT_TEX_SIZE      1048576 // bytes
#define __DEFAULT_TEX_MEM_CAP   8388608 // bytes


namespace deng {
    namespace vulkan {

        /*
         * This class is responsible for allocating buffers to
         * texture images on more abstractedly than in __vk_BufferCreator struct
         */
        class __vk_TextureAllocator : protected __vk_ResourceInfo {
        private:
            std::vector<__vk_Texture> *m_p_textures = NULL;
            Hashmap *m_p_tex_map = NULL;
            char *m_dummy_tex_uuid = NULL;
            deng_ui32_t m_tex_mem_bits = 0;

        private:
            /*
             * Create dummy texture in case any texture mapped asset
             * has invalid texture uuid
             */
            void __mkDummyTex (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue
            );

            
            /*
             * Create mipmaps for a texture image
             */
            void __mkMipMaps (
                VkDevice &device,
                VkCommandPool &cmd_pool,
                VkImage image,
                VkQueue g_queue,
                deng_i32_t width,
                deng_i32_t height,
                deng_ui32_t mip_levels
            );


            /*
             * Create texture sampler for texture image
             */
            void __mkTextureSampler (
                VkDevice &device,
                VkSampler &sampler,
                deng_ui32_t mip_levels
            );

        protected:
            __vk_TextureAllocator (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue,
                size_t sc_img_size,
                std::vector<__vk_Texture> *p_tex,
                Hashmap *p_tex_map
            );


            /*
             * Allocate memory for texture buffers
             * Despite its name this method acts like a reallocator as well
             * In that case the method expects that the img_memory is correctly 
             * allocated and data can be made
             */
            void __allocateTexMemory (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool commandpool, 
                VkQueue g_queue, 
                VkDeviceSize old_size,
                VkDeviceSize req_size
            );


            /*
             * Create new VkImage and VkImageView instances for single texture
             * This method expects that enough memory is allocated for bitmap data
             * of the given texture
             */
            void __newImage (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue,
                deng_ui32_t mip_levels,
                deng_bool_t is_lf,
                __vk_Texture &tex
            );

            char *__getDummyUUID();
        };


        /* 
         * Class for allocating buffer and image resources 
         */ 
        class __vk_ResourceManager : private __vk_TextureAllocator {
        private:
            std::vector<__vk_Asset> *m_p_assets;
            std::vector<__vk_Texture> *m_p_textures;
            char *m_dummy_tex_uuid;
            deng_ui32_t m_tex_mem_bits = 0;
            const VkPhysicalDeviceLimits &m_gpu_limits;

        private:
            /* 
             * Create new uniform buffer data buffer
             */
            void __allocateUniformBuffer (
                VkDevice device, 
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool, 
                VkQueue g_queue, 
                VkDeviceSize old_size
            );


            /* 
             * Push texture image's color uniform data to uniform buffer
             */
            void __pushColorUniformData (
                VkDevice &device,
                VkPhysicalDevice &gpu,
                VkCommandPool &cmd_pool,
                VkQueue g_queue,
                size_t sc_img_size,
                std::vector<__vk_Texture> &tex_img
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


            /*
             * This method either doubles the texture buffer memory capacity or 
             * calculates the new capacity from given textures and 
             * reallocates everything in it.
             */
            void __reallocateTextureMemory (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue
            );


            /*
             * This method reallocates ubo buffer to double its capacity
             */
            void __reallocateUniformBufferMemory (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue
            );
            
        public:
            __vk_ResourceManager (
                VkDevice device, 
                VkPhysicalDevice gpu, 
                VkExtent2D ext, 
                VkSampleCountFlagBits sample_c,
                VkRenderPass renderpass, 
                VkCommandPool cmd_pool,
                VkQueue g_queue,
                std::vector<VkImageView> sc_img_views,
                const __vk_AssetsInfo &assets_info,
                VkFormat sc_color_format,
                const VkPhysicalDeviceLimits &gpu_limits
            );


            void mkTextures (
                VkDevice device, 
                VkPhysicalDevice gpu, 
                VkCommandPool command_pool,
                deng_bool_t is_lf_supported, 
                dengMath::vec2<deng_ui32_t> tex_bounds,
                VkQueue g_queue
            );

            void mkBuffers (
                VkDevice device, 
                VkPhysicalDevice gpu, 
                VkCommandPool command_pool, 
                VkQueue g_queue
            );


            void updateUniformData (
                VkDevice device, 
                deng_ui32_t current_image, 
                deng_ui32_t img_c,
                Camera3D *p_cam,
                deng_bool_t update_tex,
                const dengMath::vec2<deng_ui32_t> &tex_bounds,
                deng_CameraUniformFlagBits flag_bits
            );


            /*
             * Copy asset color data to uniform buffer memory
             * PS! ubo_mem pointer must have enough memory allocated for copying
             * data to it 
             */
            void reserveAssetUniformData (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue,
                __vk_UniformColorData *ubo_mem,
                __vk_Asset &asset
            );


            /*
             * Replace data in main buffer with newer data from given asset vertices
             */
            void remapAssetVerts (
                VkDevice device,
                VkPhysicalDevice gpu,
                VkCommandPool cmd_pool,
                VkQueue g_queue,
                dengMath::vec2<deng_ui32_t> asset_bounds
            );

        public:
            __vk_BufferData *getBD();
            std::vector<VkFramebuffer> getFB();
            char *getDummyTexUUID();
            VkImage getDepImg();
            VkDeviceMemory getDepImgMem();
            VkImageView getDepImgView();
            VkImage getColorImg();
            VkDeviceMemory getColorImgMem();
            VkImageView getColorImgView();
        };
    }
}

#endif
