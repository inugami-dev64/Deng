#ifndef CAMERA_H
#define CAMERA_H

namespace deng {

    /* Base value structs */
    struct CameraBase {
        WindowWrap *m_p_ww = NULL;
        deng_vec_t m_draw_distance;
        deng_vec_t m_FOV; // Radians
    };

    
    struct EventBase {
    public:
        deng_i32_t m_prev_active_btn_c;
        dengMath::vec2<deng_vec_t> m_mouse_pos;
        dengMath::vec2<deng_vec_t> m_mouse_sens;
    
    public:
        void updateMouseEvData(WindowWrap *p_ww);
        void getMouseRotation (
            WindowWrap *p_ww,
            dengMath::vec2<deng_vec_t> *p_out_rot
        );
        void getMousePositionFromRot (
            WindowWrap *p_ww,
            dengMath::vec2<deng_vec_t> rot
        );
    };


    // Non mov callback function pointer callback type declaration
    typedef void(*FPPCamNonMovementCallback) (dengMath::vec2<deng_vec_t> *p_vmp);

    /* Perspective first person camera event classe */
    class FPPCameraEv : private EventBase {
    private:
        dengUtils::Timer m_mov_timer;
        dengUtils::Timer m_input_mode_timer;
        dengMath::vec4<deng_vec_t> m_move_speed;
        dengMath::vec3<deng_MovementEvent> m_movements;
        dengMath::vec2<deng_vec_t> m_frozen_mouse_pos = {0.0f, 0.0f};
        FPPCamNonMovementCallback m_callback;

    private:
        void findMovementType(WindowWrap *p_ww);
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
            dengMath::vec2<deng_vec_t> mouse_mov_speed_mul,
            dengMath::vec3<deng_vec_t> camera_mov_speed_mul,
            FPPCamNonMovementCallback callback
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
            const dengMath::vec2<deng_vec_t> &mouse_mov_speed_mul, 
            const deng_vec_t &FOV, 
            const deng_vec_t &near_plane, 
            const deng_vec_t &far_plane, 
            FPPCamNonMovementCallback callback,
            WindowWrap *p_window
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


    class EditorCameraEv : private EventBase {
    private:
        deng_EditorCameraEvent m_editor_cam_ev;
        dengMath::vec2<deng_vec_t> m_last_mouse_pos;
        dengMath::vec2<deng_vec_t> m_last_rot;
        deng_vec_t m_zoom_step;
        deng_bool_t m_is_rot_cur = false;

    public:
        EditorCameraEv (
            dengMath::vec2<deng_vec_t> mouse_mov_speed_mul,
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
        
        // Mouse position must be normalized
        void getRotFromMousePos (
            dengMath::vec2<deng_vec_t> mouse_pos,
            dengMath::vec2<deng_vec_t> *p_out_rot
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
            const dengMath::vec2<deng_vec_t> &mouse_mov_speed_mul,
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
