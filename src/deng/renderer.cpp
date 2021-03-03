#define __RENDERER_CPP
#include <deng/renderer.h>

// Shared data
dengui::MouseInputInfo ext_mii;

namespace deng {

    static deng_ui16_t text_box_c = 0;
    static deng_ui16_t texture_c = 0;
    static deng_ui16_t asset_c = 0; 


    /*******************************/
    /********** Renderer ***********/
    /*******************************/

    Renderer::Renderer (
        WindowWrap *p_win,
        void *p_cam,
        deng_CameraType camera_type
    ) { 
        m_p_camera = p_cam;
        m_main_camera_type = camera_type;
        m_p_ww = p_win; 
    }


    /* Submit assets for drawing */
    void Renderer::submitAssets (
        deng_Asset *assets, 
        deng_i32_t asset_c
    ) {
        size_t l_index = m_assets.size(); 
        deng_i32_t r_index;
        m_assets.resize(l_index + asset_c);
        
        for(r_index = 0; r_index < asset_c; l_index++, r_index++) {
            m_assets[l_index] = assets[r_index];
        }
    } 


    /* Submit preconfigured deng_Textures */
    void Renderer::submitTextures (
        deng_Texture *textures,
        deng_i32_t tex_c
    ) {
        size_t l_index = m_textures.size();
        deng_i32_t r_index;
        m_textures.resize(l_index + tex_c);

        for(r_index = 0; r_index < tex_c; l_index++, r_index++)
            m_textures[l_index].texture = textures[r_index];
    }


    /* Submit textures for mapping assets */
    void Renderer::submitTextureFile (
        std::string tex_file, 
        std::string id
    ) {
        size_t cur_index = m_textures.size();
        m_textures.resize(cur_index + 1);
        // Load all texture image information
        dasLoadTexture(&m_textures[cur_index].texture, tex_file.c_str());
        m_textures[cur_index].texture.id = (char*) calloc (
            id.size() + 1,
            sizeof(char)
        );

        strncpy (
            m_textures[cur_index].texture.id,
            id.c_str(),
            id.size()
        );
    }


    /* Submit text instances to renderer and make them into deng_Asset objects */ 
    void Renderer::submitRendStr (
        dengUtils::BitmapStr *rend_strs, 
        size_t size
    ) {
        size_t asset_index = m_assets.size();
        size_t tex_index = m_textures.size();
        size_t l_index;

        char *id_str;
        m_assets.resize(asset_index + size);
        m_textures.resize(tex_index + size);

        for(l_index = 0; l_index < size; asset_index++, tex_index++, l_index++) {
            text_box_c++;
            texture_c++;
            asset_c++;

            // Allocate memory for textbox texture id
            id_str = (char*) calloc (
                20,
                sizeof(char)
            );  

            sprintf (
                id_str,
                "Text Instance %d",
                text_box_c
            );

            // Create new asset for text box
            m_assets[asset_index].asset_mode = DENG_ASSET_MODE_2D_TEXTURE_MAPPED;
            m_assets[asset_index].description = (char*) rend_strs[l_index].text;
            m_assets[asset_index].id = id_str;
            m_assets[asset_index].is_shown = true;
            m_assets[asset_index].time_point = time(NULL);
            m_assets[asset_index].vertices.p_tex_mapped_vert_data_2d = rend_strs[l_index].vert_pos.data();
            m_assets[asset_index].vertices.size = rend_strs[l_index].vert_pos.size();
            m_assets[asset_index].indices.p_indices = rend_strs[l_index].vert_indices.data();
            m_assets[asset_index].indices.size = rend_strs[l_index].vert_indices.size();
 
            // Create new texture for text box
            m_textures[tex_index].texture.description = (char*) rend_strs[l_index].text;
            m_textures[tex_index].texture.pixel_data.width = rend_strs[l_index].box_size.first;
            m_textures[tex_index].texture.pixel_data.height = rend_strs[l_index].box_size.second;
            m_textures[tex_index].texture.pixel_data.p_pixel_data = rend_strs[l_index].tex_data.data();
            m_textures[tex_index].texture.pixel_data.size = rend_strs[l_index].tex_data.size();
            m_textures[tex_index].texture.id = id_str;
        }
    }

