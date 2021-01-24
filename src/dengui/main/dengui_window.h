#ifndef DENGUI_WINDOW_H
#define DENGUI_WINDOW_H

#include <vector>
typedef deng_ui32_t WindowID;
// In pixels
#define DENGUI_LIGHT_BORDER_THICKNESS       1
#define DENGUI_MEDIUM_BORDER_THICKNESS      2
#define DENGUI_HEAVY_BORDER_THICKNESS       5

// Define default colors
#define DENGUI_DEFAULT_PRIMARY_COLOR        {0.0f, 1.0f, 1.0f, 0.5f}
#define DENGUI_DEFAULT_SECONDARY_COLOR      {1.0f, 1.0f, 1.0f, 1.0f}
#define DENGUI_DEFAULT_TERTIARY_COLOR       {0.0f, 0.2f, 1.0f, 1.0f}

// Define default position and size
#define DENGUI_DEFAULT_POS                  {0.0f, 0.0f}
#define DENGUI_DEFAULT_SIZE                 {0.1f, 0.1f}

// Window flags
#define DENGUI_WINDOW_FLAG_NULL             0x00
#define DENGUI_WINDOW_FLAG_MENUBAR          0x01
#define DENGUI_WINDOW_FLAG_NO_COLLAPSE      0x02
#define DENGUI_WINDOW_FLAG_NO_MOVE          0x04
#define DENGUI_WINDOW_FLAG_NO_TITLEBAR      0x08
#define DENGUI_WINDOW_FLAG_ALWAYS_ON_TOP    0x10
#define DENGUI_WINDOW_FLAG_NO_RESIZE        0x20
#define DENGUI_WINDOW_FLAG_NO_CLOSE         0x40

// Default window parametres
#define DENGUI_TITLEBAR_HEIGHT              0.05f
#define DENGUI_TITLEBAR_ELEM_MARGIN         0.005f
#define DENGUI_DEFAULT_FONT_FILE            "SourceCodePro-Regular.ttf"

// Main window element ids
#define DENGUI_FORM_ID                      "form"
#define DENGUI_TITLEBAR_ID                  "titlebar"
#define DENGUI_MINIMISE_TRIANGLE_ID         "min_triangle"
#define DENGUI_MAXIMISE_TRIANGLE_ID         "max_triangle"
#define DENGUI_CLOSE_BTN_ID                 "close_btn"
#define DENGUI_TITLE_ID                     "title"

// Default string buttons 
#define DENGUI_CLOSE_BTN                    "[X]"                  

namespace dengui {

    /* Window type enums */
    enum WindowType {
        WINDOW_TYPE_FLOATING        = 0,
        WINDOW_TYPE_STATIC_RIGHT    = 1,
        WINDOW_TYPE_STATIC_LEFT     = 2,
        WINDOW_TYPE_STATIC_TOP      = 3,
        WINDOW_TYPE_STATIC_BOTTOM   = 4
    };


    /* Specify window border thickness (px) */
    enum WindowBorder {
        WINDOW_BORDER_NONE      = 0,
        WINDOW_BORDER_LIGHT     = 2,
        WINDOW_BORDER_MEDIUM    = 4,
        WINDOW_BORDER_HEAVY     = 7    
    };


    /* Specify text box origin vertex */
    enum RectangleOrigin {
        REC_ORIGIN_VERTEX_TOP_LEFT       = 0,
        REC_ORIGIN_VERTEX_TOP_RIGHT      = 1,
        REC_ORIGIN_VERTEX_BOTTOM_LEFT    = 2,
        REC_ORIGIN_VERTEX_BOTTOM_RIGHT   = 3
    };


    /* Window update hint struct for information about buffer update */
    struct WindowUpdateInfo {
        bool update;
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


    /* Drop-down menu class for menubar */
    class DropDownMenu {
    private:
        std::vector<std::string> m_elem_names;
        std::vector<WindowID> m_parent_ids;
    public:
        DropDownMenu(dengMath::vec3<float> color, WindowBorder border);
        void pushElement(std::string elem_name, WindowID redir_id);
    };

    
    /* Struct to share between main thread and ui event thread */
    struct WindowElement {
        std::string child_id;
        std::string parent_id;
        char *asset_id;
        bool is_visible;
        bool is_interactive;

        void (*onLMBClickFunc)(WindowElement*, Events*);
        void (*onMMBClickFunc)(WindowElement*, Events*);
        void (*onRMBClickFunc)(WindowElement*, Events*);
        void (*onScrUpFunc)(WindowElement*, Events*);
        void (*onScrDownFunc)(WindowElement*, Events*);
        ElementColorMode color_mode;
        deng_i32_t base_vert_size;
        deng_i32_t base_indices_size;
        std::vector<VERT_UNMAPPED_2D> unmapped_vert;
        std::vector<VERT_MAPPED_2D> mapped_vert;
        std::vector<deng_ui32_t> indices;
        std::vector<deng_ui8_t> texture;
        dengMath::vec2<deng_i32_t> tex_box;
    };


