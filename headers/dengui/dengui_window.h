#ifndef DENGUI_WINDOW_H
#define DENGUI_WINDOW_H

#ifdef __DENGUI_WINDOW_CPP
    #include <string>
    #include <array>
    #include <vector>
    #include <mutex>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>

    #include <deng/forward_dec.h>
    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/window.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <utils/font.h>
    #include <utils/shapes.h>
    #include <utils/collision.h>

    #include <dengui/ui_callbacks.h>
    #include <dengui/dengui_win_def.h>
    #include <dengui/dengui_infos.h>
    #include <dengui/dengui_child.h>
    #include <dengui/vulkan/vulkan_update.h>
    #include <dengui/dengui_events.h>
#endif


namespace dengui {
    /* Main window class */
    class Window {
    private:
        std::string m_id;
        dengUtils::StringRasterizer *m_p_sr;
        std::vector<WindowElement> m_win_elems;
        std::vector<std::pair<void*, ChildElementType>> m_child_elements;
        /* 
         * Primary color specifies the window background color          
         * Secondary color specifies the window elements (buttons,      
         * text field background etc colors)                            
         * Tertiary color specifies only the titlebar color             
         */
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

    /* Respawn existing window instance */
    void respawnWindow (
        Window *p_win,
        Events *p_ev
    );


    /* Destory window instance */
    void destroyWindow (
        Window *p_win,
        Events *p_ev
    );
}

#endif
