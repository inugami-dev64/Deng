#include "objectshandler.h"

namespace deng {
    enum MovementEvents {
        DENG_MOVEMENT_NONE = -1,
        DENG_MOVEMENT_FORWARD = 0,
        DENG_MOVEMENT_BACKWARD = 1,
        DENG_MOVEMENT_RIGHTWARD = 2,
        DENG_MOVEMENT_LEFTWARD = 3,
        DENG_MOVEMENT_UPWARD = 4,
        DENG_MOVEMENT_DOWNWARD = 5
    };

    struct Movements {
        MovementEvents x;
        MovementEvents y;
        MovementEvents z;
    };

    class Events
    {
    private:
        Timer m_timer;
        Camera *m_camera;
        Window *m_window;
        GameObject *m_obj;
        Movements m_movements;

    private:
        void getMovementType();
        void checkForObjLogRequest();
        void checkForInputModeChange();
        void handleLogging();

    public:
        Events(Window *win, Camera *camera, GameObject *obj);
        MovementEvents getMovement();
        void handleExit();
        void update();
    };
}