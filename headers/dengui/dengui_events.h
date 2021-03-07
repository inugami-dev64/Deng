#ifndef DENGUI_EVENTS_H
#define DENGUI_EVENTS_H

#define DENGUI_ITERATION_SLEEP_INTERVAL         10
#define DENGUI_EV_INPUT_SLEEP_INTERVAL          75 // milliseconds
#define DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER   DENG_MOUSE_BTN_3
#define DENGUI_DEFAULT_DRAG_BTN                 DENG_MOUSE_BTN_1


#ifdef __DENGUI_EVENTS_CPP
    #include <iostream>
    #include <stdlib.h>
    #include <vector>
    #include <array>
    #include <thread>
    #include <string>
    #include <mutex>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>
    
    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <deng/vulkan/vulkan_renderer.h>

    #include <utils/collision.h>
    #include <dengui/dengui_infos.h>
    #include <dengui/vulkan/vulkan_update.h>
#endif


namespace dengui {
    /* 
     * Element Handler 
     * This class is used to push and update window or ddm elements
     */
    class __EH : private dengUtils::Collision2D {
    private:
        std::vector<WindowData> m_win_data;
        std::vector<CursorDDMInfo> m_cur_ddm_infos;
        
        FloatInfo m_fi;
        dengMath::mat3<deng_vec_t> m_mouse_transform;

        std::mutex *m_p_res_mut;
        std::vector<deng_Asset> *m_p_assets;
        std::vector<deng::vulkan::TextureImage> *m_p_tex;
        deng::vulkan::InstanceCreator *m_p_ic;
        deng::vulkan::DescriptorCreator *m_p_desc_c;
        vulkan::__FrameUpdater *m_p_vk_frame_upd;

    private:
        /*
         * Find Window Instance index from vector
         * If Window Instance with window_id does not exist or new_win is true, then new
         * instance is created
         */
        size_t __findWindowIndex (
            const std::string &window_id, 
            size_t pref_win_i,
            deng_bool_t new_win
        );

        /*
         * Find cursor drop down menu index 
         * This method is a bit different from __findWindowIndex() method since pushing drop down menu
         * elements always creates new drop down menu instance
         */
        size_t __findCursorDDMIndex(const std::string &window_id);

        /*
         * Initialise floating mode for window assets
         */
        void __initFloating (
            dengMath::vec2<deng_ui32_t> asset_bounds,
            dengMath::vec2<deng_vec_t> mouse_pos
        );


    protected:
        deng_bool_t __EH_CheckDDMSpawn(dengMath::vec2<deng_vec_t> mouse_pos);
        deng_bool_t __EH_UpdateDDMEv (
            dengMath::vec2<deng_vec_t> mouse_pos, 
            Events *p_ev,
            deng_bool_t *p_ddm_flag
        );
        
        deng_bool_t __EH_UpdateWindows (
            dengMath::vec2<deng_vec_t> mouse_pos, 
            Events *p_ev, 
            deng_MouseButton &feedback
        );

        void __EH_UpdateFloating (
            dengMath::vec2<deng_vec_t> mouse_pos
        );
        
        WindowData *__EH_FindWindow(const std::string &win_id);
        std::vector<CursorDDMInfo> *__EH_GetDDMInfos();
        std::vector<WindowData> *__EH_GetWinData();

    public:
        __EH (
            std::vector<deng_Asset> *p_assets, 
            std::vector<deng::vulkan::TextureImage> *p_tex,
            vulkan::__FrameUpdater *p_frame_upd,
            Events *p_ev,
            deng_bool_t *p_float_flag,
            deng::vulkan::InstanceCreator *p_ic,
            deng::vulkan::DescriptorCreator *p_desc_c,
            std::mutex *p_res_mut
        );

        /* Push window elements to new window or existing one */
        void pushWindowElements (
            const std::vector<WindowElement> &elems,
            std::vector<std::string> *p_hidden_children,
            const std::string &window_id,
            size_t pref_win_i,
            deng_bool_t update_buffers,
            deng_bool_t new_win
        );

        /* Push cursor ddm elements to new drop down window or existing one */
        void pushCursorDDM (
            const std::string &window_id,
            const std::vector<DDMElement> &elems,
            deng_ObjVertData2D *active_verts,
            deng_ui32_t active_vert_c,
            deng_bool_t update_buffers
        );

        /* Window handling methods */
        void toggleDDM(const std::string &ddm_id);
        void toggleWindow(const std::string &window_id);
        void toggleVisibility (
            dengMath::vec2<deng_ui32_t> bounds,
            deng_bool_t *p_vis_flag
        );
        void checkWindowElements();
        void windowAssetRmSync(const std::string &win_id);
        void invertVisibilityByBounds(dengMath::vec2<deng_ui32_t> bounds);
    };

    
    /* Main GUI event handling class (runs on separate thread) */
    class Events : public __EH {
    private:
        deng_SurfaceWindow *m_p_win;
        deng_bool_t m_is_ddm = false;
        deng_bool_t m_wait_release = false;
        deng_bool_t m_float_mode = false;

        dengMath::vec2<deng_vec_t> m_cur_pos;
        EventInfo m_info;

    private:
        void __inputPoll();
        void __waitForNoInput(deng_MouseButton btn);

    public:
        vulkan::__FrameUpdater frame_upd;

    public:
        Events(EventInfo &ev_info);

    // Setters getters
    public:
        dengMath::vec2<deng_ui32_t> getWindowAssetBounds(const std::string &win_id);
        dengMath::vec2<deng_ui32_t> getWindowTexBounds(const std::string &win_id);
        std::vector<deng_Asset> *getAssets();
        std::vector<WindowElement> *getWinElems(const std::string &win_id);
    };
}

#endif
