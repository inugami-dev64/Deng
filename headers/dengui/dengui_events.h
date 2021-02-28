#ifndef DENGUI_EVENTS_H
#define DENGUI_EVENTS_H

#define DENGUI_ITERATION_SLEEP_INTERVAL         100  
#define DENGUI_EV_INPUT_SLEEP_INTERVAL          75 // milliseconds
#define DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER   DENG_MOUSE_BTN_3

namespace dengui {
    /* Enum for specifing element color mode */
    enum ElementColorMode {
        ELEMENT_COLOR_MODE_UNMAPPED         = 0,
        ELEMENT_COLOR_MODE_TEXTURE_MAPPED   = 1
    };


    /* Generic struct for all information related to mouse input *
     * Read-only for events thread                               */
    struct MouseInputInfo {
        std::mutex mut;
        dengMath::vec2<deng_vec_t> mouse_coords;
        deng_InputMode mouse_input;
        deng_MouseButton *active_btn;
        deng_i32_t active_btn_c;
    };
    

    /* Event creation info */
    struct EventInfo {
        dengMath::vec2<deng_ui32_t> deng_window_area;
        std::vector<deng_Asset> *p_assets;
        std::vector<deng::TextureImageData> *p_textures;
        std::mutex *p_res_mut;
        std::mutex *p_frame_mut;
        size_t sc_img_size;
    
        deng::InstanceCreator *p_ic;
        deng::ResourceAllocator *p_ra;
        deng::DrawCaller *p_dc;
        deng::DescriptorCreator *p_desc_c;
        VkExtent2D ext;
        VkRenderPass renderpass;
        dengMath::vec4<deng_vec_t> background;
    };


    /* Cursor drop down menu element information */
    struct CursorDDMInfo {
        std::string ddm_id;
        std::vector<DDMElement> elems;
        deng_bool_t is_visible;
        deng_ObjVertData2D *active_verts;
        deng_ui32_t active_vert_c;
        dengMath::vec2<deng_vec_t> ddm_size;
        dengMath::vec2<deng_ui32_t> asset_bounds;
        dengMath::vec2<deng_ui32_t> tex_bounds;
    };


    /* Store all information about dengui window */
    struct WindowData {
        std::string window_id;
        std::vector<WindowElement> win_elems;
        deng_bool_t is_visible;
        dengMath::vec2<deng_ui32_t> asset_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> tex_bounds = {0, 0};
    };

    
    /* 
     * This class is used for updating command and vertex buffers as well as
     * texture descriptors on the second thread 
     */
    class __FrameUpdater {
    private:
        EventInfo *m_p_info;
    
    public:
        __FrameUpdater(EventInfo *p_info);

        /* Asset mutex lockers */
        void lockAssets();
        void unlockAssets();

        /* Frame mutex lockers */
        void lockFrame();
        void unlockFrame();

        /* 
         * External update caller methods 
         * Frame locking is required for any of these method calls to work correctly!   
         */
        void reallocBuffer();
        void updateVerts(dengMath::vec2<deng_ui32_t> remap_bounds);
        void updateTexDS (
            deng_bool_t realloc_ds, 
            dengMath::vec2<deng_ui32_t> *p_tex_bounds
        );
        void updateTextures(dengMath::vec2<deng_ui32_t> tex_bounds);
        void updateCmdBuffers();
    };


    /* 
     * Element Handler 
     * This class is used to push and update window or ddm elements
     */
    class __EH : private dengUtils::Collision2D {
    private:
        std::vector<WindowData> m_win_data;
        std::vector<CursorDDMInfo> m_cur_ddm_infos;
        dengMath::mat3<deng_vec_t> m_cur_ddm_transform;
        std::mutex *m_p_res_mut;
        std::vector<deng_Asset> *m_p_assets;
        std::vector<deng::TextureImageData> *m_p_tex;
        deng::InstanceCreator *m_p_ic;
        deng::DescriptorCreator *m_p_desc_c;
        __FrameUpdater *m_p_frame_upd;

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
        
        WindowData *__EH_FindWindow(const std::string &win_id);
        std::vector<CursorDDMInfo> *__EH_GetDDMInfos();
        std::vector<WindowData> *__EH_GetWinData();

    public:
        __EH (
            std::vector<deng_Asset> *p_assets, 
            std::vector<deng::TextureImageData> *p_tex,
            __FrameUpdater *p_frame_upd,
            Events *p_ev,
            deng::InstanceCreator *p_ic,
            deng::DescriptorCreator *p_desc_c,
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

        dengMath::vec2<deng_vec_t> m_cur_pos;
        EventInfo m_info;

    private:
        void __inputPoll();
        void __waitForNoInput(deng_MouseButton btn);

    public:
        __FrameUpdater frame_upd;

    public:
        Events(EventInfo &ev_info);
        ~Events();

    // Setters getters
    public:
        dengMath::vec2<deng_ui32_t> getWindowAssetBounds(const std::string &win_id);
        dengMath::vec2<deng_ui32_t> getWindowTexBounds(const std::string &win_id);
        std::vector<deng_Asset> *getAssets();
        std::vector<WindowElement> *getWinElems(const std::string &win_id);
    };
}

#endif
