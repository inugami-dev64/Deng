#ifndef DENGUI_EVENTS_H
#define DENGUI_EVENTS_H

#define DENGUI_ITERATION_SLEEP_INTERVAL         100  
#define DENGUI_EV_INPUT_SLEEP_INTERVAL          75 // milliseconds

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

        deng::ResourceAllocator *p_ra;
        deng::DrawCaller *p_dc;
        deng_bool_t *p_update_flag;
        std::mutex *p_update_mut;
        std::mutex *p_frame_mut;

        VkDevice device;
        VkPhysicalDevice gpu;
        VkRenderPass renderpass;
        VkCommandPool commandpool;
        VkQueue g_queue;
        VkExtent2D extent;
        dengMath::vec4<deng_vec_t> background;
    };


    /* Main GUI event handling class (runs on separate thread) */
    class Events {
    private:
        deng_SurfaceWindow *m_p_win;
        std::vector<std::string> m_window_names; 
        std::vector<WindowElement> m_elem_infos;
        std::mutex m_elem_mut;

        dengMath::vec2<deng_vec_t> m_cur_pos;
        std::string m_click_elem_name;

        EventInfo m_info;

    private:

        deng_bool_t findCollision (
            dengMath::vec2<deng_vec_t> point, 
            WindowElement *p_elem
        );
        void inputPoll();
        void waitForNoInput(deng_MouseButton btn);

    public:
        Events(EventInfo &ev_info);
        ~Events();

        void pushWindowElements (
            std::vector<WindowElement> elems, 
            deng_bool_t update_buffers
        );
        
        void windowAssetSync();
        void updateAssetsVisibility(std::string parent_id);
        void allocateNewResources();

    // Setters getters
    public:
        std::vector<WindowElement> *getElems();
        std::vector<VkCommandBuffer> *getCmdBuffers();
        VkCommandPool *getCmdPool();
        VkBuffer *getMainBuffer();
        VkDeviceMemory *getMainBufMem();
        void setUpdateFlag();
    };


    /************************************/
    /****** CALLBACK FUNCTIONS **********/
    /************************************/
    
    /* Main window element callbacks */
    void minTriangleCallback(WindowElement *p_cur_elem, Events *p_ev);
    void maxTriangleCallback(WindowElement *p_cur_elem, Events *p_ev);
    void closeBtnCallback(WindowElement *p_cur_elem, Events *p_ev);
}

#endif
