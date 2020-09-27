#include "deng_core.h"

namespace deng {
    Camera::Camera(const vec3<float> &cameraMovementSpeedMultiplier, const vec2<float> &mouseMovementSpeedMultiplier, const float &FOV, const float &near,const float &far, Window *window) {
        this->m_mouse_pos.first = 0.0f;
        this->m_mouse_pos.second = 0.0f;
        this->m_mouse_sens.first = DENG_MOUSE_BASE_SENSITIVITY_X * (1/mouseMovementSpeedMultiplier.first);
        this->m_mouse_sens.second = DENG_MOUSE_BASE_SENSIVITY_Y * (1/mouseMovementSpeedMultiplier.second);

        this->m_p_window = window;
        this->m_is_init = DENG_TRUE;
        this->p_projection_matrix = new ProjectionMatrix(FOV, near, far,(float) (this->m_p_window->getSize().first / this->m_p_window->getSize().second));

        this->m_movementSpeed.first = DENG_CAMERA_BASE_SPEED_X * cameraMovementSpeedMultiplier.first;
        this->m_movementSpeed.second = DENG_CAMERA_BASE_SPEED_Y * cameraMovementSpeedMultiplier.second;
        this->m_movementSpeed.third = DENG_CAMERA_BASE_SPEED_Z * cameraMovementSpeedMultiplier.third;
        this->m_movementSpeed.fourth = 0.0f;
    }

    Camera::~Camera() {
        delete this->p_projection_matrix;
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
        if(this->m_is_init == DENG_TRUE) {
            this->m_mouse_pos.first = 0.0f;
            this->m_mouse_pos.second = 0.0f;
            glfwSetCursorPos(this->m_p_window->getWindow(), this->m_mouse_pos.first, this->m_mouse_pos.second);
            this->m_is_init = DENG_FALSE;
        }

        if(this->m_mouse_pos.second > this->m_mouse_sens.second) {
            this->m_mouse_pos.second = this->m_mouse_sens.second;
            glfwSetCursorPos(this->m_p_window->getWindow(), this->m_mouse_pos.first * this->m_p_window->getSize().first, this->m_mouse_sens.second * this->m_p_window->getSize().second);
        }
        else if(this->m_mouse_pos.second < -this->m_mouse_sens.second) {
            this->m_mouse_pos.second = -this->m_mouse_sens.second;
            glfwSetCursorPos(this->m_p_window->getWindow(), this->m_mouse_pos.first * this->m_p_window->getSize().first, -this->m_mouse_sens.second * this->m_p_window->getSize().second);
        }

        if(this->m_mouse_pos.first >= this->m_mouse_sens.first || this->m_mouse_pos.first <= -this->m_mouse_sens.first) {
            this->m_mouse_pos.first = 0.0f;
            glfwSetCursorPos(this->m_p_window->getWindow(), this->m_mouse_pos.first * this->m_p_window->getSize().second, this->m_mouse_pos.second * this->m_p_window->getSize().second);
        }

        this->view_matrix.setTransformationMatrix();
        this->view_matrix.setRotation(Math::getFractionNumerator(this->m_mouse_pos.second, this->m_mouse_sens.second, -90.0f), Math::getFractionNumerator(this->m_mouse_pos.first, this->m_mouse_sens.first, 360.0f));

        #if CAMERA_MOUSE_DEBUG
            LOG("Mouse x_pos: " + std::to_string(this->m_mouse_pos.first) + "/Mouse y_pos: " + std::to_string(this->m_mouse_pos.second));
        #endif
    }

    void Camera::setMousePosition(vec2<float> &mouse_position) {
        this->m_mouse_pos.first = mouse_position.first;
        this->m_mouse_pos.second = mouse_position.second;
        glfwSetCursorPos(this->m_p_window->getWindow(), this->m_mouse_pos.first * this->m_p_window->getSize().first, this->m_mouse_pos.second * this->m_p_window->getSize().second);
    }

    void Camera::getMousePosition(vec2<float> *mouse_position) {
        if(mouse_position != nullptr) {
            mouse_position->first = static_cast<float>(this->m_mouse_pos.first);
            mouse_position->second = static_cast<float>(this->m_mouse_pos.second);
        }
    }

    void Camera::updateCursorPos() {
        glfwGetCursorPos(this->m_p_window->getWindow(), &this->m_mouse_pos.first, &this->m_mouse_pos.second);
        this->m_mouse_pos.first /= this->m_p_window->getSize().first;
        this->m_mouse_pos.second /= this->m_p_window->getSize().second;
    }
}
