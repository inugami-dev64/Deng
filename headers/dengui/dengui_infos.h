#ifndef __DENGUI_INFOS_H
#define __DENGUI_INFOS_H

namespace dengui {
    /* 
     * Window type enum
     */
    enum WindowType {
        WINDOW_TYPE_FLOATING        = 0,
        WINDOW_TYPE_STATIC_RIGHT    = 1,
        WINDOW_TYPE_STATIC_LEFT     = 2,
        WINDOW_TYPE_STATIC_TOP      = 3,
        WINDOW_TYPE_STATIC_BOTTOM   = 4
    };


    /* 
     * Enum for specifing element color mode 
     */
    enum ElementColorMode {
        ELEMENT_COLOR_MODE_UNMAPPED         = 0,
        ELEMENT_COLOR_MODE_TEXTURE_MAPPED   = 1
    };


    /* Specify window border thickness (px) */
    enum WindowBorder {
        BORDER_NONE      = 0,
        BORDER_LIGHT     = 2,
        BORDER_MEDIUM    = 4,
        BORDER_HEAVY     = 7    
    };


    /******************************************/
    /******* COPIED FROM dengui_child.h *******/
    /******************************************/

    /* Specify size information about parent window */
    struct ParentInfo {
        char *id;
        VERT_UNMAPPED_2D *vert;
    };


    /* Child element specifier */
    enum ChildElementType {
        CHILD_ELEMENT_TYPE_PUSH_BUTTON              = 0, // done
        CHILD_ELEMENT_TYPE_DROP_DOWN_MENU           = 1, 
        CHILD_ELEMENT_TYPE_CURSOR_DROP_DOWN_MENU    = 2, // undone
        CHILD_ELEMENT_TYPE_SUB_FORM                 = 3,
        CHILD_ELEMENT_TYPE_TEXT_FIELD               = 4,
        CHILD_ELEMENT_TYPE_LABEL                    = 5,
        CHILD_ELEMENT_TYPE_CONTAINER                = 6 // done
    };


    /* Base child element info struct */
    struct ChildBaseInfo { 
        dengMath::vec2<deng_vec_t> origin;
        dengMath::vec2<deng_vec_t> position;
        dengMath::vec2<deng_vec_t> size;
        char *id;
    };



    /*
     ********* CONTAINER STRUCTURES ***********
     * Hierarchy level: 1
     * Callback no
     * Callback key: none
     * Cursor hove style: no
     * Parent usage: yes
     */
    struct ChildContainerInfo : public ChildBaseInfo {
        deng_ObjColorData background_color;
        deng_ObjColorData titlebar_color;
        dengMath::vec3<unsigned char> color; 
        deng_vec_t titlebar_height;
        char *label;

        WindowBorder border;
        deng_ObjColorData border_color;
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


    /* Window creation bit flag type */
    typedef deng_ui32_t dengui_WindowFlagBits;

    /* Window info struct */
    struct WindowInfo {
        std::string label;
        std::string id;
        WindowType wt;
        WindowBorder border;
        dengui_WindowFlagBits fl_b;
        dengMath::vec4<deng_vec_t> pc;
        dengMath::vec4<deng_vec_t> sc;
        dengMath::vec4<deng_vec_t> tc;
        dengMath::vec2<deng_vec_t> pos;
        dengMath::vec2<deng_vec_t> size;

        WindowInfo();
    };


    /*
     * Float Info struct
     * This struct is used to store information about floating window
     */
    struct FloatInfo {
        dengMath::vec2<deng_ui32_t> asset_bounds;
        dengMath::mat3<deng_vec_t> fl_delta_transform;
        deng_bool_t *p_float_flag;
    };


    /* 
     * Struct for storing all information related to mouse input 
     * Read-only for events thread                               
     */
    struct MouseInputInfo {
        std::mutex mut;
        dengMath::vec2<deng_px_t> mouse_coords;
        deng_bool_t is_mouse_input;
    };
    

    /* 
     * Cursor drop down menu element information 
     */
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


    /* 
     * Dengui window information
     */
    struct WindowData {
        std::string window_id;
        std::vector<WindowElement> win_elems;
        deng_bool_t is_visible;
        dengMath::vec2<deng_ui32_t> asset_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> tex_bounds = {0, 0};
    };


    /* 
     * Event creation info
     * The instance of this struct is used to construct dengui::vulkan::Events class
     */
    struct EventInfo {
        dengMath::vec2<deng_ui32_t> deng_window_area;
        std::vector<deng_Asset> *p_assets;
        std::vector<deng::vulkan::TextureImage> *p_vk_textures;
        std::mutex *p_res_mut;
        std::mutex *p_run_mut;
        std::mutex *p_frame_mut;
        size_t sc_img_size;
    
        deng::vulkan::InstanceCreator *p_ic;
        deng::vulkan::ResourceAllocator *p_ra;
        deng::vulkan::DrawCaller *p_dc;
        deng::vulkan::DescriptorCreator *p_desc_c;
        VkExtent2D ext;
        VkRenderPass renderpass;
        dengMath::vec4<deng_vec_t> background;
    };
}

#endif