    /* Set renderer hints */
    void Renderer::setHints (deng_RendererHintBits hints) {
        // Check if VSync should be enabled
        if(hints & DENG_RENDERER_HINT_ENABLE_VSYNC)
            m_enable_vsync = true;
        else m_enable_vsync = false;

        // Check if FPS counter should be enabled
        if(hints & DENG_RENDERER_HINT_SHOW_FPS_COUNTER)
            m_count_fps = true;
        else m_count_fps = false;

        // Check if Vulkan validation layers should be enabled
        if(hints & DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS)
            m_enable_validation_layers = true;
        else m_enable_validation_layers = false;

        m_p_ic = new vulkan::InstanceCreator (
            m_p_ww,
            m_enable_validation_layers
        );

        // Check for MSAA related flags
        if(hints & DENG_RENDERER_HINT_MSAA_MAX_HARDWARE_SUPPORTED)
            m_msaa_sample_count = m_p_ic->getMaxSampleCount();
        if
        (
            (hints & DENG_RENDERER_HINT_MSAA_64) && 
            m_p_ic->getMaxSampleCount() >= VK_SAMPLE_COUNT_64_BIT
        ) m_msaa_sample_count = VK_SAMPLE_COUNT_64_BIT;
        else if
        (
            (hints & DENG_RENDERER_HINT_MSAA_64) &&
            m_p_ic->getMaxSampleCount() < VK_SAMPLE_COUNT_64_BIT
        ) m_msaa_sample_count = m_p_ic->getMaxSampleCount();

        if
        (
            (hints & DENG_RENDERER_HINT_MSAA_32) &&
            m_p_ic->getMaxSampleCount() >= VK_SAMPLE_COUNT_32_BIT
        ) m_msaa_sample_count = VK_SAMPLE_COUNT_32_BIT;
        else if
        (
            (hints & DENG_RENDERER_HINT_MSAA_32) &&
            m_p_ic->getMaxSampleCount() < VK_SAMPLE_COUNT_32_BIT
        ) m_msaa_sample_count = m_p_ic->getMaxSampleCount();

        if
        (
            (hints & DENG_RENDERER_HINT_MSAA_16) &&
            m_p_ic->getMaxSampleCount() >= VK_SAMPLE_COUNT_16_BIT
        ) m_msaa_sample_count = VK_SAMPLE_COUNT_16_BIT;
        else if
        (
            (hints & DENG_RENDERER_HINT_MSAA_16) &&
            m_p_ic->getMaxSampleCount() < VK_SAMPLE_COUNT_16_BIT
        ) m_msaa_sample_count = m_p_ic->getMaxSampleCount();

        if
        (
            (hints & DENG_RENDERER_HINT_MSAA_8) &&
            m_p_ic->getMaxSampleCount() >= VK_SAMPLE_COUNT_8_BIT
        ) m_msaa_sample_count = VK_SAMPLE_COUNT_8_BIT;
        else if
        (
            (hints & DENG_RENDERER_HINT_MSAA_8) &&
            m_p_ic->getMaxSampleCount() < VK_SAMPLE_COUNT_8_BIT
        ) m_msaa_sample_count = m_p_ic->getMaxSampleCount();

        if 
        (
            (hints & DENG_RENDERER_HINT_MSAA_4) &&
            m_p_ic->getMaxSampleCount() >= VK_SAMPLE_COUNT_4_BIT
        ) m_msaa_sample_count = VK_SAMPLE_COUNT_4_BIT;
        else if
        (
            (hints & DENG_RENDERER_HINT_MSAA_4) &&
            m_p_ic->getMaxSampleCount() < VK_SAMPLE_COUNT_4_BIT
        ) m_msaa_sample_count = m_p_ic->getMaxSampleCount();

        if
        (
            (hints & DENG_RENDERER_HINT_MSAA_2) &&
            m_p_ic->getMaxSampleCount() >= VK_SAMPLE_COUNT_2_BIT
        ) m_msaa_sample_count = VK_SAMPLE_COUNT_2_BIT;
        else if
        (
            (hints & DENG_RENDERER_HINT_MSAA_2) &&
            m_p_ic->getMaxSampleCount() < VK_SAMPLE_COUNT_2_BIT
        ) m_msaa_sample_count = m_p_ic->getMaxSampleCount();
    }


