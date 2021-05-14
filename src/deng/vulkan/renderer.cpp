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


#define __RENDERER_CPP
#include <deng/vulkan/renderer.h>

/// External mouse data variable declaration
deng::__SharedMouseData __ext_md;

namespace deng {
    namespace vulkan {
        extern deng_ui32_t __max_frame_c;
        
        /********************************************/
        /********************************************/
        /*********** __vk_Renderer Class ************/
        /********************************************/

        __vk_Renderer::__vk_Renderer (
            deng::Window &win,
            const __vk_ConfigVars &cnf,
            deng::__GlobalRegistry &reg
        ) : __vk_RendererInitialiser(win, cnf, reg), 
            __vk_RuntimeUpdater(getIC(), getSCC(), getDrawCaller(),
                getResMan(), __vk_RendererInitialiser::m_assets, __vk_RendererInitialiser::m_textures),
            m_win(win) { 
            m_config = cnf;
        }


        __vk_Renderer::~__vk_Renderer() {
            idle();
            __cleanup();
        }


        /*
         * Free all depth and color image data
         */
        void __vk_Renderer::__cleanRendImgResources() {
            // Clean depth image resources
            vkDestroyImageView (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getDepImgView(), 
                NULL
            );

            vkDestroyImage (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getDepImg(), 
                NULL
            );

            vkFreeMemory (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getDepImgMem(), 
                NULL
            );

            // Clean color image resources
            vkDestroyImageView (
                __vk_RendererInitialiser::getIC()->getDev(),
                __vk_RendererInitialiser::getResMan()->getColorImgView(),
                NULL
            );

            vkDestroyImage (
                __vk_RendererInitialiser::getIC()->getDev(),
                __vk_RendererInitialiser::getResMan()->getColorImg(),
                NULL
            );

            vkFreeMemory (
                __vk_RendererInitialiser::getIC()->getDev(),
                __vk_RendererInitialiser::getResMan()->getColorImgMem(),
                NULL
            );
        }


        void __vk_Renderer::__cleanDrawCommands() {
            // Clean framebuffers
            std::vector<VkFramebuffer> fb;
            fb = __vk_RendererInitialiser::getResMan()->getFB();
            for(size_t i = 0; i < fb.size(); i++) {
                vkDestroyFramebuffer (
                    __vk_RendererInitialiser::getIC()->getDev(), 
                    fb[i], 
                    NULL
                );
            }

            // Clean commandbuffers and commandpools
            vkFreeCommandBuffers (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDrawCaller()->getComPool(), 
                static_cast<deng_ui32_t>(__vk_RendererInitialiser::getDrawCaller()->getComBufs().size()), 
                __vk_RendererInitialiser::getDrawCaller()->getComBufs().data()
            );
        }

        
        void __vk_Renderer::__cleanTextures() {
            for(size_t i = 0; i < m_textures.size(); i++) {
                RegType &reg_vk_tex = m_reg.retrieve (
                    m_textures[i], 
                    DENG_SUPPORTED_REG_TYPE_VK_TEXTURE
                );

                // Check if image has been buffered and if it then has destroy all of its Vulkan related data
                if(reg_vk_tex.vk_tex.is_buffered) {
                    // Destroy texture sampler
                    vkDestroySampler (
                        __vk_RendererInitialiser::getIC()->getDev(),
                        reg_vk_tex.vk_tex.sampler,
                        NULL
                    );

                    // Destroy texture's image views
                    vkDestroyImageView (
                        __vk_RendererInitialiser::getIC()->getDev(),
                        reg_vk_tex.vk_tex.image_view,
                        NULL
                    );

                    // Destroy texture image 
                    vkDestroyImage (
                        __vk_RendererInitialiser::getIC()->getDev(),
                        reg_vk_tex.vk_tex.image,
                        NULL
                    );
                }
            }

            // Free all memory allocated for texture images
            vkFreeMemory (
                __vk_RendererInitialiser::getIC()->getDev(),
                __vk_RendererInitialiser::getResMan()->getBD().img_memory,
                NULL
            );
        }

        
        void __vk_Renderer::__cleanAssets() {
            for(size_t i = 0; i < m_assets.size(); i++) {
                RegType &reg_vk_asset = m_reg.retrieve (
                    m_assets[i],
                    DENG_SUPPORTED_REG_TYPE_VK_ASSET
                );

                // Check if asset has descriptor sets allocated and if it does then destroy them
                if(reg_vk_asset.vk_asset.is_desc) {
                    // Retrieve the base asset
                    RegType &reg_asset = m_reg.retrieve (
                        reg_vk_asset.vk_asset.base_id,
                        DENG_SUPPORTED_REG_TYPE_ASSET
                    );

                    // Free descriptor sets
                    vkFreeDescriptorSets(__vk_RendererInitialiser::getIC()->getDev(), __vk_RendererInitialiser::getDescC()->getDescPool(reg_asset.asset.asset_mode),
                        static_cast<deng_ui32_t>(reg_vk_asset.vk_asset.desc_c), reg_vk_asset.vk_asset.desc_sets);
                }
            }
        }

        
        void __vk_Renderer::__cleanPipelines() {
            // Clean pipeline related data
            std::array<__vk_PipelineData, PIPELINE_C> &pd = 
                __vk_RendererInitialiser::getPipelineC()->getPipelines();

            // For each pipeline data instance destroy its pipeline and layout
            for(size_t i = 0; i < pd.size(); i++) { 
                // Destroy pipeline
                vkDestroyPipeline(__vk_RendererInitialiser::getIC()->getDev(), 
                    pd[i].pipeline, NULL);

                // Destroy pipeline layout
                vkDestroyPipelineLayout(__vk_RendererInitialiser::getIC()->getDev(), 
                    *__vk_RendererInitialiser::getPipelineC()->getPipeline(i).p_pipeline_layout, 
                    NULL);
            }

            // Destroy renderpass
            vkDestroyRenderPass (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getSCC()->getRp(), 
                NULL
            );
        }


