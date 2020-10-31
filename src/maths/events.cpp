#include "../core/deng_core.h"

namespace dengMath {
    Events::Events(deng::Window *p_win, deng::Camera *p_camera) {
        this->m_is_key_registered.first = DENG_FALSE;
        this->m_p_camera = p_camera;
        this->m_p_window = p_win;
    }

    void Events::getMovementType() {
        if(is_key_active(this->m_p_window->getWindow(), DENG_KEY_W) && !is_key_active(this->m_p_window->getWindow(), DENG_KEY_S)) {
            this->m_movements.third = DENG_MOVEMENT_FORWARD;
        }

        else if(!is_key_active(this->m_p_window->getWindow(), DENG_KEY_W) && is_key_active(this->m_p_window->getWindow(), DENG_KEY_S)) {
            this->m_movements.third = DENG_MOVEMENT_BACKWARD;
        }

        else {
            this->m_movements.third = DENG_MOVEMENT_NONE;
        }


        if(is_key_active(this->m_p_window->getWindow(), DENG_KEY_A) && !is_key_active(this->m_p_window->getWindow(), DENG_KEY_D)) {
            this->m_movements.first = DENG_MOVEMENT_LEFTWARD;
        }

        else if(!is_key_active(this->m_p_window->getWindow(), DENG_KEY_A) && is_key_active(this->m_p_window->getWindow(), DENG_KEY_D)) {
            this->m_movements.first = DENG_MOVEMENT_RIGHTWARD;
        }
        else {
            this->m_movements.first = DENG_MOVEMENT_NONE;
        }


        if(is_key_active(this->m_p_window->getWindow(), DENG_KEY_SPACE) && !is_key_active(this->m_p_window->getWindow(), DENG_KEY_LEFT_CTRL)) {
            this->m_movements.second = DENG_MOVEMENT_UPWARD;
        }

        else if(!is_key_active(this->m_p_window->getWindow(), DENG_KEY_SPACE) && is_key_active(this->m_p_window->getWindow(), DENG_KEY_LEFT_CTRL)) {
            this->m_movements.second = DENG_MOVEMENT_DOWNWARD;
        }

        else {
            this->m_movements.second = DENG_MOVEMENT_NONE;
        }
    }

    void Events::checkForInputModeChange() {
        if(this->m_p_window->getInputMode() == DENG_INPUT_MOVEMENT) {
            this->getMovementType();
            this->m_p_camera->updateCursorPos();
            this->m_p_camera->setCameraViewRotation();

            if(this->m_input_mode_change_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) && is_key_active(this->m_p_window->getWindow(), DENG_KEY_ESCAPE)) {

                #if CAMERA_MOUSE_DEBUG
                    LOG("frozen_mouse_position x:" + std::to_string(this->m_frozen_mouse_position.first) + "/" + std::to_string(this->m_frozen_mouse_position.second));
                #endif
                
                this->m_p_window->setInputMode(DENG_INPUT_NONMOVEMENT);
                this->m_input_mode_change_timer.setNewTimePoint();
                this->m_p_camera->getMousePosition(&this->m_frozen_mouse_position);
            }
        }

        else if(this->m_p_window->getInputMode() == DENG_INPUT_NONMOVEMENT) {
            this->m_movements.first = DENG_MOVEMENT_NONE;
            this->m_movements.second = DENG_MOVEMENT_NONE;
            this->m_movements.third = DENG_MOVEMENT_NONE;

            if(this->m_input_mode_change_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) && is_key_active(this->m_p_window->getWindow(), DENG_KEY_ESCAPE)) {
                this->m_p_camera->setMousePosition(this->m_frozen_mouse_position);
                this->m_p_window->setInputMode(DENG_INPUT_MOVEMENT);
                this->m_input_mode_change_timer.setNewTimePoint();
            }
        }
    }

    void Events::update() {
        this->checkForInputModeChange();
        
        if(this->m_movement_timer.isTimePassed(DENG_MOVEMENT_INTERVAL)) {

            switch (this->m_movements.first)
            {
            case DENG_MOVEMENT_LEFTWARD:
                this->m_p_camera->moveL();
                break;

            case DENG_MOVEMENT_RIGHTWARD:
                this->m_p_camera->moveR();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (this->m_movements.second)
            {
            case DENG_MOVEMENT_UPWARD:
                this->m_p_camera->moveUp();
                break;

            case DENG_MOVEMENT_DOWNWARD:
                this->m_p_camera->moveDown();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (this->m_movements.third)
            {
            case DENG_MOVEMENT_FORWARD:
                this->m_p_camera->moveF();
                break;

            case DENG_MOVEMENT_BACKWARD:
                this->m_p_camera->moveB();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            this->m_movement_timer.setNewTimePoint();
        }
    }
}