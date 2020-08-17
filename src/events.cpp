#include "headers/renderer.h"

namespace deng {
    Events::Events(Window *win, Camera *camera) {
        this->m_camera = camera;
        this->m_window = win;
    }

    void Events::getMovementType(GLFWwindow *window) {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT) {
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_FORWARDRIGHT;
            else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_FORWARDLEFT;
            else this->m_deng_movement = DENG_MOVEMENT_FORWARD;
        }

        else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT) {
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_BACKWARDRIGHT;
            else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_BACKWARDLEFT;
            else this->m_deng_movement = DENG_MOVEMENT_BACKWARD;
        }

        else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT) {
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_FORWARDLEFT;
            else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_BACKWARDLEFT;
            else this->m_deng_movement = DENG_MOVEMENT_LEFTWARD;
        }

        else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT) {
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_FORWARDRIGHT;
            else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT) this->m_deng_movement = DENG_MOVEMENT_BACKWARDRIGHT;
            else this->m_deng_movement = DENG_MOVEMENT_RIGHTWARD;
        }

        else this->m_deng_movement = DENG_MOVEMENT_NONE;
    }

    MovementEvents Events::getMovement() {
        return this->m_deng_movement;
    }

    void Events::update() {
        this->getMovementType(this->m_window->getWindow());
        MovementEvents movement = this->getMovement();
        
        if(this->m_timer.isTimePassed(1) ) {
            switch (movement)
            {
            case DENG_MOVEMENT_FORWARD:
                this->m_camera->moveF();
                break;

            case DENG_MOVEMENT_FORWARDRIGHT:
                this->m_camera->moveFR();
                break;

            case DENG_MOVEMENT_FORWARDLEFT:
                this->m_camera->moveFL();
                break;

            case DENG_MOVEMENT_BACKWARD:
                this->m_camera->moveB();
                break;

            case DENG_MOVEMENT_BACKWARDRIGHT:
                this->m_camera->moveBR();
                break;
            
            case DENG_MOVEMENT_BACKWARDLEFT:
                this->m_camera->moveBL();
                break;

            case DENG_MOVEMENT_RIGHTWARD:
                this->m_camera->moveR();
                break;

            case DENG_MOVEMENT_LEFTWARD:
                this->m_camera->moveL();
                break;
            
            default:
                break;
            }

            this->m_timer.setNewTimePoint();
        }
    }
}