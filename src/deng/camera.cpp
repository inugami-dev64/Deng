#include "../../headers/deng/api_core.h"

// Shared data
extern dengui::MouseInputInfo ext_mii;

namespace deng {
    Camera::Camera (
        const dengMath::vec3<float> &camera_movement_speed_multiplier, 
		const dengMath::vec2<float> &mouse_movement_speed_multiplier, 
		const float &FOV, 
		const float &near_plane, 
		const float &far_plane, 
		WindowWrap *window
	) {
        m_mouse_pos.first = 0.0f;
        m_mouse_pos.second = 0.0f;
        m_p_window_wrap = window;
        m_zero_camera_rotation_mouse_pos.first = m_p_window_wrap->getSize().first / 2;
        m_zero_camera_rotation_mouse_pos.second = m_p_window_wrap->getSize().second / 2;
        m_mouse_sens.first = 1 / mouse_movement_speed_multiplier.first;
        m_mouse_sens.second = 1 / mouse_movement_speed_multiplier.second;

        setMousePosition (
            m_zero_camera_rotation_mouse_pos
        );
        
        m_prev_active_c = 0;
        m_is_init = true;
        p_projection_matrix = new dengMath::ProjectionMatrix (
            FOV, 
            near_plane, 
            far_plane, 
            static_cast<float>(m_p_window_wrap->getSize().first / 
            m_p_window_wrap->getSize().second)
        );

        m_movement_speed.first = 
        DENG_CAMERA_BASE_SPEED_X * camera_movement_speed_multiplier.first;
        m_movement_speed.second = 
        DENG_CAMERA_BASE_SPEED_Y * camera_movement_speed_multiplier.second;
        m_movement_speed.third = 
        DENG_CAMERA_BASE_SPEED_Z * camera_movement_speed_multiplier.third;
        m_movement_speed.fourth = 0.0f;
    }

    Camera::~Camera() {
        delete p_projection_matrix;
    }

    void Camera::moveF() {
        view_matrix.addToPosition (
            m_movement_speed, 
            DENG_COORD_AXIS_Z, 
            false
        );
    }

    void Camera::moveB() {
        view_matrix.addToPosition (
            m_movement_speed, 
            DENG_COORD_AXIS_Z, 
            true
        );
    }

    void Camera::moveR() {
        view_matrix.addToPosition (
            m_movement_speed, 
            DENG_COORD_AXIS_X, 
            true
        );
    }

    void Camera::moveL() {
        view_matrix.addToPosition (
            m_movement_speed, 
            DENG_COORD_AXIS_X, 
            false
        );
    }

    void Camera::moveUp(){
        view_matrix.addToPosition (
            m_movement_speed, 
            DENG_COORD_AXIS_Y, 
            false
        );
    }

    void Camera::moveDown() {
        view_matrix.addToPosition (
            m_movement_speed, 
            DENG_COORD_AXIS_Y, 
            true
        );
    }

    void Camera::setCameraViewRotation() {
        float x_rot, y_rot;
        if(m_mouse_pos.first < 2.0f && m_mouse_pos.second > -2.0f) 
            y_rot = 360 * m_mouse_pos.first / 2;

        else { 
            m_p_window_wrap->getWindow()->virtual_mouse_position.x = 
            ((m_mouse_pos.first - 2.0f) * 
            m_mouse_sens.first * 
            m_p_window_wrap->getSize().first) / 2;
        }

        if(m_mouse_pos.second <= 1.0f && m_mouse_pos.second >= -1.0f)
            x_rot = -180 * m_mouse_pos.second / 2;

        else if(m_mouse_pos.second < -1.0f) {
            m_p_window_wrap->getWindow()->virtual_mouse_position.y = 
            (-m_mouse_sens.second * 
            m_p_window_wrap->getSize().second) / 2;

            m_mouse_pos.second = -1.0f;
            x_rot = 90;
        }

        else if(m_mouse_pos.second > 1.0f) {
            m_p_window_wrap->getWindow()->virtual_mouse_position.y = 
            (m_mouse_sens.second * 
            m_p_window_wrap->getSize().second) / 2;

            m_mouse_pos.second = 1.0f;
            x_rot = -90;
        }


        view_matrix.setTransformationMatrix();
        view_matrix.setRotation(x_rot, y_rot);

        #if CAMERA_MOUSE_DEBUG
            LOG (
                "Mouse x_pos: " + 
                std::to_string(m_mouse_pos.first) + 
                "/Mouse y_pos: " + 
                std::to_string(m_mouse_pos.second)
            );
        #endif
    }

    void Camera::setMousePosition(dengMath::vec2<float> &mouse_position) {
        m_mouse_pos.first = (double) mouse_position.first;
        m_mouse_pos.second = (double) mouse_position.second;
        deng_SetMouseCoords (
            m_p_window_wrap->getWindow(), 
            m_mouse_pos.first * (int) m_p_window_wrap->getSize().first, 
            m_mouse_pos.second * (int) m_p_window_wrap->getSize().second
        );
    }

    void Camera::getMousePosition(dengMath::vec2<float> *mouse_position) {
        if(mouse_position != nullptr) {
            mouse_position->first = static_cast<int>(m_mouse_pos.first);
            mouse_position->second = static_cast<int>(m_mouse_pos.second);
        }
    }

    void Camera::updateCursorPos() {
        std::lock_guard<std::mutex> lck(ext_mii.mut);
        deng_GetMousePos (
            m_p_window_wrap->getWindow(), 
            &ext_mii.mouse_coords.first, 
            &ext_mii.mouse_coords.second, 
            0
        );

        ext_mii.active_btn_c = m_p_window_wrap->getWindow()->active_keys.btn_count;
        if(ext_mii.active_btn_c && m_prev_active_c != ext_mii.active_btn_c) {
            deng_MouseButton *tmp = (deng_MouseButton*) realloc (
                ext_mii.active_btn,
                sizeof(deng_MouseButton) * ext_mii.active_btn_c
            );

            // Check for memory allocation error
            if(!tmp)
                MEM_ERR("could not reallocate memory for active buttons");
            
            ext_mii.active_btn = tmp;
            memcpy (
                ext_mii.active_btn,
                m_p_window_wrap->getWindow()->active_keys.p_btn,
                sizeof(deng_MouseButton) * ext_mii.active_btn_c
            );

            m_prev_active_c = ext_mii.active_btn_c;
        }

        m_mouse_pos.first = 
        (ext_mii.mouse_coords.first * 2) / (m_mouse_sens.first * 
        m_p_window_wrap->getSize().first);

        m_mouse_pos.second = 
        (ext_mii.mouse_coords.second * 2) / (m_mouse_sens.second * 
        m_p_window_wrap->getSize().second);
    }
}
