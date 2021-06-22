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
 *  Name: rm - Resource manager for Vulkan
 *  Purpose: Provide class interface for allocating, reallocating and 
 *  changing buffer contents for textures, assets and uniform data.
 *  Author: Karl-Mihkel Ott
 */ 


#ifndef __VK_RM_H
#define __VK_RM_H

#ifdef __VK_RM_CPP
    #include <vector>
    #include <array>
    #include <queue>
    #include <mutex>
    #include <cmath>
	#include <algorithm>

    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
    #include <common/hashmap.h>
    #include <common/uuid.h>
    #include <common/common.h>
    #include <common/err_def.h>
    #include <data/assets.h>

    #include <math/deng_math.h>
    #include <deng/cross_api/gpu_mem.h>
    #include <deng/window.h>
    #include <deng/camera.h>

    #include <deng/vulkan/asset_cpy.h>
    #include <deng/vulkan/sd.h>
    #include <deng/vulkan/qm.h>
    #include <deng/vulkan/resources.h>
    #include <deng/vulkan/rend_infos.h>

    #include <imgui-layer/imgui_entity.h>
    #include <deng/lighting/light_srcs.h>
    #include <deng/registry/registry.h>
    #include <deng/cross_api/offset_finder.h>
    #include <imgui-layer/imgui_entity.h>

    #include <deng/vulkan/ubm.h>
    #include <deng/vulkan/bm.h>
    #include <deng/vulkan/tm.h>

#endif


namespace deng {
    namespace vulkan {

        class __vk_ResourceManager : public __vk_BufferManager, public __vk_TextureManager {
        private:
            // Color image instances
            VkImage m_color_image;
            VkDeviceMemory m_color_image_mem;
            VkImageView m_color_image_view;

            // Depth image instances
            VkImage m_depth_image;
            VkDeviceMemory m_depth_image_mem;
            VkImageView m_depth_image_view;

            std::vector<VkFramebuffer> m_framebuffers;
            VkSampleCountFlagBits m_sample_c;
            deng_ui32_t m_tex_mem_bits = 0;

        private:

            void __mkFrameBuffers(VkDevice &device, VkRenderPass &renderpass, 
                VkExtent2D &ext, const std::vector<VkImageView> &sc_img_views);

            void __mkColorResources(VkDevice &device, VkPhysicalDevice &gpu,
                VkExtent2D &ext, VkFormat sc_color_format);

            void __mkDepthResources(VkDevice &device, VkPhysicalDevice &gpu, VkExtent2D &ext);

            
        public:
            __vk_ResourceManager(VkDevice device, VkPhysicalDevice gpu, VkExtent2D ext, VkSampleCountFlagBits sample_c,
                VkRenderPass renderpass, VkCommandPool cmd_pool, VkQueue g_queue, const std::vector<VkImageView> &sc_img_views,
                deng::__GlobalRegistry &reg, std::vector<deng_Id> &assets, std::vector<deng_Id> &textures, VkFormat sc_color_format,
                const VkPhysicalDeviceLimits &gpu_limits);


        // Getter and setter methods
        public:
            __vk_BufferData &getBD();
            std::vector<VkFramebuffer> getFB();
            VkImage getDepImg();
            VkDeviceMemory getDepImgMem();
            VkImageView getDepImgView();
            VkImage getColorImg();
            VkDeviceMemory getColorImgMem();
            VkImageView getColorImgView();

            void setUIDataPtr(__ImGuiData *p_gui);
        };
    }
}

#endif
