#ifndef DENGUI_WINDOW_H
#define DENGUI_WINDOW_H

// In pixels
#define DENGUI_LIGHT_BORDER_THICKNESS           1
#define DENGUI_MEDIUM_BORDER_THICKNESS          2
#define DENGUI_HEAVY_BORDER_THICKNESS           5

// Define default colors
#define DENGUI_DEFAULT_PRIMARY_COLOR            {0.0f, 1.0f, 1.0f, 0.5f}
#define DENGUI_DEFAULT_SECONDARY_COLOR          {1.0f, 1.0f, 1.0f, 1.0f}
#define DENGUI_DEFAULT_TERTIARY_COLOR           {0.0f, 0.2f, 1.0f, 1.0f}

// Define default position and size
#define DENGUI_DEFAULT_POS                      {0.0f, 0.0f}
#define DENGUI_DEFAULT_SIZE                     {0.1f, 0.1f}

// Window flags
#define DENGUI_WINDOW_FLAG_NULL                 0x00
#define DENGUI_WINDOW_FLAG_MENUBAR              0x01
#define DENGUI_WINDOW_FLAG_NO_COLLAPSE          0x02
#define DENGUI_WINDOW_FLAG_NO_MOVE              0x04
#define DENGUI_WINDOW_FLAG_NO_TITLEBAR          0x08
#define DENGUI_WINDOW_FLAG_ALWAYS_ON_TOP        0x10
#define DENGUI_WINDOW_FLAG_NO_RESIZE            0x20
#define DENGUI_WINDOW_FLAG_NO_CLOSE             0x40

// Default window parametres
#define DENGUI_TITLEBAR_HEIGHT                  0.05f
#define DENGUI_TITLEBAR_ELEM_MARGIN             0.005f
#define DENGUI_DEFAULT_LABEL_PADDING            5.0 // px
#define DENGUI_DEFAULT_FONT_FILE                "PressStart2P-Regular"

// Main window element ids
#define DENGUI_FORM_ID                          "form"
#define DENGUI_TITLEBAR_ID                      "titlebar"
#define DENGUI_MINIMISE_TRIANGLE_ID             "min_triangle"
#define DENGUI_MAXIMISE_TRIANGLE_ID             "max_triangle"
#define DENGUI_CLOSE_BTN_ID                     "close_btn"
#define DENGUI_TITLE_ID                         "title"

// Default string buttons 
#define DENGUI_CLOSE_BTN                        "[X]"
#define DENGUI_WINDOW_LEVEL                     0

namespace dengui {
    /* Window type enums */
    enum WindowType {
        WINDOW_TYPE_FLOATING        = 0,
        WINDOW_TYPE_STATIC_RIGHT    = 1,
        WINDOW_TYPE_STATIC_LEFT     = 2,
        WINDOW_TYPE_STATIC_TOP      = 3,
        WINDOW_TYPE_STATIC_BOTTOM   = 4
    };


    /* Window update hint struct for information about buffer update */
    struct WindowUpdateInfo {
        deng_bool_t update;
        std::vector<VERT_UNMAPPED_2D> unmapped_vert;
        std::vector<VERT_MAPPED_2D> mapped_vert;
        std::vector<deng_ui32_t> indices; 
        std::string win_id;
    };


    /* Shared window update hints */
    struct SharedWindowUpdateInfos {
        std::vector<WindowUpdateInfo> update_infos;
        std::mutex mut;
    };


    struct ElementBaseData {
        std::string child_id;
        std::string parent_id;
        char *asset_id;
        ElementColorMode color_mode;
        dengMath::vec2<size_t> col_vert_bounds;
        std::vector<VERT_UNMAPPED_2D> unmapped_vert;
        std::vector<VERT_MAPPED_2D> mapped_vert;
        std::vector<deng_ui32_t> indices;
        std::vector<deng_ui8_t> texture;
        dengMath::vec2<deng_i32_t> tex_box;
    };

    /* Struct to share between main thread and ui event thread */
    struct WindowElement : public ElementBaseData {
        deng_bool_t is_interactive;
        deng_bool_t is_drag_point;
        WindowCallback onLMBClickFunc;
        WindowCallback onMMBClickFunc;
        WindowCallback onRMBClickFunc;
        WindowCallback onHoverFunc;
        WindowCallback onScrUpFunc;
        WindowCallback onScrDownFunc;
    };


