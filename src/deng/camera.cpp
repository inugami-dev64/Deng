#define __CAMERA_CPP
#include <deng/camera.h>

// Shared data
extern dengui::MouseInputInfo ext_mii;

namespace deng {
    
    /**********************************/
    /*********** Event base ***********/
    /**********************************/
    EventBase::EventBase (
		const dengMath::vec2<deng_f64_t> &mouse_sens,
		const dengMath::vec2<deng_VCPOverflowAction> &vcp_act,
		const dengMath::vec2<dengMath::vec2<deng_f64_t>> &vc_bounds,
		const dengMath::vec2<deng_f64_t> &max_rot
    ) {
        m_mouse_sens = mouse_sens;
        m_vcp_overflow = vcp_act;
        m_vc_bounds = vc_bounds;
        m_max_rot = max_rot;

        deng_LimitVirtualPos (
            vc_bounds.first.second, 
            vc_bounds.first.first, 
            vc_bounds.second.second,
            vc_bounds.second.first
        );

        deng_SetOverflowAction(
            vcp_act.first,
            vcp_act.second
        );

        deng_SetVCSens (
            mouse_sens.first, 
            mouse_sens.second
        );
    }

    /* 
     * Camera mouse update method 
     * This method updates mouse cursor position in WindowWrap instance as well as
     * externally available MouseInputInfo
     */
    void EventBase::updateMouseEvData(WindowWrap *p_ww) {
        std::lock_guard<std::mutex> lck(ext_mii.mut);
        deng_GetMousePos (
            p_ww->getWindow(), 
            0
        );

        ext_mii.is_mouse_input = !p_ww->isVCP();
        if (p_ww->getWindow()->vc_data.is_enabled) {
            ext_mii.mouse_coords.first = 0;
            ext_mii.mouse_coords.second = 0;
            m_mouse_pos.first = p_ww->getWindow()->vc_data.x;
            m_mouse_pos.second = p_ww->getWindow()->vc_data.y;
        }
        else {
            ext_mii.mouse_coords.first = p_ww->getWindow()->mx;
            ext_mii.mouse_coords.second = p_ww->getWindow()->my;
        }
    }


    /* 
     * Calculate virtual mouse position from camera rotation value 
     * This updating is needed in order to avoid camera rotation glitches
     * when rotating editor camera
     */
    void EventBase::getMousePositionFromRot (
        WindowWrap *p_ww,
        dengMath::vec2<deng_f64_t> rot
    ) {
        p_ww->getWindow()->vc_data.x = (rot.first * m_vc_bounds.first.second / m_max_rot.first);
        p_ww->getWindow()->vc_data.y = (rot.second * m_vc_bounds.second.second / m_max_rot.second);

        m_mouse_pos.first = p_ww->getWindow()->vc_data.x;
        m_mouse_pos.second = p_ww->getWindow()->vc_data.y;
    }

    
    /* 
     * Set the camera rotation according to mouse position
     */
    dengMath::vec2<deng_f64_t> EventBase::getMouseRotation() {
        dengMath::vec2<deng_f64_t> out_rot;
        out_rot.first = m_mouse_pos.second / m_vc_bounds.second.second * m_max_rot.first;
        out_rot.second = m_mouse_pos.first / m_vc_bounds.first.second * m_max_rot.second; 
        
        return out_rot;
    }


    /*********************************************/
    /****** First person perspective camera ******/
    /*********************************************/
    
    /* FPP camera Events */
    FPPCameraEv::FPPCameraEv (
        WindowWrap *p_ww,
        const dengMath::vec2<deng_f64_t> &mouse_sens,
        const dengMath::vec3<deng_vec_t> &camera_mov_sens,
		dengMath::ViewMatrix *p_vm,
		FPPInputChangeCallback mov_disable_callback,
		FPPInputChangeCallback mov_enable_callback
    ) : EventBase (
            mouse_sens, 
            {
                DENG_VCP_OVERFLOW_ACTION_TO_OPPOSITE_POSITION, 
                DENG_VCP_OVERFLOW_ACTION_BLOCK_POSITION
            },
            { -360, 360, -90, 90},
            { PI / 2, PI * 2 }
    ) {
        m_input_enable_callback = mov_enable_callback;
        m_input_disable_callback = mov_disable_callback;

        deng_SetMouseCursorMode(p_ww->getWindow(), DENG_MOUSE_MODE_INVISIBLE);
        m_move_speed.first = (deng_vec_t) (DENG_CAMERA_BASE_SPEED_X * camera_mov_sens.first);
        m_move_speed.second = (deng_vec_t) (DENG_CAMERA_BASE_SPEED_Y * camera_mov_sens.second);
        m_move_speed.third = (deng_vec_t) (DENG_CAMERA_BASE_SPEED_Z * camera_mov_sens.third);
        m_move_speed.fourth = 0.0f;
    }


