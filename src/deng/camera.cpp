#include "../../headers/deng/api_core.h"
#include <string>

// Shared data
extern dengui::MouseInputInfo ext_mii;

namespace deng {
    
    /**********************************/
    /**********************************/
    /*********** Event base ***********/
    /**********************************/
    /**********************************/

    /* Camera mouse update method */
    void EventBase::updateMouseEvData(WindowWrap *p_ww) {
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


    /* Calculate virtual mouse position from camera rotation value */
    void EventBase::getMousePositionFromRot (
        WindowWrap *p_ww,
        dengMath::vec2<deng_vec_t> rot
    ) {
        p_ww->getWindow()->virtual_mouse_position.x = {
            (rot.second / PI) * 
            m_mouse_sens.first *
            p_ww->getSize().first / 2
        };

        p_ww->getWindow()->virtual_mouse_position.y = {
            (rot.first / PI) *
            m_mouse_sens.second *
            p_ww->getSize().second
        };

        m_mouse_pos.first = p_ww->getWindow()->virtual_mouse_position.x;
        m_mouse_pos.second = p_ww->getWindow()->virtual_mouse_position.y;
    }

    
    /* Set the camera rotation according to mouse movements */
    void EventBase::getMouseRotation (
        WindowWrap *p_ww,
        dengMath::vec2<deng_vec_t> *p_out_rot
    ) {
        p_out_rot->first = 0;
        p_out_rot->second = 0;

        if(m_mouse_pos.first < 2.0f && m_mouse_pos.first > -2.0f)
            p_out_rot->second = PI * m_mouse_pos.first;

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

        if(m_mouse_pos.second < 1.0f && m_mouse_pos.second > -1.0f)
            p_out_rot->first = PI * m_mouse_pos.second / 2;

        else if(m_mouse_pos.second <= -1.0f) {
            p_ww->getWindow()->virtual_mouse_position.y =
            (
                -m_mouse_sens.second *
                p_ww->getSize().second
            ) / 2;

            m_mouse_pos.second = 0.999f;
            p_out_rot->first = -PI / 2 + 0.001f;
        }

        else if(m_mouse_pos.second >= 1.0f) {
            p_ww->getWindow()->virtual_mouse_position.y = 
            (
                m_mouse_sens.second *
                p_ww->getSize().second
            ) / 2;

            m_mouse_pos.second = 0.999f;
            p_out_rot->first = PI / 2 - 0.001f;
        }
    }


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

    
    /* Check if input FPP camera mouse input mode has changed */
    void FPPCameraEv::checkForInputModeChange (
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) {
        EventBase::updateMouseEvData(p_ww);
        ext_mii.mouse_input = p_ww->getInputMode();
        
        if(ext_mii.mouse_input == DENG_INPUT_MOVEMENT) {
            findMovementType(p_ww);
            dengMath::vec2<deng_vec_t> rot;
            EventBase::getMouseRotation(p_ww, &rot);
            p_vm->setCameraRotation(rot.first, rot.second);
            p_vm->setTransformationMatrix(false);

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
                if(m_callback) m_callback(&m_frozen_mouse_pos);
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


    dengMath::vec4<deng_vec_t> FPPCameraEv::getMoveSpeed (
        deng_bool_t op_x,
        deng_bool_t op_y,
        deng_bool_t op_z
    ) { 
        dengMath::vec4<deng_vec_t> move_speed;

        if(op_x)
            move_speed.first = m_move_speed.first;
        else move_speed.first = -m_move_speed.first;

        if(op_y)
            move_speed.second = -m_move_speed.second;
        else move_speed.second = m_move_speed.second;

        if(op_z)
            move_speed.third = -m_move_speed.third;
        else move_speed.third = m_move_speed.third;

        return move_speed; 
    } 


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

        m_p_ww->getWindow()->virtual_mouse_position.cursor = DENG_CURSOR_HIDDEN;
        m_p_ww->getWindow()->virtual_mouse_position.is_lib_cur = false;
        m_p_ww->setInputMode(DENG_INPUT_MOVEMENT);

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
        view_matrix.addToPosition (
            getMoveSpeed(false, false, false), 
            DENG_COORD_AXIS_Z 
        );
    }

    void FPPCamera::moveRW() {
        view_matrix.addToPosition (
            getMoveSpeed(false, false, true), 
            DENG_COORD_AXIS_Z
        );
    }

    void FPPCamera::moveU() {
        view_matrix.addToPosition (
            getMoveSpeed(false, false, false), 
            DENG_COORD_AXIS_X
        );
    }

    void FPPCamera::moveRU() {
        view_matrix.addToPosition (
            getMoveSpeed(true, false, false), 
            DENG_COORD_AXIS_X
        );
    }

    void FPPCamera::moveV(){
        view_matrix.addToPosition (
            getMoveSpeed(false, false, false), 
            DENG_COORD_AXIS_Y 
        );
    }

    void FPPCamera::moveRV() {
        view_matrix.addToPosition (
            getMoveSpeed(false, true, false), 
            DENG_COORD_AXIS_Y
        );
    }


    /* FPPCamera wrapper method for event update */
    void FPPCamera::update() {
        updateEv(m_p_ww, this);
    }


    /********************************/
    /********************************/
    /********* Editor Camera ********/
    /********************************/
    /********************************/

    EditorCameraEv::EditorCameraEv (
        dengMath::vec2<deng_vec_t> mouse_mov_speed_mul,
        deng_vec_t zoom_step,
        dengMath::vec3<deng_vec_t> origin,
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) { 
        m_mouse_sens.first = 1 / mouse_mov_speed_mul.first;
        m_mouse_sens.second = 1 / mouse_mov_speed_mul.second;

        // Set default camera position and rotation values
        p_vm->addToPosition (
            {
                DENG_EDITOR_CAMERA_DEFAULT_POS_X, 
                DENG_EDITOR_CAMERA_DEFAULT_POS_Y, 
                DENG_EDITOR_CAMERA_DEFAULT_POS_Z
            }, 
            DENG_COORD_AXIS_UNDEFINED
        );

        p_vm->setPointRotation (
            origin, 
            DENG_EDITOR_CAMERA_DEFAULT_X_ROT, 
            DENG_EDITOR_CAMERA_DEFAULT_Y_ROT
        );

        getMousePositionFromRot (
            p_ww,
            {
                DENG_EDITOR_CAMERA_DEFAULT_X_ROT,
                DENG_EDITOR_CAMERA_DEFAULT_Y_ROT
            }
        );

        m_last_mouse_pos.first = p_ww->getWindow()->virtual_mouse_position.x;
        m_last_mouse_pos.second = p_ww->getWindow()->virtual_mouse_position.y;
        m_last_rot.first = DENG_EDITOR_CAMERA_DEFAULT_X_ROT;
        m_last_rot.second = DENG_EDITOR_CAMERA_DEFAULT_Y_ROT;
        m_zoom_step = zoom_step;
    } 

    
    void EditorCameraEv::findEditorEvent(WindowWrap *p_ww) {
        size_t index;
        EventBase::updateMouseEvData(p_ww);
        ext_mii.mut.lock();

        m_editor_cam_ev = DENG_EDITOR_CAMERA_NONE;
        
        // Search from mouse events 
        for(index = 0; index < (size_t) ext_mii.active_btn_c; index++) {
            if(ext_mii.active_btn[index] == DENG_MOUSE_BTN_2) {
                m_editor_cam_ev = DENG_EDITOR_CAMERA_MOUSE_ROTATE;
                break;
            }

            else if(ext_mii.active_btn[index] == DENG_MOUSE_SCROLL_DOWN) {
                m_editor_cam_ev = DENG_EDITOR_CAMERA_Z_MOV_OUT;
                break;
            }

            else if(ext_mii.active_btn[index] == DENG_MOUSE_SCROLL_UP) {
                m_editor_cam_ev = DENG_EDITOR_CAMERA_Z_MOV_IN;
                break;
            }
        }
        
        ext_mii.mut.unlock();
    }


    /* Zoom in -z direction */
    void EditorCameraEv::zoomIn (
        WindowWrap *p_ww, 
        dengMath::ViewMatrix *p_vm
    ) {
        dengMath::vec4<deng_vec_t> forward_side;
        p_vm->getSidesInWorldCoords (
            nullptr, 
            nullptr, 
            &forward_side
        );

        forward_side.norm();
        forward_side *= m_zoom_step;

        p_vm->addToPosition (
            forward_side, 
            DENG_COORD_AXIS_UNDEFINED
        );
    }


    /* Move camera position in z direction */
    void EditorCameraEv::zoomOut (
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) {
        dengMath::vec4<deng_vec_t> forward_side;
        p_vm->getSidesInWorldCoords(
            nullptr, 
            nullptr, 
            &forward_side
        );
        
        forward_side.norm();
        forward_side *= m_zoom_step;

        forward_side.first = -forward_side.first;
        forward_side.second = -forward_side.second;
        forward_side.third = -forward_side.third;
        p_vm->addToPosition (
            forward_side, 
            DENG_COORD_AXIS_UNDEFINED
        );
    }


    /* Calculate view matrix rotation from mouse coordinates */
    void EditorCameraEv::getRotFromMousePos (
        dengMath::vec2<deng_vec_t> mouse_pos,
        dengMath::vec2<deng_vec_t> *p_out_rot
    ) {
        p_out_rot->first = PI * m_mouse_pos.second / 2;
        p_out_rot->second = PI * m_mouse_pos.first;
    }


    /* Update editor camera event polling */
    void EditorCameraEv::updateEv (
        WindowWrap *p_ww,
        dengMath::vec3<deng_vec_t> origin, 
        dengMath::ViewMatrix *p_vm
    ) {
        EventBase::updateMouseEvData(p_ww);
        findEditorEvent(p_ww);

        switch(m_editor_cam_ev)
        {
        case DENG_EDITOR_CAMERA_Z_MOV_IN: {
            if(m_is_rot_cur) {
                m_last_mouse_pos.first = p_ww->getWindow()->virtual_mouse_position.x;
                m_last_mouse_pos.second = p_ww->getWindow()->virtual_mouse_position.y;
                p_ww->setInputMode(DENG_INPUT_NONMOVEMENT);
                m_is_rot_cur = false;
            }

            zoomIn(p_ww, p_vm);
            p_vm->setPointRotation (
                origin, 
                m_last_rot.first, 
                m_last_rot.second
            );
            break;
        }

        case DENG_EDITOR_CAMERA_Z_MOV_OUT: {
            if(m_is_rot_cur) {
                m_last_mouse_pos.first = p_ww->getWindow()->virtual_mouse_position.x;
                m_last_mouse_pos.second = p_ww->getWindow()->virtual_mouse_position.y;
                p_ww->setInputMode(DENG_INPUT_NONMOVEMENT);
                m_is_rot_cur = false;
            }

            zoomOut(p_ww, p_vm);
            p_vm->setPointRotation (
                origin,
                m_last_rot.first, 
                m_last_rot.second
            );

            break;
        }

        case DENG_EDITOR_CAMERA_MOUSE_ROTATE: {
            if(!m_is_rot_cur) {
                p_ww->getWindow()->virtual_mouse_position.is_lib_cur = true;
                p_ww->getWindow()->virtual_mouse_position.cursor = DENG_CURSOR_ROTATE;
                p_ww->setInputMode(DENG_INPUT_MOVEMENT);
                p_ww->getWindow()->virtual_mouse_position.x = m_last_mouse_pos.first;
                p_ww->getWindow()->virtual_mouse_position.y = m_last_mouse_pos.second;
                m_is_rot_cur = true;
            }

            EventBase::getMouseRotation(p_ww, &m_last_rot);

            p_vm->setPointRotation (
                origin,
                m_last_rot.first,
                m_last_rot.second
            );
            break;
        }
                                              
        default:
            if(m_is_rot_cur) {
                m_last_mouse_pos.first = p_ww->getWindow()->virtual_mouse_position.x;
                m_last_mouse_pos.second = p_ww->getWindow()->virtual_mouse_position.y;
                p_ww->setInputMode(DENG_INPUT_NONMOVEMENT);
                m_is_rot_cur = false;
            }
            break;
        }

        p_vm->setTransformationMatrix(true);
    }


    EditorCamera::EditorCamera (
        const deng_vec_t &zoom_step,
        const dengMath::vec3<deng_vec_t> &origin,
        const dengMath::vec2<deng_vec_t> &mouse_mov_speed_mul,
        const deng_vec_t &FOV,
        const deng_vec_t &near_plane,
        const deng_vec_t &far_plane,
        WindowWrap *p_ww
    ) : EditorCameraEv(mouse_mov_speed_mul, zoom_step, origin, p_ww, &view_matrix), 
        view_matrix(DENG_CAMERA_EDITOR) {
        m_origin = origin;
        m_p_ww = p_ww;

        m_p_ww->getWindow()->virtual_mouse_position.cursor = DENG_CURSOR_ROTATE;
        m_p_ww->getWindow()->virtual_mouse_position.is_lib_cur = true;
        m_p_ww->setInputMode(DENG_INPUT_NONMOVEMENT);

        p_projection_matrix = new dengMath::ProjectionMatrix (
            FOV,
            near_plane,
            far_plane,
            (deng_vec_t) m_p_ww->getSize().first /
            (deng_vec_t) m_p_ww->getSize().second
        );
    }

    
    EditorCamera::~EditorCamera() {
        delete p_projection_matrix;
    }


    void EditorCamera::update() {
        updateEv (
            m_p_ww, 
            m_origin,
            &view_matrix 
        );
    }
}
