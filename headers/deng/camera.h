#ifndef CAMERA_H
#define CAMERA_H

#ifdef __CAMERA_CPP
    #include <iostream>
    #include <vector>
    #include <mutex>
    #include <string.h>
    #include <string>
    #include <math.h>
    #include <array>
    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <common/common.h>
    #include <das/assets.h>

    #include <deng/forward_dec.h>
    #include <deng/surface_window.h>
    #include <deng/deng_math.h>
    #include <deng/window.h>

    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <utils/font.h>
    #include <utils/collision.h>
    #include <utils/timer.h>
    
    #include <dengui/dengui_infos.h>
#endif

namespace deng {

    /*
     * Parent struct for every camera class
     */
    struct CameraBase {
        WindowWrap *m_p_ww = NULL;
        deng_vec_t m_draw_distance;
        deng_vec_t m_FOV; // Radians
    };

    
    /*
     * Base parent class for camera event classes
     * This class contains methods mainly for handling mouse position and its usage
     */
    class EventBase {
    public:
        dengMath::vec2<deng_f64_t> m_mouse_pos;
        dengMath::vec2<deng_f64_t> m_mouse_sens;
        dengMath::vec2<deng_VCPOverflowAction> m_vcp_overflow;
        dengMath::vec2<dengMath::vec2<deng_f64_t>> m_vc_bounds;
        dengMath::vec2<deng_f64_t> m_max_rot;

    public:
        EventBase (
            const dengMath::vec2<deng_f64_t> &mouse_sens,
            const dengMath::vec2<deng_VCPOverflowAction> &vcp_act,
            const dengMath::vec2<dengMath::vec2<deng_f64_t>> &vc_bounds,
            const dengMath::vec2<deng_f64_t> &max_rot
        );

        void updateMouseEvData(WindowWrap *p_ww);
        dengMath::vec2<deng_f64_t> getMouseRotation();
        void getMousePositionFromRot (
            WindowWrap *p_ww,
            dengMath::vec2<deng_f64_t> rot
        );
    };


    // First person camera mouse input mode change callback function type
    // Parameters: current virtual position
    typedef void(*FPPInputChangeCallback) (const dengMath::vec2<deng_f64_t> &vcp);

    /* Perspective first person camera event classe */
    class FPPCameraEv : private EventBase {
    private:
        dengUtils::Timer m_mov_timer;
        dengUtils::Timer m_input_mode_timer;
        dengMath::vec4<deng_vec_t> m_move_speed;
        dengMath::vec3<deng_MovementEvent> m_movements;
        FPPInputChangeCallback m_input_disable_callback;
        FPPInputChangeCallback m_input_enable_callback;

    private:
        void findMovements(WindowWrap *p_ww);
        void checkForInputModeChange (
            WindowWrap *p_ww,
            dengMath::ViewMatrix *p_vm
        );

        void update (
            WindowWrap *p_ww, 
            FPPCamera *p_cam
        );

        deng_bool_t keyEvHandler(deng_Key key);

    public:
        FPPCameraEv (
            WindowWrap *p_ww,
            const dengMath::vec2<deng_f64_t> &mouse_sens,
            const dengMath::vec3<deng_vec_t> &camera_mov_speed,
            dengMath::ViewMatrix *p_vm,
			FPPInputChangeCallback mov_disable_callback,
			FPPInputChangeCallback mov_enable_callback
        );

        void updateEv (
            WindowWrap *p_ww, 
            FPPCamera *p_cam
        );

        dengMath::vec4<deng_vec_t> getMoveSpeed (
            deng_bool_t op_x,
            deng_bool_t op_y,
            deng_bool_t op_z
        );
    };

        
    /* First person perspective camera class */
    class FPPCamera : private FPPCameraEv, private CameraBase {
    public:
        dengMath::ViewMatrix view_matrix;
        dengMath::ProjectionMatrix *p_projection_matrix;

    public:
        FPPCamera (
            const dengMath::vec3<deng_vec_t> &camera_mov_speed_mul, 
            const dengMath::vec2<deng_f64_t> &mouse_sens, 
            const deng_vec_t &FOV, 
            const deng_vec_t &near_plane, 
            const deng_vec_t &far_plane, 
			FPPInputChangeCallback mov_disable_callback,
			FPPInputChangeCallback mov_enable_callback,
            WindowWrap *p_ww
        );
        ~FPPCamera();

        // Move camera position according to its coordinates
        void moveW();
        void moveRW();
        void moveU();
        void moveRU();
        void moveV();
        void moveRV();

        // Wrapper around event update
        void update(); 
    };


    /*
     * Event handler class for editor camera
     */
    class EditorCameraEv : private EventBase {
    private:
        deng_EditorCameraEvent m_editor_cam_ev;
        dengMath::vec2<deng_f64_t> m_last_rot = {0, 0};
        deng_vec_t m_zoom_step;
        deng_bool_t m_is_rot_cur = false;

    public:
        EditorCameraEv (
            dengMath::vec2<deng_f64_t> mouse_sens,
            deng_vec_t zoom_step,
            dengMath::vec3<deng_vec_t> origin,
            WindowWrap *p_ww,
            dengMath::ViewMatrix *p_vm
        );
        void findEditorEvent(WindowWrap *p_ww);

        void zoomIn (
            WindowWrap *p_ww,
            dengMath::ViewMatrix *p_vm
        );

        void zoomOut ( 
            WindowWrap *p_ww,
            dengMath::ViewMatrix *p_vm
        );
        
        void updateEv (
            WindowWrap *p_ww,
            dengMath::vec3<deng_vec_t> origin,
            dengMath::ViewMatrix *p_vm
        );
    };


    class EditorCamera : private EditorCameraEv, private CameraBase {
    private:
        dengMath::vec3<deng_vec_t> m_origin;

    public:
        dengMath::ViewMatrix view_matrix;
        dengMath::ProjectionMatrix *p_projection_matrix;

    public:
        EditorCamera (
            const deng_vec_t &zoom_step,
            const dengMath::vec3<deng_vec_t> &origin,
            const dengMath::vec2<deng_f64_t> &mouse_sens,
            const deng_vec_t &FOV,
            const deng_vec_t &near_plane,
            const deng_vec_t &far_plane,
            WindowWrap *p_ww
        );

        ~EditorCamera();
        
        // Wrapper around event update
        void update();
    };
}

#endif
