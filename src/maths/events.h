#ifndef EVENTS_H
#define EVENTS_H

#define DENG_INPUT_MODE_CHANGE_INTERVAL 200
#define DENG_MOVEMENT_INTERVAL 1

namespace dengMath {
    
    class Events
    {
    private:
        dengUtils::Timer m_movement_timer;
        dengUtils::Timer m_input_mode_change_timer;

        deng::Camera *m_p_camera;
        deng::Window *m_p_window;
        vec3<dengMovementEvent> m_movements;
        vec2<float> m_frozen_mouse_position;
        

        #if GENERIC_DEBUG
            dengUtils::SpecifiedObject *m_grid;
        #endif

    private:
        void getMovementType();
        void checkForInputModeChange();
        dengBool isKeyPressed(const int &key, const dengBool &isRepeated, dengBool *isReleased);
        void isMouseKeysPressed(dengBool *lmb_click, dengBool *mmb_click, dengBool *rmb_click);

        #if GENERIC_DEBUG
            void handleGridCoordinateLogging(const int32_t &log_key, const std::string &file_name);
        #endif
        

    public:
        Events(deng::Window *p_win, deng::Camera *p_camera);
        dengMovementEvent getMovement();
        void handleExit();
        void update();

        #if GENERIC_DEBUG
            void setGrid(dengUtils::SpecifiedObject *grid);
        #endif
    };
}

#endif