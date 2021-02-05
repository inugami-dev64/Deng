#include "../../headers/deng/api_core.h"

// Shared data
extern dengui::MouseInputInfo ext_mii;

namespace deng {
    /*********************************************/
    /*********************************************/
    /****** First person perspective camera ******/
    /*********************************************/
    /*********************************************/
    
    /* FPP camera Events */
    FPPCameraEv::FPPCameraEv (
        WindowWrap *p_ww,
        dengMath::vec2<deng_vec_t> mouse_mov_speed_mul,
        dengMath::vec3<deng_vec_t> camera_mov_speed_mul,
        FPPCamNonMovementCallback callback
    ) {
        m_callback = callback;
        m_prev_active_btn_c = 0;
        m_mouse_sens.first = 1 / mouse_mov_speed_mul.first;
        m_mouse_sens.second = 1 / mouse_mov_speed_mul.second;

        m_move_speed.first = 
        DENG_CAMERA_BASE_SPEED_X * camera_mov_speed_mul.first;
        m_move_speed.second =
        DENG_CAMERA_BASE_SPEED_Y * camera_mov_speed_mul.second;
        m_move_speed.third =
        DENG_CAMERA_BASE_SPEED_Z * camera_mov_speed_mul.third;
        m_move_speed.fourth = 0.0f;
    }


