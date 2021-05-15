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


#define __DESC_SET_LAYOUT_CPP
#include <deng/vulkan/desc_set_layout.h>


namespace deng {
    namespace vulkan {

        __vk_DescriptorSetLayoutCreator::__vk_DescriptorSetLayoutCreator(VkDevice device) {
            // Create all Vulkan descriptor layouts
            __mkVu2DLayout(device);
            __mkVm2DLayout(device);
            __mkVu3DLayout(device);
            __mkVm3DLayout(device);
        }


        /// Find the binding data according to the asset mode
        std::vector<VkDescriptorSetLayoutBinding> __vk_DescriptorSetLayoutCreator::__findBindings(das_AssetMode asset_mode) {
            std::vector<VkDescriptorSetLayoutBinding> bindings(2);

            // Transformation uniform data
            bindings[0].binding = 0;
            bindings[0].descriptorCount = 1;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].pImmutableSamplers = NULL;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            // Color uniform data
            bindings[1].binding = 1;
            bindings[1].descriptorCount = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].pImmutableSamplers = NULL;
            bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            // Check if any other binding are needed according to the asset mode
            switch(asset_mode) {
            case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
                bindings.resize(3);

                // Texture image sampler
                bindings[2].binding = 2;
                bindings[2].descriptorCount = 1;
                bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                bindings[2].pImmutableSamplers = NULL;
                bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;

            case DAS_ASSET_MODE_3D_UNMAPPED:
                bindings.resize(3);
                
                // Lighting data
                bindings[2].binding = 2;
                bindings[2].descriptorCount = 1;
                bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                bindings[2].pImmutableSamplers = NULL;
                bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;

            case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
                bindings.resize(4);
                
                // Lighting data
                bindings[2].binding = 2;
                bindings[2].descriptorCount = 1;
                bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                bindings[2].pImmutableSamplers = NULL;
                bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

                // Texture image sampler
                bindings[3].binding = 3;
                bindings[3].descriptorCount = 1;
                bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                bindings[3].pImmutableSamplers = NULL;
                bindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;

            default:
                break;
            }

            return bindings;
        }


        /// Create new descriptor set layout
        void __vk_DescriptorSetLayoutCreator::__mkGenericDescSetLayout (
            VkDevice device, 
            std::vector<VkDescriptorSetLayoutBinding> &bindings,
            VkDescriptorSetLayout *p_layout
        ) {
            VkDescriptorSetLayoutCreateInfo createinfo{};
            createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            createinfo.bindingCount = static_cast<deng_ui32_t>(bindings.size());
            createinfo.pBindings = bindings.data();

            if(vkCreateDescriptorSetLayout(device, &createinfo, NULL, p_layout) != VK_SUCCESS)
                VK_DESC_ERR("failed to create descriptor set layout");
        }


        void __vk_DescriptorSetLayoutCreator::__mkVu2DLayout(VkDevice device) {
            // Find the layout bindings
            std::vector<VkDescriptorSetLayoutBinding> bindings = __findBindings(
                DAS_ASSET_MODE_2D_UNMAPPED);

            // Create the layout
            __mkGenericDescSetLayout(device, bindings, &m_vu2d_layout);
        }


        void __vk_DescriptorSetLayoutCreator::__mkVm2DLayout(VkDevice device) {
            // Find the layout bindings
            std::vector<VkDescriptorSetLayoutBinding> bindings = __findBindings(
                DAS_ASSET_MODE_2D_TEXTURE_MAPPED);

            // Create the layout
            __mkGenericDescSetLayout(device, bindings, &m_vm2d_layout);
        }


        void __vk_DescriptorSetLayoutCreator::__mkVu3DLayout(VkDevice device) {
            // Find the layout bindings
            std::vector<VkDescriptorSetLayoutBinding> bindings = __findBindings(
                DAS_ASSET_MODE_3D_UNMAPPED);

            // Create the layout
            __mkGenericDescSetLayout(device, bindings, &m_vu3d_layout);
        }


        void __vk_DescriptorSetLayoutCreator::__mkVm3DLayout(VkDevice device) {
            // Find the layout bindings
            std::vector<VkDescriptorSetLayoutBinding> bindings = __findBindings(
                DAS_ASSET_MODE_3D_TEXTURE_MAPPED);

            // Create the layout
            __mkGenericDescSetLayout(device, bindings, &m_vm3d_layout);
        }


        VkDescriptorSetLayout &__vk_DescriptorSetLayoutCreator::getLayout(das_AssetMode asset_mode) {
            // Check the asset mode and return correct layout
            switch(asset_mode) {
                case DAS_ASSET_MODE_2D_UNMAPPED: return m_vu2d_layout;
                case DAS_ASSET_MODE_2D_TEXTURE_MAPPED: return m_vm2d_layout;
                case DAS_ASSET_MODE_3D_UNMAPPED: return m_vu3d_layout;
                case DAS_ASSET_MODE_3D_TEXTURE_MAPPED: return m_vm3d_layout;
                    
                // Throw error because no layout is available
                default:
                    RUN_ERR("__vk_DescriptorSetLayoutCreator::getLayout()",
                        "Invalid asset mode");
                    break;
            }
        }
    }
}