    /* Setup renderer for new rendering task */
    void Renderer::initRenderer (
        deng_RendererUsageMode usage,
        dengMath::vec4<deng_vec_t> background
    ) {
        m_asset_mut.lock();
        m_p_sr = new dengUtils::StringRasterizer("", m_p_ww);
        m_usage_mode = usage;
        m_background_color = background;

        // Default shared data initialisation
        ext_mii.mouse_coords = {0, 0};
        ext_mii.is_mouse_input= m_p_ww->isMovement();

        m_p_scc = new vulkan::SwapChainCreator (
            m_p_ic->getDev(),
            m_p_ww, 
            m_p_ic->getGpu(), 
            m_p_ic->getSu(), 
            m_p_ic->getQFF(),
            m_msaa_sample_count
        );

        m_p_ra = new vulkan::ResourceAllocator (
            m_p_ic->getDev(), 
            m_p_ic->getGpu(), 
            m_p_scc->getExt(), 
            m_msaa_sample_count,
            m_p_scc->getRp(), 
            m_p_scc->getSCImgViews(),
            m_p_scc->getSF(),
            true
        );

        m_p_dc = new vulkan::DrawCaller (
            m_p_ic->getDev(),
            m_p_ic->getQFF()
        );

        m_p_dc->mkCommandPool(m_p_ic->getDev());
        m_p_desc_c = new vulkan::DescriptorCreator (
            m_p_ic->getDev(), 
            m_p_scc->getExt(), 
            m_p_scc->getRp(), 
            &m_assets, 
            &m_textures, 
            m_p_scc->getSCImg().size(),
            m_msaa_sample_count
        );
        
        // Check if gui windows need to be generated
        if(m_usage_mode != DENG_RENDERER_USAGE_GAME_MODE) {
            dengui::EventInfo ev_info;
            ev_info.background = background;
            ev_info.deng_window_area = m_p_ww->getSize();
            ev_info.p_frame_mut = &m_frame_mut;
            ev_info.p_res_mut = &m_asset_mut;
            ev_info.p_run_mut = &m_gui_mut;
            ev_info.sc_img_size = m_p_scc->getSCImg().size();
            ev_info.p_assets = &m_assets;
            ev_info.p_vk_textures = &m_textures;
            ev_info.ext = m_p_scc->getExt();
            ev_info.renderpass = m_p_scc->getRp();

            ev_info.p_dc = m_p_dc;
            ev_info.p_ic = m_p_ic;
            ev_info.p_ra = m_p_ra;
            ev_info.p_desc_c = m_p_desc_c;

            switch (m_usage_mode)
            {
            case DENG_RENDERER_USAGE_MAP_EDITOR:
                LOG("Detected map editor usage");
                m_asset_mut.unlock();
                m_p_map_editor = new dengui::MapEditor (
                    ev_info, 
                    m_p_sr,
                    m_p_ww->getSize()
                );
                m_asset_mut.lock();
                break;
            
            default:
                break;
            }
        }


        // Create buffer resources
        m_p_ra->setAssetsData (
            &m_assets, 
            &m_textures
        );

        m_p_ra->mkTextureImages (
            m_p_ic->getDev(), 
            m_p_ic->getGpu(), 
            m_p_dc->getComPool(), 
            m_p_ic->getLFSupport(),
            {0, (deng_ui32_t) m_textures.size()},
            m_p_ic->getQFF().graphics_queue, 
            m_p_scc->getSCImg().size()
        );

        m_p_desc_c->updateTexDescriptors (
            m_p_ic->getDev(),
            m_p_scc->getSCImg().size(),
            false,
            NULL,
            m_p_ra->getBD()
        );

        m_p_ra->mkBuffers (
            m_p_ic->getDev(), 
            m_p_ic->getGpu(), 
            m_p_dc->getComPool(), 
            m_p_ic->getQFF().graphics_queue
        );

        // Make descriptor sets
        m_p_desc_c->mkUnmappedDS (
            m_p_ic->getDev(), 
            m_p_scc->getSCImg().size(), 
            *m_p_ra->getBD()
        );

        if(m_usage_mode == DENG_RENDERER_USAGE_GAME_MODE) {
            m_p_desc_c->mkTexMappedDS (
                m_p_ic->getDev(), 
                m_p_scc->getSCImg().size(), 
                {0, (deng_ui32_t) m_textures.size()},
                m_p_ra->getBD()
            );
        }

        // Record draw calls to command buffers 
        m_p_dc->setAssetsData (
            &m_assets, 
            &m_textures
        );
        
        m_p_dc->setMiscData (
            m_p_desc_c->getPipelines(), 
            m_p_ra->getFB(), 
            m_p_desc_c->getUnmappedDS() 
        );
        
        m_p_dc->allocateMainCmdBuffers (
            m_p_ic->getDev(), 
            m_p_ic->getQFF().graphics_queue, 
            m_p_scc->getRp(), 
            m_p_scc->getExt(),
            background,
            *m_p_ra->getBD()
        );

        m_asset_mut.unlock();
    }


