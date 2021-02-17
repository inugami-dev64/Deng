#ifndef DENGUI_CHILD_H
#define DENGUI_CHILD_H
#define DENGUI_PUSH_BUTTON_LABEL_PADDING    7.0 // px

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
        CHILD_ELEMENT_TYPE_PUSH_BUTTON              = 0,
        CHILD_ELEMENT_TYPE_DROP_DOWN_MENU           = 1,
        CHILD_ELEMENT_TYPE_CURSOR_DROP_DOWN_MENU    = 2,
        CHILD_ELEMENT_TYPE_SUB_FORM                 = 3,
        CHILD_ELEMENT_TYPE_TEXT_FIELD               = 4,
        CHILD_ELEMENT_TYPE_LABEL                    = 5,
        CHILD_ELEMENT_TYPE_CONTAINER                = 6
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
     * Hierarchy level: 2
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
     * Hierarchy level: 3
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
}

#endif
