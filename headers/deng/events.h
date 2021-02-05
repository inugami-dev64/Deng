#ifndef EVENTS_H
#define EVENTS_H

#define DENG_KEY_PRESS_INTERVAL 200 // milliseconds
#define DENG_MOVEMENT_INTERVAL 1

namespace dengMath {
    
    class Events
    {
    private:
        dengUtils::Timer m_movement_timer;
        dengUtils::Timer m_input_mode_change_timer;
        deng::WindowWrap *m_p_window_wrap;
        vec3<deng_MovementEvent> m_movements;
        vec2<deng_vec_t> m_frozen_mouse_position;

    private:
        void getMovementType();
        void checkForInputModeChange();
        void updateCursorPos();
        bool clickHandler(deng_Key key);

    public:
        Events (
            deng::WindowWrap *p_win_wrap, 
            deng::Camera *p_camera
        );

        deng_MovementEvent getMovement();
        void update();
    };
}

#endif
