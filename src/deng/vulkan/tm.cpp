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


#define __TM_CPP
#include <deng/vulkan/tm.h>

namespace deng {
    namespace vulkan {

        __vk_TextureManager::__vk_TextureManager (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            std::vector<deng_Id> &textures,
            deng::__GlobalRegistry &reg,
            __vk_BufferData &bd
        ) : m_textures(textures), m_bd(bd), m_reg(reg)
        {
            // Create missing texture instance
            __mkMissingTex(device, gpu, cmd_pool, g_queue);
        }


        /// Create missing texture in case any texture mapped asset
        /// has invalid texture uuid
        void __vk_TextureManager::__mkMissingTex (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue
        ) {
            deng::RegType missing_base_tex = { { 0 } };
            missing_base_tex.tex.name = (char*) "missing_base_tex";
            missing_base_tex.tex.uuid = uuid_Generate();
            missing_base_tex.tex.pixel_data.width = __DEFAULT_TEX_WIDTH;
            missing_base_tex.tex.pixel_data.height = __DEFAULT_TEX_HEIGHT;
            missing_base_tex.tex.pixel_data.size = __DEFAULT_TEX_SIZE;
            missing_base_tex.tex.pixel_data.pixel_data = reinterpret_cast<deng_ui8_t*>(malloc(__DEFAULT_TEX_SIZE));
            memset(missing_base_tex.tex.pixel_data.pixel_data, 255, __DEFAULT_TEX_SIZE);

            // Make Vulkan texture
            deng::RegType missing_vk_tex = { { 0 } };
            missing_vk_tex.vk_tex.base_id = missing_base_tex.tex.uuid;
            missing_vk_tex.vk_tex.uuid = uuid_Generate();
            missing_base_tex.tex.vk_id = missing_vk_tex.vk_tex.uuid;

            m_missing_tex_uuid = missing_base_tex.tex.uuid;
            
            // Push the texture uuid to registry
            m_reg.push(missing_base_tex.tex.uuid, 
                DENG_SUPPORTED_REG_TYPE_TEXTURE, missing_base_tex);

            // Create new image samplers and image views
            __newVkTexture(device, gpu, cmd_pool, g_queue, 1, false, false, true, missing_vk_tex.vk_tex);
            m_textures.push_back(m_missing_tex_uuid);

            m_reg.push(missing_vk_tex.vk_tex.uuid, DENG_SUPPORTED_REG_TYPE_VK_TEXTURE, missing_vk_tex);
        }