    struct DDMElement : public ElementBaseData {
        DDMCallback onLMBClickFunc;  
        DDMCallback onMMBClickFunc;  
        DDMCallback onRMBClickFunc;  
        DDMCallback onHoverFunc;
    };

    /* Window info struct */
    struct WindowInfo {
        std::string label;
        std::string id;
        WindowType wt;
        WindowBorder border;
        denguiWindowFlagBits fl_b;
        dengMath::vec4<deng_vec_t> pc;
        dengMath::vec4<deng_vec_t> sc;
        dengMath::vec4<deng_vec_t> tc;
        dengMath::vec2<deng_vec_t> pos;
        dengMath::vec2<deng_vec_t> size;

        WindowInfo();
    };


    /* Class for creating all base window shapes */
    struct WindowShapeInfo {
        dengMath::vec2<deng_vec_t> pos;
        dengMath::vec2<deng_vec_t> size;
        std::vector<VERT_UNMAPPED_2D> *p_vert;
        std::vector<deng_ui32_t> *p_indices;
    };


    /* Main window class */
    class Window {
    private:
        std::string m_id;
        dengUtils::StringRasterizer *m_p_sr;
        std::vector<WindowElement> m_win_elems;
        std::vector<std::pair<void*, ChildElementType>> m_child_elements;
        
        /* Primary color specifies the window background color          *
         * Secondary color specifies the window elements (buttons,      *
           text field background etc colors)                            *
         * Tertiary color specifies only the titlebar color             */
        dengMath::vec4<deng_vec_t> m_primary_color;
        dengMath::vec4<deng_vec_t> m_secondary_color;
        dengMath::vec4<deng_vec_t> m_tertiary_color;

        VERT_UNMAPPED_2D *m_p_base;

        // Color specification deng_bool_tean for checking if window
        deng_bool_t m_is_color_spec;
        WindowType m_wt;
    
    private:    
        void mkForm(WindowInfo *p_wi);
        
        void mkTitlebar (
            WindowInfo *p_wi, 
            VERT_UNMAPPED_2D *form_vert
        );

        void mkTitle (
            WindowInfo *p_wi,
            dengMath::vec2<deng_ui32_t> deng_win_bounds,
            VERT_UNMAPPED_2D *form_vert,
            dengMath::vec3<unsigned char> color);
        
        void mkMinimiseTriangle (
            WindowInfo *p_wi, 
            VERT_UNMAPPED_2D *form_vert
        );
        
        void mkMaximiseTriangle (
            WindowInfo *p_wi,
            VERT_UNMAPPED_2D *form_vert
        );

        void mkClose (
            dengMath::vec4<deng_vec_t> color,
            dengMath::vec2<deng_ui32_t> deng_win_bounds,
            std::string parent,
            VERT_UNMAPPED_2D *form_vert
        );


    // Getters and setters
    public:
        // Color getters
        dengMath::vec4<deng_vec_t> getPC();
        dengMath::vec4<deng_vec_t> getSC();
        dengMath::vec4<deng_vec_t> getTC();

        // Get all window elements 
        std::vector<WindowElement> getWindowElements();

        std::string getId();

        // Color setters
        void setPC(dengMath::vec4<deng_vec_t> *p_pc);
        void setSC(dengMath::vec4<deng_vec_t> *p_sc);
        void setTC(dengMath::vec4<deng_vec_t> *p_tc);

    public:
        Window (
            WindowType wt, 
            std::string id, 
            dengUtils::StringRasterizer *p_sr,
            dengMath::vec2<deng_ui32_t> window_bounds
        );
        ~Window();

        void makeWindow (
            WindowInfo *p_wi,
            dengMath::vec2<deng_ui32_t> draw_bounds
        );

        void attachChild (
            void *child,
            ChildElementType element_type
        );

        ParentInfo getParentInfo();
    };


    /********************************************/
    /********************************************/
    /******** Initialisation functions **********/
    /********************************************/
    /********************************************/

    /* Create new window instance */
    void beginWindow (
        Window **pp_window, 
        WindowInfo *p_wi, 
        dengMath::vec2<deng_ui32_t> draw_bounds,
        dengUtils::StringRasterizer *p_sr,
        Events *p_ev,
        deng_bool_t realloc_res
    );


    /* Destory window instance */
    void destroyWindow (
        Window *p_win,
        Events *p_ev
    );
}

#endif