        void __vk_Renderer::__cleanDescPools() {
            // Destroy 2D unmapped asset descriptor pool 
            vkDestroyDescriptorPool(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getDescPool(
                    DAS_ASSET_MODE_2D_UNMAPPED), 
                NULL);

            // Destroy 2D texture mapped asset descriptor pool
            vkDestroyDescriptorPool(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getDescPool(
                    DAS_ASSET_MODE_2D_TEXTURE_MAPPED), 
                NULL);

            // Destroy 3D unmapped asset descriptor pool 
            vkDestroyDescriptorPool(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getDescPool(
                    DAS_ASSET_MODE_3D_UNMAPPED), 
                NULL);

            // Destroy 3D texture mapped asset descriptor pool
            vkDestroyDescriptorPool(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getDescPool(
                    DAS_ASSET_MODE_3D_TEXTURE_MAPPED), 
                NULL);
        }

        
        void __vk_Renderer::__cleanDescSetLayouts() {
            // Destroy 2D unmapped descriptor set layout
            vkDestroyDescriptorSetLayout(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getLayout(DAS_ASSET_MODE_2D_UNMAPPED), 
                NULL);

            // Destroy 2D texture mapped descriptor set layout
            vkDestroyDescriptorSetLayout(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getLayout(
                    DAS_ASSET_MODE_2D_TEXTURE_MAPPED), 
                NULL);

            // Destroy 3D unmapped descriptor set layout
            vkDestroyDescriptorSetLayout(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getLayout(DAS_ASSET_MODE_3D_UNMAPPED), 
                NULL);

            // Destroy 3D texture mapped descriptor set layout
            vkDestroyDescriptorSetLayout(__vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDescC()->getLayout(
                    DAS_ASSET_MODE_3D_TEXTURE_MAPPED), 
                NULL);
        }


