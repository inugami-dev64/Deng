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


#define __DESC_POOL_C_CPP
#include <deng/vulkan/desc_pool.h>


namespace deng {
    namespace vulkan {
        /// Maximum frame in flight count specifier
        extern deng_ui32_t __max_frame_c;

        void __vk_DescriptorPoolCreator::__mk2DUnmappedDescPool(VkDevice device) {
            std::array<VkDescriptorPoolSize, 2> desc_pool_sizes;
            VkDescriptorPoolCreateInfo desc_pool_info{};

            // The first uniform buffer is the 2D transformation uniform
            desc_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[0].descriptorCount = m_vu2d_cap * __max_frame_c;

            // The second uniform buffer is the asset color uniform data
            desc_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[1].descriptorCount = m_vu2d_cap * __max_frame_c;

            // Set the createinfo
            desc_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            desc_pool_info.poolSizeCount = static_cast<deng_ui32_t>(desc_pool_sizes.size());
            desc_pool_info.pPoolSizes = desc_pool_sizes.data();
            desc_pool_info.maxSets = m_vu2d_cap * __max_frame_c;
            
            // Try to create descriptor pool
            if(vkCreateDescriptorPool(device, &desc_pool_info, NULL, 
               &m_vu2d_pool) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor pool for 2D unmapped assets");
        }


        void __vk_DescriptorPoolCreator::__mk2DTexMappedDescPool(VkDevice device) {
            m_vu2d_cap = m_vm2d_cap * __max_frame_c;
            std::array<VkDescriptorPoolSize, 3> desc_pool_sizes;
            VkDescriptorPoolCreateInfo desc_pool_info{};

            // The first uniform binding is the 2D transformation uniform
            desc_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[0].descriptorCount = m_vm2d_cap * __max_frame_c;

            // The second uniform binding is the asset color uniform data
            desc_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[1].descriptorCount = m_vm2d_cap * __max_frame_c;

            // The third uniform binding is texture image sampler
            desc_pool_sizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            desc_pool_sizes[2].descriptorCount = m_vm2d_cap * __max_frame_c;

            // Set the createinfo
            desc_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            desc_pool_info.poolSizeCount = static_cast<deng_ui32_t>(desc_pool_sizes.size());
            desc_pool_info.pPoolSizes = desc_pool_sizes.data();
            desc_pool_info.maxSets = m_vm2d_cap * __max_frame_c;
            
            // Try to create descriptor pool
            if(vkCreateDescriptorPool(device, &desc_pool_info, NULL, 
               &m_vm2d_pool) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor pool for 2D texture mapped assets");
        }


        void __vk_DescriptorPoolCreator::__mk3DUnmappedDescPool(VkDevice device) {
            m_vu2d_cap = m_vu3d_cap * __max_frame_c;
            std::array<VkDescriptorPoolSize, 3> desc_pool_sizes;
            VkDescriptorPoolCreateInfo desc_pool_info{};

            // The first uniform binding is the 2D transformation uniform
            desc_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[0].descriptorCount = m_vu3d_cap * __max_frame_c;

            // The second uniform binding is the asset color uniform data
            desc_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[1].descriptorCount = m_vu3d_cap * __max_frame_c;

            // The third uniform binding is the lighting uniform data
            desc_pool_sizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[2].descriptorCount = m_vu3d_cap * __max_frame_c;

            // Set the createinfo
            desc_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            desc_pool_info.poolSizeCount = static_cast<deng_ui32_t>(desc_pool_sizes.size());
            desc_pool_info.pPoolSizes = desc_pool_sizes.data();
            desc_pool_info.maxSets = m_vu3d_cap * __max_frame_c;
            
            // Try to create descriptor pool
            if(vkCreateDescriptorPool(device, &desc_pool_info, NULL, 
               &m_vu3d_pool) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor pool for unmapped assets");
        }


        void __vk_DescriptorPoolCreator::__mk3DTexMappedDescPool(VkDevice device) {
            m_vu2d_cap = m_vm3d_cap * __max_frame_c;
            std::array<VkDescriptorPoolSize, 4> desc_pool_sizes;
            VkDescriptorPoolCreateInfo desc_pool_info{};

            // The first uniform binding is the 2D transformation uniform
            desc_pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[0].descriptorCount = m_vm3d_cap * __max_frame_c;

            // The second uniform binding is the asset color uniform data
            desc_pool_sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_pool_sizes[1].descriptorCount = m_vm3d_cap * __max_frame_c;

            // The third uniform binding is texture image sampler
            desc_pool_sizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            desc_pool_sizes[2].descriptorCount = m_vm3d_cap * __max_frame_c;

            // The fourth uniform binding is lighting uniform data
            desc_pool_sizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            desc_pool_sizes[3].descriptorCount = m_vm3d_cap * __max_frame_c;

            // Set the createinfo
            desc_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            desc_pool_info.poolSizeCount = static_cast<deng_ui32_t>(desc_pool_sizes.size());
            desc_pool_info.pPoolSizes = desc_pool_sizes.data();
            desc_pool_info.maxSets = m_vm3d_cap * __max_frame_c;
            
            // Try to create descriptor pool
            if(vkCreateDescriptorPool(device, &desc_pool_info, NULL, 
               &m_vm3d_pool) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor pool for unmapped assets");
        }


        /// Destroy assets with specific asset mode
        void __vk_DescriptorPoolCreator::__destroyAssetDescriptorSets ( 
            VkDevice device, 
            std::vector<deng_Id> &destroyed, 
            das_AssetMode asset_mode
        ) {
            // Iterate through every asset and check if they need to have 
            // descriptor sets freed
            for(size_t i = 0; i < m_assets.size(); i++) {
                // Retrieve the asset from registry
                RegType &vk_asset = m_reg.retrieve(m_assets[i], DENG_SUPPORTED_REG_TYPE_VK_ASSET);
                RegType &reg_asset = m_reg.retrieve(vk_asset.vk_asset.base_id, DENG_SUPPORTED_REG_TYPE_VK_ASSET);

                // Check if the asset has descriptor sets allocated
                if(vk_asset.vk_asset.is_desc && reg_asset.asset.asset_mode == asset_mode) {
                    // Check the asset mode and free descriptor sets
                    switch(asset_mode) {
                    case DAS_ASSET_MODE_2D_UNMAPPED:
                        vkFreeDescriptorSets(device, m_vu2d_pool, 
                            vk_asset.vk_asset.desc_c, vk_asset.vk_asset.desc_sets);
                        break;

                    case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
                        vkFreeDescriptorSets(device, m_vm2d_pool, 
                            vk_asset.vk_asset.desc_c, vk_asset.vk_asset.desc_sets);
                        break;

                    case DAS_ASSET_MODE_3D_UNMAPPED:
                        vkFreeDescriptorSets(device, m_vu2d_pool, 
                            vk_asset.vk_asset.desc_c, vk_asset.vk_asset.desc_sets);
                        break;

                    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
                        vkFreeDescriptorSets(device, m_vu2d_pool, 
                            vk_asset.vk_asset.desc_c, vk_asset.vk_asset.desc_sets);
                        break;
                    
                    default:
                        break;
                    }

                    // Free memory allocated for descriptor set instances
                    free(vk_asset.vk_asset.desc_sets);
                    vk_asset.vk_asset.is_desc = false;
                    vk_asset.vk_asset.desc_c = 0;
                    
                    // Push the current id to destroyed data vector
                    destroyed.push_back(m_assets[i]);
                }
            }
        }


        /// Reallocate specific descriptor pool for specific asset type
        void __vk_DescriptorPoolCreator::__reallocDescPool(
            VkDevice device,
            std::vector<deng_Id> &destroyed_assets,    
            das_AssetMode asset_mode, 
            deng_ui32_t req_c
        ) {
            // Check the asset mode and reallocate descriptor pool accordingly
            switch(asset_mode) {
            case DAS_ASSET_MODE_2D_UNMAPPED:
                m_vu2d_cap = m_vu2d_cap << 1 < cm_ToPow2I32(req_c * 2) ? 
                             cm_ToPow2I32(req_c * 2) : m_vu2d_cap << 1;
                
                // Destroy previous descriptor sets and pools
                __destroyAssetDescriptorSets(device, destroyed_assets, DAS_ASSET_MODE_2D_UNMAPPED);
                vkDestroyDescriptorPool(device, m_vu2d_pool, NULL);
                mkDescPool(device, m_vu2d_cap, DAS_ASSET_MODE_2D_UNMAPPED);
                break;

            case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
                m_vm2d_cap = m_vm2d_cap << 1 < cm_ToPow2I32(req_c * 2) ? 
                             cm_ToPow2I32(req_c * 2) : m_vm2d_cap << 1;
                
                // Destroy previous descriptor sets and pools
                __destroyAssetDescriptorSets(device, destroyed_assets, DAS_ASSET_MODE_2D_TEXTURE_MAPPED);
                vkDestroyDescriptorPool(device, m_vm2d_pool, NULL);
                mkDescPool(device, m_vm2d_cap, DAS_ASSET_MODE_2D_TEXTURE_MAPPED);
                break;

            case DAS_ASSET_MODE_3D_UNMAPPED:
                m_vu3d_cap = m_vu3d_cap << 1 < cm_ToPow2I32(req_c * 2) ? 
                             cm_ToPow2I32(req_c * 2) : m_vu3d_cap << 1;
                
                // Destroy previous descriptor sets and pools
                __destroyAssetDescriptorSets(device, destroyed_assets, DAS_ASSET_MODE_3D_UNMAPPED);
                vkDestroyDescriptorPool(device, m_vu3d_pool, NULL);
                mkDescPool(device, m_vu3d_cap, DAS_ASSET_MODE_3D_UNMAPPED);
                break;

            case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
                m_vm3d_cap = m_vm3d_cap << 1 < cm_ToPow2I32(req_c * 2) ? 
                             cm_ToPow2I32(req_c * 2) : m_vm3d_cap << 1;
                
                // Destroy previous descriptor sets and pools
                __destroyAssetDescriptorSets(device, destroyed_assets, DAS_ASSET_MODE_3D_TEXTURE_MAPPED);
                vkDestroyDescriptorPool(device, m_vm3d_pool, NULL);
                mkDescPool(device, m_vm3d_cap, DAS_ASSET_MODE_3D_TEXTURE_MAPPED);
                break;

            default:
                break;
            }
        }


        /// Create a new descriptor pool based on the asset mode
        void __vk_DescriptorPoolCreator::mkDescPool (
            VkDevice device, 
            deng_ui32_t cap, 
            das_AssetMode mode
        ) {
            // Check for the given asset mode
            switch(mode) {
            case DAS_ASSET_MODE_2D_UNMAPPED:
                m_vu2d_cap = cap;
                __mk2DUnmappedDescPool(device);
                break;

            case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
                m_vm2d_cap = cap;
                __mk2DTexMappedDescPool(device);
                break;

            case DAS_ASSET_MODE_3D_UNMAPPED:
                m_vu3d_cap = cap;
                __mk3DUnmappedDescPool(device);
                break;

            case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
                m_vm3d_cap = cap;
                __mk3DTexMappedDescPool(device);
                break;

            default:
                // Throw runtime error, because asset mode given was invalid
                RUN_ERR("__vk_DescriptorPoolCreator::mkDescPool()", 
                    "Invalid given asset mode");
                break;
            }
        }


        /// Create new descriptor pools for all asset types
        void __vk_DescriptorPoolCreator::mkDescPools(VkDevice device, deng_ui32_t cap) {
            mkDescPool(device, cap, DAS_ASSET_MODE_2D_UNMAPPED);
            mkDescPool(device, cap, DAS_ASSET_MODE_2D_TEXTURE_MAPPED);
            mkDescPool(device, cap, DAS_ASSET_MODE_3D_UNMAPPED);
            mkDescPool(device, cap, DAS_ASSET_MODE_3D_TEXTURE_MAPPED);
        }


        /// Check if the descriptor pools need to be reallocated and reallocate
        /// the pool if needed
        __vk_DescriptorPoolReallocBits __vk_DescriptorPoolCreator::checkPoolReallocation (
            VkDevice device,
            std::vector<deng_Id> &destroyed_assets,
            deng_ui32_t vu2d_c, 
            deng_ui32_t vm2d_c, 
            deng_ui32_t vu3d_c,
            deng_ui32_t vm3d_c
        ) {
            __vk_DescriptorPoolReallocBits res = DENG_VK_DESCRIPTOR_POOL_REALLOC_RESULT_NO_REALLOC;

            // Check if unmapped 2D asset pool needs to be reallocated
            if(vu2d_c >= m_vu2d_cap) {
                __reallocDescPool(device, destroyed_assets, DAS_ASSET_MODE_2D_UNMAPPED,
                    vu2d_c);
                res |= DENG_VK_DESCRIPTOR_POOL_REALLOC_RESULT_REALLOC_VU2D;
            }

            // Check if texture mapped 2D asset pool needs to be reallocated
            if(vu2d_c >= m_vu2d_cap) {
                __reallocDescPool(device, destroyed_assets, DAS_ASSET_MODE_2D_TEXTURE_MAPPED,
                    vm2d_c);
                res |= DENG_VK_DESCRIPTOR_POOL_REALLOC_RESULT_REALLOC_VM2D;
            }

            // Check if unmapped 2D asset pool needs to be reallocated
            if(vu2d_c >= m_vu2d_cap) {
                __reallocDescPool(device, destroyed_assets, DAS_ASSET_MODE_3D_UNMAPPED,
                    vm2d_c);
                res |= DENG_VK_DESCRIPTOR_POOL_REALLOC_RESULT_REALLOC_VU3D;
            }

            // Check if texture mapped 3D asset pool needs to be reallocated
            if(vu2d_c >= m_vu2d_cap) {
                __reallocDescPool(device, destroyed_assets, DAS_ASSET_MODE_3D_UNMAPPED,
                    vm2d_c);
                res |= DENG_VK_DESCRIPTOR_POOL_REALLOC_RESULT_REALLOC_VU3D;
            }

            return res;
        }


        /// Get the descriptor pool according to the given method
        VkDescriptorPool &__vk_DescriptorPoolCreator::getDescPool(das_AssetMode mode) {
            // Check for the asset mode given
            switch(mode) {
            case DAS_ASSET_MODE_2D_UNMAPPED: 
                return m_vu2d_pool;

            case DAS_ASSET_MODE_2D_TEXTURE_MAPPED: 
                return m_vm2d_pool;

            case DAS_ASSET_MODE_3D_UNMAPPED: 
                return m_vu3d_pool;

            case DAS_ASSET_MODE_3D_TEXTURE_MAPPED: 
                return m_vm3d_pool;

            default:
                // Throw runtime error, because asset mode given was invalid
                RUN_ERR("__vk_DescriptorPoolCreator::getDescPool()", 
                    "Invalid given asset mode");
                break;
            }
        }
    }
}
