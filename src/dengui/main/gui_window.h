#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

typedef uint32_t WindowID;
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
#define DENGUI_DEFAULT_FONT_FILE            "FreeMono.ttf"

// Main window element ids
#define DENGUI_FORM_ID                      "form"
#define DENGUI_TITLEBAR_ID                  "titlebar"
#define DENGUI_MINIMISE_TRIANGLE_ID         "min_triangle"
#define DENGUI_MAXIMISE_TRIANGLE_ID         "max_triangle"
#define DENGUI_CLOSE_BTN_ID                 "close_btn"

namespace dengui {

    /* Window type enums */
    enum WindowType {
        WINDOW_TYPE_FLOATING        = 0,
        WINDOW_TYPE_STATIC_RIGHT    = 1,
        WINDOW_TYPE_STATIC_LEFT     = 2,
        WINDOW_TYPE_STATIC_TOP      = 3,
        WINDOW_TYPE_STATIC_BOTTOM   = 4
    };


    /* Specify window border thickness */ 
    enum WindowBorder {
        WINDOW_BORDER_NONE      = 0,
        WINDOW_BORDER_LIGHT     = 1,
        WINDOW_BORDER_MEDIUM    = 2,
        WINDOW_BORDER_HEAVY     = 3    
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
        std::vector<uint32_t> indices; 
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

    
    /* Menubar object specifications */
    struct MenubarElement {
        std::string m_title;
        DropDownMenu m_menu;
    };


    /* Menubar class */
    class Menubar {
    private:
        std::vector<dengMath::vec3<float>> m_vertices;
        std::vector<uint32_t> m_indices;
        
        std::vector<MenubarElement> m_elems;
        dengMath::vec4<float> m_color;
        WindowBorder wb;

    public:
        Menubar(std::vector<MenubarElement> &elements);
        void setWindowProps(dengMath::vec4<float> &color, WindowBorder wb);
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
        std::vector<VERT_UNMAPPED_2D> unmapped_vert;
        std::vector<VERT_MAPPED_2D> mapped_vert;
        std::vector<uint32_t> indices;
        std::vector<uint8_t> texture;
        dengMath::vec2<int32_t> tex_box;
    };


    /* Window info struct */
    struct WindowInfo {
        std::string id;
        Menubar *p_mb;
        WindowType wt;
        denguiWindowFlagBits fl_b;
        dengMath::vec4<float> pc;
        dengMath::vec4<float> sc;
        dengMath::vec4<float> tc;
        dengMath::vec2<float> pos;
        dengMath::vec2<float> size;

        WindowInfo();
    };


    /* Main window class */
    class Window {
    private:
        std::string m_id;
        dengUtils::FontManager *m_p_fm;
        std::vector<WindowElement> m_win_elems;
        
        /* Primary color specifies the window background color          *
         * Secondary color specifies the window elements (buttons,      *
           text field background etc colors)                            *
         * Tertiary color specifies only the titlebar color             */
        dengMath::vec4<float> m_primary_color;
        dengMath::vec4<float> m_secondary_color;
        dengMath::vec4<float> m_tertiary_color;

        // Window boundaries and position
        dengMath::vec2<float> m_size;
        dengMath::vec2<float> m_pos;

        // Color specification boolean for checking if window
        bool m_is_color_spec;
        WindowType m_wt;
        Menubar *m_p_mb;
    
    private:
        // Index all vertices
        void indexVerts();
        // Calculate all vertices
        void calcVerts();
        
        // Add generic unmapped rectangle to window vertices using absolute positions
        void addAbsUnmappedRec (
            dengMath::vec2<float> pos, 
            dengMath::vec2<float> size, 
            RectangleOrigin rec_origin,
            deng_ObjColorData color,
            std::vector<VERT_UNMAPPED_2D> &vert,
            std::vector<uint32_t> &indices
        );

        // Add generic unmapped rectangle to window vertices using relative positions
        void addRelUnmappedRec (
            dengMath::vec2<float> pos,
            dengMath::vec2<float> size,
            bool is_abs_height,
            RectangleOrigin rec_origin,
            deng_ObjColorData color,
            std::array<deng_ObjVertData2D, 4> &form_vert,
            std::vector<VERT_UNMAPPED_2D> &vert,
            std::vector<uint32_t> &indices
        );

        // Add generic triangle object to window
        void addGenTriangle (
            dengMath::vec2<float> pos,
            dengMath::vec2<float> surrond_rec_size,
            std::array<dengMath::vec2<float>, 3> rel_rec_coords,
            bool is_abs_size,
            RectangleOrigin rec_origin,
            deng_ObjColorData color,
            std::array<deng_ObjVertData2D, 4> &form_vert,
            std::vector<VERT_UNMAPPED_2D> &vert,
            std::vector<uint32_t> &indices
        );

        // Add text object to window
        void addText (
            dengMath::vec2<float> pos,
            float text_size,
            dengMath::vec2<uint32_t> draw_bounds,
            RectangleOrigin rec_origin,
            dengUtils::bitmapStr text,
            dengMath::vec3<unsigned char> color,
            std::array<deng_ObjVertData2D, 4> &form_vert,
            std::vector<VERT_MAPPED_2D> &vert,
            std::vector<uint32_t> &indices,
            std::vector<uint8_t> &tex,
            dengMath::vec2<int32_t> &tex_size
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

        // Set menubar
        void setMB(Menubar *p_mb);

    public:
        Window (
            WindowType wt, 
            std::string id, 
            dengUtils::FontManager *p_fm
        );

        void minimize (
            std::vector<deng_Asset> *p_assets, 
            std::vector<deng_Texture> *p_textures
        );

        void makeWindow (
            WindowInfo *p_wi, 
            dengMath::vec2<uint32_t> draw_bounds
        );
        void collapse();
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
        dengMath::vec2<uint32_t> draw_area
    );    


    /* Create new window instance */
    void beginWindow (
        Window **pp_window, 
        WindowInfo *p_wi, 
        dengMath::vec2<uint32_t> draw_bounds,
        dengUtils::FontManager *p_fm,
        Events *p_ev
    );

    /* Get all window elements made into assets */
    void getAssets (
        Window *windows, 
        int32_t window_c, 
        deng_Asset **p_assets, 
        int32_t *p_asset_c,
        deng_Texture **p_textures,
        int32_t *p_tex_c
    );

    // Destroy window
    void destroyWindow(Window *p_window);
}

#endif