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


#define __BM_CPP
#include <deng/vulkan/bm.h>


namespace deng {
    namespace vulkan {
        extern deng_ui32_t __max_frame_c;       

        __vk_BufferManager::__vk_BufferManager (
            const VkPhysicalDeviceLimits &gpu_limits,
            std::vector<deng_Id> &assets,
            deng::__GlobalRegistry &reg
        ) : m_assets(assets), m_gpu_limits(gpu_limits), m_reg(reg) {}

        
        /*
         * Create new uniform buffer instance and allocate memory for it.
         * WARNING: This method call expects that uniform buffer and its memory
         * have been freed or are not initialised previously
         */
        void __vk_BufferManager::__mkUniformBuffer (
            VkDevice device, 
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool, 
            VkQueue g_queue,
            deng_ui64_t asset_cap
        ) {
            m_buffer_data.ubo_asset_cap = asset_cap;
            
            // Calculate the initial uniform buffer capacity
            m_buffer_data.ubo_cap = __max_frame_c * (cm_FindChunkSize(m_gpu_limits.minUniformBufferOffsetAlignment, 
                sizeof(__vk_UniformTransformation)) + asset_cap * cm_FindChunkSize(m_gpu_limits.minUniformBufferOffsetAlignment, 
                sizeof(__vk_UniformColorData)));
            
            // Create a new uniform buffer instance
            VkMemoryRequirements mem_req = __vk_BufferCreator::makeBuffer(device, gpu, 
                m_buffer_data.ubo_cap, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                m_buffer_data.uniform_buffer);

            // Allocate memory for uniform buffer
            __vk_BufferCreator::allocateMemory(device, gpu, mem_req.size,
                m_buffer_data.uniform_buffer_mem, mem_req.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            // Bind the newly created buffer with newly allocated buffer memory
            vkBindBufferMemory(device, m_buffer_data.uniform_buffer, m_buffer_data.uniform_buffer_mem, 0);
        }


        /*
         * Reallocate memory for uniform buffer and copy previous data to
         * newly allocated buffer instance
         */
        void __vk_BufferManager::__reallocUniformBufferMemory (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            deng_ui64_t req_cap
        ) {
            // Create a new staging buffer for all the previous uniform data
            VkMemoryRequirements mem_req = __vk_BufferCreator::makeBuffer(device, gpu,
                m_buffer_data.ubo_cap, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                m_buffer_data.staging_buffer);
            
            // Allocate memory for the staging buffer
            __vk_BufferCreator::allocateMemory(device, gpu,
                mem_req.size, m_buffer_data.staging_buffer_memory, 
                mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

            // Bind the staging buffer with its memory
            vkBindBufferMemory(device, m_buffer_data.staging_buffer, m_buffer_data.staging_buffer_memory, 0);

            // Copy from uniform buffer to staging buffer
            __vk_BufferCreator::cpyBufferToBuffer(device, cmd_pool, g_queue, m_buffer_data.uniform_buffer, 
                m_buffer_data.staging_buffer, m_buffer_data.ubo_cap, 0, 0);

            // Free all resources used in uniform buffers
            vkDestroyBuffer(device, m_buffer_data.uniform_buffer, NULL);
            vkFreeMemory(device, m_buffer_data.uniform_buffer_mem, NULL);

            // Allocate memory for new uniform buffer instance
            __mkUniformBuffer(device, gpu, cmd_pool, g_queue, req_cap);

            // Copy from staging buffer to the newly created uniform buffer instance
            __vk_BufferCreator::cpyBufferToBuffer(device, cmd_pool, g_queue, m_buffer_data.staging_buffer, 
                m_buffer_data.uniform_buffer, m_buffer_data.ubo_cap, 0, 0);
        }

        
        void __vk_BufferManager::__findAssetOffsets(das_Asset &asset) {
            // In each case increment the buffer size and find correct and vertices and 
            // indices' offsets
            switch (asset.asset_mode) {
            case DAS_ASSET_MODE_2D_UNMAPPED:
                // Set all vertices' offsets and the vertices that are not available mark as
                // UINT64_MAX
                asset.offsets.pos_offset = m_buffer_data.main_buffer_size;
                asset.offsets.tex_offset = UINT64_MAX;
                asset.offsets.nor_offset = UINT64_MAX;

                m_buffer_data.main_buffer_size += asset.vertices.v2d.pn * 
                    sizeof(das_ObjPosData2D);

                asset.offsets.ind_offset = m_buffer_data.main_buffer_size;
                m_buffer_data.main_buffer_size += asset.indices.n * sizeof(deng_ui32_t);
                break;
            
            case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
                // Set all vertices' offsets and the vertices that are not available mark as
                // UINT64_MAX
                asset.offsets.pos_offset = m_buffer_data.main_buffer_size;
                asset.offsets.tex_offset = m_buffer_data.main_buffer_size +
                    asset.vertices.v2d.pn * sizeof(das_ObjPosData2D);
                asset.offsets.nor_offset = UINT64_MAX;

                m_buffer_data.main_buffer_size += asset.vertices.v2d.pn * sizeof(das_ObjPosData2D) + 
                    asset.vertices.v2d.tn * sizeof(das_ObjTextureData);

                asset.offsets.ind_offset = m_buffer_data.main_buffer_size;
                m_buffer_data.main_buffer_size += 2 * asset.indices.n * sizeof(deng_ui32_t);
                break;

            case DAS_ASSET_MODE_3D_UNMAPPED:
                // Set all vertices' offsets and the vertices that are not available mark as
                // UINT64_MAX
                asset.offsets.pos_offset = m_buffer_data.main_buffer_size;
                asset.offsets.tex_offset = UINT64_MAX;
                asset.offsets.nor_offset = m_buffer_data.main_buffer_size + 
                    asset.vertices.v3d.pn * sizeof(das_ObjPosData);

                m_buffer_data.main_buffer_size += asset.vertices.v3d.pn * sizeof(das_ObjPosData) +
                asset.vertices.v3d.nn * sizeof(das_ObjNormalData);

                asset.offsets.ind_offset = m_buffer_data.main_buffer_size;
                m_buffer_data.main_buffer_size += 2 * asset.indices.n * sizeof(deng_ui32_t);
                break;

            case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
                // Set all vertices' offsets and the vertices that are not available mark as
                // UINT64_MAX
                asset.offsets.pos_offset = m_buffer_data.main_buffer_size;
                asset.offsets.tex_offset = m_buffer_data.main_buffer_size +
                    asset.vertices.v3d.pn * sizeof(das_ObjPosData);
                asset.offsets.nor_offset = m_buffer_data.main_buffer_size +
                    asset.vertices.v3d.pn * sizeof(das_ObjPosData) + 
                    asset.vertices.v3d.tn * sizeof(das_ObjTextureData);

                m_buffer_data.main_buffer_size += asset.vertices.v3d.pn * sizeof(das_ObjPosData) +
                    asset.vertices.v3d.tn * sizeof(das_ObjTextureData) + 
                    asset.vertices.v3d.nn * sizeof(das_ObjNormalData);

                asset.offsets.ind_offset = m_buffer_data.main_buffer_size;
                m_buffer_data.main_buffer_size += 3 * asset.indices.n * sizeof(deng_ui32_t);
                break;
            
            default:
                RUN_ERR("deng::vulkan::__vk_ResourceManager::mkBuffers()", 
                    "Invalid asset vertices format for asset " + std::string(asset.uuid));
                break;
            }
        }

        
        deng_ui64_t __vk_BufferManager::__findMaxAssetSize(const dengMath::vec2<deng_ui32_t> &bounds) {
            VkDeviceSize max_size = 0;
            VkDeviceSize cur_size = 0;

            // For each asset in bounds check the size and find the largest one
            for(size_t i = bounds.first; i < bounds.second; i++) {
                // Retrieve assets from the registry
                RegType &reg_vk_asset = m_reg.retrieve (
                    m_assets[i], 
                    DENG_SUPPORTED_REG_TYPE_VK_ASSET
                );

                RegType &reg_asset = m_reg.retrieve (
                    reg_vk_asset.vk_asset.base_id,
                    DENG_SUPPORTED_REG_TYPE_ASSET
                );

                // Check how much memory the asset consumes by checking their mode, which
                // they used to allocate memory for the vertex buffer
                switch(reg_asset.asset.asset_mode) {
                case DAS_ASSET_MODE_2D_UNMAPPED:
                    cur_size = reg_asset.asset.vertices.v2d.pn * sizeof(das_ObjPosData);
                    break;

                case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
                    cur_size = reg_asset.asset.vertices.v2d.pn * sizeof(das_ObjPosData) +
                        reg_asset.asset.vertices.v2d.tn * sizeof(das_ObjTextureData);
                    break;

                case DAS_ASSET_MODE_3D_UNMAPPED:
                    cur_size = reg_asset.asset.vertices.v3d.pn * sizeof(das_ObjPosData) +
                        reg_asset.asset.vertices.v3d.nn * sizeof(das_ObjNormalData);
                    break;

                case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
                    cur_size = reg_asset.asset.vertices.v3d.pn * sizeof(das_ObjPosData) +
                        reg_asset.asset.vertices.v3d.tn * sizeof(das_ObjTextureData) +
                        reg_asset.asset.vertices.v3d.nn * sizeof(das_ObjNormalData);
                    break;

                default:
                    break;
                }

                // Perform maximum required memory check for the asset
                if(cur_size > max_size)
                    max_size = cur_size;
            }

            return max_size;
        }


        /*
         * Create main asset buffer and copy all assets data to it
         */
        void __vk_BufferManager::mkAssetBuffers (
            VkDevice device, 
            VkPhysicalDevice gpu, 
            VkCommandPool cmd_pool, 
            VkQueue g_queue
        ) {
            size_t i;
            m_buffer_data.main_buffer_size = 0;
            VkMemoryRequirements mem_req;

            // Find the total required buffer size and set correct offsets
            for(i = 0; i < m_assets.size(); i++) {
                RegType &reg_vk_asset = m_reg.retrieve(m_assets[i], 
                    DENG_SUPPORTED_REG_TYPE_VK_ASSET);

                RegType &reg_asset = m_reg.retrieve(reg_vk_asset.vk_asset.base_id, 
                    DENG_SUPPORTED_REG_TYPE_ASSET);
                
                // Find buffer offsets for the asset
                __findAssetOffsets(reg_asset.asset);
            }

            // Round the buffer size to the nearest base2 exponant
            m_buffer_data.main_buffer_size = cm_ToPow2I64(m_buffer_data.main_buffer_size);

            // Create and allocate memory for staging buffer
            mem_req = __vk_BufferCreator::makeBuffer(device, gpu, m_buffer_data.main_buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_buffer_data.staging_buffer);

            __vk_BufferCreator::allocateMemory(device, gpu, mem_req.size,
                m_buffer_data.staging_buffer_memory,  mem_req.memoryTypeBits, 
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

            // Bind staging buffer with its memory
            vkBindBufferMemory(device, m_buffer_data.staging_buffer, m_buffer_data.staging_buffer_memory, 0);
            
            // For each given asset populate staging buffer with asset data
            for(i = 0; i < m_assets.size(); i++) {
                RegType &reg_vk_asset = m_reg.retrieve (
                    m_assets[i], 
                    DENG_SUPPORTED_REG_TYPE_VK_ASSET
                );

                RegType &reg_asset = m_reg.retrieve (
                    reg_vk_asset.vk_asset.base_id,
                    DENG_SUPPORTED_REG_TYPE_ASSET
                );

                // Populate staging buffer memory with vertices data
                __AssetCpy asset_cpy;
                asset_cpy.cpyToBuffer(device, reg_asset.asset, m_buffer_data.staging_buffer_memory);
                
                // Copy asset uniform data to ubo buffer
                cpyAssetUniform(device, gpu, cmd_pool, g_queue, reg_vk_asset.vk_asset);
            }

            // Create new main buffer instance
            mem_req = __vk_BufferCreator::makeBuffer(device, gpu, m_buffer_data.main_buffer_size, 
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT, m_buffer_data.main_buffer);

            // Allocate memory for main buffer
            __vk_BufferCreator::allocateMemory(device, gpu, mem_req.size, m_buffer_data.main_buffer_memory, 
                mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            // Bind the main buffer with its memory
            vkBindBufferMemory(device, m_buffer_data.main_buffer, 
                m_buffer_data.main_buffer_memory, 0);

            // Copy data from staging buffer to main buffer
            __vk_BufferCreator::cpyBufferToBuffer(device, cmd_pool, g_queue, 
                m_buffer_data.staging_buffer, m_buffer_data.main_buffer, 
                m_buffer_data.main_buffer_size, 0, 0);

            LOG("Buffer size " + std::to_string(m_buffer_data.main_buffer_size));

            // Perform staging buffer cleanup
            vkDestroyBuffer(device, m_buffer_data.staging_buffer, NULL);
            vkFreeMemory(device, m_buffer_data.staging_buffer_memory, NULL);
        }


        /*
         * Copy asset color data to uniform buffer memory
         */
        void __vk_BufferManager::cpyAssetUniform (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            __vk_Asset &asset
        ) {
            // Retrieve the base asset and set its ubo offset
            RegType &reg_asset = m_reg.retrieve(asset.base_id, DENG_SUPPORTED_REG_TYPE_ASSET);
            reg_asset.asset.offsets.ubo_offset = m_buffer_data.ubo_size;

            // Set the ubo color according to the color data
            __vk_UniformColorData ubo;
            ubo.color = dengMath::vec4<deng_vec_t>{
                reg_asset.asset.color.col_r,
                reg_asset.asset.color.col_g,
                reg_asset.asset.color.col_b,
                reg_asset.asset.color.col_a
            };
            
            // Check if reg type is unmapped or not and set the flag accordingly
            if(reg_asset.asset.asset_mode == DAS_ASSET_MODE_2D_UNMAPPED ||
               reg_asset.asset.asset_mode == DAS_ASSET_MODE_3D_UNMAPPED ||
               reg_asset.asset.asset_mode == DAS_ASSET_MODE_3D_UNMAPPED ||
               !reg_asset.asset.tex_uuid) ubo.is_unmapped = true;
            else ubo.is_unmapped = reg_asset.asset.force_unmap; 

            // Increment the ubo buffer size
            size_t old_size = m_buffer_data.ubo_size;
            m_buffer_data.ubo_size += __max_frame_c * cm_FindChunkSize(m_gpu_limits.minUniformBufferOffsetAlignment,
                sizeof(__vk_UniformColorData));

            // Check if buffer reallocation is needed 
            if(m_buffer_data.ubo_size > m_buffer_data.ubo_cap) {
                // Require at least twice the capacity for assets
                __reallocUniformBufferMemory(device, gpu, cmd_pool, g_queue, 
                    std::max(m_buffer_data.ubo_asset_cap << 1, cm_ToPow2I64(m_buffer_data.ubo_size << 1)));
            }

            
            // For each frame copy the asset uniform to buffer
            for(size_t i = 0; i < __max_frame_c; i++) {
                // Copy all uniform data to buffer
                __vk_BufferCreator::cpyToBufferMem(device, sizeof(__vk_UniformColorData), 
                    (void*) &ubo, m_buffer_data.uniform_buffer_mem, old_size + i * cm_FindChunkSize(
                    m_gpu_limits.minUniformBufferOffsetAlignment, sizeof(__vk_UniformColorData)));
            }
        }


        /*
         * Update transfrom uniform data in the frame according to updated camera
         * and view matrix
         */
        void __vk_BufferManager::updateUboTransform (
            VkDevice device, 
            deng_ui32_t current_image, 
            Camera3D *p_cam
        ) {
            __vk_UniformTransformation ubo;
            ubo.transform = p_cam->getCameraMat();   
            ubo.view = p_cam->getViewMat();
            ubo.no_perspective = !p_cam->isPerspective();

            __vk_BufferCreator::cpyToBufferMem(device, sizeof(__vk_UniformTransformation),
                &ubo, m_buffer_data.uniform_buffer_mem, current_image * cm_FindChunkSize(
                m_gpu_limits.minUniformBufferOffsetAlignment, sizeof(__vk_UniformTransformation)));
        }

        
        /*
         * Replace data in main buffer with newer data from given asset vertices
         */
        void __vk_BufferManager::remapAssetVerts (
            VkDevice device,
            VkPhysicalDevice gpu,
            VkCommandPool cmd_pool,
            VkQueue g_queue,
            const dengMath::vec2<deng_ui32_t> &asset_bounds
        ) {
            // Find the largest asset size between the bounds
            deng_ui64_t max_mem = __findMaxAssetSize(asset_bounds);

            // Create staging buffer
            VkMemoryRequirements mem_req;
            mem_req = __vk_BufferCreator::makeBuffer(device, gpu, max_mem,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_buffer_data.staging_buffer);

            // Allocate memory for staging buffer
            __vk_BufferCreator::allocateMemory(device, gpu, mem_req.size,
                m_buffer_data.staging_buffer_memory, mem_req.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

            // Bind the staging buffer to its memory
            vkBindBufferMemory(device, m_buffer_data.staging_buffer,
                m_buffer_data.staging_buffer_memory, 0);

            // For each renewable asset perform vertices copy to the staging buffer
            VkDeviceSize offset = 0;
            VkDeviceSize size = 0;
            for(size_t i = asset_bounds.first; i < asset_bounds.second; i++) {
                // Retrieve asset from the registry
                RegType &reg_vk_asset = m_reg.retrieve (
                    m_assets[i],
                    DENG_SUPPORTED_REG_TYPE_VK_ASSET
                );

                RegType &reg_asset = m_reg.retrieve (
                    reg_vk_asset.vk_asset.base_id,
                    DENG_SUPPORTED_REG_TYPE_ASSET
                );

                // Decrement all offsets by pos_offset so that the staging buffer's initial asset offset is 0
                deng_ui64_t bpos_offset = reg_asset.asset.offsets.pos_offset;
                reg_asset.asset.offsets.pos_offset -= bpos_offset;
                reg_asset.asset.offsets.tex_offset -= bpos_offset;
                reg_asset.asset.offsets.nor_offset -= bpos_offset;
                reg_asset.asset.offsets.ind_offset -= bpos_offset;

                // Copy all data to staging buffer
                __AssetCpy asset_cpy;
                asset_cpy.cpyToBuffer(device, reg_asset.asset, m_buffer_data.staging_buffer_memory);

                // Increment all offsets to their previous values
                reg_asset.asset.offsets.pos_offset += bpos_offset;
                reg_asset.asset.offsets.tex_offset += bpos_offset;
                reg_asset.asset.offsets.nor_offset += bpos_offset;
                reg_asset.asset.offsets.ind_offset += bpos_offset;

                // Copy from staging buffer to main buffer
                __vk_BufferCreator::cpyBufferToBuffer(device, cmd_pool, g_queue, m_buffer_data.staging_buffer, 
                    m_buffer_data.main_buffer, size, 0, offset);
            }

            // Free all resources that were used in staging buffers
            vkDestroyBuffer(device, m_buffer_data.staging_buffer, NULL);
            vkFreeMemory(device, m_buffer_data.staging_buffer_memory, NULL);
        }
    }
}
