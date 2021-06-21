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
 * ----------------------------------------------------------------
 *  Name: vk_bm - buffer manager for Vulkan resource manager
 *  Purpose: Provide a parent class for resource manager, which handles
 *  all buffer related operations such as allocations, reallocations and
 *  data writings.
 *  Author: Karl-Mihkel Ott
 */ 


#ifndef __VK_BM_H
#define __VK_BM_H

#ifdef __VK_BM_CPP
    #include <vector>
    #include <string>
    #include <mutex>
    #include <queue>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <common/common.h>
    #include <common/hashmap.h>
    #include <data/assets.h>

    #include <math/deng_math.h>
    #include <deng/surface/key_definitions.h>
	#include <deng/window.h>
    #include <deng/camera.h>
    #include <deng/lighting/light_srcs.h>
    #include <deng/vulkan/resources.h>
    #include <deng/registry/registry.h>
    #include <deng/vulkan/ubo.h>
    #include <deng/vulkan/ubm.h>
    #include <deng/vulkan/asset_cpy.h>

    #include <imgui-layer/imgui_entity.h>

    #define DEF_ASSET_CAP       65536   // Initialise with 64KB of memory for assets
    #define DEF_UI_CAP          32768   // Initialise with 32KB of memory for ui elements
#endif

namespace deng {
    namespace vulkan {

        class __vk_BufferManager : public __vk_UniformBufferManager {
        private:
            std::vector<deng_Id> &m_assets;
            const VkPhysicalDeviceLimits &m_gpu_limits;
            __ImGuiData *m_p_imgui_data = NULL;
            deng_ui64_t m_ubo_chunk_size;
            deng::__GlobalRegistry &m_reg;

        protected:
            __vk_BufferData m_buffer_data;

        private:
            void __setupMainBuffer(VkDevice device, VkPhysicalDevice gpu);


            /// Create staging buffers for all asset data between bounds
            void __stageAssets(VkDevice device, VkPhysicalDevice gpu, VkCommandPool cmd_pool, VkQueue g_queue, 
                const dengMath::vec2<deng_ui32_t> &bounds, VkDeviceSize cpy_offset);


            /// Find offsets for the current asset
            void __findAssetOffsets(das_Asset &asset);


            /// Find offsets for all ImGui entities 
            void __findGuiEntitiesOffsets();


        protected:
            __vk_BufferManager(VkDevice device, VkPhysicalDevice gpu, const VkPhysicalDeviceLimits &gpu_limits, 
                std::vector<deng_Id> &assets, deng::__GlobalRegistry &reg);

            deng_ui64_t __findMaxAssetSize(const dengMath::vec2<deng_ui32_t> &bounds);


        public:
            /// Check if buffer reallocation is needed for assets and gui elements
            deng_bool_t reallocCheck(VkDevice device, VkPhysicalDevice gpu, VkCommandPool cmd_pool, VkQueue g_queue,
                const std::vector<VkFence> &fences);


            /// Check if the current capacity is enough for assets and if it isn't resize the capacity and return true
            deng_bool_t assetCapCheck();


            /// Check if the current capacity is enough for ImGui elements and if it isn't resize the capacity and return true
            deng_bool_t uiCapCheck();


            /// Allocate the reserved memory
            void allocateMainBufferMemory(VkDevice device, VkPhysicalDevice gpu);


            /// Copy all asset data between given bounds to buffer
            /// NOTE: The asset capacity in buffer has to be larger than required asset size (use assetCapCheck() for this)
            void cpyAssetsToBuffer(VkDevice device, VkPhysicalDevice gpu, 
                VkCommandPool cmd_pool, VkQueue g_queue, deng_bool_t no_offset_calc, const dengMath::vec2<deng_ui32_t> &bounds);


            /// Copy ImGui vertex and indices data to buffer
            /// NOTE: The UI element capacity has to be larger than required UI element size (use uiCapCheck() for this)
            void cpyUIDataToBuffer(VkDevice device, VkPhysicalDevice gpu,
                VkCommandPool cmd_pool, VkQueue g_queue, deng_bool_t no_offset_calc);

        // Setter methods
        public:
            void setUIDataPtr(__ImGuiData *p_gui);
        };
    }
}
#endif
