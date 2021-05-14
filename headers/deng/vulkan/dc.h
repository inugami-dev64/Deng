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


#ifndef __DC_H
#define __DC_H


#ifdef __DC_CPP
    #include <vector>
    #include <array>
	#include <string>
    
    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
    #include <common/hashmap.h>
    #include <common/err_def.h>
    #include <data/assets.h>

    #include <math/deng_math.h>
    #include <deng/window.h>
    #include <deng/vulkan/sd.h>
    #include <deng/vulkan/qm.h>
    #include <deng/vulkan/resources.h>
    
    #include <deng/vulkan/rend_infos.h>
    #include <deng/vulkan/pipeline_data.h>
    #include <deng/vulkan/pipelines.h>
    #include <deng/lighting/light_srcs.h>
    #include <deng/registry/registry.h>

#endif


namespace deng {
    namespace vulkan {

        /* 
         * Class for making drawcalls and setting up proper synchronisation 
         */
        class __vk_DrawCaller {
        private:
            std::vector<deng_Id> &m_assets;
            std::vector<deng_Id> &m_textures;
            deng::__GlobalRegistry &m_reg;
            std::vector<VkFramebuffer> m_framebuffers;
            std::array<__vk_PipelineData, PIPELINE_C> m_pl_data;
            __vk_QueueManager m_qff;

            // Commandpools and commandbuffers
            VkCommandPool m_cmd_pool;
            std::vector<VkCommandBuffer> m_cmd_bufs;

        private:
            void __mkSynchronisation(VkDevice &device);

            /// Asset commandbuffer binder methods
            void __bindVertexResourceBuffers (
                das_Asset &asset, 
                VkCommandBuffer cur_buf,
                __vk_BufferData &bd
            );

            
            /*
             * Bind asset pipeline and return its pipeline layout
             */
            VkPipelineLayout *__bindPipeline(das_Asset &asset, VkCommandBuffer cmd_buf);

        public:
            // Needed for synchronising frames
            deng_ui32_t current_frame = 0;
            std::vector<VkFence> flight_fences;
            std::vector<VkSemaphore> image_available_semaphore_set;
            std::vector<VkSemaphore> render_finished_semaphore_set;

        public:
            __vk_DrawCaller (
                VkDevice device,
                __vk_QueueManager qff,
                std::vector<deng_Id> &assets,
                std::vector<deng_Id> &textures,
                deng::__GlobalRegistry &reg
            );
            
            void setMiscData (
                const std::array<__vk_PipelineData, PIPELINE_C> &pl_data, 
                const std::vector<VkFramebuffer> &fb
            );

            void mkCommandPool(VkDevice device);

            void allocateMainCmdBuffers (
                VkDevice device, 
                VkQueue g_queue, 
                VkRenderPass renderpass, 
                VkExtent2D ext,
                dengMath::vec4<deng_vec_t> background,
                __vk_BufferData &bd
            );

            void recordMainCmdBuffers (
                VkRenderPass renderpass,
                VkExtent2D ext,
                const dengMath::vec4<deng_vec_t> &background,
                __vk_BufferData &bd
            );
        
        public:
            VkCommandPool getComPool();
            const std::vector<VkCommandBuffer> &getComBufs();
        };
    }
}

#endif
