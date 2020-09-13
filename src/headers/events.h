#include "gridgenerator.h"

#define DENG_INPUT_MODE_CHANGE_INTERVAL 200
#define DENG_MOVEMENT_INTERVAL 1

namespace deng {
    enum dengMovementEvent {
        DENG_MOVEMENT_NONE = -1,
        DENG_MOVEMENT_FORWARD = 0,
        DENG_MOVEMENT_BACKWARD = 1,
        DENG_MOVEMENT_RIGHTWARD = 2,
        DENG_MOVEMENT_LEFTWARD = 3,
        DENG_MOVEMENT_UPWARD = 4,
        DENG_MOVEMENT_DOWNWARD = 5
    };

    class Events
    {
    private:
        Timer m_movement_timer;
        Timer m_input_mode_change_timer;

        Camera *m_p_camera;
        Window *m_p_window;
        vec3<dengMovementEvent> m_movements;
        vec2<float> m_frozen_mouse_position;
        

        #if DEBUG
            SpecifiedObject *m_grid;
        #endif

    private:
        void getMovementType();
        void checkForInputModeChange();
        dengBool isKeyPressed(const int &key, const dengBool &isRepeated, dengBool *isReleased);
        void isMouseKeysPressed(dengBool *lmb_click, dengBool *mmb_click, dengBool *rmb_click);

        #if DEBUG
            void handleGridCoordinateLogging(const int32_t &log_key, const std::string &file_name);
        #endif
        

    public:
        Events(Window *p_win, Camera *p_camera);
        dengMovementEvent getMovement();
        void handleExit();
        void update();

        #if DEBUG
            void setGrid(SpecifiedObject *grid);
        #endif
    };
}