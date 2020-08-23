#include "window.h"
#define DENG_CAMERA_DEFAULT_X 0
#define DENG_CAMERA_DEFAULT_Y 0
#define DENG_CAMERA_DEFAULT_Z 1

namespace deng {
    class Camera
    {
    private:
        Timer m_timer;
        Window *m_window;
        float m_mouse_sens;
        double m_mouse_xPos, m_mouse_yPos;

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
        Camera(const vec4<float> &movementSpeed, const float &FOV, const float &near,const float &far, Window *window);
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