#ifndef EVENTS_H
#define EVENTS_H

#define DENG_INPUT_MODE_CHANGE_INTERVAL 200
#define DENG_MOVEMENT_INTERVAL 1

namespace deng {
    
    class Events
    {
    private:
        Timer m_movement_timer;
        Timer m_input_mode_change_timer;

        Camera *m_p_camera;
        Window *m_p_window;
        vec3<dengMovementEvent> m_movements;
        vec2<float> m_frozen_mouse_position;
        

        #if GENERIC_DEBUG
            SpecifiedObject *m_grid;
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
        Events(Window *p_win, Camera *p_camera);
        dengMovementEvent getMovement();
        void handleExit();
        void update();

        #if GENERIC_DEBUG
            void setGrid(SpecifiedObject *grid);
        #endif
    };
}

#endif