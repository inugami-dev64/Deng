#ifndef RENDERER_H
#define RENDERER_H

#ifdef __RENDERER_CPP
    #include <iostream>
    #include <vector>
    #include <mutex>
    #include <thread>
    #include <chrono>
    #include <vulkan/vulkan.h>
    
    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>
    #include <das/data_loader.h>

    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <deng/vulkan/vulkan_pipelines.h>
    #include <deng/vulkan/vulkan_renderer.h>

    #include <utils/timer.h>
    #include <utils/font.h>

    #include <deng/camera.h>
    #include <dengui/dengui_win_def.h>
    #include <dengui/dengui_infos.h>
    #include <dengui/dengui_child.h>
    #include <dengui/map_editor_ui.h>
#endif


namespace deng {

    class Renderer {   
    private:
        vulkan::InstanceCreator *m_p_ic = NULL;
        vulkan::SwapChainCreator *m_p_scc = NULL;
        vulkan::DescriptorCreator *m_p_desc_c;
        vulkan::ResourceAllocator *m_p_ra = NULL;
        vulkan::DrawCaller *m_p_dc = NULL;
        dengUtils::StringRasterizer *m_p_sr = NULL;
        std::mutex m_frame_mut;
        std::mutex m_asset_mut;        
        // This mutex is always locked when gui input polling is happening
        std::mutex m_gui_mut;
        
        // Render usage specifications
        deng_RendererUsageMode m_usage_mode;
        deng_bool_t m_enable_vsync;
        deng_bool_t m_enable_validation_layers;
        deng_bool_t m_count_fps;
        VkSampleCountFlagBits m_msaa_sample_count = VK_SAMPLE_COUNT_1_BIT;
        dengMath::vec4<deng_vec_t> m_background_color;

        // View distance settings
        const deng_vec_t m_near_plane = DENG_DEFAULT_NEAR_PLANE;
        const deng_vec_t m_far_plane = DENG_DEFAULT_FAR_PLANE;

        // Assets, buffers and texture images
        std::vector<deng_Asset> m_assets;
        std::vector<vulkan::TextureImage> m_textures;

        // Renderer utilities
        void *m_p_camera = NULL;
        deng_CameraType m_main_camera_type;
        dengUtils::Timer m_timer;
        WindowWrap *m_p_ww = NULL;

        dengui::MapEditor *m_p_map_editor;

    private:
        void __cleanup();
        void __cleanAssets();
        void __makeFrame();

    public:
        Renderer (
            WindowWrap *p_win,
            void *p_cam,
            deng_CameraType camera_type
        ); 
        void submitAssets (
            deng_Asset *assets, 
            deng_i32_t size
        );

        void submitTextures (
            deng_Texture *textures,
            deng_i32_t tex_c
        );
        
        void submitTextureFile (
            std::string tex_file, 
            std::string id
        );
        
        void submitRendStr (
            dengUtils::BitmapStr *rend_strs, 
            size_t size
        );

        void setHints(deng_RendererHintBits hints);
        void initRenderer (
            deng_RendererUsageMode usage, 
            dengMath::vec4<deng_vec_t> background
        );

        /* Prepare the renderer for incoming updates */

        void run();        
    };
}


#endif
