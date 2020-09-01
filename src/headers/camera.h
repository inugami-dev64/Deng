#include "window.h"
#define DENG_CAMERA_DEFAULT_X 0
#define DENG_CAMERA_DEFAULT_Y 0
#define DENG_CAMERA_DEFAULT_Z 1

#define DENG_CAMERA_BASE_SPEED_X 0.0006
#define DENG_CAMERA_BASE_SPEED_Y 0.0006
#define DENG_CAMERA_BASE_SPEED_Z -0.0006

#define DENG_MOUSE_BASE_SENSITIVITY_X 2.5
#define DENG_MOUSE_BASE_SENSIVITY_Y 0.75

namespace deng {
    class Camera
    {
    private:
        Window *m_window;
        vec2<float> m_mouse_sens;
        vec2<double> m_mouse_pos; // only reason it is double because glfwGetCursorPos() requires double
        vec4<float> m_movementSpeed;
        
        float m_draw_distance; // distance
        float m_FOV; // in degrees

        float m_current_rotation_from_X_axis;
        vec2<float> m_camera_max_left_corner_coord;
        vec2<float> m_camera_max_centre_point_coord;
        vec2<float> m_camera_max_right_corner_coord;

        DengBool m_isInit;

    public:
        ViewMatrix view_matrix;
        ProjectionMatrix *proj_matrix;

    public:
        Camera(const vec3<float> &cameraMovementSpeedMultiplier, const vec2<float> &mouseMovementSpeedMultiplier, const float &FOV, const float &near,const float &far, Window *window);
        ~Camera();
        void updateCursorPos();
        void setCameraViewRotation();
        void moveF();
        void moveB();
        void moveR();
        void moveL();
        void moveUp();
        void moveDown();

    };
    
}