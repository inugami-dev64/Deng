#ifndef CAMERA_H
#define CAMERA_H

#define DENG_CAMERA_DEFAULT_X 0.0f
#define DENG_CAMERA_DEFAULT_Y 0.0f
#define DENG_CAMERA_DEFAULT_Z 1.0f

#define DENG_CAMERA_BASE_SPEED_X 0.001
#define DENG_CAMERA_BASE_SPEED_Y 0.001
#define DENG_CAMERA_BASE_SPEED_Z -0.0007

#define DENG_MOUSE_BASE_SENSITIVITY_X 4.0
#define DENG_MOUSE_BASE_SENSIVITY_Y 1.0

namespace deng {
    
    class Camera
    {
    private:
        dengMath::vec2<float> m_zero_camera_rotation_mouse_pos;
        WindowWrap *m_p_window_wrap;
        dengMath::vec2<float> m_mouse_sens;
        dengMath::vec2<float> m_mouse_pos; 
        dengMath::vec4<float> m_movement_speed;
        
        float m_draw_distance; // distance
        float m_FOV; // in degrees

        float m_current_rotation_from_X_axis;
        dengMath::vec2<float> m_camera_max_left_corner_coord;
        dengMath::vec2<float> m_camera_max_centre_point_coord;
        dengMath::vec2<float> m_camera_max_right_corner_coord;


        deng_i32_t m_prev_active_c;
        bool m_is_init;

    public:
        dengMath::ViewMatrix view_matrix;
        dengMath::ProjectionMatrix *p_projection_matrix;

    public:
        Camera (
            const dengMath::vec3<float> &camera_movement_speed_multiplier, 
            const dengMath::vec2<float> &mouse_movement_speed_multiplier, 
            const float &FOV, 
            const float &near_plane, 
            const float &far_plane, 
            WindowWrap *p_window
        );
        ~Camera();
        void updateCursorPos();
        void setCameraViewRotation();

        void setMousePosition(dengMath::vec2<float> &mouse_pos);
        void getMousePosition(dengMath::vec2<float> *mouse_pos);

        void moveF();
        void moveB();
        void moveR();
        void moveL();
        void moveUp();
        void moveDown();

    };
    
}

#endif