    /* Renderer run method */
    void Renderer::run() {
        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;
        
        // Time points for calculating fps
        if(m_count_fps)
            begin = std::chrono::steady_clock::now();
        
        deng_ui32_t fps = 0;
        while(deng_IsRunning()) {
            if(m_count_fps) end = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() >= 1000) {
                LOG("FPS " + std::to_string(fps));
                begin = std::chrono::steady_clock::now();
                fps = 0;
            }

            else fps++;
            
            ext_mii.mut.lock();
            deng_UpdateWindow(m_p_ww->getWindow());
            ext_mii.mut.unlock();
            switch(m_main_camera_type)
            {
            case DENG_CAMERA_FPP:
                ((FPPCamera*) m_p_camera)->update();
                break;

            case DENG_CAMERA_EDITOR:
                ((EditorCamera*) m_p_camera)->update();
                break;
            
            default:
                break;
            }

            m_frame_mut.lock();
            __makeFrame();
            m_frame_mut.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
        
        vkDeviceWaitIdle(m_p_ic->getDev());
        __cleanup();
    }


    /* Frame update functions */
    void Renderer::__makeFrame() {
        // Call Vulkan fence waiter method
        vkWaitForFences (
            m_p_ic->getDev(), 
            1, 
            &m_p_dc->flight_fences[m_p_dc->current_frame], 
            VK_TRUE, 
            UINT64_MAX
        );

        // Call Vulkan next image acquire method
        deng_ui32_t image_index;
        VkResult result = vkAcquireNextImageKHR (
            m_p_ic->getDev(), 
            m_p_scc->getSC(), 
            UINT64_MAX, 
            m_p_dc->image_available_semaphore_set[m_p_dc->current_frame], 
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
        m_p_ra->updateUniformBufferData (
            m_p_ic->getDev(), 
            m_p_dc->current_frame, 
            m_p_camera, 
            m_main_camera_type,
            DENG_CAMERA_UNIFORM_NO_CAMERA_MODE_2D | 
            DENG_CAMERA_UNIFORM_PERSPECTIVE_CAMERA_MODE_3D
        );

        VkSemaphore wait_semaphores[] = 
        {m_p_dc->image_available_semaphore_set[m_p_dc->current_frame]};
        VkSemaphore signal_semaphores[] = 
        {m_p_dc->render_finished_semaphore_set[m_p_dc->current_frame]};

        VkSubmitInfo submitinfo{};
        submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkPipelineStageFlags wait_stages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitinfo.waitSemaphoreCount = 1;
        submitinfo.pWaitSemaphores = wait_semaphores;
        submitinfo.pWaitDstStageMask = wait_stages;
        submitinfo.commandBufferCount = 1;
        submitinfo.pCommandBuffers = &m_p_dc->getComBufs()[image_index];
        submitinfo.signalSemaphoreCount = 1;
        submitinfo.pSignalSemaphores = signal_semaphores;

        vkResetFences (
            m_p_ic->getDev(), 
            1, 
            &m_p_dc->flight_fences[m_p_dc->current_frame]
        );

        if
        (
            vkQueueSubmit (
                m_p_ic->getQFF().graphics_queue, 
                1, 
                &submitinfo, 
                m_p_dc->flight_fences[m_p_dc->current_frame]
            ) != VK_SUCCESS
        ) VK_FRAME_ERR("failed to submit draw command"); 

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;
        present_info.pImageIndices = &image_index;
        
        VkSwapchainKHR swapchains[] = {m_p_scc->getSC()};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;

        vkQueuePresentKHR (
            m_p_ic->getQFF().present_queue, 
            &present_info
        );

        m_p_dc->current_frame = (m_p_dc->current_frame + 1) % DENG_MAX_FRAMES_IN_FLIGHT;
    }


    /* Cleanup the renderer before destruction */
    void Renderer::__cleanup() {
        m_asset_mut.unlock();
        m_frame_mut.lock();
        size_t index;
        // Clean depth image resources
        vkDestroyImageView (
            m_p_ic->getDev(), 
            m_p_ra->getDepImgView(), 
            nullptr
        );
        vkDestroyImage (
            m_p_ic->getDev(), 
            m_p_ra->getDepImg(), 
            nullptr
        );
        vkFreeMemory (
            m_p_ic->getDev(), 
            m_p_ra->getDepImgMem(), 
            nullptr
        );

        // Clean color image resources
        vkDestroyImageView (
            m_p_ic->getDev(),
            m_p_ra->getColorImgView(),
            nullptr
        );
        vkDestroyImage (
            m_p_ic->getDev(),
            m_p_ra->getColorImg(),
            nullptr
        );
        vkFreeMemory (
            m_p_ic->getDev(),
            m_p_ra->getColorImgMem(),
            nullptr
        );

        // Clean framebuffers
        for(index = 0; index < m_p_ra->getFB().size(); index++) 
            vkDestroyFramebuffer (
                m_p_ic->getDev(), 
                m_p_ra->getFB()[index], 
                nullptr
            );

        // Clean commandbuffers and commandpools
        vkFreeCommandBuffers (
            m_p_ic->getDev(), 
            m_p_dc->getComPool(), 
            m_p_dc->getComBufs().size(), 
            m_p_dc->getComBufs().data()
        );

        // Clean pipeline related data
        std::array<vulkan::PipelineData, __DENG_PIPELINE_COUNT> pipeline_data = m_p_desc_c->getPipelines();
        for(index = 0; index < pipeline_data.size(); index++) { 
            vkDestroyPipeline (
                m_p_ic->getDev(), 
                pipeline_data[index].pipeline, 
                nullptr
            );
        }

        // Clean pipeline layout data 
        vkDestroyPipelineLayout (
            m_p_ic->getDev(), 
            m_p_desc_c->getUnmappedPL(), 
            nullptr
        );
        
        vkDestroyPipelineLayout (
            m_p_ic->getDev(), 
            m_p_desc_c->getTexMappedPL(), 
            nullptr
        );

        // Destroy renderpass
        vkDestroyRenderPass (
            m_p_ic->getDev(), 
            m_p_scc->getRp(), 
            nullptr
        );

        // Destroy swapchain
        m_p_scc->SCCleanup();

        // Clean ubo buffer data 
        for(index = 0; index < m_p_ra->getBD()->mat_uniform_buffers.size(); index++) {
            vkDestroyBuffer (
                m_p_ic->getDev(), 
                m_p_ra->getBD()->mat_uniform_buffers[index], 
                nullptr
            );

            vkFreeMemory (
                m_p_ic->getDev(), 
                m_p_ra->getBD()->mat_uniform_buffer_mem[index], 
                nullptr
            );
        }

        // Destroy descriptor pools 
        vkDestroyDescriptorPool (
            m_p_ic->getDev(), 
            m_p_desc_c->getUnmappedDP(), 
            nullptr
        );

        vkDestroyDescriptorPool (
            m_p_ic->getDev(), 
            m_p_desc_c->getTexMappedDP(), 
            nullptr
        );

        // Clean texture images
        __cleanAssets();

        // Clean descriptor set layouts
        vkDestroyDescriptorSetLayout (
            m_p_ic->getDev(), 
            m_p_desc_c->getUnmappedDSL(), 
            nullptr
        );

        vkDestroyDescriptorSetLayout (
            m_p_ic->getDev(), 
            m_p_desc_c->getTexMappedDSL(), 
            nullptr
        );

        // Clean main buffer data
        vkDestroyBuffer (
            m_p_ic->getDev(), 
            m_p_ra->getBD()->main_buffer, 
            nullptr
        );

        vkFreeMemory (
            m_p_ic->getDev(), 
            m_p_ra->getBD()->main_buffer_memory, 
            nullptr
        );


        // Clean semaphores and fences
        for(index = 0; index < DENG_MAX_FRAMES_IN_FLIGHT; index++) {
            vkDestroySemaphore (
                m_p_ic->getDev(), 
                m_p_dc->image_available_semaphore_set[index], 
                nullptr
            );
            vkDestroySemaphore (
                m_p_ic->getDev(), 
                m_p_dc->render_finished_semaphore_set[index], 
                nullptr
            );
            vkDestroyFence (
                m_p_ic->getDev(), 
                m_p_dc->flight_fences[index], 
                nullptr
            );
        }

        vkDestroyCommandPool (
            m_p_ic->getDev(), 
            m_p_dc->getComPool(), 
            nullptr
        );

        // Clean instance and devices
        vkDestroyDevice (
            m_p_ic->getDev(), 
            nullptr
        );

        if(m_enable_validation_layers) {
            vulkan::InstanceCreator::destroyDebugUtils (
                m_p_ic->getIns(), 
                m_p_ic->getDMEXT()
            );
        }

        vkDestroySurfaceKHR (
            m_p_ic->getIns(), 
            m_p_ic->getSu(), 
            nullptr
        );

        
        vkDestroyInstance (
            m_p_ic->getIns(), 
            nullptr
        );

        // Free memory from all renderer creator class instances
        if(m_p_ic)
            delete m_p_ic;
        if(m_p_scc)
            delete m_p_scc;
        if(m_p_desc_c)
            delete m_p_desc_c;
        if(m_p_ra)
            delete m_p_ra;
        if(m_p_dc)
            delete m_p_dc;
        if(m_p_sr)
            delete m_p_sr;

        m_gui_mut.lock();
        switch(m_usage_mode) 
        {
        case DENG_RENDERER_USAGE_MAP_EDITOR:
            delete m_p_map_editor;
            break;

        default:
            break;
        }
        m_gui_mut.unlock();
        m_frame_mut.unlock();
    }


    /* Clean all assets and asset textures */
    void Renderer::__cleanAssets() {
        m_asset_mut.lock();
        for(size_t index = 0; index < m_textures.size(); index++) {
            vkDestroySampler (
                m_p_ic->getDev(),
                m_textures[index].sampler, 
                nullptr
            );
            vkDestroyImageView (
                m_p_ic->getDev(), 
                m_textures[index].image_view, 
                nullptr
            );
            vkDestroyImage (
                m_p_ic->getDev(), 
                m_textures[index].image, 
                nullptr
            );
            vkFreeMemory (
                m_p_ic->getDev(), 
                m_textures[index].image_mem, 
                nullptr
            );
        }
        m_asset_mut.unlock();
    }
} 