    /* 
     * Find the current movement type and direction 
     */
    void FPPCameraEv::findMovements(WindowWrap *p_ww) {
        if
		(
			deng_FindKeyStatus(
				DENG_KEY_W,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			) &&
			!deng_FindKeyStatus(
				DENG_KEY_S,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			)
		) {
            m_movements.third = DENG_MOVEMENT_FORWARD;
            LOG("Moving forward");
        }
        
        else if
		(
			!deng_FindKeyStatus(
				DENG_KEY_W,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			) &&
			deng_FindKeyStatus(
				DENG_KEY_S,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			)
		) {
            m_movements.third = DENG_MOVEMENT_BACKWARD;
        }

        else
            m_movements.third = DENG_MOVEMENT_NONE;

        if
		(
			deng_FindKeyStatus(
				DENG_KEY_A,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			) &&
			!deng_FindKeyStatus(
				DENG_KEY_D,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			)
		) {
            m_movements.first = DENG_MOVEMENT_LEFTWARD;
            LOG("Moving leftward");
        }

        else if
		(
			!deng_FindKeyStatus(
				DENG_KEY_A,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			) &&
			deng_FindKeyStatus(
				DENG_KEY_D,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			)
		) {
            m_movements.first = DENG_MOVEMENT_RIGHTWARD;
        }
        
        else m_movements.first = DENG_MOVEMENT_NONE;

        if
		(
			deng_FindKeyStatus(
				DENG_KEY_SPACE,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			) &&
			!deng_FindKeyStatus(
				DENG_KEY_LEFT_CTRL,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			)
		) {
            m_movements.second = DENG_MOVEMENT_UPWARD;
            LOG("Moving upward");
        }

        else if
		(
			!deng_FindKeyStatus(
				DENG_KEY_SPACE,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			) &&
			deng_FindKeyStatus(
				DENG_KEY_LEFT_CTRL,
				DENG_MOUSE_BTN_UNKNOWN,
				DENG_INPUT_TYPE_KB,
				DENG_INPUT_EVENT_TYPE_ACTIVE
			)
		) {
            m_movements.second = DENG_MOVEMENT_DOWNWARD;
        }

        else m_movements.second = DENG_MOVEMENT_NONE;
    }

    
    /* 
     * Check if input FPP camera mouse input mode has changed 
     * This method is not thread safe and can cause race conditions, make sure that ext_mii.mut is locked!
     */
    void FPPCameraEv::checkForInputModeChange (
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) {
        EventBase::updateMouseEvData(p_ww);
        // Check if input mode should be changed ([ESC] key)
        if (
            m_input_mode_timer.isTimePassed(DENG_KEY_PRESS_INTERVAL) &&
            deng_FindKeyStatus(
                DENG_KEY_ESCAPE,
                DENG_MOUSE_BTN_UNKNOWN,
                DENG_INPUT_TYPE_KB,
                DENG_INPUT_EVENT_TYPE_ACTIVE
            )
        ) {
            // Disable virtual cursor mode
            if (p_ww->isVCP()) {
                p_ww->setVCMode(false, true);
                if(m_input_disable_callback) 
                    m_input_disable_callback (
                        {
                            p_ww->getWindow()->vc_data.x,
                            p_ww->getWindow()->vc_data.y
                        }
                    );
            }

            // Enable virtual cursor mode
            else {
                p_ww->setVCMode(true, true);
                if (m_input_enable_callback)
                    m_input_enable_callback(
                        {
                            p_ww->getWindow()->vc_data.x,
                            p_ww->getWindow()->vc_data.y
                        }
					);
            }
			m_input_mode_timer.setNewTimePoint();
        }
        ext_mii.is_mouse_input = !p_ww->isVCP();

        // Check if virtual mouse cursor mode is enabled and 
        // if true then update camera rotation and key events
        if(p_ww->isVCP()) {
            findMovements(p_ww);
            dengMath::vec2<deng_f64_t> rot = EventBase::getMouseRotation();
            p_vm->setCameraRotation (
                (deng_vec_t) rot.first, 
                (deng_vec_t) rot.second
            );
            p_vm->setTransformationMatrix(false);
        }

        else {
            m_movements.first = DENG_MOVEMENT_NONE;
            m_movements.second = DENG_MOVEMENT_NONE;
            m_movements.third = DENG_MOVEMENT_NONE;
        }
    }


