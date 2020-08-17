#include "camera.h"

namespace deng {
    enum MovementEvents {
        DENG_MOVEMENT_NONE = -1,
        DENG_MOVEMENT_FORWARD = 0,
        DENG_MOVEMENT_FORWARDRIGHT = 1,
        DENG_MOVEMENT_FORWARDLEFT = 2,
        DENG_MOVEMENT_BACKWARD = 3,
        DENG_MOVEMENT_BACKWARDRIGHT = 4,
        DENG_MOVEMENT_BACKWARDLEFT = 5,
        DENG_MOVEMENT_RIGHTWARD = 6,
        DENG_MOVEMENT_LEFTWARD = 7
    };

    class Events
    {
    private:
        Timer m_timer;
        MovementEvents m_deng_movement;
        Camera *m_camera;
        Window *m_window;

    private:
        void getMovementType(GLFWwindow *window);

    public:
        Events(Window *win, Camera *camera);
        MovementEvents getMovement();
        void handleExit();
        void update();
    };
}