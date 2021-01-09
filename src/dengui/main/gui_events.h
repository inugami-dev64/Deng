#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

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
        dengMath::vec2<float> mouse_coords;
        deng_InputMode mouse_input;
        DENGMouseButton *active_btn;
        int32_t active_btn_c;
    };
    

    /* Event creation info */
    struct EventInfo {
        dengMath::vec2<uint32_t> deng_window_area;
        std::vector<deng_Asset> *p_assets;
        std::vector<deng::TextureImageData> *p_textures;
        std::mutex *p_res_mut;

        deng::ResourceAllocator *p_ra;
        deng::DrawCaller *p_dc;
        bool *p_update_flag;
        std::mutex *p_update_mut;
        std::mutex *p_frame_mut;

        VkDevice device;
        VkPhysicalDevice gpu;
        VkRenderPass renderpass;
        VkCommandPool commandpool;
        VkQueue g_queue;
        VkExtent2D extent;
        dengMath::vec4<float> background;
    };


    /* Main GUI event handling class (runs on separate thread) */
    class Events {
    private:
        DENGWindow *m_p_win;
        std::vector<WindowElement> m_elem_infos;
        std::mutex m_elem_mut;

        dengMath::vec2<float> m_cur_pos;
        std::string m_click_elem_name;

        EventInfo m_info;

    private:
        bool findCollision(dengMath::vec2<float> point, WindowElement *p_elem);
        void inputPoll();
        void waitForNoInput(DENGMouseButton btn);

    public:
        Events(EventInfo &ev_info);
        ~Events();

        void pushWindowElements(std::vector<WindowElement> elems);
        void updateAssetsVisibility(std::string parent_id);
        void allocateNewResources();

    // Setters getters
    public:
        std::vector<WindowElement> *getElems();
        std::vector<VkCommandBuffer> *getCmdBuffers();
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