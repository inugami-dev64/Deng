#include "../../headers/deng/api_core.h"

namespace dengMath {
    Events::Events (
        deng::WindowWrap *p_win, 
        deng::Camera *p_camera
    ) {
        m_p_camera = p_camera;
        m_p_window_wrap = p_win;
    }

    void Events::getMovementType() {
    }

    void Events::checkForInputModeChange() {
        m_p_camera->updateCursorPos();
        ext_mii.mouse_input = m_p_window_wrap->getInputMode();
        
        if(ext_mii.mouse_input == DENG_INPUT_MOVEMENT) {
            getMovementType();
            m_p_camera->setCameraViewRotation();

            if
            (
                m_input_mode_change_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) && 
                deng_IsKeyActive(m_p_window_wrap->getWindow(), DENG_KEY_ESCAPE)
            ) {

                #if CAMERA_MOUSE_DEBUG
                    LOG (
                        "frozen_mouse_position x:" + 
                        std::to_string(m_frozen_mouse_position.first) + 
                        "/" + 
                        std::to_string(m_frozen_mouse_position.second)
                    );
                #endif
                
                m_p_window_wrap->setInputMode(DENG_INPUT_NONMOVEMENT);
                m_input_mode_change_timer.setNewTimePoint();
                m_p_camera->getMousePosition(&m_frozen_mouse_position);
            }
        }

        else if(ext_mii.mouse_input == DENG_INPUT_NONMOVEMENT) {
            m_movements.first = DENG_MOVEMENT_NONE;
            m_movements.second = DENG_MOVEMENT_NONE;
            m_movements.third = DENG_MOVEMENT_NONE;

            if
            (
                m_input_mode_change_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) && 
                deng_IsKeyActive (
                    m_p_window_wrap->getWindow(), 
                    DENG_KEY_ESCAPE
                )
            ) {
                m_p_camera->setMousePosition(m_frozen_mouse_position);
                m_p_window_wrap->setInputMode(DENG_INPUT_MOVEMENT);
                m_input_mode_change_timer.setNewTimePoint();
            }
        }
    }

    void Events::update() {
        checkForInputModeChange();
        if(m_movement_timer.isTimePassed(DENG_MOVEMENT_INTERVAL)) {

            switch (m_movements.first)
            {
            case DENG_MOVEMENT_LEFTWARD:
                m_p_camera->moveL();
                break;

            case DENG_MOVEMENT_RIGHTWARD:
                m_p_camera->moveR();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (m_movements.second)
            {
            case DENG_MOVEMENT_UPWARD:
                m_p_camera->moveUp();
                break;

            case DENG_MOVEMENT_DOWNWARD:
                m_p_camera->moveDown();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (m_movements.third)
            {
            case DENG_MOVEMENT_FORWARD:
                m_p_camera->moveF();
                break;

            case DENG_MOVEMENT_BACKWARD:
                m_p_camera->moveB();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            m_movement_timer.setNewTimePoint();
        }
    }
}
