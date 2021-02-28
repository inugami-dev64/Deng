#ifndef DENGUI_CHILD_H
#define DENGUI_CHILD_H

#define DENGUI_DROP_DOWN_LIST_TRIANGLE_SIZE     0.1 // vec
#define DENGUI_DROP_DOWN_LIST_TRIANGLE          { \
                                                    (dengMath::vec2<deng_vec_t>) {-1.0f, -1.0f}, \
                                                    (dengMath::vec2<deng_vec_t>) {1.0f, 0.0f}, \
                                                    (dengMath::vec2<deng_vec_t>) {-1.0f, 1.0f} \
                                                }


// Hierarchy Level specifiers 
#define DENGUI_CHILD_PUSH_BUTTON_LEVEL          2
#define DENGUI_CHILD_CONTAINER_LEVEL            1
#define DENGUI_DROP_DOWN_LIST_LEVEL             4
namespace dengui {

    /* Specify size information about parent window */
    struct ParentInfo {
        char *id;
        VERT_UNMAPPED_2D *vert;
    };

    /* Specify window border thickness (px) */
    enum WindowBorder {
        BORDER_NONE      = 0,
        BORDER_LIGHT     = 2,
        BORDER_MEDIUM    = 4,
        BORDER_HEAVY     = 7    
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


    /* Child window overlapping mode */
    enum ContainerOverlapMode {
        CONTAINER_OVERLAP_HIDE      = 0,
        CONTAINER_OVERLAP_SCROLL    = 1,
        CONTAINER_OVERLAP_NONE      = 2
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
    struct ChildContainerTitlebarData {
        deng_vec_t titlebar_height;
        deng_ObjColorData background_color;
        char *label;
        dengMath::vec3<unsigned char> color;
        deng_bool_t add_label;
    };


    struct ChildContainerInfo : public ChildBaseInfo {
        deng_ObjColorData background_color;
        deng_bool_t add_titlebar;
        ChildContainerTitlebarData titlebar_info;
        WindowBorder border;
        deng_ObjColorData border_color;
    };


    class ChildContainer {
    private:
        ParentInfo m_parent_info;
        ChildContainerInfo m_info;
        dengMath::vec2<deng_ui32_t> m_window_size;
        dengUtils::StringRasterizer *m_p_sr;
        std::vector<VERT_UNMAPPED_2D> m_vert;
        std::vector<deng_ui32_t> m_indices;
        dengUtils::BitmapStr m_label;
        std::vector<std::pair<void*, ChildElementType>> m_cont_child_elems;
        std::vector<std::pair<void*, ChildElementType>> m_titlebar_child_elems;
        dengMath::vec2<deng_ui32_t> m_cont_vert_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> m_cont_ind_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> m_titlebar_vert_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> m_titlebar_ind_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> m_label_vert_bounds = {0, 0};
        dengMath::vec2<deng_ui32_t> m_lavel_ind_bounds = {0, 0};

    private:
        void mkTitlebar();

    public:
        ChildContainer (
            ParentInfo parent_info,
            ChildContainerInfo m_info,
            dengMath::vec2<deng_ui32_t> window_size,
            dengUtils::StringRasterizer *m_p_sr
        );

        ParentInfo getTitlebarParentInfo();
        ParentInfo getContainerParentInfo();
        void attachChild (
            void *elem, 
            ChildElementType elem_type
        );

        char *getParentID();
        std::vector<WindowElement> getWindowElements();
    };


    /* 
     ********* PUSH BUTTON STRUCTURES *********
     * Hierarchy level: 2
     * Callback: yes
     * Callback key: LMB
     * Cursor hover style: no
     * Parent usage: no
     */
    struct PushButtonInfo : public ChildBaseInfo {
        deng_ObjColorData background_color;
        dengMath::vec3<deng_ui8_t> color;
        deng_ObjColorData border_color;
        WindowBorder border;
        void (*onClick)(WindowElement*, Events*);
        char *label;
    };


    class ChildPushButton {
    private:
        ParentInfo m_parent_info;
        PushButtonInfo m_info;
        dengUtils::StringRasterizer *m_p_sr;
        std::vector<VERT_UNMAPPED_2D> m_vert;
        std::vector<deng_ui32_t> m_indices;
        dengUtils::BitmapStr m_label;

    public:
        ChildPushButton (
            ParentInfo parent_info,
            PushButtonInfo &btn_info,
            dengMath::vec2<deng_ui32_t> window_size,
            dengUtils::StringRasterizer *p_sr 
        );

        char *getParentID();
        std::vector<WindowElement> getWindowElements();
    };


    /*
     ********* GENERAL DROP DOWN MENU STRUCTURES *********
     * Hierarchy level: 4
     * Callback: yes
     * Callback key: LMB
     * Cursor hover style: no
     * Parent usage: no
     */

    struct DropDownElementInfo {
        char *label;
        deng_bool_t is_expandable;
        std::vector<DropDownElementInfo> sub_elements;
        dengMath::vec4<deng_vec_t> show_more_tri_color;
        DDMCallback lmb_callback;
        DDMCallback mmb_callback;
        DDMCallback rmb_callback;
        DDMCallback hover_callback;
    };


    class DropDownElement {
    private:
        char *m_parent_id;
        dengUtils::BitmapStr m_label;
        dengUtils::StringRasterizer *m_p_sr;
        std::vector<DropDownElement> m_sub_elements; 
        std::vector<VERT_UNMAPPED_2D> m_vert;
        std::vector<deng_ui32_t> m_indices;
        DropDownElementInfo m_info;

    public:
        DropDownElement (
            char *parent_id,
            dengMath::vec2<deng_ui32_t> window_size,
            DropDownElementInfo info,
            dengMath::vec2<deng_vec_t> size,
            deng_vec_t y_offset,
            dengUtils::StringRasterizer *p_sr,
            deng_ObjColorData background_color,
            deng_ObjColorData border_color,
            dengMath::vec3<unsigned char> color,
            WindowBorder border
        );

        //~DropDownElement();
        
        // Position is from top left corner
        std::vector<DDMElement> getElems(dengMath::vec2<deng_ui32_t> win_size);
    };


    struct DropDownMenuInfo {
        dengMath::vec2<deng_vec_t> size;
        deng_ObjColorData background_color;
        dengMath::vec3<unsigned char> color;
        WindowBorder border;
        deng_ObjColorData border_color;
        dengUtils::StringRasterizer *p_sr;
        char *id;
    };


    class CursorDropDownMenu {
    private:
        DropDownMenuInfo m_info;
        std::vector<DropDownElement> m_menu_elems;
        dengMath::vec2<deng_ui32_t> m_win_size;
        
    private:
        std::vector<DDMElement> mkEmptyMenu();

    public:
        CursorDropDownMenu (
            DropDownMenuInfo &info,
            dengMath::vec2<deng_ui32_t> win_size
        );

        void attachMenuElement(DropDownElementInfo &elem_info);
        std::string getId();
        std::vector<DDMElement> getElems();
    };
}

#endif
