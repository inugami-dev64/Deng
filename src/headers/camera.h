#include "window.h"
#define DENG_DEFAULT_CAMERA_X 0
#define DENG_DEFAULT_CAMERA_Y 1.5696856
#define DENG_DEFAULT_CAMERA_Z 0

namespace deng {
    class Camera
    {
    private:
        CameraMath m_camera_math;
        ViewMatrix m_view_matrix;
        ProjectionMatrix m_proj_matrix;

        Timer m_timer;
        vec4<float> m_movementSpeed;
        
        float m_draw_distance; // distance
        float m_FOV; // in degrees

        float m_current_rotation_from_X_axis;
        vec2<float> m_camera_max_left_corner_coord;
        vec2<float> m_camera_max_centre_point_coord;
        vec2<float> m_camera_max_right_corner_coord;

    public:
        Camera(const vec4<float> &movementSpeed, const float &FOV, const float &draw_distance);
        void moveF();
        void moveFL();
        void moveFR();
        void moveB();
        void moveBL();
        void moveBR();
        void moveR();
        void moveL();
        void getViewMatrix(mat4<float> *matrix);
    };
    
}