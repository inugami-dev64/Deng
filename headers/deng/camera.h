#ifndef CAMERA_H
#define CAMERA_H


namespace deng {

    /* Base value structs */
    struct CameraBaseValues {
        WindowWrap *m_p_ww;
        deng_vec_t m_draw_distance;
        deng_vec_t m_FOV; // Radians
    };

    
    struct EventBaseValues {
        deng_i32_t m_prev_active_btn_c;
        dengMath::vec2<deng_vec_t> m_mouse_pos;
        dengMath::vec2<deng_vec_t> m_mouse_sens;
    };


    // Non mov callback function pointer callback type declaration
    typedef void(*FPPCamNonMovementCallback)(dengMath::vec2<deng_vec_t> *p_vmp);

    /* Perspective first person camera event classe */
    class FPPCameraEv : private EventBaseValues {
    private:
        dengUtils::Timer m_mov_timer;
        dengUtils::Timer m_input_mode_timer;
        dengMath::vec4<deng_vec_t> m_move_speed;
        dengMath::vec3<deng_MovementEvent> m_movements;
        deng_bool_t is_set = false;
        dengMath::vec2<deng_vec_t> m_frozen_mouse_pos = {0.0f, 0.0f};
        FPPCamNonMovementCallback m_callback;

    private:
        void findMovementType(WindowWrap *p_ww);
        
        void checkForInputModeChange (
            WindowWrap *p_ww,
            dengMath::ViewMatrix *p_vm
        );

        void updateMouseEvData(WindowWrap *p_ww);
        void setCameraViewRotation (
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
        dengMath::vec4<deng_vec_t> getMoveSpeed();
    };

        
    /* First person perspective camera class */
    class FPPCamera : private FPPCameraEv, private CameraBaseValues {
    private:
        deng_i32_t m_prev_active_c;
        bool m_is_init;

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
}

#endif
