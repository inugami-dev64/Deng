#include "headers/renderer.h"

namespace deng {
    Camera::Camera(const vec4<float> &movementSpeed, const float &FOV, const float &near,const float &far, Window *window) {
        this->view_matrix.setAxes({1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f});
        this->view_matrix.setPosition({DENG_CAMERA_DEFAULT_X, DENG_CAMERA_DEFAULT_Y, DENG_CAMERA_DEFAULT_Z, 1.0f});
        this->proj_matrix = new ProjectionMatrix(FOV, near, far,(float) (window->getSize().x / window->getSize().y));
        this->m_movementSpeed = movementSpeed;
    }

    Camera::~Camera() {
        delete this->proj_matrix;
    }

    void Camera::moveF() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
    }

    void Camera::moveFL() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::moveFR() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::moveB() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
    }

    void Camera::moveBL() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::moveBR() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_FALSE);
}

    void Camera::moveR() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_FALSE);
    }

    void Camera::moveL() {
        this->view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }
}