        void __vk_Renderer::__freeBuffers() {
            // Clean ubo buffer data 
            vkDestroyBuffer (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getBD().uniform_buffer, 
                NULL
            );

            vkFreeMemory (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getBD().uniform_buffer_mem, 
                NULL
            );


            // Clean main buffer data
            vkDestroyBuffer (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getBD().main_buffer, 
                NULL
            );

            vkFreeMemory (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getBD().main_buffer_memory, 
                NULL
            );
        }

        
        void __vk_Renderer::__cleanSemaphores() {
            // Clean semaphores and fences
            for(size_t i = 0; i < __max_frame_c; i++) {
                vkDestroySemaphore (
                    __vk_RendererInitialiser::getIC()->getDev(), 
                    __vk_RendererInitialiser::getDrawCaller()->image_available_semaphore_set[i], 
                    NULL
                );
                vkDestroySemaphore (
                    __vk_RendererInitialiser::getIC()->getDev(), 
                    __vk_RendererInitialiser::getDrawCaller()->render_finished_semaphore_set[i], 
                    NULL
                );
                vkDestroyFence (
                    __vk_RendererInitialiser::getIC()->getDev(), 
                    __vk_RendererInitialiser::getDrawCaller()->flight_fences[i], 
                    NULL
                );
            }
        }

        
        void __vk_Renderer::__cleanDevice() {
            vkDestroyCommandPool (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDrawCaller()->getComPool(), 
                NULL
            );

            // Clean instance and devices
            vkDestroyDevice (
                __vk_RendererInitialiser::getIC()->getDev(), 
                NULL
            );

            if(m_config.enable_validation_layers) {
                __vk_InstanceCreator::destroyDebugUtils (
                    __vk_RendererInitialiser::getIC()->getIns(), 
                    __vk_RendererInitialiser::getIC()->getDMEXT()
                );
            }

            vkDestroySurfaceKHR (
                __vk_RendererInitialiser::getIC()->getIns(), 
                __vk_RendererInitialiser::getIC()->getSu(), 
                NULL
            );

            
            vkDestroyInstance (
                __vk_RendererInitialiser::getIC()->getIns(), 
                NULL
            );
        }


        /*
         * Free and destroy all active vulkan instances
         */
        void __vk_Renderer::__cleanup() {
            __cleanRendImgResources();
            __cleanDrawCommands();
            __cleanTextures();
            __cleanAssets();
            __cleanPipelines();
            __vk_RendererInitialiser::getSCC()->SCCleanup();
            __cleanDescPools();
            __cleanDescSetLayouts();
            __cleanSemaphores();
            __freeBuffers();
            __cleanDevice();
        }


        /*
         * Add new assets to renderer asset list.
         * Also no descriptor sets are allocated since that
         * will be done when renderer is being fully initialised.
         */
        void __vk_Renderer::submitAssets (
            deng_Id *assets,
            deng_ui32_t asset_c
        ) {
            deng_ui32_t old_size = static_cast<deng_ui32_t>(m_assets.size());
            m_assets.resize(m_assets.size() + asset_c);

            // Create new assets with their descriptor sets
            for(deng_ui32_t i = old_size; i < static_cast<deng_ui32_t>(m_assets.size()); i++) { 
                RegType &reg_asset = m_reg.retrieve (
                    assets[i - old_size], 
                    DENG_SUPPORTED_REG_TYPE_ASSET
                );

                RegType reg_vk_asset;
                reg_vk_asset.vk_asset.base_id = reg_asset.asset.uuid;
                reg_vk_asset.vk_asset.tex_uuid = reg_asset.asset.tex_uuid;
                reg_vk_asset.vk_asset.uuid = uuid_Generate();
                reg_vk_asset.vk_asset.is_desc = false;
                reg_asset.asset.vk_id = reg_vk_asset.vk_asset.uuid;

                // Allocate uniform buffer memory for uniform color data
                if(m_is_init) {
                    __vk_RendererInitialiser::getResMan()->cpyAssetUniform (
                        __vk_RendererInitialiser::getIC()->getDev(), 
                        __vk_RendererInitialiser::getIC()->getGpu(),
                        __vk_RendererInitialiser::getDrawCaller()->getComPool(),
                        __vk_RendererInitialiser::getIC()->getQFF().graphics_queue,
                        reg_vk_asset.vk_asset
                    );
                }

                m_reg.push (
                    reg_vk_asset.vk_asset.uuid, 
                    DENG_SUPPORTED_REG_TYPE_VK_ASSET,
                    reg_vk_asset
                );
                m_assets[i] = reg_vk_asset.vk_asset.uuid;
            }
        }


