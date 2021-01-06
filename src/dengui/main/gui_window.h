#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

typedef uint32_t WindowID;
// In pixels
#define LIGHT_BORDER_THICKNESS      1
#define MEDIUM_BORDER_THICKNESS     2
#define HEAVY_BORDER_THICKNESS      5

// Define default colors
#define DEFAULT_PRIMARY_COLOR       {0.0f, 1.0f, 1.0f, 0.5f}
#define DEFAULT_SECONDARY_COLOR     {1.0f, 1.0f, 1.0f, 1.0f}
#define DEFAULT_TERTIARY_COLOR      {0.0f, 0.2f, 1.0f, 1.0f}

// Define default position and size
#define DEFAULT_POS                 {0.0f, 0.0f}
#define DEFAULT_SIZE                {0.1f, 0.1f}

// Window flags
#define WINDOW_FLAG_NULL            0x00
#define WINDOW_FLAG_MENUBAR         0x01
#define WINDOW_FLAG_NO_COLLAPSE     0x02
#define WINDOW_FLAG_NO_MOVE         0x04
#define WINDOW_FLAG_NO_TITLEBAR     0x08
#define WINDOW_FLAG_ALWAYS_ON_TOP   0x10
#define WINDOW_FLAG_NO_RESIZE       0x20
#define WINDOW_FLAG_NO_CLOSE        0x40

// Default window parametres
#define TITLEBAR_HEIGHT             0.05f
#define TITLEBAR_ELEM_MARGIN        0.005f
#define DEFAULT_FONT_FILE           "FreeMono.ttf"

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
        REC_ORIGIN_VERTEX_TOP_LEFT      = 0,
        REC_ORIGIN_VERTEX_TOP_RIGHT     = 1,
        REC_ORIGIN_VERTEX_BOTTOM_LEFT   = 2,
        REC_ORIGIN_VERTEX_BOTTOM_RIGHT  = 3
    };


    /* Drop-down menu class for menubar */
    class DropDownMenu {
    private:
        std::vector<std::string> m_elem_names;
        std::vector<WindowID> m_window_ids;
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


    /* Window info struct */
    struct WindowInfo {
        std::string title;
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
        std::string m_title;
        dengUtils::FontManager *m_p_fm;
        
        // Non texture mapped window vertices
        std::vector<VERT_UNMAPPED_2D> m_vertices;
        std::vector<uint32_t> m_indices;
        std::vector<UNI_OFFSET> m_offsets;   // Offsets in vector elements not in bytes!!!

        // Bitmap string instances
        std::vector<dengUtils::bitmapStr> m_rend_text;
        
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
            OBJColorData color
        );

        // Add generic unmapped rectangle to window vertices using relative positions
        void addRelUnmappedRec (
            dengMath::vec2<float> pos,
            dengMath::vec2<float> size,
            bool is_abs_height,
            RectangleOrigin rec_origin,
            OBJColorData color
        );

        // Add generic triangle object to window
        void addGenTriangle (
            dengMath::vec2<float> pos,
            dengMath::vec2<float> surrond_rec_size,
            std::array<dengMath::vec2<float>, 3> rel_rec_coords,
            bool is_abs_size,
            RectangleOrigin rec_origin,
            OBJColorData color
        );

        // Add text object to window
        void addText (
            dengMath::vec2<float> pos,
            float text_size,
            dengMath::vec2<float> draw_bounds,
            RectangleOrigin rec_origin,
            dengUtils::bitmapStr text,
            dengMath::vec3<unsigned char> color
        );

        // Add element to clickable element list


    // Getters and setters
    public:
        // Color getters
        dengMath::vec4<float> getPC();
        dengMath::vec4<float> getSC();
        dengMath::vec4<float> getTC();

         // Vertices and Indices getters
        std::vector<VERT_UNMAPPED_2D> getVerts();
        std::vector<uint32_t> getInds();
        std::vector<dengUtils::bitmapStr> getTextBoxes(); 
        // Offsets getter
        std::vector<UNI_OFFSET> getOffsets();

        std::string getTitle();

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
        Window(WindowType wt, std::string title, dengUtils::FontManager *p_fm);
        std::vector<ElementInfo> makeWindow(WindowInfo *p_wi, dengMath::vec2<float> draw_bounds);
        void collapse();
    };


    /********************************************/
    /********************************************/
    /******** Initialisation functions **********/
    /********************************************/
    /********************************************/

    // Create new window instance
    void beginWindow (
        Window **pp_window, 
        WindowInfo *p_wi, 
        dengMath::vec2<float> draw_bounds,
        Events *p_ev,
        dengUtils::FontManager *p_fm
    );

    // Get all window elements made into assets
    void getAssets (
        Window *windows, 
        int32_t window_c, 
        DENGAsset **p_assets, 
        int32_t *p_asset_c,
        dengUtils::bitmapStr **p_bm_strs,
        int32_t *p_bm_str_c
    );
    
    // Clear window
    void clearWindow(Window *p_window); 
}

#endif