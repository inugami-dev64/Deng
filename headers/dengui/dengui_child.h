#ifndef DENGUI_CHILD_H
#define DENGUI_CHILD_H


#ifdef __DENGUI_CHILD_CPP
    #include <stdlib.h>
    #include <vector>
    #include <array>
    #include <mutex>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/common.h>
    #include <common/err_def.h>
    #include <das/assets.h>

    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <utils/font.h>
    #include <utils/timer.h>
    #include <utils/shapes.h>
    
    #include <dengui/dengui_win_def.h>
    #include <dengui/dengui_infos.h>
#endif


namespace dengui {

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
