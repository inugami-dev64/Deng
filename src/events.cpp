#include "headers/renderer.h"

namespace deng {
    Events::Events(Window *win, Camera *camera, GameObject *obj) {
        this->m_camera = camera;
        this->m_window = win;
        this->m_obj = obj;
    }

    void Events::getMovementType() {
        if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), GLFW_KEY_W) == GLFW_REPEAT) {
            this->m_movements.z = DENG_MOVEMENT_FORWARD;
        }

        else if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), GLFW_KEY_S) == GLFW_REPEAT) {
            this->m_movements.z = DENG_MOVEMENT_BACKWARD;
        }
        else {
            this->m_movements.z = DENG_MOVEMENT_NONE;
        }


        if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), GLFW_KEY_A) == GLFW_REPEAT) {
            this->m_movements.x = DENG_MOVEMENT_LEFTWARD;
        }

        else if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), GLFW_KEY_D) == GLFW_REPEAT) {
            this->m_movements.x = DENG_MOVEMENT_RIGHTWARD;
        }
        else {
            this->m_movements.x = DENG_MOVEMENT_NONE;
        }


        if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_REPEAT) {
            this->m_movements.y = DENG_MOVEMENT_UPWARD;
        }

        else if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_REPEAT) {
            this->m_movements.y = DENG_MOVEMENT_DOWNWARD;
        }

        else {
            this->m_movements.y = DENG_MOVEMENT_NONE;
        }

    }

    void Events::checkForInputModeChange() {
        switch (this->m_window->getInputMode())
        {
        case DENG_INPUT_MOVEMENT:
            this->getMovementType();
            this->m_camera->updateCursorPos();
            this->m_camera->setCameraViewRotation();

            if(glfwGetKey(this->m_window->getWindow(), GLFW_KEY_ESCAPE)) {
                this->m_window->setInputMode(DENG_INPUT_NONMOVEMENT);
            }
            break;

        case DENG_INPUT_NONMOVEMENT:
            this->m_movements.x = DENG_MOVEMENT_NONE;
            this->m_movements.y = DENG_MOVEMENT_NONE;
            this->m_movements.z = DENG_MOVEMENT_NONE;
            if(glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                this->m_window->setInputMode(DENG_INPUT_MOVEMENT);
            }
            break;
        
        default:
            break;
        }
    }

    void Events::update() {
        this->checkForInputModeChange();
        
        if(this->m_timer.isTimePassed(1)) {

            switch (this->m_movements.x)
            {
            case DENG_MOVEMENT_LEFTWARD:
                this->m_camera->moveL();
                break;

            case DENG_MOVEMENT_RIGHTWARD:
                this->m_camera->moveR();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (this->m_movements.y)
            {
            case DENG_MOVEMENT_UPWARD:
                this->m_camera->moveUp();
                break;

            case DENG_MOVEMENT_DOWNWARD:
                this->m_camera->moveDown();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (this->m_movements.z)
            {
            case DENG_MOVEMENT_FORWARD:
                this->m_camera->moveF();
                break;

            case DENG_MOVEMENT_BACKWARD:
                this->m_camera->moveB();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            this->m_timer.setNewTimePoint();
        }
    }
}