    /* Window info struct */
    struct WindowInfo {
        std::string id;
        WindowType wt;
        WindowBorder border;
        denguiWindowFlagBits fl_b;
        dengMath::vec4<float> pc;
        dengMath::vec4<float> sc;
        dengMath::vec4<float> tc;
        dengMath::vec2<float> pos;
        dengMath::vec2<float> size;

        WindowInfo();
    };


    /* Class for creating all base window shapes */
    struct WindowShapeInfo {
        dengMath::vec2<deng_vec_t> pos;
        dengMath::vec2<deng_vec_t> size;
        RectangleOrigin rec_origin;
        std::vector<VERT_UNMAPPED> *p_vert;
        std::vector<deng_ui32_t> *p_indices;
    };


    class BaseWindowShapes {
    private:
        dengUtils::StringRasterizer *m_p_sr;
        dengMath::vec2<deng_ui32_t> m_window_bounds;

    public:
        BaseWindowShapes (
            dengUtils::StringRasterizer *p_sr,
            dengMath::vec2<deng_ui32_t> &window_bounds
        );

        // Add text object to window according to its relative position
        void addRelText (
            dengMath::vec2<deng_vec_t> pos,
            deng_vec_t text_size,
            dengMath::vec2<deng_ui32_t> draw_bounds,
            RectangleOrigin rec_origin,
            dengUtils::BitmapStr text,
            dengMath::vec3<unsigned char> color,
            VERT_UNMAPPED_2D *form_vert,
            std::vector<VERT_MAPPED_2D> &vert,
            std::vector<deng_ui32_t> &indices,
            std::vector<deng_ui8_t> &tex,
            dengMath::vec2<deng_i32_t> &tex_size
        );
    };


    /* Main window class */
    class Window : private BaseWindowShapes{
    private:
        std::string m_id;
        dengUtils::StringRasterizer *m_p_sr;
        std::vector<WindowElement> m_win_elems;
        
        /* Primary color specifies the window background color          *
         * Secondary color specifies the window elements (buttons,      *
           text field background etc colors)                            *
         * Tertiary color specifies only the titlebar color             */
        dengMath::vec4<deng_vec_t> m_primary_color;
        dengMath::vec4<deng_vec_t> m_secondary_color;
        dengMath::vec4<deng_vec_t> m_tertiary_color;

        // Window boundaries and position
        dengMath::vec2<float> m_size;
        dengMath::vec2<float> m_pos;

        // Color specification boolean for checking if window
        bool m_is_color_spec;
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
        dengMath::vec4<float> getPC();
        dengMath::vec4<float> getSC();
        dengMath::vec4<float> getTC();

        // Get all window elements 
        std::vector<WindowElement> getWindowElements();

        std::string getId();

        // Color setters
        void setPC(dengMath::vec4<float> *p_pc);
        void setSC(dengMath::vec4<float> *p_sc);
        void setTC(dengMath::vec4<float> *p_tc);

        // Position and size setters
        void setPos(dengMath::vec2<float> &pos);
        void setSize(dengMath::vec2<float> &size);

    public:
        Window (
            WindowType wt, 
            std::string id, 
            dengUtils::StringRasterizer *p_sr,
            dengMath::vec2<deng_ui32_t> window_bounds
        );

        void makeWindow (
            WindowInfo *p_wi,
            dengMath::vec2<deng_ui32_t> draw_bounds
        );
    };


    /********************************************/
    /********************************************/
    /******** Initialisation functions **********/
    /********************************************/
    /********************************************/
    
    /* Create new DENGUI events handler */
    void beginEventHandler (
        Events *p_ev,
        std::vector<deng_Asset> *p_assets,
        std::vector<deng::TextureImageData> *p_textures,
        std::mutex *p_asset_mut,
        deng::DrawCaller *p_dc,
        deng::ResourceAllocator *p_ra,
        deng::DescriptorCreator *p_desc_c,
        VkDevice device,
        VkQueue g_queue,
        VkRenderPass renderpass,
        VkExtent2D extent,
        dengMath::vec4<float> background,
        dengMath::vec2<deng_ui32_t> draw_area
    );    

    /* Create new window instance */
    void beginWindow (
        Window **pp_window, 
        WindowInfo *p_wi, 
        dengMath::vec2<deng_ui32_t> draw_bounds,
        dengUtils::StringRasterizer *p_sr,
        Events *p_ev,
        deng_bool_t realloc_res
    );

    
    /* Attach child to the window */
    // void attachChild (
    //     Window *p_window,
    //     Events *p_ev,
    //     ChildInfo child_info
    // );
}

#endif
