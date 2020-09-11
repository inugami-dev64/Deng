#include "headers/renderer.h"

namespace deng {
    Events::Events(Window *p_win, Camera *p_camera) {
        this->m_camera = p_camera;
        this->m_window = p_win;
    }

    dengBool Events::isKeyPressed(const int &key) {
        if(glfwGetKey(this->m_window->getWindow(), key) == GLFW_PRESS || glfwGetKey(this->m_window->getWindow(), key) == GLFW_REPEAT) {
            return DENG_TRUE;
        }

        else return DENG_FALSE;
    }

    // logging debug functions 
    #if DEBUG
        void Events::setGrid(SpecifiedObject *grid) {
            this->m_grid = grid;
        }

        void Events::handleGridCoordinateLogging(const int32_t &log_key, const std::string &file_name) {
            if(this->isKeyPressed(log_key)) {
                FileManager fm;
                fm.writeToFile(file_name, "#entry point", DENG_WRITEMODE_REWRITE);
            
                for(SpecifiedVertexData &vertex_data : this->m_grid->vertex_data) {
                    fm.writeToFile(file_name, "{" + std::to_string(vertex_data.position_vec.first) + ";" + std::to_string(vertex_data.position_vec.second) + ";" + std::to_string(vertex_data.position_vec.third) + "}", DENG_WRITEMODE_FROM_END);
                }
            }
        }
    #endif

    void Events::isMouseKeysPressed(dengBool *lmb_click, dengBool *mmb_click, dengBool *rmb_click) {
        if(glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS || glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_REPEAT) {
            if(lmb_click != nullptr) *lmb_click = DENG_TRUE;
        }

        else if(lmb_click != nullptr) *lmb_click = DENG_FALSE;

        if(glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS || glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_REPEAT) {
            if(rmb_click != nullptr) *rmb_click = DENG_TRUE;
        }

        else if(rmb_click != nullptr) *rmb_click = DENG_FALSE;

        if(glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_3) == GLFW_PRESS || glfwGetMouseButton(this->m_window->getWindow(), GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
            if(mmb_click != nullptr) *mmb_click = DENG_TRUE;
        }

        else if(mmb_click != nullptr) *mmb_click = DENG_FALSE;
    }

    void Events::getMovementType() {
        if(this->isKeyPressed(GLFW_KEY_W)) {
            this->m_movements.z = DENG_MOVEMENT_FORWARD;
        }

        else if(this->isKeyPressed(GLFW_KEY_S)) {
            this->m_movements.z = DENG_MOVEMENT_BACKWARD;
        }
        else {
            this->m_movements.z = DENG_MOVEMENT_NONE;
        }


        if(this->isKeyPressed(GLFW_KEY_A)) {
            this->m_movements.x = DENG_MOVEMENT_LEFTWARD;
        }

        else if(this->isKeyPressed(GLFW_KEY_D)) {
            this->m_movements.x = DENG_MOVEMENT_RIGHTWARD;
        }
        else {
            this->m_movements.x = DENG_MOVEMENT_NONE;
        }


        if(this->isKeyPressed(GLFW_KEY_SPACE)) {
            this->m_movements.y = DENG_MOVEMENT_UPWARD;
        }

        else if(this->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
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

            if(this->isKeyPressed(GLFW_KEY_ESCAPE)) {
                this->m_window->setInputMode(DENG_INPUT_NONMOVEMENT);
            }
            break;

        case DENG_INPUT_NONMOVEMENT:
            this->m_movements.x = DENG_MOVEMENT_NONE;
            this->m_movements.y = DENG_MOVEMENT_NONE;
            this->m_movements.z = DENG_MOVEMENT_NONE;

            dengBool local_lmb_click;
            this->isMouseKeysPressed(&local_lmb_click, nullptr, nullptr);
            if(local_lmb_click) {
                this->m_window->setInputMode(DENG_INPUT_MOVEMENT);
            }
            break;
        
        default:
            break;
        }
    }

    void Events::update() {
        this->checkForInputModeChange();

        #if DEBUG
            this->handleGridCoordinateLogging(GLFW_KEY_F12, "grid_coord.log");
        #endif
        
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