        /*
         * Add new textures to renderer texture list.
         */
        void __vk_Renderer::submitTextures (
            deng_Id *textures,
            deng_ui32_t tex_c
        ) {
            deng_ui32_t old_size = static_cast<deng_ui32_t>(m_textures.size());
            m_textures.resize(m_textures.size() + tex_c);

            for(deng_ui32_t i = old_size; i < static_cast<deng_ui32_t>(m_textures.size()); i++) {
                RegType reg_vk_tex;
                reg_vk_tex.vk_tex.base_id = textures[i - old_size];
                reg_vk_tex.vk_tex.uuid = uuid_Generate();
                reg_vk_tex.vk_tex.is_buffered = false;

                m_textures[i] = reg_vk_tex.vk_tex.uuid;

                m_reg.push (
                    reg_vk_tex.vk_tex.uuid,
                    DENG_SUPPORTED_REG_TYPE_VK_TEXTURE,
                    reg_vk_tex
                );

                RegType &reg_tex = m_reg.retrieve (
                    reg_vk_tex.vk_asset.base_id, 
                    DENG_SUPPORTED_REG_TYPE_TEXTURE
                );

                reg_tex.tex.vk_id = reg_vk_tex.vk_tex.uuid;
            }

            // Create vulkan image resources
            __vk_RendererInitialiser::getResMan()->mkTextures (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getIC()->getGpu(),
                __vk_RendererInitialiser::getDrawCaller()->getComPool(),
                false,
                {old_size, static_cast<deng_ui32_t>(m_textures.size())},
                __vk_RendererInitialiser::getIC()->getQFF().graphics_queue
            );
        }


        /*
         * Submit new draw calls and update uniform data
         */
        void __vk_Renderer::makeFrame() {
            // Call Vulkan fence waiter function
            vkWaitForFences (
                __vk_RendererInitialiser::getIC()->getDev(), 
                1, 
                &__vk_RendererInitialiser::getDrawCaller()->flight_fences[__vk_RendererInitialiser::getDrawCaller()->current_frame], 
                VK_TRUE, 
                UINT64_MAX
            );

            // Call Vulkan next image acquire method
            deng_ui32_t image_index;
            VkResult result = vkAcquireNextImageKHR (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getSCC()->getSC(), 
                UINT64_MAX, 
                __vk_RendererInitialiser::getDrawCaller()->image_available_semaphore_set[__vk_RendererInitialiser::getDrawCaller()->current_frame], 
                VK_NULL_HANDLE, 
                &image_index
            );
            
            if(result == VK_ERROR_OUT_OF_DATE_KHR) {
                VK_FRAME_ERR("image acquiring timed out");
                return;
            }
            
            else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
                VK_FRAME_ERR("failed to acquire swap chain image");

            // Update uniform data buffer
            __vk_RendererInitialiser::getResMan()->updateUboTransform3D(
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getDrawCaller()->current_frame, m_config.p_cam);

            VkSemaphore wait_semaphores[] = 
            {__vk_RendererInitialiser::getDrawCaller()->image_available_semaphore_set[__vk_RendererInitialiser::getDrawCaller()->current_frame]};

            VkSemaphore signal_semaphores[] = 
            {__vk_RendererInitialiser::getDrawCaller()->render_finished_semaphore_set[__vk_RendererInitialiser::getDrawCaller()->current_frame]};

            VkSubmitInfo submitinfo{};
            submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitinfo.waitSemaphoreCount = 1;
            submitinfo.pWaitSemaphores = wait_semaphores;
            submitinfo.pWaitDstStageMask = wait_stages;
            submitinfo.commandBufferCount = 1;
            submitinfo.pCommandBuffers = &__vk_RendererInitialiser::getDrawCaller()->getComBufs()[image_index];
            submitinfo.signalSemaphoreCount = 1;
            submitinfo.pSignalSemaphores = signal_semaphores;

            vkResetFences(__vk_RendererInitialiser::getIC()->getDev(), 
                1, 
                &__vk_RendererInitialiser::getDrawCaller()->flight_fences[__vk_RendererInitialiser::getDrawCaller()->current_frame]
            );

            if(vkQueueSubmit(__vk_RendererInitialiser::getIC()->getQFF().graphics_queue, 1, &submitinfo, 
               __vk_RendererInitialiser::getDrawCaller()->flight_fences[__vk_RendererInitialiser::getDrawCaller()->current_frame]) != VK_SUCCESS) 
                VK_FRAME_ERR("failed to submit draw command"); 

            VkPresentInfoKHR present_info{};
            present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = signal_semaphores;
            present_info.pImageIndices = &image_index;
            
            VkSwapchainKHR swapchains[] = {__vk_RendererInitialiser::getSCC()->getSC()};
            present_info.swapchainCount = 1;
            present_info.pSwapchains = swapchains;

            vkQueuePresentKHR (
                __vk_RendererInitialiser::getIC()->getQFF().present_queue, 
                &present_info
            );

            __vk_RendererInitialiser::getDrawCaller()->current_frame = (__vk_RendererInitialiser::getDrawCaller()->current_frame + 1) % __max_frame_c;
        }