    /* Find the current movement direction */
    void FPPCameraEv::findMovementType(WindowWrap *p_ww) {
        if
        (
            deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_W) && 
            !deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_S)
        ) m_movements.third = DENG_MOVEMENT_FORWARD;
        

        else if
        (
            !deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_W) && 
            deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_S)
        ) m_movements.third = DENG_MOVEMENT_BACKWARD;

        else 
            m_movements.third = DENG_MOVEMENT_NONE;


        if
        (
            deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_A) && 
            !deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_D)
        ) m_movements.first = DENG_MOVEMENT_LEFTWARD;

        else if
        (
            !deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_A) && 
            deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_D)
        ) m_movements.first = DENG_MOVEMENT_RIGHTWARD;

        else m_movements.first = DENG_MOVEMENT_NONE;

        if
        (
            deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_SPACE) && 
            !deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_LEFT_CTRL)
        ) m_movements.second = DENG_MOVEMENT_UPWARD;

        else if 
        (
            !deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_SPACE) && 
            deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_LEFT_CTRL)
        ) m_movements.second = DENG_MOVEMENT_DOWNWARD;

        else m_movements.second = DENG_MOVEMENT_NONE;
    }


    /* FPP camera mouse update method */
    void FPPCameraEv::updateMouseEvData(WindowWrap *p_ww) {
        std::lock_guard<std::mutex> lck(ext_mii.mut);
        deng_GetMousePos (
            p_ww->getWindow(), 
            &ext_mii.mouse_coords.first, 
            &ext_mii.mouse_coords.second, 
            0
        );

        ext_mii.active_btn_c = p_ww->getWindow()->active_keys.btn_count;
        if(ext_mii.active_btn_c && m_prev_active_btn_c != ext_mii.active_btn_c) {
            deng_MouseButton *tmp = (deng_MouseButton*) realloc (
                ext_mii.active_btn,
                sizeof(deng_MouseButton) * ext_mii.active_btn_c
            );

            // Check for memory allocation error
            cm_CheckMemoryAlloc(tmp);
            
            ext_mii.active_btn = tmp;
            memcpy (
                ext_mii.active_btn,
                p_ww->getWindow()->active_keys.p_btn,
                sizeof(deng_MouseButton) * ext_mii.active_btn_c
            );

            m_prev_active_btn_c = ext_mii.active_btn_c;
        }

        m_mouse_pos.first = 
        (ext_mii.mouse_coords.first * 2) / 
        (m_mouse_sens.first * p_ww->getSize().first);

        m_mouse_pos.second = 
        (ext_mii.mouse_coords.second * 2) / 
        (m_mouse_sens.second * p_ww->getSize().second);
    }

    
    /* Set the camera rotation according to mouse movements */
    void FPPCameraEv::setCameraViewRotation (
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) {
        deng_vec_t x_rot = 0;
        deng_vec_t y_rot = 0;
        if(m_mouse_pos.first < 2.0f && m_mouse_pos.first > -2.0f)
            y_rot = PI * m_mouse_pos.first;

        else if(m_mouse_pos.first >= 2.0f) {
            p_ww->getWindow()->virtual_mouse_position.x = 
            (
                (m_mouse_pos.first - 2.0f) *
                m_mouse_sens.first *
                p_ww->getSize().first
            ) / 2;
        }

        else if(m_mouse_pos.first <= -2.0f) {
            p_ww->getWindow()->virtual_mouse_position.x =
            (
                (m_mouse_pos.first + 2.0f) *
                m_mouse_sens.first *
                p_ww->getSize().first
            ) / 2;
        }

        if(m_mouse_pos.second <= 1.0f && m_mouse_pos.second >= -1.0f)
            x_rot = PI * m_mouse_pos.second / 2;

        else if(m_mouse_pos.second < -1.0f) {
            p_ww->getWindow()->virtual_mouse_position.y =
            (
                -m_mouse_sens.second *
                p_ww->getSize().second
            ) / 2;

            m_mouse_pos.second = -1.0f;
            x_rot = -PI / 2;
        }

        else if(m_mouse_pos.second > 1.0f) {
            p_ww->getWindow()->virtual_mouse_position.y = 
            (
                m_mouse_sens.second *
                p_ww->getSize().second
            ) / 2;

            m_mouse_pos.second = 1.0f;
            x_rot = PI / 2;
        }

        p_vm->setTransformationMatrix();
        p_vm->setRotation(x_rot, y_rot);
    }


    /* Check if input FPP camera mouse input mode has changed */
    void FPPCameraEv::checkForInputModeChange (
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) {
        updateMouseEvData(p_ww);
        ext_mii.mouse_input = p_ww->getInputMode();
        
        if(ext_mii.mouse_input == DENG_INPUT_MOVEMENT) {
            findMovementType(p_ww);
            setCameraViewRotation(p_ww, p_vm);

            if
            (
                m_input_mode_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) && 
                deng_IsKeyActive(p_ww->getWindow(), DENG_KEY_ESCAPE)
            ) {

                #if CAMERA_MOUSE_DEBUG
                    LOG (
                        "frozen_mouse_position x:" + 
                        std::to_string(m_frozen_mouse_position.first) + 
                        "/" + 
                        std::to_string(m_frozen_mouse_position.second)
                    );
                #endif

                m_frozen_mouse_pos.first = p_ww->getWindow()->virtual_mouse_position.x;
                m_frozen_mouse_pos.second = p_ww->getWindow()->virtual_mouse_position.y;
                p_ww->setInputMode(DENG_INPUT_NONMOVEMENT);
                m_input_mode_timer.setNewTimePoint();
                if(m_callback)
                    m_callback(&m_frozen_mouse_pos);
            }
        }

        else if(ext_mii.mouse_input == DENG_INPUT_NONMOVEMENT) {
            m_movements.first = DENG_MOVEMENT_NONE;
            m_movements.second = DENG_MOVEMENT_NONE;
            m_movements.third = DENG_MOVEMENT_NONE;

            if
            (
                m_input_mode_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) && 
                deng_IsKeyActive (
                    p_ww->getWindow(), 
                    DENG_KEY_ESCAPE
                )
            ) {
                p_ww->setInputMode(DENG_INPUT_MOVEMENT);
                p_ww->getWindow()->virtual_mouse_position.x = m_frozen_mouse_pos.first;
                p_ww->getWindow()->virtual_mouse_position.y = m_frozen_mouse_pos.second;
                m_input_mode_timer.setNewTimePoint();
            }
        }
    }


    /* FPP Camera update method */
    void FPPCameraEv::updateEv (
        WindowWrap *p_ww, 
        FPPCamera *p_cam
    ) {
        checkForInputModeChange(p_ww, &p_cam->view_matrix);
        if(m_mov_timer.isTimePassed(DENG_MOVEMENT_INTERVAL)) {
            switch (m_movements.first)
            {
            case DENG_MOVEMENT_LEFTWARD:
                p_cam->moveRU();
                break;

            case DENG_MOVEMENT_RIGHTWARD:
                p_cam->moveU();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (m_movements.second)
            {
            case DENG_MOVEMENT_UPWARD:
                p_cam->moveV();
                break;

            case DENG_MOVEMENT_DOWNWARD:
                p_cam->moveRV();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            switch (m_movements.third)
            {
            case DENG_MOVEMENT_FORWARD:
                p_cam->moveW();
                break;

            case DENG_MOVEMENT_BACKWARD:
                p_cam->moveRW();
                break;

            case DENG_MOVEMENT_NONE: break;
            
            default:
                break;
            }

            m_mov_timer.setNewTimePoint();
        }
    }


    dengMath::vec4<deng_vec_t> FPPCameraEv::getMoveSpeed() { return m_move_speed; } 


    /* FPP Camera class */
    FPPCamera::FPPCamera (
        const dengMath::vec3<deng_vec_t> &camera_mov_speed_mul, 
		const dengMath::vec2<deng_vec_t> &mouse_mov_speed_mul, 
		const deng_vec_t &FOV, 
		const deng_vec_t &near_plane, 
		const deng_vec_t &far_plane, 
        FPPCamNonMovementCallback callback,
		WindowWrap *p_ww
	) : FPPCameraEv(p_ww, mouse_mov_speed_mul, camera_mov_speed_mul, callback),
        view_matrix(DENG_CAMERA_FPP) {
        m_p_ww = p_ww;
        m_is_init = true;
        p_projection_matrix = new dengMath::ProjectionMatrix (
            FOV, 
            near_plane, 
            far_plane, 
            (deng_vec_t) m_p_ww->getSize().first / 
            (deng_vec_t) m_p_ww->getSize().second
        );
    }

    FPPCamera::~FPPCamera() {
        delete p_projection_matrix;
    }
    

    /* FPPCamera movement methods */
    void FPPCamera::moveW() {
        LOG("forward");
        view_matrix.addToPosition (
            getMoveSpeed(), 
            DENG_COORD_AXIS_Z, 
            false
        );
    }

    void FPPCamera::moveRW() {
        LOG("backward");
        view_matrix.addToPosition (
            getMoveSpeed(), 
            DENG_COORD_AXIS_Z, 
            true
        );
    }

    void FPPCamera::moveU() {
        LOG("right");
        view_matrix.addToPosition (
            getMoveSpeed(), 
            DENG_COORD_AXIS_X, 
            true
        );
    }

    void FPPCamera::moveRU() {
        LOG("left");
        view_matrix.addToPosition (
            getMoveSpeed(), 
            DENG_COORD_AXIS_X, 
            false
        );
    }

    void FPPCamera::moveV(){
        view_matrix.addToPosition (
            getMoveSpeed(), 
            DENG_COORD_AXIS_Y, 
            false
        );
    }

    void FPPCamera::moveRV() {
        view_matrix.addToPosition (
            getMoveSpeed(), 
            DENG_COORD_AXIS_Y, 
            true
        );
    }


    /* FPPCamera wrapper method for event update */
    void FPPCamera::update() {
        updateEv(m_p_ww, this);
    }
}
