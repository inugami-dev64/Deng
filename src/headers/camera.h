#include "window.h"
#define DENG_CAMERA_DEFAULT_X 0
#define DENG_CAMERA_DEFAULT_Y 0
#define DENG_CAMERA_DEFAULT_Z 0

namespace deng {
    class Camera
    {
    private:
        Timer m_timer;
        vec4<float> m_movementSpeed;
        
        float m_draw_distance; // distance
        float m_FOV; // in degrees

        float m_current_rotation_from_X_axis;
        vec2<float> m_camera_max_left_corner_coord;
        vec2<float> m_camera_max_centre_point_coord;
        vec2<float> m_camera_max_right_corner_coord;

    public:
        ViewMatrix view_matrix;
        ProjectionMatrix *proj_matrix;

    public:
        Camera(const vec4<float> &movementSpeed, const float &FOV, const float &near,const float &far, Window *window);
        ~Camera();
        void moveF();
        void moveFL();
        void moveFR();
        void moveB();
        void moveBL();
        void moveBR();
        void moveR();
        void moveL();

    };
    
}