        /// Create mipmaps for texture images
        void __vk_TextureManager::__mkMipMaps (
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
            __vk_CommandBufferRecorder::beginCommandBufferSingleCommand(
                device, cmd_pool, &cmd_buf);

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

            for(deng_ui32_t i = 1; i < mip_levels; i++) {
                mem_barrier.subresourceRange.baseMipLevel = i - 1;
                mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                mem_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                mem_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                // Record pipeline barrier
                vkCmdPipelineBarrier(cmd_buf, VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1,
                    &mem_barrier);

                // Set blit image struct for mipmapping
                blit.srcOffsets[0] = {0, 0, 0};
                blit.srcOffsets[1] = {mip_width, mip_height, 1};
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.baseArrayLayer = 0;
                blit.srcSubresource.mipLevel = i - 1;
                blit.srcSubresource.layerCount = 1;
                blit.dstOffsets[0] = {0, 0, 0};
                blit.dstOffsets[1] = {mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1};
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.baseArrayLayer = 0;
                blit.dstSubresource.mipLevel = i;
                blit.dstSubresource.layerCount = 1;

                vkCmdBlitImage(cmd_buf, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

                mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                mem_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                mem_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                vkCmdPipelineBarrier(cmd_buf, VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1,
                    &mem_barrier);

                if(mip_width > 1) mip_width /= 2;
                if(mip_height > 1) mip_height /= 2;
            }

            // Final mip level transitioning
            mem_barrier.subresourceRange.baseMipLevel = mip_levels - 1;
            mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            mem_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            mem_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(cmd_buf, VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &mem_barrier);

            __vk_CommandBufferRecorder::endCommandBufferSingleCommand(device,
                g_queue, cmd_pool, &cmd_buf);
        }


        /// Create texture sampler for texture image
        void __vk_TextureManager::__mkTextureSampler (
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
            if(vkCreateSampler(device, &sampler_info, NULL, &sampler) != VK_SUCCESS)
                VK_RES_ERR("failed to create texture sampler!");
        }


        /// Allocate memory for texture buffers
        /// req_size must be larger than the memory required for all textures combined
        void __vk_TextureManager::__allocateTexMemory (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool, 
            VkQueue g_queue, 
            VkDeviceSize size
        ) {
            size = cm_ToPow2I64(size);
            m_bd.img_memory_cap = size + 1;

            // Allocate new memory for new texture image buffer
            __vk_BufferCreator::allocateMemory ( device, gpu, size, m_bd.img_memory,
                m_tex_mem_bits,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );
        }


        /// Calculate miplevels for texture specified with specific width and height
        const deng_ui32_t __vk_TextureManager::__findMipLevels (
            const deng_bool_t is_lf, 
            const deng_ui16_t width, 
            const deng_ui16_t height
        ) {
            // If linear filtering is supported, set the mip levels accordingly
            // Otherwise set mip levels as 1
            if(is_lf) return log2l(std::max<VkDeviceSize>(static_cast<VkDeviceSize>(width), static_cast<VkDeviceSize>(height))) + 1;
            else return 1;
        }
        

        /*
         * Destroy previous texture memory instances, allocate new one and 
         * copy texture data over to the new memory area.
         *
         * Allocated memory size is at least min_size floored up to the nearest base 2 exponant,
         * in case the required allocation size floored to nearest base 2 exponant is smaller 
         * than the double of the previous capacity the latter will be used instead
         */
        void __vk_TextureManager::__reallocTexMemory (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            VkDeviceSize min_size,
            deng_bool_t is_lf,
            deng_ui32_t mip_levels
        ) {
            min_size = min_size < 2 * m_bd.img_memory_cap ? 2 * m_bd.img_memory_cap : min_size;
            min_size = cm_ToPow2I64(min_size);

            std::queue<deng_Id> realloc_queue;

            // Iterate through every texture instance and destroy its VkImage and VkImageView instances
            for(size_t i = 0; i < m_textures.size(); i++) {
                RegType &reg_vk_tex = m_reg.retrieve(m_textures[i], 
                    DENG_SUPPORTED_REG_TYPE_VK_TEXTURE, NULL);

                if(reg_vk_tex.vk_tex.is_buffered) {
                    vkDestroyImageView(device, reg_vk_tex.vk_tex.image_view, NULL);
                    vkDestroyImage(device, reg_vk_tex.vk_tex.image, NULL);
                    realloc_queue.push(m_textures[i]);
                }
            }

            vkFreeMemory(device, m_bd.img_memory, NULL);
            __allocateTexMemory(device, gpu, cmd_pool, g_queue, min_size);

            // Create new VkImages and VkImageViews for all textures that were deallocated
            while(!realloc_queue.empty()) {
                RegType &reg_vk_tex = m_reg.retrieve (
                    realloc_queue.front(), 
                    DENG_SUPPORTED_REG_TYPE_VK_TEXTURE,
                    NULL
                );

                realloc_queue.pop();

                // Create a new Vulkan texture instance
                __newVkTexture(device, gpu, cmd_pool, g_queue,
                    mip_levels, is_lf, true, false, reg_vk_tex.vk_tex);
            }
        }

            
        /// Check if linear filtering is requested and if mipmaps should be created
        void __vk_TextureManager::__mipmapTransition (
            VkDevice device,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            deng_bool_t is_lf,
            deng_ui32_t mip_levels,
            __vk_Texture &tex
        ) {
            RegType &reg_tex = m_reg.retrieve(tex.base_id, 
                DENG_SUPPORTED_REG_TYPE_TEXTURE, NULL);

            // Check if linear filtering is enabled and if mipmapping should be enabled
            if(is_lf) {
                __mkMipMaps(device, cmd_pool, tex.image, g_queue,
                    static_cast<deng_ui32_t>(reg_tex.tex.pixel_data.width),
                    static_cast<deng_ui32_t>(reg_tex.tex.pixel_data.height),
                    mip_levels);
            }

            else {
                __vk_ImageCreator::transitionImageLayout(device, tex.image, cmd_pool, 
                    g_queue, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mip_levels);
            }

        }

        
        /// Create VkImageView for texture image
        void __vk_TextureManager::__mkImageView (
            VkDevice device,
            __vk_Texture &tex,
            deng_ui32_t mip_levels
        ) {
            VkImageViewCreateInfo viewinfo = __vk_ImageCreator::getImageViewInfo(
                tex.image, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mip_levels);

            if(vkCreateImageView(device, &viewinfo, NULL, &tex.image_view) != VK_SUCCESS) 
                VK_RES_ERR("Failed to create texture image view!");
            LOG("Created image view for texture with id: " + std::string(tex.uuid));
        }


        /// Copy all bitmap data to image bitmap buffer
        /// WARNING: This method performs no buffer bounds check and can cause errors!
        /// NOTE: Texture object must have a valid VkImage instance created before calling this 
        /// method
        void __vk_TextureManager::cpyBitmap (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            deng_ui32_t mip_levels,
            __vk_Texture &tex
        ) {
            LOG("Updating texture with base id: " + std::string(tex.base_id));
            RegType &reg_tex = m_reg.retrieve(tex.base_id, DENG_SUPPORTED_REG_TYPE_TEXTURE, NULL);

            // Create the staging buffer
            VkMemoryRequirements mem_req = __vk_BufferCreator::makeBuffer(device, gpu,
                reg_tex.tex.pixel_data.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_bd.staging_buffer);

            // Allocate memory for staging buffer
            __vk_BufferCreator::allocateMemory(device, gpu, mem_req.size, m_bd.staging_buffer_memory, 
                mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            // Bind staging buffer to its memory
            vkBindBufferMemory(device, m_bd.staging_buffer, m_bd.staging_buffer_memory, 0);

            // Copy all bitmap data to staging buffer
            __vk_BufferCreator::cpyToBufferMem(device, reg_tex.tex.pixel_data.size,
                reg_tex.tex.pixel_data.pixel_data, m_bd.staging_buffer_memory, 0);

            // Copy data from staging buffer to image buffer
            __vk_ImageCreator::cpyBufferToImage(device, cmd_pool, g_queue, m_bd.staging_buffer,
                tex.image, reg_tex.tex.pixel_data.width, reg_tex.tex.pixel_data.height);

            // Clean the staging buffer
            vkDestroyBuffer(device, m_bd.staging_buffer, NULL);

            // Free staging buffer's memory
            vkFreeMemory(device, m_bd.staging_buffer_memory, NULL);
        }


        /// Create new VkImage and VkImageView instances for single texture
        void __vk_TextureManager::__newVkTexture (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool, VkQueue g_queue,
            deng_ui32_t mip_levels,
            deng_bool_t is_lf,
            deng_bool_t ignore_mem_check,
            deng_bool_t set_default_mem_req,
            __vk_Texture &tex
        ) {
            // Retrieve the base texture uuid from registry
            deng::RegType &base_tex = m_reg.retrieve(tex.base_id, 
                DENG_SUPPORTED_REG_TYPE_TEXTURE, NULL);

            // Create new VkImage instance
            VkMemoryRequirements mem_req = __vk_ImageCreator::makeImage (
                device, gpu, tex.image, (deng_ui32_t) base_tex.tex.pixel_data.width,
                (deng_ui32_t) base_tex.tex.pixel_data.height, mip_levels, VK_FORMAT_B8G8R8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_SAMPLE_COUNT_1_BIT);

            // If default memory type specification is required set them
            if(set_default_mem_req) m_tex_mem_bits = mem_req.memoryTypeBits;
            
            // Check if image memory needs to be allocated
            if(!ignore_mem_check && m_bd.img_memory_offset + mem_req.size > m_bd.img_memory_cap) {
                VkDeviceSize req_size = 0;
                if(!m_bd.img_memory_cap) {
                    req_size = __DEFAULT_TEX_MEM_CAP > mem_req.size ? __DEFAULT_TEX_MEM_CAP : mem_req.size;
                    __allocateTexMemory(device, gpu, cmd_pool, g_queue, req_size);
                }
                else __reallocTexMemory(device, gpu, cmd_pool, g_queue, req_size, is_lf, mip_levels);
            }

            // Retrieve the base image from the registry
            RegType &reg_tex = m_reg.retrieve(tex.base_id, DENG_SUPPORTED_REG_TYPE_TEXTURE, NULL);

            // Check if the image memory location should be appended to the end of image memory area
            reg_tex.tex.pixel_data.memory_offset = m_bd.img_memory_offset;
            m_bd.img_memory_offset += mem_req.size;
            
            // Bind the image to its memory with correct offsets
            vkBindImageMemory(device, tex.image, m_bd.img_memory, reg_tex.tex.pixel_data.memory_offset);

            // Transition the image layout to make it suitable for recieving data 
            __vk_ImageCreator::transitionImageLayout(device, tex.image, cmd_pool, g_queue,
                VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                mip_levels);

            // Copy bitmap data over to image buffer
            cpyBitmap(device, gpu, cmd_pool, g_queue, mip_levels, tex);
            tex.is_buffered = true;
            
            // Create mipmaps if required
            __mipmapTransition(device, cmd_pool, g_queue, is_lf, mip_levels, tex);

            // Create VkImageView for the texture instance
            __mkImageView(device, tex, mip_levels);

            // Create texture sampler for the texture
            __mkTextureSampler(device, tex.sampler, mip_levels);
        }


        /// Set up texture image buffers, samplers and mipmaps if they
        /// are supported
        void __vk_TextureManager::mkTexture (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool cmd_pool,
            deng_bool_t is_lf, 
            deng_Id id,
            VkQueue g_queue
        ) {
            // Retrieve Vulkan and base textures from the registry
            RegType &tex_reg = m_reg.retrieve(id, DENG_SUPPORTED_REG_TYPE_TEXTURE, NULL);
            RegType &vk_tex_reg = m_reg.retrieve(tex_reg.tex.vk_id, DENG_SUPPORTED_REG_TYPE_VK_TEXTURE, NULL);
            
            // Find mipmapping levels
            deng_ui32_t mip_levels = __findMipLevels(is_lf, tex_reg.tex.pixel_data.width, tex_reg.tex.pixel_data.height);
                
            // Create a new Vulkan image instance
            __newVkTexture(device, gpu, cmd_pool, g_queue, mip_levels, is_lf, false, false, vk_tex_reg.vk_tex);
        }


        deng_Id __vk_TextureManager::getMissingTextureUUID() { return m_missing_tex_uuid; }
    }
}