        /*
         * Setup the renderer, create descriptor sets, allocate buffers
         * and record command buffers
         */
        void __vk_Renderer::setup() {
            // Create graphics pipelines
            __vk_RendererInitialiser::getPipelineC()->mkPipelines (
                __vk_RendererInitialiser::getIC()->getDev(),
                __vk_RendererInitialiser::getSCC()->getExt(),
                __vk_RendererInitialiser::getSCC()->getRp(),
                m_config.msaa_sample_count
            );

            // Make and allocate memory for main buffer 
            __vk_RendererInitialiser::getResMan()->mkAssetBuffers (
                __vk_RendererInitialiser::getIC()->getDev(),
                __vk_RendererInitialiser::getIC()->getGpu(),
                __vk_RendererInitialiser::getDrawCaller()->getComPool(),
                __vk_RendererInitialiser::getIC()->getQFF().graphics_queue
            );

            // Create descriptor sets for all currently available assets
            __vk_RendererInitialiser::getDescC()->mkDS (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getResMan()->getBD(),
                __vk_RendererInitialiser::getResMan()->getMissingTextureUUID(),
                { 0, (deng_ui32_t) m_assets.size() },
                __vk_RendererInitialiser::getIC()->getGpuLimits().minUniformBufferOffsetAlignment,
                __vk_RendererInitialiser::getResMan()->getUboChunkSize()
            );

            // Start recording command buffers
            __vk_RendererInitialiser::getDrawCaller()->setMiscData (
                __vk_RendererInitialiser::getPipelineC()->getPipelines(),
                __vk_RendererInitialiser::getResMan()->getFB()
            );
            
            __vk_RendererInitialiser::getDrawCaller()->allocateMainCmdBuffers (
                __vk_RendererInitialiser::getIC()->getDev(), 
                __vk_RendererInitialiser::getIC()->getQFF().graphics_queue, 
                __vk_RendererInitialiser::getSCC()->getRp(),
                __vk_RendererInitialiser::getSCC()->getExt(),
                m_config.background,
                __vk_RendererInitialiser::getResMan()->getBD()
            );
        }


        /*
         * Wait for any queue operation to finish
         * This method needs to be called whenever any 
         * command or data buffers need to be updated
         */
        void __vk_Renderer::idle() {
            vkDeviceWaitIdle(__vk_RendererInitialiser::getIC()->getDev());
        }
    }
}