    /* FPP Camera update method */
    void FPPCameraEv::updateEv (
        WindowWrap *p_ww, 
        FPPCamera *p_cam
    ) {
        checkForInputModeChange(p_ww, &p_cam->view_matrix);
        if (m_mov_timer.isTimePassed(DENG_MOVEMENT_INTERVAL)) {
            switch (m_movements.first)
            {
            case DENG_MOVEMENT_LEFTWARD:
                p_cam->moveRU();
                LOG("Moving left");
                break;

            case DENG_MOVEMENT_RIGHTWARD:
                p_cam->moveU();
                LOG("Moving right");
                break;

            case DENG_MOVEMENT_NONE: break;

            default:
                break;
            }

            switch (m_movements.second)
            {
            case DENG_MOVEMENT_UPWARD:
                p_cam->moveV();
                LOG("Moving up");
                break;

            case DENG_MOVEMENT_DOWNWARD:
                p_cam->moveRV();
                LOG("Moving down");
                break;

            case DENG_MOVEMENT_NONE: break;

            default:
                break;
            }

            switch (m_movements.third)
            {
            case DENG_MOVEMENT_FORWARD:
                p_cam->moveW();
                LOG("Moving forward");
                break;

            case DENG_MOVEMENT_BACKWARD:
                p_cam->moveRW();
                LOG("Moving backward");
                break;

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
        const dengMath::vec3<deng_vec_t> &camera_mov_sens, 
		const dengMath::vec2<deng_f64_t> &mouse_sens, 
		const deng_vec_t &FOV, 
		const deng_vec_t &near_plane, 
		const deng_vec_t &far_plane, 
        FPPInputChangeCallback mov_disable_callback,
        FPPInputChangeCallback mov_enable_callback,
		WindowWrap *p_ww
	) : FPPCameraEv (
            p_ww, 
            mouse_sens, 
            camera_mov_sens, 
            &view_matrix,
            mov_disable_callback,
            mov_enable_callback
        ),
        view_matrix(DENG_CAMERA_FPP) {
        m_p_ww = p_ww;

        #ifdef __linux
            m_p_ww->getWindow()->vc_data.cursor = DENG_CURSOR_HIDDEN;
            m_p_ww->getWindow()->vc_data.is_lib_cur = false;
        #endif
        m_p_ww->setVCMode(true, true);

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
        dengMath::vec2<deng_f64_t> mouse_sens,
        deng_vec_t zoom_step,
        dengMath::vec3<deng_vec_t> origin,
        WindowWrap *p_ww,
        dengMath::ViewMatrix *p_vm
    ) : EventBase (
            mouse_sens,
            {
                DENG_VCP_OVERFLOW_ACTION_TO_OPPOSITE_POSITION,
                DENG_VCP_OVERFLOW_ACTION_BLOCK_POSITION
            },
            {-360, 360, -90, 90},
            {PI / 2, PI * 2}
        ) { 

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

        m_zoom_step = zoom_step;
    } 

    
    void EditorCameraEv::findEditorEvent(WindowWrap *p_ww) {
        EventBase::updateMouseEvData(p_ww);
        ext_mii.mut.lock();

        // Check if rotation mode should be enabled
        if (
            deng_FindKeyStatus(
                DENG_KEY_UNKNOWN,
                DENG_MOUSE_BTN_2,
                DENG_INPUT_TYPE_MOUSE,
                DENG_INPUT_EVENT_TYPE_ACTIVE
            )
        ) m_editor_cam_ev = DENG_EDITOR_CAMERA_MOUSE_ROTATE;
        

        else if (
            deng_FindKeyStatus (
                DENG_KEY_UNKNOWN,
                DENG_MOUSE_SCROLL_DOWN,
                DENG_INPUT_TYPE_MOUSE,
                DENG_INPUT_EVENT_TYPE_ACTIVE
            )
        ) {
            m_editor_cam_ev = DENG_EDITOR_CAMERA_Z_MOV_OUT;
        }

        else if (
            deng_FindKeyStatus (
                DENG_KEY_UNKNOWN,
                DENG_MOUSE_SCROLL_UP,
                DENG_INPUT_TYPE_MOUSE,
                DENG_INPUT_EVENT_TYPE_ACTIVE
            )        
        ) {
            m_editor_cam_ev = DENG_EDITOR_CAMERA_Z_MOV_IN;
        }

        else m_editor_cam_ev = DENG_EDITOR_CAMERA_NONE;
        
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
                p_ww->setVCMode(false, false);
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
                p_ww->setVCMode(false, false);
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
                #ifdef __linux__
                    p_ww->getWindow()->vc_data.is_lib_cur = true;
                    p_ww->getWindow()->vc_data.cursor = DENG_CURSOR_ROTATE;
                #endif
                p_ww->setVCMode(true, false);
                m_is_rot_cur = true;
            }

            m_last_rot = EventBase::getMouseRotation();

            p_vm->setPointRotation (
                origin,
                m_last_rot.first,
                m_last_rot.second
            );
            break;
        }
                                              
        default:
            if(m_is_rot_cur) {
                p_ww->setVCMode(false, false);
                m_is_rot_cur = false;
            }
            break;
        }

        p_vm->setTransformationMatrix(true);
    }


    EditorCamera::EditorCamera (
        const deng_vec_t &zoom_step,
        const dengMath::vec3<deng_vec_t> &origin,
        const dengMath::vec2<deng_f64_t> &mouse_sens,
        const deng_vec_t &FOV,
        const deng_vec_t &near_plane,
        const deng_vec_t &far_plane,
        WindowWrap *p_ww
    ) : EditorCameraEv (
            mouse_sens, 
            zoom_step, 
            origin, 
            p_ww, 
            &view_matrix
        ), 
        view_matrix(DENG_CAMERA_EDITOR) {
        m_origin = origin;
        m_p_ww = p_ww;

        #ifdef __linux__
            m_p_ww->getWindow()->vc_data.cursor = DENG_CURSOR_ROTATE;
            m_p_ww->getWindow()->vc_data.is_lib_cur = true;
        #endif
        m_p_ww->setVCMode(false, false);

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
