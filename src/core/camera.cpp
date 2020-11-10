#include "deng_core.h"

namespace deng {
    Camera::Camera(const dengMath::vec3<float> &camera_movement_speed_multiplier, const dengMath::vec2<float> &mouse_movement_speed_multiplier, const float &FOV, const float &near_plane, const float &far_plane, Window *window) {
        this->m_mouse_pos.first = 0.0f;
        this->m_mouse_pos.second = 0.0f;
        this->m_p_window = window;
        this->m_zero_camera_rotation_mouse_pos.first = this->m_p_window->getSize().first / 2;
        this->m_zero_camera_rotation_mouse_pos.second = this->m_p_window->getSize().second / 2;
        this->m_mouse_sens.first = 1 / mouse_movement_speed_multiplier.first;
        this->m_mouse_sens.second = 1 / mouse_movement_speed_multiplier.second;

        this->setMousePosition(this->m_zero_camera_rotation_mouse_pos);

        this->m_is_init = DENG_TRUE;
        this->p_projection_matrix = new dengMath::ProjectionMatrix(FOV, near_plane, far_plane, static_cast<float>(this->m_p_window->getSize().first / this->m_p_window->getSize().second));

        this->m_movement_speed.first = DENG_CAMERA_BASE_SPEED_X * camera_movement_speed_multiplier.first;
        this->m_movement_speed.second = DENG_CAMERA_BASE_SPEED_Y * camera_movement_speed_multiplier.second;
        this->m_movement_speed.third = DENG_CAMERA_BASE_SPEED_Z * camera_movement_speed_multiplier.third;
        this->m_movement_speed.fourth = 0.0f;
    }

    Camera::~Camera() {
        delete this->p_projection_matrix;
    }

    void Camera::moveF() {
        this->view_matrix.addToPosition(this->m_movement_speed, DENG_COORD_AXIS_Z, DENG_FALSE);
    }

    void Camera::moveB() {
        this->view_matrix.addToPosition(this->m_movement_speed, DENG_COORD_AXIS_Z, DENG_TRUE);
    }

    void Camera::moveR() {
        this->view_matrix.addToPosition(this->m_movement_speed, DENG_COORD_AXIS_X, DENG_TRUE);
    }

    void Camera::moveL() {
        this->view_matrix.addToPosition(this->m_movement_speed, DENG_COORD_AXIS_X, DENG_FALSE);
    }

    void Camera::moveUp(){
        this->view_matrix.addToPosition(this->m_movement_speed, DENG_COORD_AXIS_Y, DENG_FALSE);
    }

    void Camera::moveDown() {
        this->view_matrix.addToPosition(this->m_movement_speed, DENG_COORD_AXIS_Y, DENG_TRUE);
    }

    void Camera::setCameraViewRotation() {
        float x_rot, y_rot;
        if(this->m_mouse_pos.first < 2.0f && this->m_mouse_pos.second > -2.0f) 
            y_rot = 360 * this->m_mouse_pos.first / 2;

        else 
            this->m_p_window->getWindow()->virtual_mouse_position.x = ((this->m_mouse_pos.first - 2.0f) * this->m_mouse_sens.first * this->m_p_window->getSize().first) / 2;
        
        if(this->m_mouse_pos.second <= 1.0f && this->m_mouse_pos.second >= -1.0f)
            x_rot = -180 * this->m_mouse_pos.second / 2;

        else if(this->m_mouse_pos.second < -1.0f) {
            this->m_p_window->getWindow()->virtual_mouse_position.y = (-this->m_mouse_sens.second * this->m_p_window->getSize().second) / 2;
            this->m_mouse_pos.second = -1.0f;
            x_rot = 90;
        }

        else if(this->m_mouse_pos.second > 1.0f) {
            this->m_p_window->getWindow()->virtual_mouse_position.y = (this->m_mouse_sens.second * this->m_p_window->getSize().second) / 2;
            this->m_mouse_pos.second = 1.0f;
            x_rot = -90;
        }


        this->view_matrix.setTransformationMatrix();
        this->view_matrix.setRotation(x_rot, y_rot);

        #if CAMERA_MOUSE_DEBUG
            LOG("Mouse x_pos: " + std::to_string(this->m_mouse_pos.first) + "/Mouse y_pos: " + std::to_string(this->m_mouse_pos.second));
        #endif
    }

    void Camera::setMousePosition(dengMath::vec2<float> &mouse_position) {
        this->m_mouse_pos.first = (double) mouse_position.first;
        this->m_mouse_pos.second = (double) mouse_position.second;
        set_mouse_coords(this->m_p_window->getWindow(), this->m_mouse_pos.first * (int) this->m_p_window->getSize().first, this->m_mouse_pos.second * (int) this->m_p_window->getSize().second);
    }

    void Camera::getMousePosition(dengMath::vec2<float> *mouse_position) {
        if(mouse_position != nullptr) {
            mouse_position->first = static_cast<int>(this->m_mouse_pos.first);
            mouse_position->second = static_cast<int>(this->m_mouse_pos.second);
        }
    }

    void Camera::updateCursorPos() {
        get_mouse_pos(this->m_p_window->getWindow(), &this->m_mouse_pos.first, &this->m_mouse_pos.second, 0);
        this->m_mouse_pos.first = (this->m_mouse_pos.first) / (this->m_mouse_sens.first * this->m_p_window->getSize().first);
        this->m_mouse_pos.second = (this->m_mouse_pos.second * 2) / (this->m_mouse_sens.second * this->m_p_window->getSize().second);
    }
}
