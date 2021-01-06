#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

#define DENGUI_EV_REFRESH_INTERVAL  500
#define DENGUI_EV_SLEEP_INTERVAL    75 // milliseconds

namespace dengui {
    /* Enum for specifing element color mode */
    enum ElementColorMode {
        ELEMENT_COLOR_MODE_UNMAPPED         = 0,
        ELEMENT_COLOR_MODE_TEXTURE_MAPPED   = 1
    };


    /* Generic struct for all information related to mouse input *
     * Read-only for events thread                               */
    struct MouseInputInfo {
        std::mutex mouse_input_mut;
        dengMath::vec2<float> mouse_coords;
        dengInputMode mouse_input;
        DENGMouseButton *active_btn;
        int32_t active_btn_c;
    };


    /* Struct to share between main thread and ui event thread */
    struct ElementInfo {
        std::string element_name;
        void (*onLMBClickFunc)();
        void (*onMMBClickFunc)();
        void (*onRMBClickFunc)();
        void (*onScrUpFunc)();
        void (*onScrDownFunc)();
        ElementColorMode color_mode;
        std::vector<VERT_UNMAPPED_2D> unmapped_vert;
        std::vector<VERT_MAPPED_2D> mapped_vert;
        std::vector<uint32_t> indices;
    };

    /* Main GUI event handling class (runs on separate thread) */
    class Events {
    private:
        DENGWindow *m_p_win;
        dengMath::vec2<uint32_t> m_deng_window_size{};
        std::vector<ElementInfo> m_elem_infos{};
        dengMath::vec2<float> m_cur_pos;
        std::string m_click_elem_name;

    private:
        bool findCollision(dengMath::vec2<float> point, ElementInfo *p_elem);
        void inputPoll();
        void waitForNoInput(DENGMouseButton btn);

    public:
        Events(std::vector<ElementInfo> &elem, dengMath::vec2<uint32_t> deng_window_area);
    };


    /************************************/
    /****** CALLBACK FUNCTIONS **********/
    /************************************/
    
    /* Main window element callbacks */
    void minTriangleCallback();
    void closeBtnCallback();
}

#endif