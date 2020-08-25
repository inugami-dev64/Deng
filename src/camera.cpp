#include "headers/renderer.h"

namespace deng {
    Camera::Camera(const vec4<float> &movementSpeed, const float &FOV, const float &near,const float &far, Window *window) {
        this->m_mouse_xPos = 0.0f;
        this->m_mouse_yPos = 0.0f;
        this->m_mouse_sens = 3.0f; //the bigger the value the slower the sensitivity

        this->m_window = window;
        this->m_isInit = DENG_TRUE;
        this->proj_matrix = new ProjectionMatrix(FOV, near, far,(float) (this->m_window->getSize().x / this->m_window->getSize().y));
        this->m_movementSpeed = movementSpeed;
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
            this->m_mouse_xPos = 0.0f;
            this->m_mouse_yPos = 0.0f;
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_xPos, this->m_mouse_yPos);
            this->m_isInit = DENG_FALSE;
        }

        if(this->m_mouse_yPos > (this->m_mouse_sens / 4)) {
            this->m_mouse_yPos = this->m_mouse_sens / 4;
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_xPos * this->m_window->getSize().x, (this->m_mouse_sens / 4) * this->m_window->getSize().y);
        }
        else if(this->m_mouse_yPos < -(this->m_mouse_sens / 4)) {
            this->m_mouse_yPos = -(this->m_mouse_sens / 4);
            glfwSetCursorPos(this->m_window->getWindow(), this->m_mouse_xPos * this->m_window->getSize().x, -(this->m_mouse_sens / 4) * this->m_window->getSize().y);
        }

        if(this->m_mouse_xPos >= this->m_mouse_sens || this->m_mouse_xPos <= -(this->m_mouse_sens)) {
            this->m_mouse_xPos = 0.0f;
            glfwSetCursorPos(this->m_window->getWindow(), 0.0f, this->m_mouse_yPos * this->m_window->getSize().y);
        }
        
        this->view_matrix.setTransformationMatrix();
        this->view_matrix.setRotation(getFractionNumerator(this->m_mouse_yPos, (this->m_mouse_sens / 4), -90.0f), getFractionNumerator(this->m_mouse_xPos, this->m_mouse_sens, 360.0f));

    }

    void Camera::updateCursorPos() {
        glfwGetCursorPos(this->m_window->getWindow(), &this->m_mouse_xPos, &this->m_mouse_yPos);
        this->m_mouse_xPos /= this->m_window->getSize().x;
        this->m_mouse_yPos /= this->m_window->getSize().y;
    }
}