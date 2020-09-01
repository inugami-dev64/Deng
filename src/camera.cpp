#include "headers/renderer.h"

namespace deng {
    Camera::Camera(const vec3<float> &cameraMovementSpeedMultiplier, const vec2<float> &mouseMovementSpeedMultiplier, const float &FOV, const float &near,const float &far, Window *window) {
        this->m_mouse_pos.x = 0.0f;
        this->m_mouse_pos.y = 0.0f;
        this->m_mouse_sens.x = DENG_MOUSE_BASE_SENSITIVITY_X * mouseMovementSpeedMultiplier.x;
        this->m_mouse_sens.y = DENG_MOUSE_BASE_SENSIVITY_Y * mouseMovementSpeedMultiplier.y;

        this->m_window = window;
        this->m_isInit = DENG_TRUE;
        this->proj_matrix = new ProjectionMatrix(FOV, near, far,(float) (this->m_window->getSize().x / this->m_window->getSize().y));

        this->m_movementSpeed.x = DENG_CAMERA_BASE_SPEED_X * cameraMovementSpeedMultiplier.x;
        this->m_movementSpeed.y = DENG_CAMERA_BASE_SPEED_Y * cameraMovementSpeedMultiplier.y;
        this->m_movementSpeed.z = DENG_CAMERA_BASE_SPEED_Z * cameraMovementSpeedMultiplier.z;
        this->m_movementSpeed.w = 0.0f;
    }

    Camera::~Camera() {
        delete this->proj_matrix;
    }

    void Camera::moveF() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Z, DENG_FALSE);
    }

    void Camera::moveB() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Z, DENG_TRUE);
    }

    void Camera::moveR() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
    }

    void Camera::moveL() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
    }

    void Camera::moveUp(){
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_FALSE);
    }

    void Camera::moveDown() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::setCameraViewRotation() {
        if(this->m_isInit == DENG_TRUE) {
            this->m_mouse_pos.x = 0.0f;
            this->m_mouse_pos.y = 0.0f;
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_pos.x, this->m_mouse_pos.y);
            this->m_isInit = DENG_FALSE;
        }

        if(this->m_mouse_pos.y > this->m_mouse_sens.y) {
            this->m_mouse_pos.y = this->m_mouse_sens.y;
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_pos.x * this->m_window->getSize().x, this->m_mouse_sens.y * this->m_window->getSize().y);
        }
        else if(this->m_mouse_pos.y < -this->m_mouse_sens.y) {
            this->m_mouse_pos.y = -this->m_mouse_sens.y;
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_pos.x * this->m_window->getSize().x, -this->m_mouse_sens.y * this->m_window->getSize().y);
        }

        if(this->m_mouse_pos.x >= this->m_mouse_sens.x || this->m_mouse_pos.x <= -this->m_mouse_sens.x) {
            this->m_mouse_pos.x = 0.0f;
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_pos.x, this->m_mouse_pos.y * this->m_window->getSize().y);
        }
        
        this->view_matrix.setTransformationMatrix();
        this->view_matrix.setRotation(getFractionNumerator(this->m_mouse_pos.y, this->m_mouse_sens.y, -90.0f), getFractionNumerator(this->m_mouse_pos.x, this->m_mouse_sens.x, 360.0f));

        // LOG("Mouse x_pos: " + std::to_string(this->m_mouse_pos.x) + "/Mouse y_pos: " + std::to_string(this->m_mouse_pos.y));
    }

    void Camera::updateCursorPos() {
        glfwGetCursorPos(this->m_window->getWindow(), &this->m_mouse_pos.x, &this->m_mouse_pos.y);
        this->m_mouse_pos.x /= this->m_window->getSize().x;
        this->m_mouse_pos.y /= this->m_window->getSize().y;
    }
}