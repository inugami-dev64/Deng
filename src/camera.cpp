#include "headers/renderer.h"

namespace deng {
    Camera::Camera(const vec4<float> &movementSpeed, const float &FOV, const float &draw_distance) {
        this->m_view_matrix.setPosition({DENG_DEFAULT_CAMERA_X, DENG_DEFAULT_CAMERA_Y, DENG_DEFAULT_CAMERA_Z, 1.0f});
        this->m_movementSpeed = movementSpeed;
        this->m_view_matrix.setAxes({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0});
        this->m_FOV = FOV;
        this->m_draw_distance = draw_distance;
        this->m_camera_math.getMaxCoords(0, this->m_FOV, this->m_draw_distance, &this->m_camera_max_centre_point_coord, &this->m_camera_max_left_corner_coord, &this->m_camera_max_right_corner_coord);
    }

    void Camera::moveF() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
    }

    void Camera::moveFL() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::moveFR() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_FALSE);
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::moveB() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
    }

    void Camera::moveBL() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::moveBR() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_X, DENG_TRUE);
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_FALSE);
}

    void Camera::moveR() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_FALSE);
    }

    void Camera::moveL() {
        this->m_view_matrix.addToPosition(this->m_movementSpeed, DENG_Y, DENG_TRUE);
    }

    void Camera::getViewMatrix(mat4<float> *matrix) {
        this->m_view_matrix.getViewMatrix(matrix);
    }

}