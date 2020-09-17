#include "../core/dengcore.h"

namespace deng {
    Events::Events(Window *p_win, Camera *p_camera) {
        this->m_p_camera = p_camera;
        this->m_p_window = p_win;
    }

    // logging debug functions 
    #if DEBUG
        void Events::setGrid(SpecifiedObject *grid) {
            this->m_grid = grid;
        }

        void Events::handleGridCoordinateLogging(const int32_t &log_key, const std::string &file_name) {
            if(this->isKeyPressed(log_key, DENG_FALSE, nullptr)) {
                FileManager fm;
                fm.writeToFile(file_name, "#entry point", DENG_WRITEMODE_REWRITE);

                for(SpecifiedVertexData &vertex_data : this->m_grid->vertex_data) {
                    fm.writeToFile(file_name, "{" + std::to_string(vertex_data.position_vec.first) + ";" + std::to_string(vertex_data.position_vec.second) + ";" + std::to_string(vertex_data.position_vec.third) + "}", DENG_WRITEMODE_FROM_END);
                }
            }
        }
    #endif

    dengBool Events::isKeyPressed(const int &key, const dengBool &isRepeated, dengBool *isReleased) {
        switch (isRepeated)
        {
        case DENG_TRUE: {
            if(glfwGetKey(this->m_p_window->getWindow(), key) == GLFW_PRESS || glfwGetKey(this->m_p_window->getWindow(), key) == GLFW_REPEAT) {
                return DENG_TRUE;
            }

            else if(isReleased != nullptr && glfwGetKey(this->m_p_window->getWindow(), key) == GLFW_RELEASE) {
                *isReleased = DENG_TRUE;
                return DENG_FALSE;
            }

            else return DENG_FALSE;
            break;
        }

        case DENG_FALSE: {
            if(glfwGetKey(this->m_p_window->getWindow(), key) == GLFW_PRESS) {
                return DENG_TRUE;
            }

            else if(isReleased != nullptr && glfwGetKey(this->m_p_window->getWindow(), key) == GLFW_RELEASE) {
                *isReleased = DENG_TRUE;
                return DENG_FALSE;
            }

            else return DENG_FALSE;
            break;
        }
        
        default:
            return DENG_FALSE;
            break;
        }
    }

    void Events::isMouseKeysPressed(dengBool *lmb_click, dengBool *mmb_click, dengBool *rmb_click) {
        if(glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS || glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_REPEAT) {
            if(lmb_click != nullptr) *lmb_click = DENG_TRUE;
        }

        else if(lmb_click != nullptr) *lmb_click = DENG_FALSE;

        if(glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS || glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_REPEAT) {
            if(rmb_click != nullptr) *rmb_click = DENG_TRUE;
        }

        else if(rmb_click != nullptr) *rmb_click = DENG_FALSE;

        if(glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_3) == GLFW_PRESS || glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
            if(mmb_click != nullptr) *mmb_click = DENG_TRUE;
        }

        else if(mmb_click != nullptr) *mmb_click = DENG_FALSE;
    }

    void Events::getMovementType() {
        if(this->isKeyPressed(GLFW_KEY_W, DENG_TRUE, nullptr)) {
            this->m_movements.third = DENG_MOVEMENT_FORWARD;
        }

        else if(this->isKeyPressed(GLFW_KEY_S, DENG_TRUE, nullptr)) {
            this->m_movements.third = DENG_MOVEMENT_BACKWARD;
        }
        else {
            this->m_movements.third = DENG_MOVEMENT_NONE;
        }


        if(this->isKeyPressed(GLFW_KEY_A, DENG_TRUE, nullptr)) {
            this->m_movements.first = DENG_MOVEMENT_LEFTWARD;
        }

        else if(this->isKeyPressed(GLFW_KEY_D, DENG_TRUE, nullptr)) {
            this->m_movements.first = DENG_MOVEMENT_RIGHTWARD;
        }
        else {
            this->m_movements.first = DENG_MOVEMENT_NONE;
        }


        if(this->isKeyPressed(GLFW_KEY_SPACE, DENG_TRUE, nullptr)) {
            this->m_movements.second = DENG_MOVEMENT_UPWARD;
        }

        else if(this->isKeyPressed(GLFW_KEY_LEFT_CONTROL, DENG_TRUE, nullptr)) {
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

            if(this->m_input_mode_change_timer.isTimePassed(DENG_INPUT_MODE_CHANGE_INTERVAL) && this->isKeyPressed(GLFW_KEY_ESCAPE, DENG_FALSE, nullptr)) {
                LOG("frozen_mouse_position x:" + std::to_string(this->m_frozen_mouse_position.first) + "/" + std::to_string(this->m_frozen_mouse_position.second));
                this->m_p_window->setInputMode(DENG_INPUT_NONMOVEMENT);
                this->m_input_mode_change_timer.setNewTimePoint();
                this->m_p_camera->getMousePosition(&this->m_frozen_mouse_position);
            }
        }

        else if(this->m_p_window->getInputMode() == DENG_INPUT_NONMOVEMENT) {
            this->m_movements.first = DENG_MOVEMENT_NONE;
            this->m_movements.second = DENG_MOVEMENT_NONE;
            this->m_movements.third = DENG_MOVEMENT_NONE;

            if(this->m_input_mode_change_timer.isTimePassed(DENG_INPUT_MODE_CHANGE_INTERVAL) && this->isKeyPressed(GLFW_KEY_ESCAPE, DENG_FALSE, nullptr)) {
                this->m_p_camera->setMousePosition(this->m_frozen_mouse_position);
                this->m_p_window->setInputMode(DENG_INPUT_MOVEMENT);
                this->m_input_mode_change_timer.setNewTimePoint();
            }
        }
    }

    void Events::update() {
        this->checkForInputModeChange();

        #if DEBUG
            this->handleGridCoordinateLogging(GLFW_KEY_F12, "grid_coord.log");
        #endif
        
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