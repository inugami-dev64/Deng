#include "window.h"
#define DENG_CAMERA_DEFAULT_X 0.0f
#define DENG_CAMERA_DEFAULT_Y 0.8f
#define DENG_CAMERA_DEFAULT_Z 1.0f

#define DENG_CAMERA_BASE_SPEED_X 0.0007
#define DENG_CAMERA_BASE_SPEED_Y 0.0007
#define DENG_CAMERA_BASE_SPEED_Z -0.0007

#define DENG_MOUSE_BASE_SENSITIVITY_X 4.0
#define DENG_MOUSE_BASE_SENSIVITY_Y 1.0

namespace deng {
    class Camera
    {
    private:
        Window *m_p_window;
        vec2<float> m_mouse_sens;
        vec2<double> m_mouse_pos; // only reason it is double because glfwGetCursorPos() double parametre
        vec4<float> m_movementSpeed;
        
        float m_draw_distance; // distance
        float m_FOV; // in degrees

        float m_current_rotation_from_X_axis;
        vec2<float> m_camera_max_left_corner_coord;
        vec2<float> m_camera_max_centre_point_coord;
        vec2<float> m_camera_max_right_corner_coord;

        dengBool m_is_init;

    public:
        ViewMatrix view_matrix;
        ProjectionMatrix *p_projection_matrix;

    public:
        Camera(const vec3<float> &camera_movement_speed_multiplier, const vec2<float> &mouse_movement_speed_multiplier, const float &FOV, const float &near, const float &far, Window *p_window);
        ~Camera();
        void updateCursorPos();
        void setCameraViewRotation();

        void setMousePosition(vec2<float> &mouse_pos);
        void getMousePosition(vec2<float> *mouse_pos);

        void moveF();
        void moveB();
        void moveR();
        void moveL();
        void moveUp();
        void moveDown();

    };
    
}