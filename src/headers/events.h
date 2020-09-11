#include "gridgenerator.h"

namespace deng {
    enum dengMovementEvents {
        DENG_MOVEMENT_NONE = -1,
        DENG_MOVEMENT_FORWARD = 0,
        DENG_MOVEMENT_BACKWARD = 1,
        DENG_MOVEMENT_RIGHTWARD = 2,
        DENG_MOVEMENT_LEFTWARD = 3,
        DENG_MOVEMENT_UPWARD = 4,
        DENG_MOVEMENT_DOWNWARD = 5
    };

    struct Movements {
        dengMovementEvents x;
        dengMovementEvents y;
        dengMovementEvents z;
    };

    class Events
    {
    private:
        Timer m_timer;
        Camera *m_camera;
        Window *m_window;
        Movements m_movements;

        #if DEBUG
            SpecifiedObject *m_grid;
        #endif

    private:
        void getMovementType();
        void checkForInputModeChange();
        dengBool isKeyPressed(const int &key);
        void isMouseKeysPressed(dengBool *lmb_click, dengBool *mmb_click, dengBool *rmb_click);

        #if DEBUG
            void handleGridCoordinateLogging(const int32_t &log_key, const std::string &file_name);
        #endif
        

    public:
        Events(Window *p_win, Camera *p_camera);
        dengMovementEvents getMovement();
        void handleExit();
        void update();

        #if DEBUG
            void setGrid(SpecifiedObject *grid);
        #endif
    };
}